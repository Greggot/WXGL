#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <PhysicalDevice/CameraWindow.hpp>
#include <Common/DialogValue.hpp>
#include <Common/Dialog.ModelProperty.hpp>

#include "Core.hpp"

enum ConfiguratorID
{
	Delete = 7,
	Coordinates,
	Angles,
	Scale,
	Test,
	Properties
};

namespace Assembly
{
	class Configurator : public wxMenu
	{
	private:
		inline void AppendItem(wxString Name);
		uint16_t index;
		Core& core;

		inline wxMenuItem* AppendMenuItem(int ID, wxString Name, wxString Description,
			void(Assembly::Configurator::* Method)(wxCommandEvent&));

		void Delete(wxCommandEvent&);
		void Translation(wxCommandEvent&);
		void Rotation(wxCommandEvent&);
		void Scale(wxCommandEvent&);
		
		void ShowCamera(wxCommandEvent&);
		void Properties(wxCommandEvent&);
	public:
		Configurator(uint16_t index, Core& core);
	};
}