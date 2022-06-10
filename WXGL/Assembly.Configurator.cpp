#include "Assemly.Configurator.hpp"
using namespace Assembly;

Configurator::Configurator(wxString ModelName)
	: wxMenu(ModelName)
{
	AppendItem("Set Active");
	AppendItem("Lock");
	AppendSeparator();
	AppendItem("Hide");
	AppendItem("Show");
	AppendSeparator();
	AppendItem("Connect to...");
}

inline void Configurator::AppendItem(wxString Name)
{
	wxMenuItem* Item = new wxMenuItem(this, wxID_ANY, Name);
	Append(Item);
}