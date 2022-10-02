#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Assembly/Core.hpp>
#include <Assembly/Loader.hpp>
#include <Assembly/Viewer.hpp>

#include <thread>

class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    Assembly::Core core;
    Assembly::Viewer* Viewer;
    Assembly::Loader* Loader;
    
    std::thread Update;
    bool isUpdating;

    void CloseEvent(wxCloseEvent& event);
};