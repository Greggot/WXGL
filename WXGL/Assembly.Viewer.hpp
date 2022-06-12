#pragma once
#pragma warning(disable : 4996)

#include "resource.h"
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

        std::vector<OBJ::Model*> Assembly;
        size_t ModelAmount;
        int ActiveIndex;

        void Render(wxPaintEvent& event);
        void RemoveLink(size_t index);
    public:
        Viewer(wxFrame* parent);

        void Append(OBJ::Model* model);
        void Remove(size_t index);
        void RemoveAll();

        ~Viewer();
    protected:
        void SwitchActive();
        DECLARE_EVENT_TABLE()
    };
}