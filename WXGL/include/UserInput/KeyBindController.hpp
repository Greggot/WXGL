#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <string_view>
#include <functional>
#include <map>

namespace UserInput
{
	class KeyBind
	{
	private:
		using evt = std::function<void(wxKeyEvent&)>;

		std::string_view name;
		evt call;
	public:
		KeyBind(evt call) : name(""), call(call) {}
		KeyBind(std::string_view name, evt call) : name(name), call(call) {}

		void operator()(wxKeyEvent& event) { call(event); }
	};

	class KeyBindController
	{
	private:
		std::map<wxKeyCode, KeyBind> keys;
	public:
		KeyBindController(wxWindow*);

		void append(wxKeyCode, const KeyBind&&);
	};
}
