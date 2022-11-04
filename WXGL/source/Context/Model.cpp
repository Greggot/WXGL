#include <Context/Model.hpp>
using namespace Context;

Model::Model(uint16_t index, Assembly::Core& core)
	: index(index), core(core), wxMenu(core[index].Name)
{
	AppendMenuItem(Mod::Delete, "Delete", &Model::Delete);
	AppendSeparator();

	const auto degree = core[index].GetDegreeOfFreedom();
	if(degree.raw & 0b111)
		AppendMenuItem(Mod::Coordinates, "Coordinates...", &Model::Translation);
	if(degree.raw & 0b111000)
		AppendMenuItem(Mod::Angles, "Angles...", &Model::Rotation);
	AppendMenuItem(Mod::Scale, "Scale...", &Model::Scale);
	
	AppendSeparator();
	AppendMenuItem(Mod::Properties, "Properties...", &Model::Properties);
}

inline void Model::AppendItem(wxString Name)
{
	wxMenuItem* Item = new wxMenuItem(this, wxID_ANY, Name);
	Append(Item);
}

inline wxMenuItem* Model::AppendMenuItem(int ID, wxString&& Name,
	void(Context::Model::* Method)(wxCommandEvent&), wxString Description)
{
	wxMenuItem* item = new wxMenuItem(NULL, ID, Name, Description);
	Append(item);
	Bind(wxEVT_MENU, Method, this, ID);
	return item;
}

void Model::Delete(wxCommandEvent&)
{
	core[index].RemoveFromTree();
	core.remove(index);
}

void Model::Translation(wxCommandEvent&)
{
	auto& model = core[index];
	vertex t = model.GetTranslation();
	auto degree = model.GetDegreeOfFreedom();

	std::list<changeLambda<float>> changeLambdas;
	if (degree.x)
		changeLambdas.push_back({ "X", t.x, [&model](const float&& value) {
				model.Set(axis_t::x, value);
			} });
	if (degree.y)
		changeLambdas.push_back({ "Y", t.y, [&model](const float&& value) {
				model.Set(axis_t::y, value);
			} });
	if (degree.z)
		changeLambdas.push_back({ "Z", t.z, [&model](const float&& value) {
				model.Set(axis_t::z, value);
			} });
	auto dialog = new DialogLambda<float>(nullptr, wxGetMousePosition(), changeLambdas);
	dialog->Show();
}

void Model::Rotation(wxCommandEvent&)
{
	auto& model = core[index];
	vertex r = model.GetRotation();
	auto degree = model.GetDegreeOfFreedom();

	std::list<changeLambda<float>> changeLambdas;
	if(degree.alpha)
		changeLambdas.push_back({ "angle X", r.x, [&model](const float&& value) {
				model.Set(angles_t::x, value);
			} });
	if (degree.beta)
		changeLambdas.push_back({ "angle Y", r.y, [&model](const float&& value) {
				model.Set(angles_t::y, value);
			} });
	if (degree.gamma)
		changeLambdas.push_back({ "angle Z", r.z, [&model](const float&& value) {
				model.Set(angles_t::z, value);
			} });
	auto dialog = new DialogLambda<float>(nullptr, wxGetMousePosition(), changeLambdas);
	dialog->Show();
}

void Model::Scale(wxCommandEvent&)
{
	DialogValue* value = new DialogValue(nullptr, wxGetMousePosition(), {
		{"Scale ", &core[index].Scale} });
	value->Show();
}

void Model::Properties(wxCommandEvent&)
{
	auto win = new ModelProperties(nullptr, core[index]);
	win->Show();
}
