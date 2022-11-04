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
    Viewer = new Assembly::Viewer(splitter, core, api);
    Tree = new Assembly::DependencyTree(splitter, core);

    apipanel = new SkyBlue::APIPanel(this, core);
    connectPanel = new wxPanel(this);
    connector = new wxButton(connectPanel, wxID_ANY, "Connect...", wxPoint(20, 10), wxSize(100, 25));
    
    SizerInit();
    MenuBarInit();
    StartUpdateThread(60);

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::CloseEvent, this);
    // TODO: Add several variants of connection
    connector->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        Dialog::APIconnect* apiconnect = new Dialog::APIconnect(this, "Server connect", api, apipanel, sizer);
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
    splitter->SplitVertically(Tree, Viewer, 150);

    auto uppersizer = new wxBoxSizer(wxHORIZONTAL);
    uppersizer->AddSpacer(10);
    uppersizer->Add(connectPanel);
    uppersizer->Add(apipanel, 1, wxEXPAND);
    uppersizer->AddSpacer(10);

    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddSpacer(10);
    sizer->Add(uppersizer);
    sizer->AddSpacer(10);
    sizer->Add(splitter, 1, wxEXPAND);
    sizer->AddSpacer(10);

    SetSizer(sizer);
}

void MainFrame::MenuBarInit()
{
    Loader = new Assembly::Loader(this, core, *Tree);

    wxMenuBar* MenuBar = new wxMenuBar();
    MenuBar->Append(Loader, "File");
    SetMenuBar(MenuBar);
}

void MainFrame::CloseEvent(wxCloseEvent& event)
{
    FinishUpdateThread();
    Destroy();
}
