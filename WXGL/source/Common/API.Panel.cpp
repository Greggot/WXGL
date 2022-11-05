#include <Common/API.Server.Panel.hpp>
#include <Context/API.Panel.hpp>
using namespace SkyBlue;

ModuleUI::ModuleUI(wxWindow* Host, wxString&& name) : wxPanel(Host) {
	new wxButton(this, wxID_ANY, name, wxDefaultPosition, wxSize(50, 50));
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
	auto mod = new ModuleUI(this, typeToString(id.type));
	add(id, mod);
	sizer->AddNonStretched(mod);
}

void APIPanel::Update() 
{
	DrawableModel* ptr = nullptr;
	for (auto& pair : modules)
		((ModuleUI*)pair.second)->SetModel(FindModelWith(pair.first));

	for (const auto& pair : modules)
	{
		auto model = ((ModuleUI*)pair.second)->Model();
		if (model)
		{
			if (model->getID().type == type_t::rotorservo)
				model->SetRotation([pair, this](const vertex& r) {
					write(pair.first, &r, sizeof(r));
				});
		}
	}
}

void APIPanel::Report() 
{
	wxString overallReport;
	for (const auto& pair : modules)
	{
		auto const id = pair.first;
		wxString idstring = wxString::Format("%s(%u) - ", typeToString(id.type), id.number);
		auto model = ((ModuleUI*)pair.second)->Model();
		wxString modelname = model ? model->Name : "Not Found";
		overallReport += idstring + modelname + "\n";
	}
	wxMessageBox(overallReport);
}

void APIPanel::clear() 
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