#include "WXGL.hpp"
#include "MouseOperator.hpp"
#include "OBJ.hpp"

BEGIN_EVENT_TABLE(ModelViewer, wxGLCanvas)
EVT_PAINT(ModelViewer::Render)
EVT_CLOSE(ModelViewer::Close)

EVT_MOUSEWHEEL(MouseOperator::Zoom)
EVT_LEFT_DOWN(MouseOperator::StartRotateX)
EVT_RIGHT_DOWN(MouseOperator::StartRotateY)
EVT_MIDDLE_DOWN(MouseOperator::StartRotateZ)
EVT_MOUSE_EVENTS(MouseOperator::Rotate)

EVT_KEY_DOWN(MouseOperator::Move)

END_EVENT_TABLE()

ModelViewer::ModelViewer(wxFrame* parent)
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

    MouseOperator::AppendKeyEvent(WXK_RETURN, [this](wxKeyEvent& event) {
        SwitchActive();
    });
}

void ModelViewer::Close(wxCloseEvent& event)
{
    isUpdating = false;
    Update.join();
}

void ModelViewer::Render(wxPaintEvent& event)
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

void ModelViewer::Append(OBJ::Model model)
{
    OBJ::Model* allocmodel = new OBJ::Model(model);
    if(ModelAmount)
        allocmodel->LinkTo(Assembly[ModelAmount - 1]);
    Assembly.push_back(allocmodel);
    ++ModelAmount;
    
    SwitchActive();
}

void ModelViewer::SwitchActive()
{
    Assembly[ActiveIndex]->Active = false;
    if (++ActiveIndex >= ModelAmount)
        ActiveIndex = 0;
    Assembly[ActiveIndex]->Active = true;
    MouseOperator::Init(Assembly[ActiveIndex]);
}