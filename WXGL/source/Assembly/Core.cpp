#include <Assembly/Core.hpp>
using namespace Assembly;

void Core::append(DrawableModel* model)
{
	Models.push_back(model);
	setActive(size() - 1);
	if (size() > 1)
		model->LinkTo(Models[size() - 2]);
}

void Core::remove(uint16_t index) {
	delete Models[index];
	Models.erase(Models.begin() + index);
}

void Core::clear() {
	for (auto model : Models)
		delete model;
}

void Core::setActive(size_t Index) {
	this->Index = Index;
	if (Active)
		Active->Active = false;
	Active = Models[Index];
	Active->Active = true;
}

void Core::setActive(DrawableModel* model)
{
	if (Active)
		Active->Active = false;
	Active = model;
	if(Active)
		Active->Active = true;
}