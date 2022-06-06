#include "Assembly.Loader.hpp"
using namespace Assembly;

inline void Loader::AppendMenuItem(int ID, wxString Name, wxString Description)
{
	Append(new wxMenuItem(NULL, ID, Name, Description, wxITEM_NORMAL, NULL));
}

Loader::Loader(wxFrame* Host, Viewer& viewer)
	: wxMenu(), Host(Host), viewer(viewer)
{
	AppendMenuItem(LoaderItem::Open, "Open...", "Load OBJ model");
	AppendMenuItem(LoaderItem::Close, "Close", "Close model");
    
	Bind(wxEVT_MENU, &Loader::Open, this, LoaderItem::Open);
}


void Loader::Open(wxCommandEvent& event)
{
    wxFileDialog openFile(Host, wxString("Open Model"), wxEmptyString, wxEmptyString, "OBJ files(*.obj)|*.obj", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFile.ShowModal() == wxID_CANCEL)
        return;
    
    wxString modelPath = openFile.GetPath();
    viewer.Append(OBJ::Model(modelPath.mb_str().data()));
}