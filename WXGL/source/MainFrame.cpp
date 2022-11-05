#include <MainFrame.hpp>

class MyApp : public wxApp {
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
    
    splitter = new wxSplitterWindow(this, wxID_ANY);
    Tree = new Assembly::DependencyTree(splitter);
    apipanel = new SkyBlue::APIPanel(this, *Tree);
    Viewer = new Assembly::Viewer(splitter, *Tree, *apipanel);

    connector = new wxButton(this, wxID_ANY, "Connect...", wxPoint(20, 10), wxSize(100, 25));
    
    SizerInit();
    MenuBarInit();
    StartUpdateThread(60);

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::CloseEvent, this);
    // TODO: Add several variants of connection
    connector->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        Dialog::APIconnect* apiconnect = new Dialog::APIconnect(this, "Server connect", apipanel, sizer);
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
            Viewer->Refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
        }
    });
}

void MainFrame::FinishUpdateThread()
{
    isUpdating = false;
    Update.join();
}

void MainFrame::SizerInit()
{
    /* [Connect  button] [API panel with modules - EXPANDABLE]
    *  [Dependency tree] [Model/Assembly Viewer  - EXPANDABLE]  - SPLITTER
    *         |                                  |
    *     EXPANDABLE                         EXPANDABLE
    */        
    auto uppersizer = new PaddingSizer(10, { connector, apipanel });

    splitter->SplitVertically(Tree, Viewer, 150);
    
    sizer = new PaddingSizer(10, { uppersizer }, wxVERTICAL);
    sizer->AddStretched(splitter);
    SetSizer(sizer);
}

void MainFrame::MenuBarInit()
{
    Loader = new Assembly::Loader(this, *Tree);

    wxMenuBar* MenuBar = new wxMenuBar();
    MenuBar->Append(Loader, "File");
    SetMenuBar(MenuBar);
}

void MainFrame::CloseEvent(wxCloseEvent& event)
{
    FinishUpdateThread();
    Destroy();
}
