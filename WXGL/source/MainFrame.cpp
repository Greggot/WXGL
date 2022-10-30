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
    wxMenuBar* MenuBar = new wxMenuBar();
    MenuBar->Append(Loader, wxString("File"));
    SetMenuBar(MenuBar);

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::CloseEvent, this);
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
