#pragma once
#include "OBJ.hpp"
#include "Assembly.Loader.hpp"
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
        static Loader* loader;
        static OBJ::Model* Model;
        static int cameraangle[OrtAmount];

        inline void RotateCamera(Sign sign, Ort ort);
        inline void ApplyMovementTo(const int end, const int start, const Ort ort);
    public:
        static void Init(Loader* loader);
        static void SetTarget(OBJ::Model* Model);
        
        void Zoom(wxMouseEvent& event);
        void StartRotateXY(wxMouseEvent& event);
        void StartRotateZ(wxMouseEvent& event);
        void Rotate(wxMouseEvent& event);

        void Move(wxKeyEvent& event);
        static void AppendKeyEvent(wxKeyCode key, std::function<void(wxKeyEvent&)> call);
    };

}