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
    AppendMenuItem(LoaderItem::OpenAssembly, "Open assembly...", "Close model");
    AppendMenuItem(LoaderItem::SaveAssembly, "Save assembly...", "Close model");
    
    modelNumber = 0;
    Bind(wxEVT_MENU, &Loader::Open, this, LoaderItem::Open);
    Bind(wxEVT_MENU, &Loader::SaveAssembly, this, LoaderItem::SaveAssembly);
    Bind(wxEVT_MENU, &Loader::LoadAssembly, this, LoaderItem::OpenAssembly);
}

static inline wxString getFileNameFrom(wxString FullPath)
{
    size_t pos = FullPath.size() - 1;
    while (FullPath[pos] != '\\' && FullPath[pos] != '/' && --pos)
    {}
    return FullPath.Mid(++pos);
}

void Loader::Load(wxString Path)
{
    Paths.push_back(Path);
    OBJ::Model* Model = new OBJ::Model(Path.mb_str().data());
    Assembly.push_back(Model);
    viewer.Append(Model);
}

void Loader::Open(wxCommandEvent& event)
{
    wxFileDialog open(Host, wxString("Open Model"), wxEmptyString, wxEmptyString, "OBJ files(*.obj)|*.obj", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (open.ShowModal() == wxID_CANCEL)
        return;
    Load(open.GetPath());
}

void Loader::SaveAssembly(wxCommandEvent& event)
{
    wxFileDialog save(Host, wxString("Save assembly"), wxEmptyString, wxEmptyString, "ASMBL assembly files(*.asmbl)|*.asmbl", wxFD_SAVE);
    if (save.ShowModal() == wxID_CANCEL)
        return;
    wxString assemblyPath = save.GetPath();

    FILE* out = fopen(assemblyPath.mb_str().data(), "wb");
    for (auto path : Paths)
        fprintf(out, "%s\n", path.mb_str().data());
    fprintf(out, "b\n");
    for (auto model : Assembly)
    {
        fwrite(model->getTransformVector(), sizeof(vertex), 1, out);
        fwrite(model->getRotationVector(),  sizeof(vertex), 1, out);
    }
    fclose(out);
    // Struct:  1. Models: File Path, Translate, Rotate
    //          2. Dependency Tree: index-index-..., ... lists
}

void Loader::LoadAssembly(wxCommandEvent& event)
{
    wxFileDialog open(Host, wxString("Save assembly"), wxEmptyString, wxEmptyString, "ASMBL assembly files(*.asmbl)|*.asmbl", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (open.ShowModal() == wxID_CANCEL)
        return;
    wxString assemblyPath = open.GetPath();

    FILE* in = fopen(assemblyPath.mb_str().data(), "rb");
    
    char Buffer[0xFF] = {};
    bool modelsFound = false;
    while (!modelsFound)
    {
        fgets(Buffer, 0xFF, in);
        if (*Buffer == 'b')
            modelsFound = true;
        else
        {
            size_t ind = 0;
            while (Buffer[ind++] != '\n')
            {}
            Buffer[--ind] = 0;
            Load(Buffer);
        }
    }
    for (auto model : Assembly)
    {
        fread(model->getTransformVector(), sizeof(vertex), 1, in);
        fread(model->getRotationVector(),  sizeof(vertex), 1, in);
    }
    fclose(in);
}

void Loader::Unload(size_t index)
{
    viewer.Remove(index);

    OBJ::Model* Model = Assembly[index];
    delete Model;
    Assembly.erase(Assembly.begin() + index);
}

Loader::~Loader()
{
    for (auto model : Assembly)
        delete model;
    Assembly.clear();
    Paths.clear();
}