#include <Dialog/API.Connect.hpp>
using namespace Dialog;

APIconnect::APIconnect(wxWindow* Host, wxString title,
	SkyBlue::Device& device, SkyBlue::APIPanel* panel, wxSizer* sizerToUpdate) :
	wxFrame(Host, wxID_ANY, title), device(device), panel(panel)
{
	SetBackgroundColour({ 0xFF, 0xFF, 0xFF });

	iptext = new wxTextCtrl(this, wxID_ANY);
	porttext = new wxTextCtrl(this, wxID_ANY);
	OKinit(sizerToUpdate);

	SizerInit();
}

void APIconnect::OKinit(wxSizer* sizerToUpdate)
{
	ok = new wxButton(this, wxID_ANY, "OK", wxDefaultPosition, wxSize(75, 25));
	ok->Bind(wxEVT_BUTTON, [this, sizerToUpdate](wxCommandEvent&) {
		auto IP = iptext->GetValue();
		auto Port = porttext->GetValue();
		TCP::Address address{ IP.mb_str().data(), Port.mb_str().data() };
		device.connect(&address);

		FillUpPanel();
		sizerToUpdate->Layout();
		Close();
	});
}

void APIconnect::SizerInit() 
{
	auto sizer = new PaddingSizer(10, wxVERTICAL);
	
	auto ippanel = new PaddingSizer(10, { new wxStaticText(this, wxID_ANY, "IP"), iptext });
	auto portpanel = new PaddingSizer(10, { new wxStaticText(this, wxID_ANY, "Port"), porttext });
	auto oksizer = new PaddingSizer(10, { ok });

	sizer->AddNonStretched(ippanel);
	sizer->AddNonStretched(portpanel);
	sizer->Add(oksizer, 0, wxALIGN_RIGHT);

	SetSizer(sizer);
	Fit();
}

void APIconnect::FillUpPanel() 
{
	auto result = device.report();
	for (const SkyBlue::ID id : result)
		panel->Apply(id);
	panel->Report();
}
