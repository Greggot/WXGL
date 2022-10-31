#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include <initializer_list>
#include <string>
#include <list>

template<class type>
struct changeLambda {
	const char* description;
	type initalvalue;
	std::function<void(const type&&)> action;
};

template<class type>
class DialogLambda : public wxFrame
{
public:
	DialogLambda(wxFrame* Host, wxPoint mousepos, std::list<changeLambda<type>> values)
		: wxFrame(Host, wxID_ANY, wxT("Redact"), mousepos, wxSize(270, 150))
	{
		SetBackgroundColour({ 0xFF, 0xFF, 0xFF });

		auto sizer = new wxGridSizer(values.size(), 2, 10, 10);
		for (auto cv : values)
		{
			auto text = new wxTextCtrl(this, wxID_ANY, std::to_string(cv.initalvalue));
			text->SetWindowStyle(wxTE_PROCESS_ENTER);
			text->Bind(wxEVT_TEXT, [cv, text](wxCommandEvent&) {
				try {
					cv.action(std::atof(text->GetValue().mb_str().data()));
				}
				catch (const std::exception&) {}
			});
			text->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&) { Close(); });

			sizer->Add(new wxStaticText(this, wxID_ANY, cv.description), 1, wxEXPAND | wxALL);
			sizer->Add(text);
		}
		SetSizer(sizer);
		Fit();
		auto size = GetSize();
		SetSize({ size.x + 20, size.y + 20 });
	}
};

struct changeValue
{
	const char* description;
	float* value;
};

class DialogValue : public wxFrame
{
public:
	DialogValue(wxFrame* Host, wxPoint&& mousepos, std::initializer_list<changeValue> values)
		: wxFrame(Host, wxID_ANY, wxT("Redact"), mousepos, wxSize(270, 150))
	{
		SetBackgroundColour({ 0xFF, 0xFF, 0xFF });
		auto sizer = new wxGridSizer(values.size(), 2, 10, 10);
		for (auto cv : values)
		{
			auto text = new wxTextCtrl(this, wxID_ANY, std::to_string(*cv.value));
			text->SetWindowStyle(wxTE_PROCESS_ENTER);
			text->Bind(wxEVT_TEXT, [cv, text](wxCommandEvent&) {
				try {
					*cv.value = std::atof(text->GetValue().mb_str().data());
				}
				catch(const std::exception&){}
			});
			text->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&) { Close(); });

			sizer->Add(new wxStaticText(this, wxID_ANY, cv.description), 1, wxEXPAND);
			sizer->Add(text);
		}
		SetSizer(sizer);
		Fit();
		auto size = GetSize();
		SetSize({ size.x + 20, size.y + 20 });

		Bind(wxEVT_KEY_DOWN, [this](wxKeyEvent& evt) {
			if (evt.GetKeyCode() == WXK_RETURN)
				Close();
		});
	}
};