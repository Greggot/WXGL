#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/splitter.h>

#include <Assembly/Core.hpp>
#include <Assembly/Loader.hpp>
#include <Assembly/Viewer.hpp>
#include <Assembly/DependencyTree.hpp>

#include <Dialog/API.Connect.hpp>
#include <Common/PaddingSizer.hpp>

#include <thread>

class MainFrame : public wxFrame
{
public:
    MainFrame();

    void StartUpdateThread(const int fps);
    void FinishUpdateThread();

    void SizerInit();
    void MenuBarInit();
private:
    wxButton* connector;

    SkyBlue::APIPanel* apipanel;

    Assembly::Viewer* Viewer;
    Assembly::Loader* Loader;
    Assembly::DependencyTree* Tree;

    wxSplitterWindow* splitter;
    PaddingSizer* sizer;
    
    std::thread Update;
    bool isUpdating;

    void CloseEvent(wxCloseEvent& event);
};