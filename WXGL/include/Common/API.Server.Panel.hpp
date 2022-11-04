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
	class ModuleUI : public wxPanel
	{
	private:
		const ID id;
	public:
		ModuleUI(wxWindow* Host, ID id);
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

		DrawableModel* FindModelWith(ID id);
	public:
		APIPanel(wxWindow* Host, Assembly::Core& core);
		
		void Apply(ID id);
		void Update();
		void Clear();

		void Report();
	};
}