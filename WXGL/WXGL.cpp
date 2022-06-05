#include "WXGL.hpp"
#include "MouseOperator.hpp"
#include "OBJparser.hpp"

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

static inline void DrawPoly(std::vector<point> V, poly p)
{
    glVertex3f(V[p.begin - 1].x, V[p.begin - 1].y, V[p.begin - 1].z);
    glVertex3f(V[p.mid - 1].x, V[p.mid - 1].y, V[p.mid - 1].z);
    glVertex3f(V[p.end - 1].x, V[p.end - 1].y, V[p.end - 1].z);
}

static inline void DrawPolyOutline(std::vector<point>V, poly p)
{
    glVertex3f(V[p.begin - 1].x, V[p.begin - 1].y, V[p.begin - 1].z);
    glVertex3f(V[p.mid - 1].x,   V[p.mid - 1].y,   V[p.mid - 1].z);
    glVertex3f(V[p.mid - 1].x,   V[p.mid - 1].y,   V[p.mid - 1].z);
    glVertex3f(V[p.end - 1].x,   V[p.end - 1].y,   V[p.end - 1].z);
    glVertex3f(V[p.end - 1].x,   V[p.end - 1].y,   V[p.end - 1].z);
    glVertex3f(V[p.begin - 1].x, V[p.begin - 1].y, V[p.begin - 1].z);
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
    {
        glPushMatrix();
        
        // Translation before Rotation because otherwise
        // it would be rotating aroung absolute zero coordinate.
        // Not the model's one
        glTranslatef(Model->Translation.x, Model->Translation.y, Model->Translation.z);
        glRotatef(Model->Rotation.x, 1.0, 0.0, 0.0);
        glRotatef(Model->Rotation.y, 0.0, 1.0, 0.0);
        glRotatef(Model->Rotation.z, 0.0, 0.0, 1.0);

        for (auto polies : Model->Parts)
        {
            color Color = polies.Color;
            int gradientscale = polies.Polygons.size();

            glBegin(GL_TRIANGLES);
            for (auto triangle : polies.Polygons)
            {
                glColor3f(Color.r, Color.g, Color.b);
                Color.g += 1.0 / gradientscale;
                DrawPoly(Model->Points, triangle);
            }
            glEnd();
        }
        if (Model->Active)
        {
            for (auto polies : Model->Parts)
            {
                glBegin(GL_LINES);
                glLineWidth(1);
                glColor3f(0, 0, 0);
                for (auto triangle : polies.Polygons)
                    DrawPolyOutline(Model->Points, triangle);
                glEnd();
            }
        }
        glPopMatrix();
    }

    glFlush();
    SwapBuffers();
}

void ModelViewer::Append(OBJmodel model)
{
    OBJmodel* allocmodel = new OBJmodel(model);
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