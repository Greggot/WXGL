#include "Assembly.Viewer.hpp"
#include "Assembly.Operator.hpp"
#include "OBJ.hpp"

using namespace Assembly;

BEGIN_EVENT_TABLE(Viewer, wxGLCanvas)
EVT_PAINT(Viewer::Render)

EVT_MOUSEWHEEL(Operator::Zoom)
EVT_LEFT_DOWN(Operator::StartRotateXY)
EVT_MIDDLE_DOWN(Operator::StartRotateZ)
EVT_RIGHT_DOWN(Operator::RightClickOnModel)
EVT_MOUSE_EVENTS(Operator::Rotate)

EVT_KEY_DOWN(Operator::Move)

END_EVENT_TABLE()

Viewer::Viewer(wxFrame* parent)
    :wxGLCanvas(parent, wxID_ANY, 0, wxPoint(60, 10), wxSize(400, 300), 0, wxT("GLCanvas"))
{
    m_context = new wxGLContext(this);
    ModelAmount = 0;
    ActiveIndex = 0;

    Operator::AppendKeyEvent(WXK_RETURN, [this](wxKeyEvent& event) {
        SwitchActive();
    });
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
    if (++ActiveIndex >= ModelAmount)
        ActiveIndex = 0;
    Operator::SetTarget(Assembly[ActiveIndex]);
}

Viewer::~Viewer()
{
    Assembly.clear();
}