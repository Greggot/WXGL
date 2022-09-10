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

void Model::Draw() const
{
	glPushMatrix();
	ApplyMovement();

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

	if (Active)
		ActiveOutlineDraw();
	
	glPopMatrix();
}

inline void Model::ActiveOutlineDraw() const
{
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	for (auto T : Tokens)
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

void Model::ColorSelectDraw(uint32_t ID) const
{
	glPushMatrix();
	ApplyMovement();

	setColorFrom(ID);
	for (auto T : Tokens)
		for (auto v : T.verticies)
			v.draw();

	glPopMatrix();
}