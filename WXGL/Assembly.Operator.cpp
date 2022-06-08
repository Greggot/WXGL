#include "Assembly.Operator.hpp"
using namespace Assembly;

Buttons ActiveOne;
MouseMovement ButtonMovements[Buttons::Amount];
const rotateVector RotationMatrices[Buttons::Amount] = { {2, 1, 0, 0}, {2, 0, 1, 0}, {-2, 0, 0, 1} };

OBJ::Model* Operator::Model;
vertex* Rotation;
vertex* Transform;

void Operator::Init(OBJ::Model* _Model)
{
    Model = _Model;
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

void Operator::StartRotateX(wxMouseEvent& event)
{
    ActiveOne = Buttons::Left;
    ButtonMovements[Buttons::Left].before = event.GetPosition();
}
void Operator::StartRotateY(wxMouseEvent& event)
{
    ActiveOne = Buttons::Right;
    ButtonMovements[Buttons::Right].before = event.GetPosition();
}
void Operator::StartRotateZ(wxMouseEvent& event)
{
    ActiveOne = Buttons::Middle;
    ButtonMovements[Buttons::Middle].before = event.GetPosition();
}

void Operator::Rotate(wxMouseEvent& event)
{
    static rotateVector vector;
    if (event.Dragging())
    {
        MouseMovement& mov = ButtonMovements[ActiveOne];
        vector = RotationMatrices[ActiveOne];
        mov.after = event.GetPosition();

        if (mov.after.x - mov.before.x > 5)
            glRotatef(vector.angle, vector.x, vector.y, vector.z);
        else if (mov.after.x - mov.before.x < -5)
            glRotatef(-vector.angle, vector.x, vector.y, vector.z);

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
