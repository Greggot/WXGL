#pragma once
#include "OBJ.hpp"
#include "Assembly.Viewer.hpp"

enum LoaderItem
{
	Open = 3,
	Close,
};

namespace Assembly
{

	class Loader : public wxMenu
	{
	private:
		wxFrame* Host;
		Viewer& viewer;

		inline void AppendMenuItem(int ID, wxString Name, wxString Description);
		void Open(wxCommandEvent& event);
	public:
		Loader(wxFrame* Host, Viewer& viewer);
	};
}