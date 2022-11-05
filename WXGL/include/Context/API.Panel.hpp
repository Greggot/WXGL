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
			AppendMenuItem(APIid::Update, "Update", [panel](wxCommandEvent&) {
				panel->Update();
				panel->Report();
			});
		}
	};
}