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
    
    Viewer = new Assembly::Viewer(this, core);

    PhysicalDevice::RGB565* data = new PhysicalDevice::RGB565[320 * 240];
    FILE* f = fopen("C:/Users/007/Desktop/photos/2.jpg", "rb");
    fread(data, sizeof(PhysicalDevice::RGB565), 320 * 240, f);
    fclose(f);

    auto image = new ImagePanel(nullptr, { 0x42, 0x87,0xF5 }, "CAM 0", 320, 240);
    image->Show();
    for (int row = 0; row < 240; ++row)
    {
        for (int column = 0; column < 320; ++column)
        {
            const PhysicalDevice::RGB565 color = data[row * 320 + column];
            image->Set(column, row, color.red8(), color.green8(), color.blue8());
        }
    }
    image->Update();
    delete[] data;

    Tree = new Assembly::DependencyTree(this, core);
    apipanel = new SkyBlue::APIPanel(this);
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
    *  [Dependency tree] [Model/Assembly Viewer  - EXPANDABLE]
    *         |                                  |
    *     EXPANDABLE                         EXPANDABLE
    */    
    sizer = new wxFlexGridSizer(2,2, 10,10);
    sizer->Add(connectPanel);
    sizer->Add(apipanel);
    sizer->Add(Tree);
    sizer->Add(Viewer, 1, wxEXPAND);
    
    sizer->AddGrowableRow(1);
    sizer->AddGrowableCol(1);
    
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
