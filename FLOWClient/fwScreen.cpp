//
//  fwScreen.cpp
//  FLOW
//
//  Created by Nick Vitsas on 9/13/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#include "fwScreen.h"
#include "fwInputHandler.h"

FLOWScreen::FLOWScreen(wxFrame* parent) :
wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    // load the file... ideally add a check to see if loading was successful
    /*m_BackData = (unsigned char *)malloc(200*200*3);
    for(int i = 0; i < (200*200*3); i++) {
        if(((i+1)%3)==0)
            m_BackData[i] = 255;
        else
            m_BackData[i] = 0;
    }
    m_FrontData = (unsigned char *)malloc(200*200*3);
    for(int i = 0; i < (200*200*3); i++) {
        if(((i+1)%3)==0)
            m_FrontData[i] = 255;
        else
            m_FrontData[i] = 0;
    }*/
    m_BackData = NULL;
    m_BackBuffer = new wxImage(1,1,false);
    m_FrontBuffer = new wxImage(1,1,false);;
    
    //back->Create(200,200);
    /*wxAlphaPixelData bmdata(*back);
    wxAlphaPixelData::Iterator dst(bmdata);
    printf("Mpla Mpla\n");
    for(int y=0; y<200; y++) {
        dst.MoveTo(bmdata, 0, y);
        for(int x=0; x<200; x++) {
            // wxBitmap contains rgb values pre-multiplied with alpha
            //dst.Red()=0;
            //dst.Green()=0;
            //dst.Blue()=100;
            //dst.Alpha()=0;
            dst++;
        }
    }*/
}

FLOWScreen::~FLOWScreen() {
    if(m_BackBuffer)
        delete m_BackBuffer;
    if(m_FrontBuffer) {
        delete m_BackData;
        delete m_FrontBuffer;
    }
}

void FLOWScreen::SwapImage(AVFrame* fr, InputHandler* inh) {
    inh->sws_ctx = sws_getCachedContext(inh->sws_ctx,
              inh->video_dec_ctx->width, inh->video_dec_ctx->height,
              inh->video_dec_ctx->pix_fmt,
              this->GetSize().x, this->GetSize().y,
              PIX_FMT_RGB24, SWS_FAST_BILINEAR,
              NULL, NULL, NULL);

    //avcodec_get_frame_defaults(inh->frameRGB);

    //int m_NumBytes=avpicture_get_size(PIX_FMT_RGB24, this->GetSize().x,
    //this->GetSize().y);
    //BYTE *m_PictureBuffer=(BYTE *)av_malloc(m_NumBytes*sizeof(BYTE));
    //avpicture_fill((AVPicture *)inh->frameRGB, m_PictureBuffer, PIX_FMT_RGB24, this->GetSize().x, this->GetSize().y);
    
    sws_scale
    (
        inh->sws_ctx,
        (uint8_t const * const *)fr->data,
        fr->linesize,
        0,
        inh->video_dec_ctx->height,
        inh->frameRGB->data,
        inh->frameRGB->linesize
    );
    //printf("LUDAS\n");
    BYTE *tmp_ptr = m_BackData;
    m_BackData = new BYTE[this->GetSize().x*this->GetSize().y*3];
    
    for(int y=0; y<this->GetSize().y; y++) {
        memcpy(m_BackData+y*this->GetSize().x*3, 
            inh->frameRGB->data[0]+y*inh->frameRGB->linesize[0], 
            this->GetSize().x*3);
    }
    //printf("Size is %d\n", this->GetSize().x*this->GetSize().y*3);
    wxImage* tmp;
    //m_BackBuffer->SetData(m_BackData);
    m_BackBuffer->Create(this->GetSize(), m_BackData, true);

    delete tmp_ptr;

    tmp = m_BackBuffer;
    m_BackBuffer = m_FrontBuffer;
    m_FrontBuffer = tmp;
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void FLOWScreen::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void FLOWScreen::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void FLOWScreen::render(wxDC&  dc)
{
    //dc.DrawText(wxT("AUEB"),0,0);
    //m_FrontBuffer->Rescale(this->GetSize().x, this->GetSize().y);
    double start = av_gettime() / 1000000.0;
      
      
    wxBitmap bmp(*m_FrontBuffer);
    dc.DrawBitmap( bmp, 0, 0 );
    double end = av_gettime() / 1000000.0;
    double luda = end - start;
}

BEGIN_EVENT_TABLE(FLOWScreen, wxPanel)
// some useful events
/*
 EVT_MOTION(wxImagePanel::mouseMoved)
 EVT_LEFT_DOWN(wxImagePanel::mouseDown)
 EVT_LEFT_UP(wxImagePanel::mouseReleased)
 EVT_RIGHT_DOWN(wxImagePanel::rightClick)
 EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow)
 EVT_KEY_DOWN(wxImagePanel::keyPressed)
 EVT_KEY_UP(wxImagePanel::keyReleased)
 EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
 */

// catch paint events
EVT_PAINT(FLOWScreen::paintEvent)

END_EVENT_TABLE()


// some useful events
/*
 void wxImagePanel::mouseMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseDown(wxMouseEvent& event) {}
 void wxImagePanel::mouseWheelMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseReleased(wxMouseEvent& event) {}
 void wxImagePanel::rightClick(wxMouseEvent& event) {}
 void wxImagePanel::mouseLeftWindow(wxMouseEvent& event) {}
 void wxImagePanel::keyPressed(wxKeyEvent& event) {}
 void wxImagePanel::keyReleased(wxKeyEvent& event) {}
 */

