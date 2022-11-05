#pragma once
#pragma warning(disable : 4996)

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/glcanvas.h>
#include <gl/GLU.h>
#include <gl/GL.h>

#include "DependencyTree.hpp"

#include <thread>
#include <Context/Model.hpp>

#include <UserInput/Operator.hpp>
#include <UserInput/KeyBindController.hpp>
#include <Common/MovablePanel.hpp>

#include <SkyBlue/Client.API.hpp>

namespace Assembly
{

    class Viewer : public wxGLCanvas
    {
    private:
        std::unique_ptr<wxGLContext> context;
        UserInput::Operator camera;
        UserInput::KeyBindController keybinds;
        DependencyTree& Tree;
        SkyBlue::Device& device;

        inline void PrepareRender();
        inline void FinishRender();
        
        void IsometricCameraRotation();
        void DrawAxis();
        
        void InitScene(wxPaintEvent&);
        void Render(wxPaintEvent&);
        void RightClickOnModel(wxMouseEvent&);

        void KeyBindingsInit();
        std::function<void(wxKeyEvent&)> ModelChange(std::function<void(DrawableModel*)>);
    public:
        Viewer(wxWindow* parent, DependencyTree& Tree, SkyBlue::Device& device);

        ~Viewer();
    protected:
        DECLARE_EVENT_TABLE()
    };
}