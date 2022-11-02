#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <SkyBlue/Client.API.hpp>
#include <Common/API.Server.Panel.hpp>

namespace Dialog
{
	class APIconnect : public wxFrame
	{
	private:
		SkyBlue::TCPclientAPI& api;
		SkyBlue::APIPanel* panel;
		wxTextCtrl* iptext;
		wxTextCtrl* porttext;
		wxButton* ok;

		void OKinit(wxSizer*);

		void SizerInit();
		void FillUpPanel();
		void SetCorrectWindowSize();
	public:
		APIconnect(wxWindow* Host, wxString title,
			SkyBlue::TCPclientAPI& api, SkyBlue::APIPanel* panel, wxSizer* sizerToUpdate);
	};
}