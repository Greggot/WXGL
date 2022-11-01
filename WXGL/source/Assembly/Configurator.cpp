#include <Assembly/Configurator.hpp>
using namespace Assembly;

Configurator::Configurator(uint16_t index, Core& core)
	: index(index), core(core), wxMenu(core[index].Name)
{
	AppendMenuItem(ConfiguratorID::Delete, "Delete", "", &Configurator::Delete);
	AppendSeparator();

	const auto degree = core[index].GetDegreeOfFreedom();

	if(degree.raw & 0b111)
		AppendMenuItem(ConfiguratorID::Coordinates, "Coordinates...", "", &Configurator::Translation);
	if(degree.raw & 0b111000)
		AppendMenuItem(ConfiguratorID::Angles, "Angles...", "", &Configurator::Rotation);
	AppendMenuItem(ConfiguratorID::Scale, "Scale...", "", &Configurator::Scale);
	AppendSeparator();
	AppendMenuItem(ConfiguratorID::Test, "Camera View...", "", &Configurator::ShowCamera);

	AppendSeparator();
	AppendMenuItem(ConfiguratorID::Properties, "Properties", "", &Configurator::Properties);
}

inline void Configurator::AppendItem(wxString Name)
{
	wxMenuItem* Item = new wxMenuItem(this, wxID_ANY, Name);
	Append(Item);
}


inline wxMenuItem* Configurator::AppendMenuItem(int ID, wxString Name, wxString Description,
	void(Assembly::Configurator::* Method)(wxCommandEvent&))
{
	wxMenuItem* item = new wxMenuItem(NULL, ID, Name, Description);
	Append(item);
	Bind(wxEVT_MENU, Method, this, ID);
	return item;
}

void Configurator::ShowCamera(wxCommandEvent&)
{
	static TCP::client client;
	static SkyBlue::TCPclientAPI api;

	auto win = new PhysicalDevice::CameraWindow(nullptr, 320, 240, api);
	win->Show();
}

void Configurator::Delete(wxCommandEvent&)
{
	core[index].RemoveFromTree();
	core.remove(index);
}

void Configurator::Translation(wxCommandEvent&)
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

void Configurator::Rotation(wxCommandEvent&)
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

void Configurator::Scale(wxCommandEvent&)
{
	DialogValue* value = new DialogValue(nullptr, wxGetMousePosition(), {
		{"Scale ", &core[index].Scale} });
	value->Show();
}

void Configurator::Properties(wxCommandEvent&)
{
	auto win = new ModelProperties(nullptr, core[index]);
	win->Show();
}
