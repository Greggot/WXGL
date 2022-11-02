#include <Dialog/API.Connect.hpp>
using namespace Dialog;

APIconnect::APIconnect(wxWindow* Host, wxString title,
	SkyBlue::TCPclientAPI& api, SkyBlue::APIPanel* panel, wxSizer* sizerToUpdate) :
	wxFrame(Host, wxID_ANY, title), api(api), panel(panel)
{
	SetBackgroundColour({ 0xFF, 0xFF, 0xFF });

	iptext = new wxTextCtrl(this, wxID_ANY);
	porttext = new wxTextCtrl(this, wxID_ANY);
	OKinit(sizerToUpdate);

	SizerInit();
	SetCorrectWindowSize();
}

void APIconnect::OKinit(wxSizer* sizerToUpdate)
{
	ok = new wxButton(this, wxID_ANY, "OK", wxDefaultPosition, wxSize(75, 25));
	ok->Bind(wxEVT_BUTTON, [this, sizerToUpdate](wxCommandEvent&) {
		auto IP = iptext->GetValue();
		auto Port = porttext->GetValue();
		this->api.connect({ IP.mb_str().data(), Port.mb_str().data() });

		FillUpPanel();
		sizerToUpdate->Layout();
		Close();
	});
}

void APIconnect::SizerInit() 
{
	auto sizer = new wxFlexGridSizer(3, 2, wxSize(10, 10));
	sizer->Add(new wxStaticText(this, wxID_ANY, "IP"), 0, wxSHRINK, 10);
	sizer->Add(iptext, 0, wxEXPAND, 20);

	sizer->Add(new wxStaticText(this, wxID_ANY, "Port"), 0, wxSHRINK, 10);
	sizer->Add(porttext, 0, wxEXPAND, 20);

	sizer->AddSpacer(0);
	sizer->Add(ok, 0, wxRIGHT, 10);

	sizer->AddGrowableCol(1);
	sizer->AddGrowableRow(0);
	sizer->AddGrowableRow(1);
	
	SetSizer(sizer);
}

void APIconnect::FillUpPanel() 
{
	auto result = this->api.report();
	for (const SkyBlue::ID id : result)
		panel->Apply(id);
	panel->Report();
}

void APIconnect::SetCorrectWindowSize()
{
	Fit();
	auto size = GetSize();
	SetSize({ size.x + 20, size.y + 20 });
}