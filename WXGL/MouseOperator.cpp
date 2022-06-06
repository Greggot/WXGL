#include "MouseOperator.hpp"

Buttons ActiveOne;
MouseMovement ButtonMovements[Buttons::Amount];
const rotateVector RotationMatrices[Buttons::Amount] = { {2, 1, 0, 0}, {2, 0, 1, 0}, {-2, 0, 0, 1} };

OBJ::Model* MouseOperator::Model;
void MouseOperator::Init(OBJ::Model* _Model)
{
    Model = _Model; 
}

void MouseOperator::Zoom(wxMouseEvent& event)
{
    static const GLfloat downscale = 0.9;
    static const GLfloat upscale = 1.1;
    int rotation = event.GetWheelRotation();

    if (rotation < 0)
        glScalef(downscale, downscale, downscale);
    else
        glScalef(upscale, upscale, upscale);
}

void MouseOperator::StartRotateX(wxMouseEvent& event)
{
    ActiveOne = Buttons::Left;
    ButtonMovements[Buttons::Left].before = event.GetPosition();
}
void MouseOperator::StartRotateY(wxMouseEvent& event)
{
    ActiveOne = Buttons::Right;
    ButtonMovements[Buttons::Right].before = event.GetPosition();
}
void MouseOperator::StartRotateZ(wxMouseEvent& event)
{
    ActiveOne = Buttons::Middle;
    ButtonMovements[Buttons::Middle].before = event.GetPosition();
}

void MouseOperator::Rotate(wxMouseEvent& event)
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
void MouseOperator::Move(wxKeyEvent& event)
{
    int key = event.GetKeyCode();
    auto evt = KeyEvents.find((wxKeyCode)key);
    if (evt != KeyEvents.end())
        evt->second(event);
    switch (key)
    {
    case WXK_LEFT:
        --Model->Translation.x;
        break;
    case WXK_RIGHT:
        ++Model->Translation.x;
        break;
    case WXK_UP:
        ++Model->Translation.y;
        break;
    case WXK_DOWN:
        --Model->Translation.y;
        break;
    case WXK_SPACE:
        ++Model->Translation.z;
        break;
    case WXK_CONTROL:
        --Model->Translation.z;
        break;
    case WXK_HOME:
        ++Model->Rotation.z;
        break;
    case WXK_END:
        --Model->Rotation.z;
        break;
    }
}

void MouseOperator::AppendKeyEvent(wxKeyCode key, std::function<void(wxKeyEvent&)> call)
{
    auto evt = KeyEvents.find(key);
    if (evt == KeyEvents.end())
        KeyEvents.insert({ key, call });
    else
        evt->second = call;
}
