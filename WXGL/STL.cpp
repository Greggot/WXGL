#include "STL.hpp"
using namespace STL;

Model::Model(const char* path)
	: BaseModel(path)
{
	FILE* in = fopen(path, "rb");
	header Header;
	fread(&Header, sizeof(Header), 1, in);

	token Token;
	while (Header.facetsamount--)
	{
		fread(&Token, sizeof(Token), 1, in);
		Tokens.push_back(Token);
	}
}

inline void DrawPoint(const vertex& v)
{
	glVertex3f(v.x, v.y, v.z);
}

void Model::Draw() const
{
	
	vertex Color{0.4, 0.4, 0.4};
	float maxcolor = 0.4;

	int gradientscale = Tokens.size();
	vertex Gradient(maxcolor / gradientscale);

	glPointSize(5);
	glBegin(GL_TRIANGLES);
	for (auto T : Tokens)
	{
		glColor3f(Color.x, Color.y, Color.z);
		Color += Gradient;

		DrawPoint(T.verticies[0]);
		DrawPoint(T.verticies[1]);
		DrawPoint(T.verticies[2]);
	}
	glEnd();
}
