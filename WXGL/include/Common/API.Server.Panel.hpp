#pragma once
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
	class ModuleUI : public wxPanel, public Module
	{
	private:
		DrawableModel* model;
	public:
		ModuleUI(wxWindow* Host, wxString&& name);

		DrawableModel* Model() const { return model; }
		void SetModel(DrawableModel* m) { model = m; }
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
	class APIPanel : public wxPanel, public Device
	{
	private:
		Assembly::Core& core;
		PaddingSizer* sizer;

		DrawableModel* FindModelWith(ID id);
	public:
		APIPanel(wxWindow* Host, Assembly::Core& core);
		
		void Apply(ID id);
		void Update();

		void clear() override;

		void Report();
	};
}