#include "Assembly.Viewer.hpp"
#include "Assembly.Operator.hpp"
#include "OBJ.hpp"

using namespace Assembly;

BEGIN_EVENT_TABLE(Viewer, wxGLCanvas)
EVT_PAINT(Viewer::Render)

EVT_MOUSEWHEEL(Operator::Zoom)
EVT_LEFT_DOWN(Operator::StartRotateXY)
EVT_MIDDLE_DOWN(Operator::StartRotateZ)
EVT_RIGHT_DOWN(Viewer::RightClickOnModel)
EVT_MOUSE_EVENTS(Operator::Rotate)

EVT_KEY_DOWN(Operator::Move)

END_EVENT_TABLE()

#define KeyBind(key, bind)  \
    Operator::AppendKeyEvent(key, [this](wxKeyEvent& event) { bind; })

void Viewer::KeyBindingsInit()
{
    Operator::AppendKeyEvent(WXK_RETURN, [this](wxKeyEvent&) {
        size_t size = core.size();
        if (size == 0)
            return;

        size_t index = core.activeindex();
        if (index++ >= size)
            index = 0;
        core.setActive(index);
    });
}

Viewer::Viewer(wxFrame* parent, Core& core)
    :wxGLCanvas(parent, wxID_ANY, 0, wxPoint(60, 10), wxSize(400, 300), 0, wxT("GLCanvas")),
    core(core)
{
    m_context = new wxGLContext(this);
    KeyBindingsInit();
}

inline void Viewer::GLSceneInit()
{
    SetCurrent(*m_context);
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

    float scale = Operator::getScale() * 4;

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

static inline void IsometricCameraRotation()
{
    Operator::RotateCamera(-135, Y);
    Operator::RotateCamera(45, Z);
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
        model->ColorSelectDraw(ID++);
    
    wxPoint pixel = event.GetPosition();
    ID = BaseModel::GetColorSelection(pixel.x, pixel.y);
    if (ID > core.size())
        return; // Add here scene general settings later maybe

    Configurator config(wxString(core[ID].Name));
    PopupMenu(&config, event.GetPosition());
}

static inline void RemoveLink(BaseModel* Model)
{
    BaseModel* Host = Model->Host;
    BaseModel* Leaf = Model->Leaf;
    if (Host)
        Host->Leaf = Leaf;
    if (Leaf)
        Leaf->Host = Host;
}

Viewer::~Viewer()
{
    core.clear();
}