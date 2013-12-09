// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".

#include "fwInterface.h"

class PRIMEClient : public wxApp
{
	MainFrame *m_frame;

public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(PRIMEClient);

bool PRIMEClient::OnInit()
{
#ifdef _DEBUG
    Utilities::SetStdOutToNewConsole();
#endif	
    avformat_network_init();
    if(wxSocketBase::Initialize()){
        printf("All Good\n");
    } else {
        printf("All Bad\n");
    }

    m_frame = new MainFrame( (wxFrame *)NULL );
    m_frame->SetIcon(wxICON(FLOW));
    m_frame->Show( true );

    return true;
}

//void MainFrame::OnExit(wxCommandEvent& event)
//{
//    Close( true );
//}

/*
class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_Hello = 1
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Hello,   MyFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
*/