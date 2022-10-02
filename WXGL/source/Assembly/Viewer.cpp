#include <Assembly/Viewer.hpp>
using namespace Assembly;
using UserInput::Operator;

BEGIN_EVENT_TABLE(Viewer, wxGLCanvas)
EVT_PAINT(Viewer::Render)

EVT_RIGHT_DOWN(Viewer::RightClickOnModel)

END_EVENT_TABLE()

std::function<void(wxKeyEvent&)> Viewer::ModelChange(std::function<void(BaseModel*)> call)
{
    return [this, call](wxKeyEvent&) {
        auto model = core.active();
        if (model == nullptr)
            return;
        call(model);
    };
}

void Viewer::KeyBindingsInit()
{
    keybinds.append(WXK_RETURN, { [this](wxKeyEvent&) {
        size_t size = core.size();
        if (size == 0)
            return;

        size_t index = core.activeindex();
        if (++index >= size)
            index = 0;
        core.setActive(index);
    } });
    keybinds.append(WXK_END, ModelChange([](BaseModel* model) { ++model->Rotation.z; }));
    keybinds.append(WXK_HOME, ModelChange([](BaseModel* model) { --model->Rotation.z; }));
    keybinds.append(WXK_PAGEUP, ModelChange([](BaseModel* model) { ++model->Rotation.y; }));
    keybinds.append(WXK_PAGEDOWN, ModelChange([](BaseModel* model) { --model->Rotation.y; }));
    keybinds.append(WXK_NUMPAD4, ModelChange([](BaseModel* model) { ++model->Rotation.x; }));
    keybinds.append(WXK_NUMPAD6, ModelChange([](BaseModel* model) { --model->Rotation.x; }));

    keybinds.append(WXK_SPACE, ModelChange([](BaseModel* model) { ++model->Translation.z; }));
    keybinds.append(WXK_CONTROL, ModelChange([](BaseModel* model) { --model->Translation.z; }));
    keybinds.append((wxKeyCode)'W', ModelChange([](BaseModel* model) { ++model->Translation.x; }));
    keybinds.append((wxKeyCode)'A', ModelChange([](BaseModel* model) { ++model->Translation.y; }));
    keybinds.append((wxKeyCode)'S', ModelChange([](BaseModel* model) { --model->Translation.x; }));
    keybinds.append((wxKeyCode)'D', ModelChange([](BaseModel* model) { --model->Translation.y; }));
}

Viewer::Viewer(wxFrame* parent, Core& core)
    :wxGLCanvas(parent, wxID_ANY, 0, wxPoint(60, 10), wxSize(400, 300), 0, wxT("GLCanvas")),
    core(core), context(new wxGLContext(this)), camera(this), keybinds(this)
{
    KeyBindingsInit();
}

inline void Viewer::GLSceneInit()
{
    SetCurrent(*context.get());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(1.0, 1.0, 1.0, 1.0);
}
inline void Viewer::GLSceneRender()
{
    glFlush();
    SwapBuffers();
}

void Viewer::DrawAxis()
{
    static const vertex ZERO = { 0, 0, 0 };
    static vertex Axises[3] = {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 }
    };

    float scale = camera.getScale() * 4;

    glLineWidth(3);
    glBegin(GL_LINES);
    for (auto axis : Axises)
    {
        glColor3f(axis.x, axis.y, axis.z);
        glVertex3f(ZERO.x, ZERO.y, ZERO.z);
        glVertex3f(axis.x / scale, axis.y / scale, axis.z / scale);
    }
    glEnd();

}

void Viewer::IsometricCameraRotation()
{
    using UserInput::Ort;
    camera.RotateCamera(Ort::Y, -135);
    camera.RotateCamera(Ort::Z, 45);
}

void Viewer::Render(wxPaintEvent& event)
{
    wxPaintDC(this);
    GLSceneInit();
    static bool init = true;
    if (init)
    {
        IsometricCameraRotation();
        init = false;
    }

    for (auto model : core)
        model->Draw();

    glDisable(GL_DEPTH_TEST);
    DrawAxis();

    GLSceneRender();
}

void Viewer::RightClickOnModel(wxMouseEvent& event)
{
    wxClientDC(this);
    GLSceneInit();

    uint32_t ID = 0;
    for (auto model : core)
        model->DrawSelectionMode(ID++);
    
    wxPoint pixel = event.GetPosition();
    ID = BaseModel::GetColorSelection(pixel.x, pixel.y);
    if (ID > core.size())
        return; // Add here scene general settings later maybe

    Configurator config(ID, core);
    PopupMenu(&config, event.GetPosition());
}

Viewer::~Viewer()
{
    core.clear();
}