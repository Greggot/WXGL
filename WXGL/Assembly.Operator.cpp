#include "Assembly.Operator.hpp"
#include <cmath>
using namespace Assembly;

Buttons ActiveOne;
MouseMovement ButtonMovements[Buttons::Amount];
int Operator::cameraangle[OrtAmount] = {0,0,0};
float Operator::scale = 1;

void Operator::Zoom(wxMouseEvent& event)
{
    static const GLfloat downscale = 0.9f;
    static const GLfloat upscale = 1.1f;
    float overallscale = 0;

    if (event.GetWheelRotation() < 0)
        overallscale = downscale;
    else
        overallscale = upscale;
    
    scale *= overallscale;
    glScalef(overallscale, overallscale, overallscale);
}

void Operator::StartRotateXY(wxMouseEvent& event)
{
    ActiveOne = Buttons::Left;
    ButtonMovements[Buttons::Left].before = event.GetPosition();
}
void Operator::StartRotateZ(wxMouseEvent& event)
{
    ActiveOne = Buttons::Middle;
    ButtonMovements[Buttons::Middle].before = event.GetPosition();
}

#define PI 3.14159265
static inline void ModifyRotationOrts(int angleZ, int complimentaryAngle, rotateVector& Ort)
{
    float angle = complimentaryAngle % 360;
    if (angle < 0)
        angle = -angle;
    bool sign = angle > 90 && angle < 270;

    Ort.angle = sign ? angleStep : -angleStep;
    Ort.x = sin((float)angleZ * PI / 180);
    Ort.y = cos((float)angleZ * PI / 180);
    Ort.z = 0;
}

inline rotateVector Operator::GetModifiedRotationVector(Ort ort)
{
    static rotateVector RotationMatrices[] = { {-angleStep, 1,0,0},
                                               {-angleStep, 0,1,0},
                                               {-angleStep, 0,0,1} };
    ModifyRotationOrts(cameraangle[Z], cameraangle[Y], RotationMatrices[X]);
    ModifyRotationOrts(cameraangle[Z] + 90, cameraangle[X], RotationMatrices[Y]);
    
    return RotationMatrices[ort];
}

void Operator::RotateCamera(int value, Ort ort)
{   
    rotateVector vector = GetModifiedRotationVector(ort);
    cameraangle[ort] = value;

    glRotatef(value, vector.x, vector.y, vector.z);
}

inline void Operator::RotateCameraQuant(Sign sign, Ort ort)
{
    rotateVector vector = GetModifiedRotationVector(ort);
    cameraangle[ort] += vector.angle;

    if (sign)
        vector.angle = -vector.angle;
    glRotatef(vector.angle, vector.x, vector.y, vector.z);
}

inline void Operator::ApplyMovementTo(const int end, const int start, const Ort ort)
{
    if (end - start > rotateAmplitude)
        RotateCameraQuant(positive, ort);
    else if (end - start < -rotateAmplitude)
        RotateCameraQuant(negative, ort);
}

void Operator::Rotate(wxMouseEvent& event)
{
    static rotateVector vector;
    if (event.Dragging())
    {
        MouseMovement& mov = ButtonMovements[ActiveOne];
        mov.after = event.GetPosition();

        if (ActiveOne == Buttons::Middle)
            ApplyMovementTo(mov.after.x, mov.before.x, Z);
        else
        {
            ApplyMovementTo(mov.after.x, mov.before.x, X);
            ApplyMovementTo(mov.after.y, mov.before.y, Y);
        }
        mov.before = mov.after;
    }
}
std::map < wxKeyCode, std::function<void(wxKeyEvent&)>> KeyEvents;
void Operator::Move(wxKeyEvent& event)
{
    auto evt = KeyEvents.find((wxKeyCode)event.GetKeyCode());
    if (evt != KeyEvents.end())
        evt->second(event);
}

void Operator::AppendKeyEvent(wxKeyCode key, std::function<void(wxKeyEvent&)> call)
{
    auto evt = KeyEvents.find(key);
    if (evt == KeyEvents.end())
        KeyEvents.insert({ key, call });
    else
        evt->second = call;
}
