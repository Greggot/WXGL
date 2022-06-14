#include "OBJ.hpp"
using namespace OBJ;

static inline std::vector<std::string> GetStringTokens(const char* buffer)
{
	uint8_t index = 0;
	std::vector<std::string> Tokens;
	std::string temp;

	while (buffer[index] != '\n')
	{
		if (buffer[index] == ' ')
		{
			Tokens.push_back(temp);
			temp = "";
		}
		else
			temp += buffer[index];
		++index;
	}
	Tokens.push_back(temp);
	return Tokens;
}

color GetColorFromMTLfile(const char* path, const char* base)
{
	color texture(0,0,1);
	FILE* mtl = fopen(path, "r");

	std::vector<std::string> mtldata;
	
	char buffer[0xFF];
	bool findcolor = true;
	bool findmaterial = true;

	while (findcolor && fgets(buffer, sizeof(buffer), mtl))
	{
		uint8_t index = 0;
		uint8_t stringindex = 0;
		mtldata = GetStringTokens(buffer);

		switch (mtldata[0][0])
		{
		case 'n':
			if (mtldata[1] == base)
				findmaterial = true;
			break;
		case 'K':
			if (!findmaterial)
				break;

			texture.r = atof(mtldata[1].c_str());
			texture.g = atof(mtldata[2].c_str());
			texture.b = atof(mtldata[3].c_str());

			fclose(mtl);
			return texture;
		}
	}
	return texture;
}
/*
* @brief Open .obj file and get vertexes/indexes from it
**/
Model::Model(const char* path)
{
	Path = std::string(path);
	Name = Path.substr(Path.find_last_of("\\") + 1);

	FILE* in = fopen(path, "r");
	
	char buffer[0xFF];
	std::vector<std::string> data;
	
	static const color standardColor(0.5, 0, 1);
	vertex p;
	Part part = { {}, standardColor };
	
	std::string mtlibFile = path;
	mtlibFile.erase(mtlibFile.find_last_of("\\"), 0xFF);

	while (fgets(buffer, sizeof(buffer), in))
	{
		data = GetStringTokens(buffer);

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
				part.Color = GetColorFromMTLfile(mtlibFile.c_str(), data[1].c_str());
				Parts.push_back(part);
				part.Polygons.clear();
			}
			break;
		case 'm':
			mtlibFile += '\\';
			mtlibFile += data[1].c_str();
			break;
		}
	}
	part.Color = GetColorFromMTLfile(mtlibFile.c_str(), data[1].c_str());
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

static inline void ApplyMovement(vertex Transform, vertex Rotation)
{
	glTranslatef(Transform.x, Transform.y, Transform.z);
	glRotatef(Rotation.x, 1.0, 0.0, 0.0);
	glRotatef(Rotation.y, 0.0, 1.0, 0.0);
	glRotatef(Rotation.z, 0.0, 0.0, 1.0);
}

inline void Model::ApplyMovementFromBottomToTop() const
{
	BaseModel* root = Host;
	if (root)
	{
		while (root->Host)
			root = root->Host;
		BaseModel* head = root;
		do
		{
			ApplyMovement(head->Translation, head->Rotation);
			head = head->Leaf;
		} while (head != this);
	}
}

void Model::Draw() const
{
	glPushMatrix();

	ApplyMovementFromBottomToTop();
	ApplyMovement(Translation, Rotation);

	for (auto part : Parts)
	{
		color Color = part.Color;
		
		float maxcolor = Color.r;
		if (Color.g > maxcolor)
			maxcolor = Color.g;
		if (Color.b > maxcolor)
			maxcolor = Color.b;
		maxcolor = 1 - maxcolor;
		
		int gradientscale = part.Polygons.size();
		color Gradient(maxcolor / gradientscale);

		glBegin(GL_TRIANGLES);
		for (auto triangle : part.Polygons)
		{
			glColor3f(Color.r, Color.g, Color.b);
			Color += Gradient;
			DrawPoly(Points, triangle);
		}
		glEnd();
	}
	if (Active)
	{
		glLineWidth(1);
		for (auto polies : Parts)
		{
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			for (auto triangle : polies.Polygons)
				DrawPolyOutline(Points, triangle);
			glEnd();
		}
	}
	glPopMatrix();
}

static inline void setColorFrom(uint32_t ID)
{
	static GLuint Color[3] = { 0 };
	Color[2] = ID & 0xFF;
	ID >>= 8;
	Color[1] = ID & 0xFF;
	ID >>= 8;
	Color[0] = ID & 0xFF;
	glColor3ub(Color[0], Color[1], Color[2]);
}

/*
* @brief Draw each of model's polygons the same color
*	so it could differ from others
* @param ID will be converted to color
**/
void Model::ColorSelectDraw(uint32_t ID) const
{
	glPushMatrix();

	ApplyMovementFromBottomToTop();
	ApplyMovement(Translation, Rotation);

	setColorFrom(ID);

	for (auto part : Parts)
	{
		glBegin(GL_TRIANGLES);
		for (auto triangle : part.Polygons)
			DrawPoly(Points, triangle);
		glEnd();
	}
	glPopMatrix();
}