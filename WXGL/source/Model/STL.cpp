#include <Model/STL.hpp>
using namespace STL;

Model::Model(const char* path)
	: DrawableModel(path)
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

void Model::PolygoneRender() const
{
	color Color(standardColor);
	color Gradient = GradientStep(Color, Tokens.size());

	glBegin(GL_TRIANGLES);
	for (auto T : Tokens)
	{
		glColor3f(Color.x, Color.y, Color.z);
		Color += Gradient;
		for (auto v : T.verticies)
			v.draw();
	}
	glEnd();
}

inline void Model::OutlineRender() const
{
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	for (const token& T : Tokens)
	{
		T.verticies[0].draw();
		T.verticies[1].draw();

		T.verticies[1].draw();
		T.verticies[2].draw();

		T.verticies[2].draw();
		T.verticies[0].draw();
	}
	glEnd();
}

void Model::SelectRender(int ID)
{
	glPushMatrix();
	
	glScalef(Scale, Scale, Scale);
	ApplyHierarchyMovement();
	ApplyOwnMovement();

	glBegin(GL_TRIANGLES);
	setColorFrom(ID);
	for (auto T : Tokens)
		for (auto v : T.verticies)
			v.draw();
	glEnd();

	glPopMatrix();
}