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
    AppendMenuItem(LoaderItem::OpenAssembly, "Open assembly...", "");
    AppendMenuItem(LoaderItem::SaveAssembly, "Save assembly...", "");
    AppendMenuItem(LoaderItem::Close, "Close ", "");
    
    modelNumber = 0;
    Bind(wxEVT_MENU, &Loader::Open, this, LoaderItem::Open);
    Bind(wxEVT_MENU, &Loader::SaveAssembly, this, LoaderItem::SaveAssembly);
    Bind(wxEVT_MENU, &Loader::LoadAssembly, this, LoaderItem::OpenAssembly);
    Bind(wxEVT_MENU, &Loader::UnloadAll, this, LoaderItem::Close);
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
    fprintf(out, "b\n");
    for (auto model : Assembly)
    {
        fwrite(&model->Translation, sizeof(vertex), 1, out);
        fwrite(&model->Rotation,    sizeof(vertex), 1, out);
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
        fread(&model->Translation, sizeof(vertex), 1, in);
        fread(&model->Rotation,    sizeof(vertex), 1, in);
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

template<class type>
static inline int FindIndexOfElement(std::vector<type> vector, type element)
{
    int size = vector.size();
    for (int i = 0; i < size; ++i)
    {
        if (vector[i] == element)
            return i;
    }
    return -1;
}

void Loader::Unload(OBJ::Model* Model)
{
    int index = FindIndexOfElement(Assembly, Model);
    if (index < 0)
        return;
    Unload(index);
}

void Loader::UnloadAll(wxCommandEvent& event)
{
    viewer.RemoveAll();
    for (auto model : Assembly)
        delete model;
    Assembly.clear();
}

Loader::~Loader()
{
    wxCommandEvent empty;
    UnloadAll(empty);
}