#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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
	wxTextCtrl* TextField(changeValue&&);

	void SetCorrectWindowSize();
	void CloseOnEnterInit();
public:
	DialogValue(wxFrame* Host, wxPoint&& mousepos, std::initializer_list<changeValue>);
};