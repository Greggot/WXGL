#include <MainFrame.hpp>

class MyApp : public wxApp
{
    virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MainFrame* Window = new MainFrame();
    Window->Show(true);
    return true;
}

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, wxT("Model View"), wxPoint(50, 50), wxSize(800, 600))
{
    SetBackgroundColour(wxColor(0xE5, 0xE5, 0xE5));
    StartUpdateThread(60);
    
    Viewer = new Assembly::Viewer(this, core);
    Loader = new Assembly::Loader(this, core);
    
    auto connectbutton = new wxButton(this, wxID_ANY, "Connect...", wxPoint(20, 10), wxSize(100, 25));
    wxFlexGridSizer* sizer = new wxFlexGridSizer(2, 2, 10, 10);
    sizer->Add(connectbutton);
    sizer->Add(Viewer, 1, wxEXPAND);
    // Make Viewer expand as window size grows
    sizer->AddGrowableRow(0);
    sizer->AddGrowableCol(1);
    SetSizer(sizer);

    wxMenuBar* MenuBar = new wxMenuBar();
    MenuBar->Append(Loader, wxString("File"));
    SetMenuBar(MenuBar);

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::CloseEvent, this);
   
    connectbutton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        Dialog::APIconnect* apiconnect = new Dialog::APIconnect(this, "Server connect", api);
        apiconnect->Show();
    });
}

void MainFrame::StartUpdateThread(const int fps)
{
    isUpdating = true;
    Update = std::thread([this, fps] {
        int delayms = 1000 / fps;
        while (isUpdating)
        {
            Refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
        }
    });
}

void MainFrame::FinishUpdateThread()
{
    isUpdating = false;
    Update.join();
}

void MainFrame::CloseEvent(wxCloseEvent& event)
{
    FinishUpdateThread();
    Destroy();
}
