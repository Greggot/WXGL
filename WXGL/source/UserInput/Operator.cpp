#include <UserInput/Operator.hpp>
using namespace UserInput;

Operator::Operator(wxGLCanvas* canvas)
{
    if (canvas == nullptr)
        throw std::invalid_argument("Canvas doesn\'t exist");

    ApplyZoom(canvas);
    ApplyStartRotate(canvas);
    ApplyFinishRotate(canvas);
}

void Operator::ApplyZoom(wxGLCanvas* canvas)
{
    canvas->Bind(wxEVT_MOUSEWHEEL, [this](wxMouseEvent& event) {
        float overallscale = 0;

        if (event.GetWheelRotation() < 0)
            overallscale = downscale;
        else
            overallscale = upscale;

        scale *= overallscale;
        glScalef(overallscale, overallscale, overallscale);
    });
}

void Operator::ApplyStartRotate(wxGLCanvas* canvas)
{
    canvas->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent& event) {
        ActiveOne = Buttons::Left;
        ButtonMovements[static_cast<int>(Buttons::Left)].before = event.GetPosition();
    });

    canvas->Bind(wxEVT_MIDDLE_DOWN, [this](wxMouseEvent& event) {
        ActiveOne = Buttons::Middle;
        ButtonMovements[static_cast<int>(Buttons::Middle)].before = event.GetPosition();
    });
}

void Operator::ApplyFinishRotate(wxGLCanvas* canvas)
{
    canvas->Bind(wxEVT_MOTION, [this](wxMouseEvent& event) {
        static rotateVector vector;
        if (event.Dragging())
        {
            MouseMovement& mov = ButtonMovements[static_cast<int>(ActiveOne)];
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
    });
}

inline void ModifyRotationOrts(int angleZ, int complimentaryAngle, rotateVector& Ort)
{
    static const float PI = 3.14159265;
    float angle = complimentaryAngle % 360;
    if (angle < 0)
        angle = -angle;
    bool sign = angle > 90 && angle < 270;

    Ort.angle = sign ? angleStep : -angleStep;
    Ort.direction.x = sin((float)angleZ * PI / 180);
    Ort.direction.y = cos((float)angleZ * PI / 180);
    Ort.direction.z = 0;
}

inline rotateVector Operator::GetModifiedRotationVector(Ort ort)
{
    static rotateVector RotationMatrices[] = { {-angleStep, eX},
                                               {-angleStep, eY},
                                               {-angleStep, eZ} };
    ModifyRotationOrts(angle[Z], angle[Y], RotationMatrices[X]);
    ModifyRotationOrts(angle[Z] + 90, angle[X], RotationMatrices[Y]);
    
    return RotationMatrices[ort];
}

void Operator::RotateCamera(const Ort&& ort, int value)
{   
    if (ort == Amount)
        throw std::invalid_argument("Invalid ort");

    rotateVector vector = GetModifiedRotationVector(ort);
    angle[ort] = value;
    glRotatef(value, vector.direction.x, vector.direction.y, vector.direction.z);
}

inline void Operator::RotateCameraQuant(Sign s, Ort ort)
{
    rotateVector vector = GetModifiedRotationVector(ort);
    angle[ort] += vector.angle;

    if (s == Sign::negative)
        vector.angle = -vector.angle;
    glRotatef(vector.angle, vector.direction.x, vector.direction.y, vector.direction.z);
}

inline void Operator::ApplyMovementTo(const int end, const int start, const Ort&& ort)
{
    if (end - start > rotateAmplitude)
        RotateCameraQuant(Sign::positive, ort);
    else if (end - start < -rotateAmplitude)
        RotateCameraQuant(Sign::negative, ort);
}
