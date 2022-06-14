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
void Viewer::Render(wxPaintEvent& event)
{
    wxPaintDC(this);
    SetCurrent(*m_context);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    for (auto Model : Assembly)
        Model->Draw();

    glFlush();
    SwapBuffers();
}

void Viewer::Append(OBJ::Model* model)
{
    if(ModelAmount)
        model->LinkTo(Assembly[ModelAmount - 1]);
    Assembly.push_back(model);
    
    ActiveIndex = ModelAmount++;
    Operator::SetTarget(model);
}

static inline void setColorFromID(uint32_t ID)
{
    static GLuint Color[3] = {0};
    Color[2] = ID & 0xFF;
    ID >>= 8;
    Color[1] = ID & 0xFF;
    ID >>= 8;
    Color[0] = ID & 0xFF;
    glColor3ub(Color[0], Color[1], Color[2]);
}

static inline uint32_t getIDfromPosition(wxPoint p)
{
    GLubyte Color[4] = { 0 };
    GLint viewport[4] = { 0 };
    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(p.x, viewport[3] - p.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, Color);
    
    return (Color[0] << 16) | (Color[1] << 8) | Color[2];
}


void Viewer::RightClickOnModel(wxMouseEvent& event)
{
    wxClientDC(this);
    SetCurrent(*m_context);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    uint32_t ID = 0;
    for (auto Model : Assembly)
    {
        Model->Select = true;
        setColorFromID(ID++);
        Model->Draw();
        Model->Select = false;
    }
    ID = getIDfromPosition(event.GetPosition());

    if (ID > ModelAmount)
        return; // Add here scene general settings later maybe
    Configurator config(wxString::Format("%08X", ID));
    PopupMenu(&config, event.GetPosition());
}

void Viewer::RemoveLink(size_t index)
{
    OBJ::Model* Model = Assembly[index];
    OBJ::Model* Host = Model->getHost();
    OBJ::Model* Leaf = Model->getLeaf();
    if (Host)
        Host->setLeaf(Leaf);
    if (Leaf)
        Leaf->setHost(Host);
}

void Viewer::Remove(size_t index)
{
    RemoveLink(index);
    Assembly.erase(Assembly.begin() + index);
    --ModelAmount;

    if (ActiveIndex == index)
        SwitchActive();
}

void Viewer::SwitchActive()
{
    if (ModelAmount == 0)
        return;
    if (++ActiveIndex >= ModelAmount)
        ActiveIndex = 0;
    Operator::SetTarget(Assembly[ActiveIndex]);
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