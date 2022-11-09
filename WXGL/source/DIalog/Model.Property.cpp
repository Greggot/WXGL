#include <Dialog/Model.Property.hpp>

ModelProperties::ModelProperties(wxWindow* host, DrawableModel& model)
	: wxFrame(host, wxID_ANY, model.Name + " properties", wxGetMousePosition()),
	model(model)
{
	SetBackgroundColour({ 0xFF, 0xFF, 0xFF });
	vertical = new PaddingSizer(5, wxVERTICAL);

	TextVariablesInit();
	CheckboxesInit();
	IDchangeInit();
	OkInit();

	SetSizer(vertical);
	Fit();
}


wxStaticText* ModelProperties::Label(wxString&& text) { 
	return new wxStaticText(this, wxID_ANY, text); 
}
wxCheckBox* ModelProperties::Checkbox(wxString&& text) { 
	return new wxCheckBox(this, wxID_ANY, text); 
}

void ModelProperties::TextVariablesInit()
{
	auto pathtxt = new wxTextCtrl(this, wxID_ANY, model.Path);
	pathtxt->SetWindowStyle(wxTE_READONLY);
	auto nametxt = new wxTextCtrl(this, wxID_ANY, model.Name);

	auto pathsizer = new PaddingSizer(10, { Label("Path"), pathtxt });
	auto namesizer = new PaddingSizer(10, { Label("Name"), nametxt });
	
	vertical->AddNonStretched(pathsizer);
	vertical->AddNonStretched(namesizer);
	vertical->AddStretchSpacer(1);
}

void ModelProperties::CheckboxesInit()
{
	const auto& degree = model.GetDegreeOfFreedom();
	checks[0] = Checkbox("X");
	checks[1] = Checkbox("Y");
	checks[2] = Checkbox("Z");
	checks[3] = Checkbox("X");
	checks[4] = Checkbox("Y");
	checks[5] = Checkbox("Z");

	for (int i = 0; i < 6; ++i)
		checks[i]->SetValue(degree.raw & (1 << i));

	auto sizer = new PaddingSizer(10, { Label("Linear DoF") });
	for (int i = 0; i < 3; ++i)
		sizer->AddStretched(checks[i]);
	auto anglesizer = new PaddingSizer(10, { Label("Angle DoF") });
	for (int i = 3; i < 6; ++i)
		anglesizer->AddStretched(checks[i]);

	vertical->AddNonStretched(sizer);
	vertical->AddNonStretched(anglesizer);
	vertical->AddStretchSpacer(1);
}

void ModelProperties::IDchangeInit()
{
	const auto& id = model.getID();
	auto descriptions = SkyBlue::TypeDescriptions();

	wxArrayString choises{ static_cast<int>(SkyBlue::type_t::amount), SkyBlue::TypeDescriptions() };
	descrs = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, choises, wxCB_READONLY);
	descrs->SetSelection(descrs->FindString(SkyBlue::typeToString(id.type)));

	idnumber = new wxTextCtrl(this, wxID_ANY, std::to_string(id.number));

	auto sizer = new PaddingSizer(10, { Label("Type") , descrs });
	sizer->AddStatic(Label("ID"));
	sizer->AddStretched(idnumber);

	vertical->AddNonStretched(sizer);
	vertical->AddStretchSpacer(1);
}

void ModelProperties::OkInit()
{
	auto ok = new wxButton(this, wxID_ANY, "OK");
	ok->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		DegreeOfFreedom degree{ 0 };
		for (int i = 0; i < 6; ++i)
			degree.raw |= (checks[i]->GetValue() << i);
		((ModuleModel&)model).Set(degree);

		auto id = model.getID();
		id.number = std::atoi(idnumber->GetValue().mb_str().data());
		model.Change({ id.number, static_cast<SkyBlue::type_t>(descrs->GetSelection()) });

		Close();
	});
	
	vertical->Add(ok, 0, wxALIGN_CENTER_HORIZONTAL);
}
