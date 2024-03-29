#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Model/BaseModel.hpp>
#include <Common/PaddingSizer.hpp>

class ModelProperties : public wxFrame
{
private:
	DrawableModel& model;
	PaddingSizer* vertical;
	wxCheckBox* checks[6];
	wxComboBox* descrs;

	wxStaticText* Label(wxString&& text);
	wxCheckBox* Checkbox(wxString&& text);
	
	void TextVariablesInit();
	void CheckboxesInit();
	void IDchangeInit();
	void OkInit();
public:
	ModelProperties(wxWindow* host, DrawableModel& model);
};