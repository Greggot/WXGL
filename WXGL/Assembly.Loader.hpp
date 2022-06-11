#pragma once
#include "OBJ.hpp"
#include "Assembly.Viewer.hpp"

enum LoaderItem
{
	Open = 3,
	OpenAssembly,
	SaveAssembly,
};

namespace Assembly
{

	class Loader : public wxMenu
	{
	private:
		wxFrame* Host;
		Viewer& viewer;
		uint32_t modelNumber;

		std::vector<wxString> Paths;
		std::vector<OBJ::Model*> Assembly;

		inline void AppendMenuItem(int ID, wxString Name, wxString Description);
		
		void Open(wxCommandEvent& event);
		void SaveAssembly(wxCommandEvent& event);
		void LoadAssembly(wxCommandEvent& event);

		void Load(wxString Path);
	public:
		Loader(wxFrame* Host, Viewer& viewer);

		void Unload(size_t index);
		void Unload(OBJ::Model* Model);
		~Loader();
	};
}