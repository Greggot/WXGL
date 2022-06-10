#pragma once
#include <wx/glcanvas.h>
#include <gl/GLU.h>
#include <gl/GL.h>

struct vector
{
	float x;
	float y;
	float z;
	float w;
};


class Light
{
public:
	vector Ambient;
	vector Diffuse = { 0.7, 0.7, 0.7 };
	vector Specular;
	vector Position = { -1.0, 1.0, 1.0, 1.0 };
	vector SpotDirection;
	
	float SpotExponent;
	float SpotCutoff;
	float ConstantAttenuation = 6;
	float LinearAttenuation = -9;
	float QuadraticAttenuation = 10;

	Light();
	
	void Render();
};