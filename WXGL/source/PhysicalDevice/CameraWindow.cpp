#include <PhysicalDevice/CameraWindow.hpp>
#include <Compression.hpp>
using namespace PhysicalDevice;

CameraWindow::CameraWindow(wxFrame* host, int width, int height, SkyBlue::clientAPI& api)
	: wxFrame(host, wxID_ANY, "Camera View", wxDefaultPosition, wxSize(width, height)), api(api),
	width(width), height(height)
{
	server.Start({ {192, 168, 1, 134}, 5555 });

	data = new RGB565[width * height];
	shot = wxImage(wxSize(width, height));

	// Main thread taking pictures as fast as possible
	main = std::thread([width, height, this]() {
		while (isRunning)
			TakePicture();
	});

	// UDP receive server
	udp = std::thread([this]() {
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
			memcpy(&data[pos.start / sizeof(RGB565)], buffer + sizeof(pos), pos.length);
		}
	});

	Bind(wxEVT_PAINT, [this](wxPaintEvent&) {
		wxPaintDC* dc = new wxPaintDC(this);
		dc->DrawBitmap(wxBitmap(shot), wxPoint(0, 0));
		delete dc;
	});
}

void CameraWindow::TakePicture() {
	static const SkyBlue::ID cameraID = { 0, SkyBlue::type_t::camera };
	api.write(cameraID, nullptr, 0);

	static uint32_t index = 0;
	api.read([this](const SkyBlue::buffer&) {
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				const RGB565 color = data[i * width + j];
				shot.SetRGB(j, i, color.red8(), color.green8(), color.blue8());
			}
		}
		Refresh();
	});
}

CameraWindow::~CameraWindow() {
	isRunning = false;
	main.join();
	udp.join();

	delete[] buffer;
	delete[] data;
}