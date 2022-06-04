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
    Viewer = new ModelViewer(this);
    ModelLoader* Loader = new ModelLoader(this, *Viewer);

    wxMenuBar* MenuBar = new wxMenuBar();
    MenuBar->Append(Loader, wxString("File"));
    SetMenuBar(MenuBar);
}