#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Common/DialogValue.hpp>
#include "Core.hpp"

enum ConfiguratorID
{
	Delete = 7,
	Coordinates,
	Angles,
	Scale
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
	public:
		Configurator(uint16_t index, Core& core);
	};
}