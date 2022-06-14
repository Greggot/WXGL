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
    KeyBind(WXK_RETURN,     SwitchActive());
    KeyBind(WXK_LEFT,       --activeTransform->x);
    KeyBind(WXK_RIGHT,      ++activeTransform->x);
    KeyBind(WXK_UP,         --activeTransform->y);
    KeyBind(WXK_DOWN,       ++activeTransform->y);
    KeyBind(WXK_CONTROL,    --activeTransform->z);
    KeyBind(WXK_SPACE,      ++activeTransform->z);

    KeyBind(WXK_HOME,       --activeRotation->x);
    KeyBind(WXK_END,        ++activeRotation->x);
    KeyBind(WXK_PAGEUP,     --activeRotation->y);
    KeyBind(WXK_PAGEDOWN,   ++activeRotation->y);
    KeyBind(WXK_NUMPAD4,    --activeRotation->z);
    KeyBind(WXK_NUMPAD6,    ++activeRotation->z);
}

Viewer::Viewer(wxFrame* parent)
    :wxGLCanvas(parent, wxID_ANY, 0, wxPoint(60, 10), wxSize(400, 300), 0, wxT("GLCanvas"))
{
    m_context = new wxGLContext(this);
    ModelAmount = 0;
    ActiveIndex = 0;

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

    for (auto Model : Assembly)
        Model->Draw();

    glDisable(GL_DEPTH_TEST);
    DrawAxis();

    GLSceneRender();
}

void Viewer::Append(BaseModel* Model)
{
    if(ModelAmount)
        Model->LinkTo(Assembly[ModelAmount - 1]);
    Assembly.push_back(Model);
    
    ActiveIndex = ModelAmount++;
    SetActive(Model);
}

void Viewer::RightClickOnModel(wxMouseEvent& event)
{
    wxClientDC(this);
    GLSceneInit();

    uint32_t ID = 0;
    for (auto Model : Assembly)
        Model->ColorSelectDraw(ID++);
    
    wxPoint pixel = event.GetPosition();
    ID = BaseModel::GetColorSelection(pixel.x, pixel.y);
    if (ID > ModelAmount)
        return; // Add here scene general settings later maybe

    Configurator config(wxString(Assembly[ID]->Name));
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

void Viewer::Remove(size_t index)
{
    RemoveLink(Assembly[index]);
    Assembly.erase(Assembly.begin() + index);
    --ModelAmount;

    if (ActiveIndex == index)
        SwitchActive();
}

void Viewer::SetActive(BaseModel* Target)
{
    static BaseModel* ActiveOne = nullptr;
    if (ActiveOne)
        ActiveOne->Active = false;
    ActiveOne = Target;
    ActiveOne->Active = true;

    activeRotation = &ActiveOne->Rotation;
    activeTransform = &ActiveOne->Translation;
}

void Viewer::SwitchActive()
{
    if (ModelAmount == 0)
        return;
    
    if (++ActiveIndex >= ModelAmount)
        ActiveIndex = 0;
  
    SetActive(Assembly[ActiveIndex]);
}

void Viewer::RemoveAll()
{
    ModelAmount = 0;
    ActiveIndex = 0;
    Assembly.clear();
}

Viewer::~Viewer()
{
    Assembly.clear();
}