#include <PhysicalDevice/CameraWindow.hpp>
using namespace PhysicalDevice;

FILE* f;
CameraWindow::CameraWindow(wxFrame* host, int width, int height, SkyBlue::clientAPI& api)
	: wxFrame(host, wxID_ANY, "Camera View", wxDefaultPosition, wxSize(width, height)), api(api),
	width(width), height(height)
{
	server.Start({ {192, 168, 1, 134}, 5555 });
	std::thread([this]() {
		struct position {
			uint32_t start;
			uint32_t length;
		};
		position pos;
		uint8_t buffer[1290];
		while (true) 
		{
			server.Receive(buffer, 1290);
			memcpy(&pos, buffer, sizeof(pos));
			memcpy(&data[pos.start / sizeof(RGBunit)], buffer + sizeof(pos), pos.length);
		}
	}).detach();

	std::thread([this]() {
		while (true)
		{
			TakePicture();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			Refresh();
		}
	}).detach();

	data = new RGBunit[width * height + 2];
	shot = wxImage(wxSize(width, height));

	f = fopen("log.log", "a");
	main = std::thread([width, height, this]() {
		while (isRunning)
			TakePicture();
	});

	Bind(wxEVT_PAINT, [this](wxPaintEvent&) {
		wxPaintDC* dc = new wxPaintDC(this);
		bm = wxBitmap(shot);
		dc->DrawBitmap(bm, wxPoint(0, 0));
	});
}

void CameraWindow::TakePicture() {
	static const SkyBlue::ID cameraID = { 0, SkyBlue::type_t::camera };
	api.write(cameraID, nullptr, 0);

	api.read([this](const SkyBlue::buffer&) {
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				const RGBunit color = data[i * width + j];
				shot.SetRGB(j, i, color.red8(), color.green8(), color.blue8());
			}
		}
		Refresh();
	});
}

CameraWindow::~CameraWindow() {
	isRunning = false;
	main.join();
	delete[] data;
}