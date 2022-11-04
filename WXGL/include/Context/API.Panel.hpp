#pragma once
#include <Common/API.Server.Panel.hpp>
#include "ID.hpp"

namespace Context
{
	class API : public wxMenu
	{
	private:
		template<class functor>
		inline void AppendMenuItem(int ID, wxString&& Name,
			functor Method, wxString Description = "")
		{
			wxMenuItem* item = new wxMenuItem(nullptr, ID, Name, Description);
			Append(item);
			Bind(wxEVT_MENU, Method, ID);
		}
	public:
		API(SkyBlue::APIPanel* panel)
		{
			/*wxMenuItem* item = new wxMenuItem(nullptr, APIid::Update, "Update");
			Append(item);
			Bind(wxEVT_MENU, [panel](wxMenuEvent&) {
				panel->Update();
				panel->Report();
			}, this, APIid::Update);*/
			AppendMenuItem(APIid::Update, "Update", [panel](wxCommandEvent&) {
				panel->Update();
				panel->Report();
			});
		}
	};
}