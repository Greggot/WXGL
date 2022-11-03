#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif 

#include <thread>
#include <string>
#include <stdexcept>

#include <SkyBlue/Client.API.hpp>
#include <UDP/server.hpp>
#include <wx/mstream.h>

#include "RGB565.hpp"

namespace PhysicalDevice
{
	class CameraWindow : public wxFrame
	{
	private:
		wxImage shot;
		RGB565* data;
		uint8_t* buffer;


		const int width;
		const int height;

		SkyBlue::Device& api;
		UDP::server server;

		bool isRunning = true;
		std::thread main;
		std::thread udp;

		void TakePicture();
	public:
		CameraWindow(wxFrame* host, int width, int height, SkyBlue::Device& api);

		~CameraWindow();
	};
}