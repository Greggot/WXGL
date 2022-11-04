#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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

	void StartMain() 
	{
		cam = new SkyBlue::Module();

		cam->setWrite([this](const void*, unsigned int) {
			if (!isRunning)
				return;
			for (int i = 0; i < 240; ++i)
			{
				for (int j = 0; j < 320; ++j)
				{
					const RGB565 color = imgdata[i * 320 + j];
					image->Set(j, i, color.red8(), color.green8(), color.blue8());
				}
			}
			image->Update();
			api.write(camid, nullptr, 0);
		});

		api.add(camid, cam);
		api.listen();
		api.write(camid, nullptr, 0);
	}
public:
	ThreadHolder(SkyBlue::Device& api, ImagePanel* image) : api(api), image(image) {
		udp = std::thread([this] {
			UDP::server server;
			server.Start({ {192, 168, 1, 134}, 5555 });

			struct position {
				uint32_t start;
				uint32_t length;
			};
			position pos;
			uint8_t buffer[1290];
			while (isRunning)
			{
				server.Receive(buffer, 1290);
				memcpy(&pos, buffer, sizeof(pos));
				memcpy(&imgdata[pos.start / sizeof(RGB565)], buffer + sizeof(pos), pos.length);
			}
		});
		StartMain();
	}

	~ThreadHolder() {
		api.deaf();
		api.remove(camid);
		isRunning = false;
		udp.join();

		delete[] imgdata;
	}
};

namespace Context
{
	class Camera : public Model
	{
	private:
		void ShowInit(SkyBlue::Device& api) {
			wxMenuItem* item = new wxMenuItem(NULL, Mod::Camera, "Camera View...");
			Append(item);
			Bind(wxEVT_MENU, [&api, this](wxCommandEvent& e) {
				auto image = new ImagePanel(nullptr, { 0x42, 0x87,0xF5 }, "CAM 0", 320, 240);
				image->Show();

				ThreadHolder* holder = new ThreadHolder(api, image);
				image->Bind(wxEVT_CLOSE_WINDOW, [holder](wxCloseEvent& e) {
					delete holder;
					e.Skip();
				});
			}, Mod::Camera);
		}
	public:
		Camera(uint16_t index, Assembly::Core& core, SkyBlue::Device& api)
			: Model(index, core)
		{
			AppendSeparator();

			ShowInit(api);
		}
	};
}