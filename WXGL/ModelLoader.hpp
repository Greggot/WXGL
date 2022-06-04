#pragma once
#include "OBJparser.hpp"
#include "WXGL.hpp"

enum LoaderItem
{
	Open = 3,
	Close,
};

class ModelLoader : public wxMenu
{
private:
	wxFrame* Host;
	ModelViewer& Viewer;

	inline void AppendMenuItem(int ID, wxString Name, wxString Description);
	void Open(wxCommandEvent& event);
public:
	ModelLoader(wxFrame* Host, ModelViewer& Viewer);
};