#include "OutputHandler.h"

static void				audio_header_fill(uint8_t *data, AVStream *st);
static void				video_header_fill(uint8_t *data, AVStream *st);
static void				frame_header_fill(uint8_t *data, int size, AVPacket *pkt, AVStream *st, AVRational new_tb, int64_t base_ts);
static enum CodecID		libav_codec_id(uint8_t mytype);

OutputHandler::OutputHandler() {
	outctx = 0;
	this->streams = 1;
	this->video_codec_id = libav_codec_id(0);
	this->vwidth = 0;
	this->vheight = 0;
	this->video_time_base.den = 0;
	this->video_time_base.num = 0;
}

#define BUFSIZE 10000

BOOL   fConnected = FALSE; 
DWORD  dwThreadId = 0; 
HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL; 
LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");   

bool OutputHandler::Open(const char *fname) {
	output_format = strdup("h264");

	if (fname && fname[0]) {
		output_file = strdup(fname);
	} else {
		output_file = strdup("-");
	}
 
	fprintf(stderr, "%s %s\n",output_file, output_format);
	selected_streams = 0x01;
  
	return true;
}

void OutputHandler::Close() {
	av_write_trailer(outctx);
	avio_close(outctx->pb);
}

void OutputHandler::WriteChunk(Chunk c) {
	int			header_size;
	int			frames, i, media_type;
	uint8_t*	p;

	if (c.data[0] == 0) {
		fprintf(stderr, "Error! strange chunk: %x!!!\n", c.data[0]);
	return;
	} else if (c.data[0] < 127) {
		header_size = VIDEO_PAYLOAD_HEADER_SIZE;
		media_type = 1;
	} else {
		header_size = AUDIO_PAYLOAD_HEADER_SIZE;
		media_type = 2;
	}
	if (outctx == NULL) {
		format_init(&c);
		
		fflush(stdout);
		snprintf(outctx->filename, sizeof(outctx->filename), "%s", "pipe:1");
		//av_dump_format(outctx, 0, output_file, 1);
		if (avio_open(&outctx->pb, "pipe:1", AVIO_FLAG_WRITE)< 0) {
            fprintf(stderr, "Could not open '%s'\n", output_file);
			system("PAUSE");
            exit(1);
        }
		fflush(stdout);
		avformat_write_header(outctx, NULL);
	}
	if ((streams & media_type) == 0) {
		return;		/* Received a chunk for a non-selected stream */
	}
	frames = c.data[header_size - 1];
	p = c.data + header_size + FRAME_HEADER_SIZE * frames;
	
	for (i = 0; i < frames; i++) {
		AVPacket pkt;
		int64_t pts, dts;
		int frame_size;

		frame_header_parse(c.data + header_size + FRAME_HEADER_SIZE * i,
						&frame_size, &pts, &dts);
		//printf("Frame %d PTS1: %d\n", i, pts);
		av_init_packet(&pkt);
		pkt.stream_index = 0;//(media_type == 2) && (((streams & 0x01) == 0x01));
		if (pts != -1) {
			pts += (pts < prev_pts - ((1LL << 31) - 1)) ? ((prev_pts >> 32) + 1) << 32 : (prev_pts >> 32) << 32;
			//dprintf(" PTS2: %d\n", pts);
			prev_pts = pts;
			//printf("Frame %d has size %d --- PTS: %lld DTS: %lld\n", i, frame_size,
			//                                       av_rescale_q(pts, outctx->streams[0]->codec->time_base, AV_TIME_BASE_Q),
			//                                       av_rescale_q(dts, outctx->streams[0]->codec->time_base, AV_TIME_BASE_Q));
			pkt.pts = av_rescale_q(pts, outctx->streams[0]->codec->time_base, outctx->streams[0]->time_base);
		} else {
			pkt.pts = AV_NOPTS_VALUE;
		}
		dts += (dts < prev_dts - ((1LL << 31) - 1)) ? ((prev_dts >> 32) + 1) << 32 : (prev_dts >> 32) << 32;
		prev_dts = dts;
		pkt.dts = av_rescale_q(dts, outctx->streams[0]->codec->time_base, outctx->streams[0]->time_base);
		pkt.data = p;
		p += frame_size;
		pkt.size = frame_size;
		fflush(stdout);
		av_write_frame(outctx, &pkt);

		//write(1, c.data + header_size + frames * FRAME_HEADER_SIZE, c.size - header_size - frames * FRAME_HEADER_SIZE);
		//write(1, pkt.data, pkt.size);
	}
}

