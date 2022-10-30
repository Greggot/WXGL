#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <SkyBlue/Client.API.hpp>

namespace SkyBlue
{
	class ModuleUI : public wxPanel
	{
	private:
		const ID id;
	public:
		ModuleUI(wxWindow* Host, ID id) : wxPanel(Host), id(id) {
			new wxButton(this, wxID_ANY, wxString::Format("%s", typeToString(id.type)), wxDefaultPosition, wxSize(50, 50));
			SetBackgroundColour({ 0xFA, 0xDA, 0x45 });
		}
	};

	class APIPanel : public wxPanel
	{
	private:
		std::vector<ModuleUI*> modules;
		wxBoxSizer* sizer;
	public:
		APIPanel(wxWindow* Host) : wxPanel(Host) {
			sizer = new wxBoxSizer(wxHORIZONTAL);
			SetSizer(sizer);
		}

		void Apply(ID id) {
			auto mod = new ModuleUI(this, id);
			modules.push_back(mod);
			sizer->Add(mod);
		}

		void Clear() {
			sizer->Clear();
			modules.clear();
		}
	};
}