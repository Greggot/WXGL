#pragma once
#pragma warning(disable : 4996)

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/glcanvas.h>
#include <gl/GLU.h>
#include <gl/GL.h>

#include <thread>
#include "Core.hpp"
#include "Configurator.hpp"

#include <UserInput/Operator.hpp>
#include <UserInput/KeyBindController.hpp>

namespace Assembly
{

    class Viewer : public wxGLCanvas
    {
    private:
        std::unique_ptr<wxGLContext> context;
        UserInput::Operator camera;
        UserInput::KeyBindController keybinds;
        Core& core;

        inline void PrepareRender();
        inline void FinishRender();
        
        void IsometricCameraRotation();
        void DrawAxis();
        
        void InitScene(wxPaintEvent&);
        void Render(wxPaintEvent&);
        void RightClickOnModel(wxMouseEvent&);

        void KeyBindingsInit();
        std::function<void(wxKeyEvent&)> ModelChange(std::function<void(BaseModel*)>);
    public:
        Viewer(wxFrame* parent, Core& core);

        ~Viewer();
    protected:
        DECLARE_EVENT_TABLE()
    };
}