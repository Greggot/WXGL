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
        auto model = Tree.active();
        if (model == nullptr)
            return;
        call(model);
    };
}

void Viewer::KeyBindingsInit()
{
    keybinds.append(WXK_RETURN, { [this](wxKeyEvent&) {
        size_t size = Tree.size();
        if (size == 0)
            return;

        size_t index = Tree.activeindex();
        if (++index >= size)
            index = 0;
        Tree.setActive(index);
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

Viewer::Viewer(wxWindow* parent, DependencyTree& Tree, SkyBlue::Device& device)
    :wxGLCanvas(parent),
    Tree(Tree), context(new wxGLContext(this)), camera(this), keybinds(this), device(device)
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
    static const vertex Axises[3] = { eX, eY, eZ };
    float scale = camera.getScale() * 4;

    glLineWidth(2.4f);
    glBegin(GL_LINES);
    for (auto axis : Axises)
    {
        glColor3f(axis.x, axis.y, axis.z);
        glVertex3f(0, 0, 0);
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

    for (auto model : Tree)
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
    for (auto model : Tree)
        model->SelectRender(ID++);
    
    wxPoint pixel = event.GetPosition();
    ID = DrawableModel::GetColorSelection(pixel.x, pixel.y);
    // TODO: change condition to 'be equeal to background color'
    if (ID > Tree.size())
        return; // Add here scene general settings later maybe

    wxMenu* config;
    const auto& id = Tree[ID].getID();
    switch (id.type)
    {
    case SkyBlue::type_t::camera:
        config = new Context::Camera(ID, Tree, device);
        break;
    default:
        config = new Context::Model(ID, Tree);
        break;
    }

    //Configurator config(ID, core);
    PopupMenu(config, event.GetPosition());
    delete config;
}

Viewer::~Viewer() {
}