#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <PhysicalDevice/CameraWindow.hpp>
#include <Dialog/Values.Change.hpp>
#include <Dialog/Model.Property.hpp>

#include "Core.hpp"
#include "DependencyTree.hpp"

enum ConfiguratorID
{
	Delete = 7,
	Coordinates,
	Angles,
	Scale,
	Properties,
};

namespace Assembly
{
	class Configurator : public wxMenu
	{
	private:
		void Delete(wxCommandEvent&);
		void Translation(wxCommandEvent&);
		void Rotation(wxCommandEvent&);
		void Scale(wxCommandEvent&);
		
		void Properties(wxCommandEvent&);
	protected:
		uint16_t index;
		Core& core;

		inline void AppendItem(wxString Name);
		inline wxMenuItem* AppendMenuItem(int ID, wxString&& Name,
			void(Assembly::Configurator::* Method)(wxCommandEvent&), wxString Description = "");
	public:
		Configurator(uint16_t index, Core& core);
	};
}