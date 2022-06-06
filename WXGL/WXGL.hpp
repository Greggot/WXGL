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
#include "OBJparser.hpp"

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



class ModelViewer : public wxGLCanvas
{
    private:
        bool isUpdating = true;
        wxGLContext* m_context;
        std::thread Update;

        std::vector<OBJ::Model*> Assembly;
        size_t ModelAmount;
        int ActiveIndex;

        void Render(wxPaintEvent& event);
        void Close(wxCloseEvent& event);

        void GeneralCommands(wxKeyEvent& event);
    public:
        ModelViewer(wxFrame* parent);

        void Append(OBJ::Model model);
    protected:
        void SwitchActive();
        DECLARE_EVENT_TABLE()
};