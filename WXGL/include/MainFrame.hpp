#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Assembly/Core.hpp>
#include <Assembly/Loader.hpp>
#include <Assembly/Viewer.hpp>

#include <Common/Dialog.APIconnect.hpp>

#include <thread>

class MainFrame : public wxFrame
{
public:
    MainFrame();

    void StartUpdateThread(const int fps);
    void FinishUpdateThread();
private:
    SkyBlue::TCPclientAPI api;

    Assembly::Core core;
    Assembly::Viewer* Viewer;
    Assembly::Loader* Loader;
    
    std::thread Update;
    bool isUpdating;

    void CloseEvent(wxCloseEvent& event);
};