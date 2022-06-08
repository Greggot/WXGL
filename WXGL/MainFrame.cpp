#include "MainFrame.hpp"

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
    SetBackgroundColour(wxColor(0xFF, 0xFF, 0xFF));
    isUpdating = true;
    Update = std::thread ([this] {
        while (isUpdating)
        {
            Refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    });
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::CloseEvent, this);
    Viewer = new Assembly::Viewer(this);
    Loader = new Assembly::Loader(this, *Viewer);

    wxMenuBar* MenuBar = new wxMenuBar();
    MenuBar->Append(Loader, wxString("File"));
    SetMenuBar(MenuBar);
}

void MainFrame::CloseEvent(wxCloseEvent& event)
{
    isUpdating = false;
    Update.join();
    Destroy();
}
