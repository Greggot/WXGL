#pragma once
#include <Model/OBJ.hpp>
#include <Model/STL.hpp>

#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/frame.h>

#include "Core.hpp"
#include "DependencyTree.hpp"

enum LoaderID
{
	Open = 3,
	OpenAssembly,
	SaveAssembly,
	Close,
};

namespace Assembly
{
	/*
	* @brief Menu which will load/save/unload models
	**/
	class Loader : public wxMenu
	{
	private:
		wxFrame* Host;
		Core& core;
		DependencyTree& Tree;

		inline void AppendMenuItem(int ID, wxString Name, wxString Description,
			void(Assembly::Loader::* Method)(wxCommandEvent&));
		
		void Open(wxCommandEvent& event);
		void SaveAssembly(wxCommandEvent& event);
		void LoadAssembly(wxCommandEvent& event);
		void UnloadAll(wxCommandEvent& event);

		void Load(wxString Path);
	public:
		Loader(wxFrame* Host, Core& core, DependencyTree& Tree);

		~Loader();
	};
}