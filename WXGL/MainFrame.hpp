#pragma once

#include "resource.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Assembly.Loader.hpp"

class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    Assembly::Viewer* Viewer;
    Assembly::Loader* Loader;
    
    std::thread Update;
    bool isUpdating;

    void CloseEvent(wxCloseEvent& event);
};