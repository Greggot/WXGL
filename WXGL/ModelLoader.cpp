#include "ModelLoader.hpp"

inline void ModelLoader::AppendMenuItem(int ID, wxString Name, wxString Description)
{
	Append(new wxMenuItem(NULL, ID, Name, Description, wxITEM_NORMAL, NULL));
}

ModelLoader::ModelLoader(wxFrame* Host, ModelViewer& Viewer)
	: wxMenu(), Host(Host), Viewer(Viewer)
{
	AppendMenuItem(LoaderItem::Open, "Open...", "Load OBJ model");
	AppendMenuItem(LoaderItem::Close, "Close", "Close model");
    
	Bind(wxEVT_MENU, &ModelLoader::Open, this, LoaderItem::Open);
}


void ModelLoader::Open(wxCommandEvent& event)
{
    wxFileDialog openFile(Host, wxString("Open Model"), wxEmptyString, wxEmptyString, "OBJ files(*.obj)|*.obj", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFile.ShowModal() == wxID_CANCEL)
        return;
    
    wxString modelPath = openFile.GetPath();
    Viewer.Append(ParseFile(modelPath.mb_str().data()));
}