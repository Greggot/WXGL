#include <Assembly/Configurator.hpp>
using namespace Assembly;

Configurator::Configurator(uint16_t index, Core& core)
	: index(index), core(core), wxMenu(core[index].Name)
{
	AppendItem("Lock...");	// Window changing degrees of freedom
	AppendMenuItem(ConfiguratorID::Delete, "Delete", "", &Configurator::Delete);
	AppendSeparator();

	AppendMenuItem(ConfiguratorID::Coordinates, "Coordinates...", "", &Configurator::Translation);
	AppendMenuItem(ConfiguratorID::Angles, "Angles...", "", &Configurator::Rotation);
	AppendMenuItem(ConfiguratorID::Scale, "Scale...", "", &Configurator::Scale);
	AppendSeparator();
	
	AppendItem("Connect to...");	// Window for Desktop-Controller API
}

inline void Configurator::AppendItem(wxString Name)
{
	wxMenuItem* Item = new wxMenuItem(this, wxID_ANY, Name);
	Append(Item);
}


inline wxMenuItem* Configurator::AppendMenuItem(int ID, wxString Name, wxString Description,
	void(Assembly::Configurator::* Method)(wxCommandEvent&))
{
	wxMenuItem* item = new wxMenuItem(NULL, ID, Name, Description, wxITEM_NORMAL, NULL);
	Append(item);
	Bind(wxEVT_MENU, Method, this, ID);
	return item;
}

void Configurator::Delete(wxCommandEvent&)
{
	core[index].RemoveFromTree();
	core.remove(index);
}

void Configurator::Translation(wxCommandEvent&)
{
	DialogValue* value = new DialogValue(nullptr, wxGetMousePosition(), {
		{"X", &core[index].Translation.x}, {"Y", &core[index].Translation.y}, {"Z", &core[index].Translation.z} });
	value->Show();
}

void Configurator::Rotation(wxCommandEvent&)
{
	DialogValue* value = new DialogValue(nullptr, wxGetMousePosition(), {
		{"angle X", &core[index].Rotation.x}, {"angle Y", &core[index].Rotation.y}, {"angle Z", &core[index].Rotation.z} });
	value->Show();
}
void Configurator::Scale(wxCommandEvent&)
{
	DialogValue* value = new DialogValue(nullptr, wxGetMousePosition(), {
		{"Scale ", &core[index].Scale} });
	value->Show();
}