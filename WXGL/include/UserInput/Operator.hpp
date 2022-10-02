#pragma once
#include <wx/glcanvas.h>
#include <gl/GLU.h>
#include <gl/GL.h>

#include <cmath>
#include <map>
#include <stdexcept>

#include <vertex.h>

namespace UserInput
{
    struct MouseMovement
    {
        wxPoint before;
        wxPoint after;
    };
    struct rotateVector
    {
        GLfloat angle;
        vertex direction;
    };

    enum class Buttons : uint8_t
    {
        Left,
        Middle,

        Amount,
    };
    enum Ort : uint8_t {X, Y, Z,  Amount};
    enum class Sign
    {
        positive,
        negative,
    };

    // TODO - Add to global settings 
    static const int angleStep = 2;
    static const int rotateAmplitude = 5;

    class Operator 
    {
    private:
        Buttons ActiveOne;
        int angle[Ort::Amount] {0, 0, 0};
        MouseMovement ButtonMovements[static_cast<int>(Buttons::Amount)];

        // TODO - Add to global settings 
        const GLfloat downscale = 0.9f;
        const GLfloat upscale = 1.1f;
        float scale = 1;

        // TODO - add camrea movement along X,Y,Z

        inline void ApplyMovementTo(const int end, const int start, const Ort&&);
        inline void RotateCameraQuant(Sign sign, Ort ort);
        inline rotateVector GetModifiedRotationVector(Ort ort);

        void ApplyZoom(wxGLCanvas*);
        void ApplyStartRotate(wxGLCanvas*);
        void ApplyFinishRotate(wxGLCanvas*);
    public:
        Operator(wxGLCanvas* canvas);

        void RotateCamera(const Ort&&, int value);
        float getScale() { return scale; };
    };

}