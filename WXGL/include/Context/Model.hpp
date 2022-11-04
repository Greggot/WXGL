#pragma once
#include <Dialog/Value.Change.hpp>
#include <Dialog/Lambda.Change.hpp>
#include <Dialog/Model.Property.hpp>

#include <Assembly/Core.hpp>
#include <Assembly/DependencyTree.hpp>

#include "ID.hpp"

namespace Context
{
	class Model : public wxMenu
	{
	private:
		void Delete(wxCommandEvent&);
		void Translation(wxCommandEvent&);
		void Rotation(wxCommandEvent&);
		void Scale(wxCommandEvent&);
		
		void Properties(wxCommandEvent&);
	protected:
		uint16_t index;
		Assembly::Core& core;

		inline void AppendItem(wxString Name);
		inline wxMenuItem* AppendMenuItem(int ID, wxString&& Name,
			void(Context::Model::* Method)(wxCommandEvent&), wxString Description = "");
	public:
		Model(uint16_t index, Assembly::Core& core);
	};
}

#include <Common/MovablePanel.hpp>
#include <SkyBlue/Client.API.hpp>
#include <UDP/server.hpp>
#include <PhysicalDevice/RGB565.hpp>

using PhysicalDevice::RGB565;

class ThreadHolder
{
private:
	SkyBlue::Device& api;
	const SkyBlue::ID camid { 0, SkyBlue::type_t::camera };
	SkyBlue::Module* cam;
	ImagePanel* image;

	std::thread udp;
	bool isRunning = true;

	RGB565* imgdata = new RGB565[320 * 240];

	void StartMain();
public:
	ThreadHolder(SkyBlue::Device& api, ImagePanel* image);
	~ThreadHolder();
};

namespace Context
{
	class Camera : public Model
	{
	private:
		void ShowInit(SkyBlue::Device& api);
	public:
		Camera(uint16_t index, Assembly::Core& core, SkyBlue::Device& api);
	};
}