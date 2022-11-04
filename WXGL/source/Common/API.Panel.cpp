#include <Common/API.Server.Panel.hpp>
#include <Context/API.Panel.hpp>
using namespace SkyBlue;

ModuleUI::ModuleUI(wxWindow* Host, ID id) : wxPanel(Host), id(id) {
	new wxButton(this, wxID_ANY, wxString::Format("%s", typeToString(id.type)), wxDefaultPosition, wxSize(50, 50));
	SetBackgroundColour({ 0xFA, 0xDA, 0x45 });
}

APIPanel::APIPanel(wxWindow* Host, Assembly::Core& core)
	: wxPanel(Host), core(core) 
{
	sizer = new PaddingSizer(5, wxHORIZONTAL);
	SetSizer(sizer);

	Bind(wxEVT_RIGHT_DOWN, [this](wxMouseEvent& e) {
		auto menu = Context::API(this);
		PopupMenu(&menu, e.GetPosition());
	});
}

void APIPanel::Apply(ID id) 
{
	auto mod = new ModuleUI(this, id);
	modules.insert({ mod, FindModelWith(id) });
	sizer->AddNonStretched(mod);
}

void APIPanel::Update() 
{
	DrawableModel* ptr = nullptr;
	for (auto& pair : modules)
		pair.second = FindModelWith(pair.first->getID());
}

void APIPanel::Report() 
{
	wxString overallReport;
	for (const auto& pair : modules)
	{
		auto const id = pair.first->getID();
		wxString idstring = wxString::Format("%s(%u) - ", typeToString(id.type), id.number);
		wxString model = pair.second ? pair.second->Name : "Not Found";
		overallReport += idstring + model + "\n";
	}
	wxMessageBox(overallReport);
}

void APIPanel::Clear() 
{
	sizer->Clear();
	modules.clear();
}

DrawableModel* APIPanel::FindModelWith(ID id) {
	for (auto model : core)
		if (model->getID() == id)
			return model;
	return nullptr;
}