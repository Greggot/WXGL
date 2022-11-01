#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Model/BaseModel.hpp>

class ModelProperties : public wxFrame
{
private:
	DrawableModel& model;
	wxBoxSizer* vertical;
	wxCheckBox* checks[6];
	wxComboBox* descrs;

	wxStaticText* Label(wxString&& text) { return new wxStaticText(this, wxID_ANY, text); }
	void TextVariablesInit()
	{
		auto sizer = new wxFlexGridSizer(2, 2, wxSize(10, 10));
		sizer->Add(Label("Path"));
		sizer->Add(new wxTextCtrl(this, wxID_ANY, model.Path), 1, wxEXPAND);
		sizer->Add(Label("Name"));
		sizer->Add(new wxTextCtrl(this, wxID_ANY, model.Name));

		sizer->AddGrowableCol(1);
		vertical->Add(sizer, 1, wxEXPAND);
	}

	wxCheckBox* Checkbox(wxString&& text) { return new wxCheckBox(this, wxID_ANY, text); }
	void CheckboxesInit() 
	{
		const auto& degree = model.GetDegreeOfFreedom();
		checks[0] = Checkbox("X");
		checks[1] = Checkbox("Y");
		checks[2] = Checkbox("Z");
		checks[3] = Checkbox("angle X");
		checks[4] = Checkbox("angle Y");
		checks[5] = Checkbox("angle Z");

		for (int i = 0; i < 6; ++i)
			checks[i]->SetValue(degree.raw & (1 << i));
		
		auto horizontal = new wxBoxSizer(wxHORIZONTAL);
		
		horizontal->Add(Label("Degrees of freedom"));
		for (const auto& check : checks)
			horizontal->Add(check, 1, wxEXPAND);
		
		vertical->Add(horizontal, 1, wxEXPAND);
	}

	void IDchangeInit()
	{
		const auto& id = model.getID();
		auto descriptions = SkyBlue::TypeDescriptions();

		wxArrayString choises{ static_cast<int>(SkyBlue::type_t::amount), SkyBlue::TypeDescriptions() };
		descrs = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, choises, wxCB_READONLY);
		descrs->SetSelection(descrs->FindString(SkyBlue::typeToString(id.type)));

		auto horizontal = new wxBoxSizer(wxHORIZONTAL);
		horizontal->Add(Label("Type"), 1, wxEXPAND);
		horizontal->Add(descrs);
		vertical->Add(horizontal, 0, wxEXPAND);
	}

	void OkInit()
	{
		auto ok = new wxButton(this, wxID_ANY, "OK");
		ok->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
			DegreeOfFreedom degree{0};
			for (int i = 0; i < 6; ++i)
				degree.raw |= (checks[i]->GetValue() << i);
			((ModuleModel&)model).Set(degree);

			auto id = model.getID();
			model.Change({ id.number, static_cast<SkyBlue::type_t>(descrs->GetSelection()) });

			Close();
		});
		vertical->Add(ok, 0, wxALIGN_CENTER_HORIZONTAL);
	}
public:
	ModelProperties(wxWindow* host, DrawableModel& model) 
		: wxFrame(host, wxID_ANY, model.Name + " properties", wxGetMousePosition()),
		model(model)
	{
		SetBackgroundColour({ 0xFF, 0xFF, 0xFF });
		vertical = new wxBoxSizer(wxVERTICAL);

		TextVariablesInit();
		CheckboxesInit();
		IDchangeInit();
		OkInit();

		SetSizer(vertical);
		Fit();
	}
};