#if 0
void OutputHandler::format_gen(const Chunk *c)
{
	uint8_t codec;

	codec = c->data[0];
	if ((codec < 128) && ((streams & 0x01) == 0)) {
		int width, height, frame_rate_n, frame_rate_d;
		printf("Video Initialized\n");
		this->streams |= 0x01;
		this->video_codec_id = codec_type(codec);
		video_payload_header_parse(c->data, &codec, &width, &height, &frame_rate_n, &frame_rate_d);
		this->width = width;
		this->height = height;
		this->video_time_base.den = frame_rate_n;
		this->video_time_base.num = frame_rate_d;
	} else if ((codec > 128) && ((streams & 0x02) == 0)) {
		uint8_t channels;
		int sample_rate, frame_size;
		printf("Tha to deis mwre para polla\n");
		audio_payload_header_parse(c->data, &codec, &channels, &sample_rate, &frame_size);
		this->streams |= 0x02;
		this->audio_codec_id = codec_type(codec);
		this->sample_rate = sample_rate;
		this->channels = channels;
		this->frame_size = frame_size;
		this->audio_time_base.num = frame_size;
		this->audio_time_base.den = sample_rate;
	}
	printf("Tha to deis mwre Streams : %d\n", streams);
	printf("Tha to deis mwre Streams : %d\n", selected_streams);
	if (streams == selected_streams) {
		AVCodecContext *cod;
		printf("Tha to deis mwre lougra\n");
		format_init();
		if (streams & 0x01) {
			//printf("Tha to deis mwre\n");
			av_new_stream(outctx, 0);
			cod = outctx->streams[outctx->nb_streams - 1]->codec;
			cod->codec_id = video_codec_id;
			cod->codec_type = AVMEDIA_TYPE_VIDEO;
			cod->width = width;
			cod->height = height;
			cod->time_base.den = video_time_base.den;
			cod->time_base.num = video_time_base.num;
			outctx->streams[0]->avg_frame_rate.num = video_time_base.den;
			outctx->streams[0]->avg_frame_rate.den = video_time_base.num;
			cod->pix_fmt = PIX_FMT_YUV420P;
			cod->sample_rate = 0;
			if (outctx->oformat->flags & AVFMT_GLOBALHEADER)
				cod->flags |= CODEC_FLAG_GLOBAL_HEADER;
		}
		if (streams & 0x02) {
			//printf("Tha to deis mwre\n");
			av_new_stream(outctx, 1);
			cod = outctx->streams[outctx->nb_streams - 1]->codec;
			cod->codec_id = audio_codec_id;
			cod->codec_type = AVMEDIA_TYPE_AUDIO;
			cod->sample_rate = sample_rate;
			cod->channels = channels;
			cod->frame_size = frame_size;
			cod->time_base.num = audio_time_base.num;
			cod->time_base.den = audio_time_base.den;
		}
		prev_pts = 0;
		prev_dts = 0;
	}
}
#endif

