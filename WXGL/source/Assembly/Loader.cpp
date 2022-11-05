#include <Assembly/Loader.hpp> 
using namespace Assembly;

Loader::Loader(wxFrame* Host, DependencyTree& Tree)
	: wxMenu(), Host(Host), Tree(Tree)
{
	AppendMenuItem(LoaderID::Open, "Open...", "Load OBJ model", &Loader::Open);
    AppendMenuItem(LoaderID::OpenAssembly, "Open assembly...", "", &Loader::LoadAssembly);
    AppendMenuItem(LoaderID::SaveAssembly, "Save assembly...", "", &Loader::SaveAssembly);
    AppendMenuItem(LoaderID::Close, "Close ", "", &Loader::UnloadAll);
}

inline void Loader::AppendMenuItem(int ID, wxString Name, wxString Description,
    void(Assembly::Loader::* Method)(wxCommandEvent&))
{
    Append(new wxMenuItem(NULL, ID, Name, Description, wxITEM_NORMAL, NULL));
    Bind(wxEVT_MENU, Method, this, ID);
}

void Loader::Load(wxString Path)
{
    wxString extention = Path.Mid(Path.Index('.') + 1);
    DrawableModel* Model = nullptr;
    
    if (extention == "obj")
        Model = new OBJ::Model(Path.mb_str().data());
    else if (extention == "stl")
        Model = new STL::Model(Path.mb_str().data());
    else
        return;
    Tree.append(Model);
}

void Loader::Open(wxCommandEvent& event)
{
    wxFileDialog open(Host, wxString("Open Model"), "", "", "OBJ files(*.obj)|*.obj|STL files(*.stl)|*.stl", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (open.ShowModal() == wxID_CANCEL)
        return;
    Load(open.GetPath());
    Tree.Update();
}

void Loader::SaveAssembly(wxCommandEvent& event)
{
    wxFileDialog save(Host, wxString("Save assembly"), "", "", "ASMBL assembly files(*.asmbl)|*.asmbl", wxFD_SAVE);
    if (save.ShowModal() == wxID_CANCEL)
        return;
    wxString assemblyPath = save.GetPath();

    FILE* out = fopen(assemblyPath.mb_str().data(), "wb");
    fprintf(out, "b\n");

    for (auto model : Tree)
    {
        vertex temp = model->GetTranslation();
        fwrite(&temp, sizeof(vertex), 1, out);
        temp = model->GetTranslation();
        fwrite(&temp, sizeof(vertex), 1, out);
    }
    fclose(out);
    // Struct:  1. Models: File Path, Translate, Rotate
    //          2. Dependency Tree: index-index-..., ... lists
}

void Loader::LoadAssembly(wxCommandEvent& event)
{
    wxFileDialog open(Host, wxString("Load assembly"), wxEmptyString, wxEmptyString, "ASMBL assembly files(*.asmbl)|*.asmbl", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
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
    for (auto model : Tree)
    {
        vertex temp;
        fread(&temp, sizeof(vertex), 1, in);
        model->SetTranslation(temp);
        fread(&temp, sizeof(vertex), 1, in);
        model->SetRotation(temp);
    }
    fclose(in);
}

void Loader::UnloadAll(wxCommandEvent& event)
{
    Tree.clear();
}

Loader::~Loader()
{
    wxCommandEvent empty;
    UnloadAll(empty);
}