#include "OBJ.hpp"
using namespace OBJ;
/*
* @brief Open .obj file and get vertexes/indexes from it
**/
Model::Model(const char* path)
{
	FILE* in = fopen(path, "r");
	
	char buffer[0xFF];
	static const uint8_t stringMaxAmount = 4;
	std::string data[stringMaxAmount];
	std::string temp;
	vertex p;
	
	static const color standardColor = { 0.5, 0, 1 };
	Part part = { {}, standardColor };

	while (fgets(buffer, sizeof(buffer), in))
	{
		uint8_t index = 0;
		uint8_t stringindex = 0;
		while (buffer[index] != '\n')
		{
			if (buffer[index] == ' ')
			{
				data[stringindex++] = temp;
				temp = "";
			}
			else
				temp += buffer[index];
			++index;
		}
		if(stringindex < stringMaxAmount)
			data[stringindex] = temp;
		temp = "";

		switch (data[0][0])
		{
		case 'v':
			p = {	(float)atof(data[1].c_str()), 
					(float)atof(data[2].c_str()), 
					(float)atof(data[3].c_str()) };
			Points.push_back(p);
			break;
		case 'f':
			part.Polygons.push_back({ atoi(data[1].c_str()),
							   atoi(data[2].c_str()),
							   atoi(data[3].c_str())});
			break;
		case 'u':
			if (part.Polygons.size())
			{
				Parts.push_back(part);
				part.Polygons.clear();
			}
			break;
		}

		for (auto& s : data)
			s = "";
	}
	Parts.push_back(part);
	fclose(in);
}

static inline void DrawPoly(std::vector<vertex> V, poly p)
{
	glVertex3f(V[p.begin - 1].x, V[p.begin - 1].y, V[p.begin - 1].z);
	glVertex3f(V[p.mid - 1].x, V[p.mid - 1].y, V[p.mid - 1].z);
	glVertex3f(V[p.end - 1].x, V[p.end - 1].y, V[p.end - 1].z);
}

static inline void DrawPolyOutline(std::vector<vertex>V, poly p)
{
	glVertex3f(V[p.begin - 1].x, V[p.begin - 1].y, V[p.begin - 1].z);
	glVertex3f(V[p.mid - 1].x, V[p.mid - 1].y, V[p.mid - 1].z);
	glVertex3f(V[p.mid - 1].x, V[p.mid - 1].y, V[p.mid - 1].z);
	glVertex3f(V[p.end - 1].x, V[p.end - 1].y, V[p.end - 1].z);
	glVertex3f(V[p.end - 1].x, V[p.end - 1].y, V[p.end - 1].z);
	glVertex3f(V[p.begin - 1].x, V[p.begin - 1].y, V[p.begin - 1].z);
}

static inline void ApplyTransfrom(vertex Transform)
{
	glTranslatef(Transform.x, Transform.y, Transform.z);
}

static inline void ApplyRotation(vertex Rotation)
{
	glRotatef(Rotation.x, 1.0, 0.0, 0.0);
	glRotatef(Rotation.y, 0.0, 1.0, 0.0);
	glRotatef(Rotation.z, 0.0, 0.0, 1.0);
}

void Model::Draw() const
{
	glPushMatrix();

	Model* host = Host;
	if (host)
	{
		while (host->getHost())
			host = host->getHost();
		Model* Leaf = host;
		do
		{
			ApplyTransfrom(*Leaf->getTransformVector());
			ApplyRotation(*Leaf->getRotationVector());
			Leaf = Leaf->getLeaf();
		} while (Leaf != this);
	}
	
	ApplyTransfrom(Translation);
	ApplyRotation(Rotation);

	for (auto part : Parts)
	{
		color Color = part.Color;
		int gradientscale = part.Polygons.size();

		glBegin(GL_TRIANGLES);
		for (auto triangle : part.Polygons)
		{
			glColor3f(Color.r, Color.g, Color.b);
			Color.g += 1.0 / gradientscale;
			DrawPoly(Points, triangle);
		}
		glEnd();
	}
	if (Active)
	{
		for (auto polies : Parts)
		{
			glBegin(GL_LINES);
			glLineWidth(1);
			glColor3f(0, 0, 0);
			for (auto triangle : polies.Polygons)
				DrawPolyOutline(Points, triangle);
			glEnd();
		}
	}
	glPopMatrix();
}