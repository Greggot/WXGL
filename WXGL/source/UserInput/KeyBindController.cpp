#include <UserInput/KeyBindController.hpp>
using namespace UserInput;

KeyBindController::KeyBindController(wxWindow* win)
{
	win->Bind(wxEVT_KEY_DOWN, [this](wxKeyEvent& event) {
		auto evt = keys.find((wxKeyCode)event.GetKeyCode());
		    if (evt != keys.end())
		        evt->second(event);
	});
}

void KeyBindController::append(wxKeyCode code, const KeyBind&& key)
{
	auto evt = keys.find(code);
	if (evt == keys.end())
	    keys.insert({ code, key });
	else
	    evt->second = key;
}