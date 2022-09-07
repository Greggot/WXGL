#include "OBJ.hpp"
using namespace OBJ;

static inline std::vector<std::string> GetStringTokens(const char* buffer);
static inline poly stringContainerToPoly(const std::vector<std::string>&);
static inline vertex stringContainerToVertex(const std::vector<std::string>&);

static inline std::string getMTLpathFrom(std::string modelpath, const std::string& filename);
static color GetColorFromMTLfile(const char* path, const char* base);
/*
* @brief Open .obj file and get vertexes/indexes from it
**/
Model::Model(const char* path)
	: BaseModel(path)
{
	FILE* in = fopen(path, "r");
	if (in == nullptr)
		throw std::runtime_error("Cannot open model file\n");
	
	Part part;
	
	std::string mtlibFile = "";
	std::string base = "";

	char buffer[0xFF]{ 0 };
	while (fgets(buffer, sizeof(buffer), in))
	{
		auto data = GetStringTokens(buffer);
		switch ((symbol)data[0][0])
		{
		case symbol::vertex:
			Points.push_back(stringContainerToVertex(data));
			break;
		case symbol::polygon:
			part.Polygons.push_back(stringContainerToPoly(data));
			break;
		case symbol::part:
			// Part identificator lies before part itself
			// AddPart here saves read data to process a new one
			base = data[1];
			if (part.Polygons.size())
				AddPart(part, mtlibFile.c_str(), base.c_str());
			break;
		case symbol::material:
			mtlibFile = getMTLpathFrom(Path, data[1]);
			break;
		}
	}
	// Adds last part of file
	AddPart(part, mtlibFile.c_str(), base.c_str());
	fclose(in);
}


void Model::Draw() const
{
	glPushMatrix();

	ApplyMovementFromBottomToTop();
	ApplyMovement(Translation, Rotation);

	for (auto part : Parts)
	{
		color Color = part.Color;

		float maxcolor = Color.x;
		if (Color.y > maxcolor)
			maxcolor = Color.y;
		if (Color.z > maxcolor)
			maxcolor = Color.z;
		maxcolor = 1 - maxcolor;

		int gradientscale = part.Polygons.size();
		color Gradient(maxcolor / gradientscale);

		glBegin(GL_TRIANGLES);
		for (auto triangle : part.Polygons)
		{
			glColor3f(Color.x, Color.y, Color.z);
			Color += Gradient;
			DrawPoly(triangle);
		}
		glEnd();
	}
	if (Active)
		DrawModelOutline();
	glPopMatrix();
}

inline void Model::DrawPoly(const poly& p) const
{
	DrawPoint(Points[p.begin]);
	DrawPoint(Points[p.mid]);
	DrawPoint(Points[p.end]);
}

inline void Model::DrawPolyOutline(const poly& p) const
{
	DrawPoint(Points[p.begin]);
	DrawPoint(Points[p.mid]);
	DrawPoint(Points[p.mid]);
	DrawPoint(Points[p.end]);
	DrawPoint(Points[p.end]);
	DrawPoint(Points[p.begin]);
}

inline void Model::DrawPoint(const vertex& v) const
{
	glVertex3f(v.x, v.y, v.z);
}

// TODO: Replace Euler angles with quaternions
inline void Model::ApplyMovement(vertex Transform, vertex Rotation)
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

inline void Model::DrawModelOutline() const
{
	glLineWidth(1);
	for (auto polies : Parts)
	{
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		for (auto triangle : polies.Polygons)
			DrawPolyOutline(triangle);
		glEnd();
	}
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
			DrawPoly(triangle);
		glEnd();
	}
	glPopMatrix();
}

// Replace *.obj with *mtl in full path 
static inline std::string getMTLpathFrom(std::string modelpath, const std::string& filename)
{
	modelpath.erase(modelpath.find_last_of("\\"), 0xFF);
	modelpath += '\\';
	modelpath += filename;
	return modelpath;
}

static color GetColorFromMTLfile(const char* path, const char* base)
{
	FILE* mtl = fopen(path, "r");
	if (mtl == nullptr)
		return standardColor;

	bool findmaterial = false;
	char buffer[0xFF]{ 0 };
	while (fgets(buffer, sizeof(buffer), mtl))
	{
		auto mtldata = GetStringTokens(buffer);
		switch ((symbol)mtldata[0][0])
		{
		case symbol::nameOfMaterial:
			findmaterial = mtldata[1] == base;
			break;
		case symbol::color:
			if (!findmaterial)
				break;

			fclose(mtl);
			return stringContainerToVertex(mtldata);
		}
	}
	fclose(mtl);
	return standardColor;
}

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

void Model::AddPart(Part& part, const char mtlpath[], const char basename[])
{
	part.Color = GetColorFromMTLfile(mtlpath, basename);
	Parts.push_back(part);
	part.Polygons.clear();
}

static inline poly stringContainerToPoly(const std::vector<std::string>& strings)
{
	return { atoi(strings[1].c_str()) - 1,
			 atoi(strings[2].c_str()) - 1,
			 atoi(strings[3].c_str()) - 1 };
}

static inline vertex stringContainerToVertex(const std::vector<std::string>& strings)
{
	return { strtof(strings[1].c_str(), nullptr),
			 strtof(strings[2].c_str(), nullptr),
			 strtof(strings[3].c_str(), nullptr) };
}