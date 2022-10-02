#pragma once
#include "Viewer.hpp"

#include <map>
#define angleStep 2
#define rotateAmplitude 5

enum Buttons : uint8_t
{
    Left,
    Middle,

    Amount,
};
#define OrtAmount 3
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

    class Operator : public wxGLCanvas
    {
    private:
        static int cameraangle[OrtAmount];
        static float scale;

        inline void ApplyMovementTo(const int end, const int start, const Ort ort);
        static inline void RotateCameraQuant(Sign sign, Ort ort);
        static inline rotateVector GetModifiedRotationVector(Ort ort);
    public:
        static void RotateCamera(int value, Ort ort);
        static float getScale() { return scale; };

        void Zoom(wxMouseEvent& event);
        void StartRotateXY(wxMouseEvent& event);
        void StartRotateZ(wxMouseEvent& event);
        void Rotate(wxMouseEvent& event);

        void Move(wxKeyEvent& event);
        static void AppendKeyEvent(wxKeyCode key, std::function<void(wxKeyEvent&)> call);
    };

}