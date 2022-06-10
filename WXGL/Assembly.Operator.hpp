#pragma once
#include "Assembly.Viewer.hpp"
#include <map>

enum Buttons : uint8_t
{
    Left,
    Middle,

    Amount,
};
enum Ort : uint8_t
{
    X,
    Y,
    Z,
};
enum Sign
{
    positive,
    negative,
};

namespace Assembly
{

    class Operator : public Viewer
    {
        static int cameraangle[3];
        static OBJ::Model* Model;
        static void Init(OBJ::Model* Model);

        void Zoom(wxMouseEvent& event);
        void StartRotateXY(wxMouseEvent& event);
        void StartRotateZ(wxMouseEvent& event);
        void Rotate(wxMouseEvent& event);

        void Move(wxKeyEvent& event);
        static void AppendKeyEvent(wxKeyCode key, std::function<void(wxKeyEvent&)> call);
        
        void RightClickOnModel(wxMouseEvent& event);
        inline void RotateCamera(Sign sign, Ort ort);
        inline void ApplyMovementTo(const int end, const int start, const Ort ort);

        friend class Viewer;
    };

}