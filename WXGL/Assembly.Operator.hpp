#pragma once
#include "Assembly.Viewer.hpp"
#include <map>

enum Buttons : uint8_t
{
    Left,
    Right,
    Middle,

    Amount,
};

namespace Assembly
{

    class Operator : public Viewer
    {
        static OBJ::Model* Model;
        static void Init(OBJ::Model* Model);

        void Zoom(wxMouseEvent& event);
        void StartRotateX(wxMouseEvent& event);
        void StartRotateY(wxMouseEvent& event);
        void StartRotateZ(wxMouseEvent& event);
        void Rotate(wxMouseEvent& event);

        void Move(wxKeyEvent& event);
        static void AppendKeyEvent(wxKeyCode key, std::function<void(wxKeyEvent&)> call);

        friend class Viewer;
    };

}