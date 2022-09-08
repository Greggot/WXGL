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
#include "OBJ.hpp"
#include "Assemly.Configurator.hpp"
#include "Assembly.Core.hpp"

#ifndef WIN32
#include <unistd.h> // FIXME: This work/necessary in Windows?
//Not necessary, but if it was, it needs to be replaced by process.h AND io.h
#endif

struct MouseMovement
{
    wxPoint before;
    wxPoint after;
};
struct rotateVector
{
    GLfloat angle;
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

namespace Assembly
{

    class Viewer : public wxGLCanvas
    {
    private:
        wxGLContext* m_context;
        Core& core;

        inline void GLSceneInit();
        void Render(wxPaintEvent& event);
        inline void GLSceneRender();
        
        void DrawAxis();
        
        void RightClickOnModel(wxMouseEvent& event);
        void KeyBindingsInit();
    public:
        Viewer(wxFrame* parent, Core& core);

        ~Viewer();
    protected:
        DECLARE_EVENT_TABLE()
    };
}