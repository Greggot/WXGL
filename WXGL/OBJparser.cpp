#include "OBJparser.hpp"

OBJmodel ParseFile(const char* path)
{
	FILE* in = fopen(path, "r");
	
	char buffer[0xFF];
	std::string data[4];
	std::string temp;

	point p;
	std::vector<point> points;
	std::list<Part> overallparts;
	
	static const color standardColor = { 0.5, 0, 1 };
	Part part;
	std::list<poly> polies;
	
	while (fgets(buffer, 0xFF, in))
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
		if(stringindex < 4)
			data[stringindex] = temp;
		temp = "";

		switch (data[0][0])
		{
		case 'v':
			p = {	(float)atof(data[1].c_str()), 
					(float)atof(data[2].c_str()), 
					(float)atof(data[3].c_str()) };
			points.push_back(p);
			break;
		case 'f':
			polies.push_back({ atoi(data[1].c_str()),
							   atoi(data[2].c_str()),
							   atoi(data[3].c_str())});
			break;
		case 'u':
			if (polies.size())
			{
				part = { polies, standardColor };
				overallparts.push_back(part);
				polies.clear();
			}
			break;
		}

		for (auto& s : data)
			s = "";
	}
	part = { polies, standardColor };
	overallparts.push_back(part);
	return { points, overallparts, false, {0, 0, 0}, {0, 0, 0} };
}