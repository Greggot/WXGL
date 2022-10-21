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

namespace PhysicalDevice
{
	union RGBunit
	{
		uint16_t raw;
		struct
		{
			uint16_t red : 5;
			uint16_t green : 6;
			uint16_t blue : 5;
		};

		RGBunit(uint16_t raw = 0) { this->raw = raw; }
		RGBunit(uint8_t r, uint8_t g, uint8_t b) { red = r; green = g, blue = b; }

		uint8_t red8() const { return (red * 527 + 23) >> 6; }
		uint8_t green8() const { return (green * 259 + 33) >> 6; }
		uint8_t blue8() const { return (blue * 527 + 23) >> 6; }
	};

	class CameraWindow : public wxFrame
	{
	private:
		wxBitmap bm;
		wxImage shot;
		RGBunit* data;

		const int width;
		const int height;

		SkyBlue::clientAPI& api;
		UDP::server server;

		bool isRunning = true;
		std::thread main;

		void TakePicture();
	public:
		CameraWindow(wxFrame* host, int width, int height, SkyBlue::clientAPI& api);

		~CameraWindow();
	};
}