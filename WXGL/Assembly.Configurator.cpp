#include "Assemly.Configurator.hpp"
using namespace Assembly;

inline wxMenuItem* Configurator::AppendMenuItem(int ID, wxString Name, wxString Description,
	void(Assembly::Configurator::* Method)(wxCommandEvent&), wxMenu* menu)
{
	wxMenuItem* item = new wxMenuItem(NULL, ID, Name, Description, wxITEM_NORMAL, NULL);
	menu->Append(item);
	Bind(wxEVT_MENU, Method, this, ID);
	return item;
}

Configurator::Configurator(uint16_t index, Core& core)
	: index(index), core(core), wxMenu(core[index].Name)
{
	AppendItem("Lock...");	// Window changing degrees of freedom
	AppendMenuItem(ConfiguratorID::Delete, "Delete", "", &Configurator::Delete);
	AppendSeparator();
	
	wxMenuItem* coor = new wxMenuItem(NULL, wxID_ANY, "Coordinates");
	wxMenu* coordinates = new wxMenu();
	coordinates->Append(new wxMenuItem(NULL, wxID_ANY, wxString::Format("%.2f", core[index].Translation.x)));
	coordinates->Append(new wxMenuItem(NULL, wxID_ANY, wxString::Format("%.2f", core[index].Translation.y)));
	coordinates->Append(new wxMenuItem(NULL, wxID_ANY, wxString::Format("%.2f", core[index].Translation.z)));
	coor->SetSubMenu(coordinates);
	
	wxMenuItem* ang = new wxMenuItem(NULL, wxID_ANY, "Angles");
	wxMenu* angles = new wxMenu();
	angles->Append(new wxMenuItem(NULL, wxID_ANY, wxString::Format("%.2f", core[index].Rotation.x)));
	angles->Append(new wxMenuItem(NULL, wxID_ANY, wxString::Format("%.2f", core[index].Rotation.y)));
	angles->Append(new wxMenuItem(NULL, wxID_ANY, wxString::Format("%.2f", core[index].Rotation.z)));
	ang->SetSubMenu(angles);

	Append(coor);
	Append(ang);

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

void Configurator::Delete(wxCommandEvent& event)
{
	core[index].RemoveFromTree();
	core.remove(index);
}