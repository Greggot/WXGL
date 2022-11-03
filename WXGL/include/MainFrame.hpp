#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Assembly/Core.hpp>
#include <Assembly/Loader.hpp>
#include <Assembly/Viewer.hpp>
#include <Assembly/DependencyTree.hpp>

#include <Dialog/API.Connect.hpp>

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
    wxPanel* connectPanel;
    wxButton* connector;

    SkyBlue::Device api;
    SkyBlue::APIPanel* apipanel;

    Assembly::Core core;
    Assembly::Viewer* Viewer;
    Assembly::Loader* Loader;
    Assembly::DependencyTree* Tree;

    wxFlexGridSizer* sizer;
    
    std::thread Update;
    bool isUpdating;

    void CloseEvent(wxCloseEvent& event);
};