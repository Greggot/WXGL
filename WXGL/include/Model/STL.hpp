#pragma once
#pragma once
#pragma warning(disable : 4996)

#include <list>
#include <stdexcept>

#include <cstdio>
#include <string>
#include <vector>

#include <wx/glcanvas.h>
#include <gl/GLU.h>
#include <gl/GL.h>

#include "BaseModel.hpp"

#pragma pack(push, 2)
namespace STL
{
    struct header
    {
        char ASCII[80]{0};
        uint32_t facetsamount = 0;
    };

    struct token
    {
        vertex normal;
        vertex verticies[3];
        uint16_t attrbytecount;
    };

    class Model : public BaseModel
    {
    private:
        std::vector<token> Tokens;

        inline void ApplyMovementFromBottomToTop() const {};
    public:
        Model() { }
        Model(const char* FilePath);

        void ConcreteDraw() const override;
        void DrawSelectionMode(uint32_t ID) const override;
        void ActiveOutlineDraw() const override;

        ~Model() { };
    };
}
#pragma pack(pop)