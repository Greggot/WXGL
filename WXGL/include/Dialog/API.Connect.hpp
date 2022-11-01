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

		void SizerInit() {
			auto sizer = new wxFlexGridSizer(3, 2, wxSize(10, 10));
			sizer->Add(new wxStaticText(this, wxID_ANY, "IP"), 0, wxSHRINK, 10);
			sizer->Add(iptext, 1, wxEXPAND, 20);
			sizer->Add(new wxStaticText(this, wxID_ANY, "Port"), 0, wxSHRINK, 10);
			sizer->Add(porttext, 1, wxEXPAND, 20);
			sizer->AddSpacer(0);
			sizer->Add(ok, 0, wxRIGHT, 10);

			sizer->AddGrowableCol(1);
			sizer->AddGrowableRow(0);
			sizer->AddGrowableRow(1);
			SetSizer(sizer);
		}
	public:
		APIconnect(wxWindow* Host, wxString title, 
			SkyBlue::TCPclientAPI& api, SkyBlue::APIPanel* panel, wxSizer* updszr) :
				wxFrame(Host, wxID_ANY, title), api(api), panel(panel)
		{
			SetBackgroundColour({ 0xFF, 0xFF, 0xFF });

			iptext = new wxTextCtrl(this, wxID_ANY, "");
			porttext = new wxTextCtrl(this, wxID_ANY, "");
			ok = new wxButton(this, wxID_ANY, "OK", wxDefaultPosition, wxSize(75, 25));

			ok->Bind(wxEVT_BUTTON, [this, updszr, Host](wxCommandEvent&) {
				auto IP = iptext->GetValue();
				auto Port = porttext->GetValue();
				this->api.connect({ IP.mb_str().data(), Port.mb_str().data() });

				auto result = this->api.report();
				for (const SkyBlue::ID id : result)
					this->panel->Apply(id);
				updszr->Layout();
				Close();
			});

			SizerInit();
		}
	};
}