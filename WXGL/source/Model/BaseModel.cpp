#include <Model/BaseModel.hpp>

void vertex::draw() const { glVertex3f(x, y, z); }
float vertex::maxvalue() const { return x > y ? (x > z ? x : z) : (y > z ? y : z); }

void BaseModel::setColorFrom(uint32_t ID) 
{
    static GLuint Color[3] = { 0 };
    Color[2] = ID & 0xFF;
    ID >>= 8;
    Color[1] = ID & 0xFF;
    ID >>= 8;
    Color[0] = ID & 0xFF;
    glColor3ub(Color[0], Color[1], Color[2]);
}

inline void BaseModel::ApplyMovementFromBottomToTop() const 
{
    BaseModel* head = parent;
    std::list<move> Moves;
    if (parent)
    {
        do
        {
            Moves.push_front({ head->Translation, head->Rotation });
            head = head->parent;
        } while (head);

        for (const move& m : Moves)
            ApplyMovement(m.T, m.R);
    }
}
// TODO: Replace Euler angles with quaternions
void BaseModel::ApplyMovement(const vertex& T, const vertex& R) const {
    glTranslatef(T.x, T.y, T.z);
    glRotatef(R.x, 1.0, 0.0, 0.0);
    glRotatef(R.y, 0.0, 1.0, 0.0);
    glRotatef(R.z, 0.0, 0.0, 1.0);
}
void BaseModel::ApplyMovement() const
{
    ApplyMovementFromBottomToTop();
    ApplyMovement(Translation, Rotation);
}

uint32_t BaseModel::GetColorSelection(uint32_t x, uint32_t y)
{
    GLint viewport[4] = { 0 };
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLubyte Color[4] = { 0 };
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, Color);

    return (Color[0] << 16) | (Color[1] << 8) | Color[2];
}

void BaseModel::LinkTo(BaseModel* Parent) 
{
    parent = Parent;
    if (parent == nullptr)
        return;
    
    RemoveFromTree();
    parent->children.push_back(this);
}

void BaseModel::Orphan() {
    parent = nullptr;
}

void BaseModel::RemoveFromTree() 
{
    if (parent == nullptr)
    {
        for (auto child : children)
            child->Orphan();
        return;
    }

    parent->children.remove(this);
    for (auto child : children)
        child->LinkTo(parent);
}