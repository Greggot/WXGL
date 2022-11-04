#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <SkyBlue/Client.API.hpp>
#include <Assembly/Core.hpp>
#include <Common/PaddingSizer.hpp>
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

	/* TODO: Add inherited from ModuleUI class, which will handle camera type
	*	class CameraModulePanel : ModuleUI
	*	{
	*		CameraModulePanel()
	*			Change icon to something camera like
	*			Set wxMenu with "Camera view"
	*							"Passport"
	*	}
	* 
	*	class ServoModulePanel : ModuleUI
	*	{
	*		ServoModulePanel
	*			Change icon
	*			Set wxMenu with "Real time value"
	*							"Passport"
	*							"Properties"
	*	}
	* 
	*	class SDpanel : ModuleUI
	*	{
	*		SDpanel 
	*			Change icon
	*			Fileview
	*	}
	*
	*/

	class APIPanel : public wxPanel
	{
	private:
		Assembly::Core& core;
		std::map<ModuleUI*, DrawableModel*> modules;
		PaddingSizer* sizer;

		DrawableModel* FindModelWith(ID id) {
			for (auto model : core)
				if (model->getID() == id)
					return model;
			return nullptr;
		}
	public:
		APIPanel(wxWindow* Host, Assembly::Core& core) 
			: wxPanel(Host), core(core) {
			sizer = new PaddingSizer(5, wxHORIZONTAL);
			SetSizer(sizer);
		}

		void Apply(ID id) {
			auto mod = new ModuleUI(this, id);
			modules.insert({ mod, FindModelWith(id)});
			sizer->AddNonStretched(mod);
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