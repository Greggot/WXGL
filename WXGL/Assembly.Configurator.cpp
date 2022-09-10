#include "Assemly.Configurator.hpp"
using namespace Assembly;

Configurator::Configurator(BaseModel& model)
	: model(model), wxMenu(model.Name)
{
	AppendItem("Set Active");
	AppendItem("Lock");
	AppendSeparator();
	AppendItem("Hide");
	AppendItem("Show");
	AppendSeparator();
	AppendItem("Coordinates");
	AppendItem(wxString::Format("%.2f", model.Translation.x));
	AppendItem(wxString::Format("%.2f", model.Translation.y));
	AppendItem(wxString::Format("%.2f", model.Translation.z));
	AppendItem("Angles");
	AppendItem(wxString::Format("%.2f", model.Rotation.x));
	AppendItem(wxString::Format("%.2f", model.Rotation.y));
	AppendItem(wxString::Format("%.2f", model.Rotation.z));
}

inline void Configurator::AppendItem(wxString Name)
{
	wxMenuItem* Item = new wxMenuItem(this, wxID_ANY, Name);
	Append(Item);
}