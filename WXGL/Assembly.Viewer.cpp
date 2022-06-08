#include "Assembly.Viewer.hpp"
#include "Assembly.Operator.hpp"
#include "OBJ.hpp"

using namespace Assembly;

BEGIN_EVENT_TABLE(Viewer, wxGLCanvas)
EVT_PAINT(Viewer::Render)
EVT_CLOSE(Viewer::Close)

EVT_MOUSEWHEEL(Operator::Zoom)
EVT_LEFT_DOWN(Operator::StartRotateX)
EVT_RIGHT_DOWN(Operator::StartRotateY)
EVT_MIDDLE_DOWN(Operator::StartRotateZ)
EVT_MOUSE_EVENTS(Operator::Rotate)

EVT_KEY_DOWN(Operator::Move)

END_EVENT_TABLE()

Viewer::Viewer(wxFrame* parent)
    :wxGLCanvas(parent, wxID_ANY, 0, wxPoint(60, 10), wxSize(400, 300), 0, wxT("GLCanvas"))
{
    m_context = new wxGLContext(this);
    Update = std::thread([this] {
        while (isUpdating)
        {
            Refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    });
    ModelAmount = 0;
    ActiveIndex = 0;

    Operator::AppendKeyEvent(WXK_RETURN, [this](wxKeyEvent& event) {
        SwitchActive();
    });
}

void Viewer::Close(wxCloseEvent& event)
{
    isUpdating = false;
    Update.join();
}

void Viewer::Render(wxPaintEvent& event)
{
    wxPaintDC(this);
    SetCurrent(*m_context);
    static float activeangle = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    for (auto Model : Assembly)
        Model->Draw();

    glFlush();
    SwapBuffers();
}

void Viewer::Append(OBJ::Model model)
{
    OBJ::Model* allocmodel = new OBJ::Model(model);
    if(ModelAmount)
        allocmodel->LinkTo(Assembly[ModelAmount - 1]);
    Assembly.push_back(allocmodel);
    ++ModelAmount;
    
    SwitchActive();
}

void Viewer::SwitchActive()
{
    Assembly[ActiveIndex]->Active = false;
    if (++ActiveIndex >= ModelAmount)
        ActiveIndex = 0;
    Assembly[ActiveIndex]->Active = true;
    Operator::Init(Assembly[ActiveIndex]);
}

Viewer::~Viewer()
{
    for (auto model : Assembly)
        delete model;
}