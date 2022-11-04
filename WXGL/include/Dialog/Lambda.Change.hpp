#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <Common/PaddingSizer.hpp>

#include <list>
#include <functional>
#include <string>

template<class type>
struct changeLambda {
	const char* description;
	type initalvalue;
	std::function<void(const type&&)> action;
};


template<class type>
class DialogLambda : public wxFrame
{
private:
	wxTextCtrl* TextField(changeLambda<type>&& cl) 
	{
		auto text = new wxTextCtrl(this, wxID_ANY, std::to_string(cl.initalvalue));
		text->SetWindowStyle(wxTE_PROCESS_ENTER);
		text->Bind(wxEVT_TEXT, [cl, text](wxCommandEvent&) {
			try {
				cl.action(std::atof(text->GetValue().mb_str().data()));
			}
			catch (const std::exception&) {}
			});
		text->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&) { Close(); });
		return text;
	}


	void CloseOnEnterInit() {
		Bind(wxEVT_KEY_DOWN, [this](wxKeyEvent& evt) {
			if (evt.GetKeyCode() == WXK_RETURN)
				Close();
		});
	}


	wxStaticText* Label(const wxString& text) {
		return new wxStaticText(this, wxID_ANY, text);
	}
public:
	DialogLambda(wxFrame* Host, wxPoint mousepos, std::list<changeLambda<type>> values)
		: wxFrame(Host, wxID_ANY, wxT("Redact"), mousepos)
	{
		SetBackgroundColour({ 0xFF, 0xFF, 0xFF });

		auto sizer = new PaddingSizer(5, wxVERTICAL);
		for (auto cl : values)
			sizer->AddNonStretched(new PaddingSizer(10, {
				Label(cl.description),
				TextField(std::move(cl)) 
			}));
		SetSizer(sizer);
		Fit();

		CloseOnEnterInit();
	}
};