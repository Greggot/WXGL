#pragma once

#include "resource.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "ModelLoader.hpp"

class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    ModelViewer* Viewer;
};