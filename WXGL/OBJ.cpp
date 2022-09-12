#include "OBJ.hpp"
using namespace OBJ;

static inline std::vector<std::string> GetStringTokens(const char* buffer);
static inline poly stringContainerToPoly(const std::vector<std::string>&);
// static inline vertex stringContainerToVertex(const std::vector<std::string>&);

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

	// Make model fit into the screen
	Scale = 1 / Scale;
}


void Model::Draw() const
{
	glPushMatrix();

	glScalef(Scale, Scale, Scale);

	ApplyMovement();

	for (auto part : Parts)
	{
		color Color = part.Color;
		color Gradient = GradientStep(Color, part.Polygons.size());

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
		ActiveOutlineDraw();
	glPopMatrix();
}

inline void Model::DrawPoly(const poly& p) const
{
	Points[p.begin].draw();
	Points[p.mid].draw();
	Points[p.end].draw();
}

inline void Model::DrawPolyOutline(const poly& p) const
{
	Points[p.begin].draw();
	Points[p.mid].draw();
	Points[p.mid].draw();
	Points[p.end].draw();
	Points[p.end].draw();
	Points[p.begin].draw();
}

inline void Model::ActiveOutlineDraw() const
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

/*
* @brief Draw each of model's polygons the same color
*	so it could differ from others
* @param ID will be converted to color
**/
void Model::ColorSelectDraw(uint32_t ID) const
{
	glPushMatrix();

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

color Model::GetColorFromMTLfile(const char* path, const char* base)
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

inline vertex Model::stringContainerToVertex(const std::vector<std::string>& strings)
{
	vertex v(strtof(strings[1].c_str(), nullptr),
				strtof(strings[2].c_str(), nullptr),
				strtof(strings[3].c_str(), nullptr));
	// Scale = std::max( v.max(), Scale );
	return v;
}