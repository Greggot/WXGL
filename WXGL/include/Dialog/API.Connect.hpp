#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <SkyBlue/Client.API.hpp>
#include <Common/API.Server.Panel.hpp>
#include <Common/PaddingSizer.hpp>

namespace Dialog
{
	class APIconnect : public wxFrame
	{
	private:
		SkyBlue::APIPanel* panel;
		wxTextCtrl* iptext;
		wxTextCtrl* porttext;
		wxButton* ok;

		void OKinit(wxSizer*);

		void SizerInit();
		void FillUpPanel();
	public:
		APIconnect(wxWindow* Host, wxString title,
			SkyBlue::APIPanel*, wxSizer* sizerToUpdate);
	};
}