#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <initializer_list>

class PaddingSizer : public wxBoxSizer
{
private:
	const unsigned int pad;
	
	template<class UI>
	void Init(const std::initializer_list<UI>& elements) {
		AddSpacer(pad);
		if (elements.size() == 0)
			return;

		AddStatic(*elements.begin());
		if (elements.size() == 1)
			return;

		for (auto win = elements.begin() + 1; win != elements.end(); ++win)
			AddStretched(*win);
	}
public:
	PaddingSizer(unsigned int pad, std::initializer_list<wxWindow*> elements = {}, wxOrientation ori = wxHORIZONTAL)
		: wxBoxSizer(ori), pad(pad)
	{
		Init(elements);
	}

	PaddingSizer(unsigned int pad, std::initializer_list<wxSizer*> elements = {}, wxOrientation ori = wxHORIZONTAL)
		: wxBoxSizer(ori), pad(pad)
	{
		Init(elements);
	}

	PaddingSizer(unsigned int pad, wxOrientation ori = wxHORIZONTAL)
		: wxBoxSizer(ori), pad(pad)
	{
	}

	template<class UI>
	void AddStatic(UI win) {
		Add(win, 0);
		AddSpacer(pad);
	}

	template<class UI>
	void AddNonStretched(UI win) {
		Add(win, 0, wxEXPAND);
		AddSpacer(pad);
	}

	template<class UI>
	void AddStretched(UI win) {
		Add(win, 1, wxEXPAND);
		AddSpacer(pad);
	}
};