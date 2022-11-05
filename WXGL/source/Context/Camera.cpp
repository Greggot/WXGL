#include <Context/Model.hpp>
using namespace Context;
void ThreadHolder::StartMain()
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

ThreadHolder::ThreadHolder(SkyBlue::Device& api, ImagePanel* image) : api(api), image(image) {
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

ThreadHolder::~ThreadHolder() {
	api.deaf();
	api.remove(camid);
	isRunning = false;
	udp.join();

	delete[] imgdata;
}

void Camera::ShowInit(SkyBlue::Device& api) {
	wxMenuItem* item = new wxMenuItem(NULL, Mod::CameraWin, "Camera View...");
	Append(item);
	Bind(wxEVT_MENU, [&api, this](wxCommandEvent& e) {
		auto image = new ImagePanel(nullptr, { 0x42, 0x87,0xF5 }, "CAM 0", 320, 240);
		image->Show();

		ThreadHolder* holder = new ThreadHolder(api, image);
		image->Bind(wxEVT_CLOSE_WINDOW, [holder](wxCloseEvent& e) {
			delete holder;
			e.Skip();
			});
		}, Mod::CameraWin);
}

Camera::Camera(uint16_t index, Assembly::DependencyTree& Tree, SkyBlue::Device& api)
	: Model(index, Tree)
{
	AppendSeparator();
	ShowInit(api);
}