void OutputHandler::format_init(const Chunk *c)
{
	AVCodecContext *cod;
	AVOutputFormat *outfmt;
	int width, height, frame_rate_n, frame_rate_d;
	uint8_t codec;

	av_register_all();

	video_payload_header_parse(c->data, &codec, &width, &height, &frame_rate_n, &frame_rate_d);
	fprintf(stderr, "Frame size: %dx%d -- Frame rate: %d / %d\n", width, height, frame_rate_n, frame_rate_d);
	outfmt = av_guess_format(output_format, "pipe:1", NULL);
	fprintf(stderr, "Name : %s  %s\n", outfmt->name, outfmt->long_name);
	outctx = avformat_alloc_context();
	if (outctx == NULL) {
		return;
	}

	outctx->oformat = outfmt;
	av_new_stream(outctx, 0);
	
	AVCodec *codecPtr = avcodec_find_decoder(CODEC_ID_H264);
		if (!codecPtr) exit(1);
 
	//outctx->streams[0]->codec = avcodec_alloc_context();
	if (!(avcodec_open(outctx->streams[0]->codec, codecPtr) < 0) )
		fprintf(stderr,"\n\n\n\n\nPARA POLU !!!!!!!\n\n\n\n\n");

	cod = outctx->streams[0]->codec;
	cod->codec_id = libav_codec_id(codec);
	cod->ticks_per_frame = 2;
	cod->codec_type = AVMEDIA_TYPE_VIDEO;
	cod->width = width;
	cod->height= height;
	cod->time_base.den = frame_rate_d;
	cod->time_base.num = frame_rate_n;
	//AVRational rat = av_inv_q( cod->time_base );
	//outctx->streams[0]->time_base.num = rat.num;
	//outctx->streams[0]->time_base.den = rat.den;
	outctx->streams[0]->avg_frame_rate.num = frame_rate_d;
	outctx->streams[0]->avg_frame_rate.den = frame_rate_n;
	outctx->streams[0]->r_frame_rate.num = frame_rate_d;
	outctx->streams[0]->r_frame_rate.den = frame_rate_n;
	cod->pix_fmt = PIX_FMT_YUV420P;

	if (outctx->oformat->flags & AVFMT_GLOBALHEADER)
		cod->flags |= CODEC_FLAG_GLOBAL_HEADER;


	this->prev_pts = 0;
	this->prev_dts = 0;
	/*
	AVOutputFormat *outfmt;
  
	av_register_all();
	//printf("Tha to deis mwre\n");
	outfmt = av_guess_format(output_format, output_file, NULL);
	outctx = avformat_alloc_context();
	if (outctx == NULL) {
		printf("Tha to deis mwre\n");
		return;
	}
	//printf("Tha to deis mwre\n");
	outctx->oformat = outfmt;*/
}

static void audio_header_fill(uint8_t *data, AVStream *st)
{
	audio_payload_header_write(data, libav_codec_id(st->codec->codec_id), st->codec->channels, st->codec->sample_rate, st->codec->frame_size);
}

static void video_header_fill(uint8_t *data, AVStream *st)
{
	int num, den;

	num = st->avg_frame_rate.num;
	den = st->avg_frame_rate.den;
	//fprintf(stderr, "Rate: %d/%d\n", num, den);
	if (num == 0) {
		num = st->r_frame_rate.num;
		den = st->r_frame_rate.den;
	}
	if (num > (1 << 16)) {
		num /= 1000;
		den /= 1000;
	}
	video_payload_header_write(data, libav_codec_id(st->codec->codec_id), st->codec->width, st->codec->height, num, den);
}

static void frame_header_fill(uint8_t *data, int size, AVPacket *pkt, AVStream *st, AVRational new_tb, int64_t base_ts)
{
	int32_t pts, dts;

	if (pkt->pts != AV_NOPTS_VALUE) {
		pts = av_rescale_q(pkt->pts, st->time_base, new_tb),
		pts += av_rescale_q(base_ts, AV_TIME_BASE_Q, new_tb);
	} else {
		pts = -1;
	}
	//dprintf("pkt->pts=%ld PTS=%d",pkt->pts, pts);
	if (pkt->dts != AV_NOPTS_VALUE) {
		dts = av_rescale_q(pkt->dts, st->time_base, new_tb);
		dts += av_rescale_q(base_ts, AV_TIME_BASE_Q, new_tb);
	} else {
		fprintf(stderr, "No DTS???\n");
		dts = 0;
	}
	//dprintf(" DTS=%d\n",dts);
	frame_header_write(data, size, pts, dts);
}

static enum CodecID libav_codec_id(uint8_t mytype)
{
  switch (mytype) {
    case 1:
      return CODEC_ID_MPEG2VIDEO;
    case 2:
      return CODEC_ID_H261;
    case 3:
      return CODEC_ID_H263P;
    case 4:
      return CODEC_ID_MJPEG;
    case 5:
      return CODEC_ID_MPEG4;
    case 6:
      return CODEC_ID_FLV1;
    case 7:
      return CODEC_ID_SVQ3;
    case 8:
      return CODEC_ID_DVVIDEO;
    case 9:
      return CODEC_ID_H264;
    case 10:
      return CODEC_ID_THEORA;
    case 11:
      return CODEC_ID_SNOW;
    case 12:
      return CODEC_ID_VP6;
    case 13:
      return CODEC_ID_DIRAC;
    default:
      fprintf(stderr, "Unknown codec %d\n", mytype);
      return (CodecID)0;
  }
}