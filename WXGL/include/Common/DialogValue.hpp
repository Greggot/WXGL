#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include <initializer_list>
#include <string>

struct changeValue
{
	const char* description;
	float* value;
};

class DialogValue : public wxFrame
{
private:
	std::vector<wxTextCtrl*> textfields;
	std::vector<changeValue> valuefields;

	wxPoint position{ 10, 10 };
	const wxSize textsize{ 75, 25 };
	const wxSize labelsize{ 75, 25 };
	const int pand = 30;
public:
	DialogValue(wxFrame* Host, wxPoint&& mousepos, std::initializer_list<changeValue> values)
		: wxFrame(Host, wxID_ANY, wxT("Redact"), mousepos, wxSize(270, 150))
	{
		SetBackgroundColour({ 0xFF, 0xFF, 0xFF });
		for (auto cv : values)
		{
			new wxStaticText(this, wxID_ANY, cv.description, position, labelsize);
			auto text = new wxTextCtrl(this, wxID_ANY, std::to_string(*cv.value), {position.x + labelsize.x + 20, position.y}, textsize);
			text->Bind(wxEVT_TEXT, [cv, text](wxCommandEvent&) {
				try {
					*cv.value = std::atof(text->GetValue().mb_str().data());
				}
				catch(const std::exception&){}
			});
			position.y += pand;
		}
	}
};