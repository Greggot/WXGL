#include <Assembly/Viewer.hpp>
using namespace Assembly;
using UserInput::Operator;

BEGIN_EVENT_TABLE(Viewer, wxGLCanvas)
EVT_PAINT(Viewer::Render)

EVT_RIGHT_DOWN(Viewer::RightClickOnModel)

END_EVENT_TABLE()

std::function<void(wxKeyEvent&)> Viewer::ModelChange(std::function<void(DrawableModel*)> call)
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
    keybinds.append(WXK_END, ModelChange([](DrawableModel* model)      { model->Rotate(angles_t::z, 1); }));
    keybinds.append(WXK_HOME, ModelChange([](DrawableModel* model)     { model->Rotate(angles_t::z, -1); }));
    keybinds.append(WXK_PAGEUP, ModelChange([](DrawableModel* model)   { model->Rotate(angles_t::y, 1); }));
    keybinds.append(WXK_PAGEDOWN, ModelChange([](DrawableModel* model) { model->Rotate(angles_t::y, -1);; }));
    keybinds.append(WXK_NUMPAD4, ModelChange([](DrawableModel* model)  { model->Rotate(angles_t::x, 1); }));
    keybinds.append(WXK_NUMPAD6, ModelChange([](DrawableModel* model)  { model->Rotate(angles_t::x, -1); }));

    keybinds.append(WXK_SPACE, ModelChange([](DrawableModel* model)      { model->Move(axis_t::z, 1); }));
    keybinds.append(WXK_CONTROL, ModelChange([](DrawableModel* model)    { model->Move(axis_t::z, -1); }));
    keybinds.append((wxKeyCode)'W', ModelChange([](DrawableModel* model) { model->Move(axis_t::x, 1); }));
    keybinds.append((wxKeyCode)'A', ModelChange([](DrawableModel* model) { model->Move(axis_t::y, 1); }));
    keybinds.append((wxKeyCode)'S', ModelChange([](DrawableModel* model) { model->Move(axis_t::x, -1); }));
    keybinds.append((wxKeyCode)'D', ModelChange([](DrawableModel* model) { model->Move(axis_t::y, -1); }));
}

Viewer::Viewer(wxFrame* parent, Core& core)
    :wxGLCanvas(parent),
    core(core), context(new wxGLContext(this)), camera(this), keybinds(this)
{
    KeyBindingsInit();
    Bind(wxEVT_SIZE, [this](wxSizeEvent& evt) {
        auto size = evt.GetSize();
        auto factor = GetContentScaleFactor();
        glViewport(0, 0, size.x * factor, size.y * factor);
    });

    Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent&) {
        SetFocus();
    });

    Bind(wxEVT_PAINT, &Viewer::InitScene, this);
}

inline void Viewer::PrepareRender()
{
    SetCurrent(*context.get());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(1.0, 1.0, 1.0, 1.0);
}
inline void Viewer::FinishRender()
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

void Viewer::InitScene(wxPaintEvent&)
{
    wxPaintDC(this);
    PrepareRender();
    
    IsometricCameraRotation();

    Unbind(wxEVT_PAINT, &Viewer::InitScene, this);
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
    PrepareRender();

    for (auto model : core)
        model->Draw();

    glDisable(GL_DEPTH_TEST);
    DrawAxis();

    FinishRender();
}

void Viewer::RightClickOnModel(wxMouseEvent& event)
{
    wxClientDC(this);
    PrepareRender();

    uint32_t ID = 0;
    for (auto model : core)
        model->SelectRender(ID++);
    
    wxPoint pixel = event.GetPosition();
    ID = DrawableModel::GetColorSelection(pixel.x, pixel.y);
    // TODO: change condition to 'be equeal to background color'
    if (ID > core.size())
        return; // Add here scene general settings later maybe

    Configurator config(ID, core);
    PopupMenu(&config, event.GetPosition());
}

Viewer::~Viewer() {
    core.clear();
}