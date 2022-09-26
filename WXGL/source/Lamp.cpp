#include "Lamp.hpp"

Light::Light()
{
	
}

void Light::Render()
{	
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	
	glEnable(GL_NORMALIZE);
	glNormal3f(0.0, 0.0, -1.0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT2);
	//glLightfv(GL_LIGHT2, GL_AMBIENT, (float*)&Ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, (float*)&Diffuse);
	//glLightfv(GL_LIGHT2, GL_SPECULAR, (float*)&Specular);
	glLightfv(GL_LIGHT2, GL_POSITION, (float*)&Position);
	//glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, (float*)&Specular);

	//glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, SpotExponent);
	//glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, SpotCutoff);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, ConstantAttenuation);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, LinearAttenuation);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, QuadraticAttenuation);

	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3b(0xFF, 0, 0);
	glVertex3f(Position.x, Position.y, Position.z);
	glEnd();
}