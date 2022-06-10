#include "Assembly.Operator.hpp"
#include <cmath>
using namespace Assembly;

Buttons ActiveOne;
MouseMovement ButtonMovements[Buttons::Amount];

int Operator::cameraangle[3] = {0,0,0};

OBJ::Model* Operator::Model;
vertex* Rotation;
vertex* Transform;

void Operator::Init(OBJ::Model* Model)
{
    if (Operator::Model)
        Operator::Model->Active = false;
    Model->Active = true;
    
    Operator::Model = Model;
    Rotation = Model->getRotationVector();
    Transform = Model->getTransformVector();
}

void Operator::Zoom(wxMouseEvent& event)
{
    static const GLfloat downscale = 0.9;
    static const GLfloat upscale = 1.1;
    int rotation = event.GetWheelRotation();

    if (rotation < 0)
        glScalef(downscale, downscale, downscale);
    else
        glScalef(upscale, upscale, upscale);
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

void Operator::RightClickOnModel(wxMouseEvent& event)
{
    Configurator* config = new Configurator("Name.obj");
    PopupMenu(config, event.GetPosition());
}

#define PI 3.14159265
#define angleStep 2
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

inline void Operator::RotateCamera(Sign sign, Ort ort)
{
    static rotateVector RotationMatrices[] = { {-angleStep, 1,0,0},
                                               {-angleStep, 0,1,0},
                                               {-angleStep, 0,0,1} };
    ModifyRotationOrts(cameraangle[Z], cameraangle[Y], RotationMatrices[X]);
    ModifyRotationOrts(cameraangle[Z] + 90, cameraangle[X], RotationMatrices[Y]);

    rotateVector vector = RotationMatrices[ort];
    if (sign)
        vector.angle = -vector.angle;
    glRotatef(vector.angle, vector.x, vector.y, vector.z);
    cameraangle[ort] += vector.angle;
}

inline void Operator::ApplyMovementTo(const int end, const int start, const Ort ort)
{
    static const uint8_t amplitude = 5;
    if (end - start > amplitude)
        RotateCamera(positive, ort);
    else if (end - start < -amplitude)
        RotateCamera(negative, ort);
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
    int key = event.GetKeyCode();
    auto evt = KeyEvents.find((wxKeyCode)key);
    if (evt != KeyEvents.end())
        evt->second(event);

    switch (key)
    {
    case WXK_LEFT:
        --Transform->x;
        break;
    case WXK_RIGHT:
        ++Transform->x;
        break;
    case WXK_UP:
        ++Transform->y;
        break;
    case WXK_DOWN:
        --Transform->y;
        break;
    case WXK_SPACE:
        ++Transform->z;
        break;
    case WXK_CONTROL:
        --Transform->z;
        break;
    case WXK_HOME:
        ++Rotation->z;
        break;
    case WXK_END:
        --Rotation->z;
        break;
    case WXK_PAGEUP:
        ++Rotation->x;
        break;
    case WXK_PAGEDOWN:
        --Rotation->x;
        break;
    case WXK_NUMPAD4:
        ++Rotation->y;
        break;
    case WXK_NUMPAD6:
        --Rotation->y;
        break;
    }
}

void Operator::AppendKeyEvent(wxKeyCode key, std::function<void(wxKeyEvent&)> call)
{
    auto evt = KeyEvents.find(key);
    if (evt == KeyEvents.end())
        KeyEvents.insert({ key, call });
    else
        evt->second = call;
}
