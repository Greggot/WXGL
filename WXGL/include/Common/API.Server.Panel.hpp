#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <SkyBlue/Client.API.hpp>
#include <Assembly/Core.hpp>
#include <map>

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

		const ID& getID() { return id; }
	};

	class APIPanel : public wxPanel
	{
	private:
		Assembly::Core& core;
		std::map<ModuleUI*, DrawableModel*> modules;
		wxBoxSizer* sizer;

		DrawableModel* FindModelWith(ID id) {
			for (auto model : core)
				if (model->getID() == id)
					return model;
			return nullptr;
		}
	public:
		APIPanel(wxWindow* Host, Assembly::Core& core) 
			: wxPanel(Host), core(core) {
			sizer = new wxBoxSizer(wxHORIZONTAL);
			SetSizer(sizer);
		}

		void Apply(ID id) {
			auto mod = new ModuleUI(this, id);
			modules.insert({ mod, FindModelWith(id)});
			sizer->Add(mod);
		}

		void Update() {
			DrawableModel* ptr = nullptr;
			for (auto& pair : modules)
				pair.second = FindModelWith(pair.first->getID());
		}

		void Report() {
			wxString overallReport;
			for (const auto& pair : modules)
			{
				auto const id = pair.first->getID();
				wxString idstring = wxString::Format("%s(%u) - ", typeToString(id.type), id.number);
				wxString model = pair.second ? pair.second->Name : "Not Found";
				overallReport += idstring + model + "\n";
			}
			wxMessageBox(overallReport);
		}

		void Clear() {
			sizer->Clear();
			modules.clear();
		}
	};
}