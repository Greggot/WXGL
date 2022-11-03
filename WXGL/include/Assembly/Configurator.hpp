#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Dialog/Value.Change.hpp>
#include <Dialog/Lambda.Change.hpp>
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

	Camera,
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

#include <Common/MovablePanel.hpp>
#include <SkyBlue/Client.API.hpp>
namespace Assembly
{
	class CameraConfigurator : public Configurator
	{
	private:
		SkyBlue::TCPclientAPI& api;
		ImagePanel* image;
	public:
		CameraConfigurator(uint16_t index, Core& core, SkyBlue::TCPclientAPI& api)
			: Configurator(index, core), api(api)
		{
			AppendSeparator();

namespace Assembly
{
	class CameraConfigurator : public Configurator
	{
	private:
		void ShowInit(SkyBlue::Device& api) {
			wxMenuItem* item = new wxMenuItem(NULL, ConfiguratorID::Camera, "Camera View...");
			Append(item);
			Bind(wxEVT_MENU, [&api, this](wxCommandEvent& e) {
				auto image = new ImagePanel(nullptr, { 0x42, 0x87,0xF5 }, "CAM 0", 320, 240);
				image->Show();
			}, ConfiguratorID::Camera);
		}
	public:
		CameraConfigurator(uint16_t index, Core& core, SkyBlue::Device& api)
			: Configurator(index, core)
		{
			AppendSeparator();

			ShowInit(api);
		}
	};
}