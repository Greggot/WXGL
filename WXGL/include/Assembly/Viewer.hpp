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

namespace Assembly
{

    class Viewer : public wxGLCanvas
    {
    private:
        std::unique_ptr<wxGLContext> context;
        Core& core;

        inline void GLSceneInit();
        void Render(wxPaintEvent& event);
        inline void GLSceneRender();
        
        void DrawAxis();
        
        void RightClickOnModel(wxMouseEvent& event);
        void KeyBindingsInit();
        std::function<void(wxKeyEvent&)> ModelChange(std::function<void(BaseModel*)>);
    public:
        Viewer(wxFrame* parent, Core& core);

        ~Viewer();
    protected:
        DECLARE_EVENT_TABLE()
    };
}