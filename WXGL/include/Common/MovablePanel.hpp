#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MovablePanel : public wxFrame
{
private:
    bool shouldDrag = false;
    wxPoint startPos;

protected:
    std::function<void(wxMouseEvent&)> dragstart = [this](wxMouseEvent& e) {
        shouldDrag = true;
        startPos = e.GetPosition();
    };
    std::function<void(wxMouseEvent&)> dragstop = [this](wxMouseEvent& e) {
        shouldDrag = false;
    };
public:
	MovablePanel(wxWindow* host, wxColor background)
		: wxFrame(host, wxID_ANY, "")
	{
        Bind(wxEVT_LEFT_DOWN, dragstart);
        Bind(wxEVT_LEFT_UP, dragstop);
        SetBackgroundColour(background);

        host->Bind(wxEVT_MOTION, [this](wxMouseEvent& e) {
            if (shouldDrag)
            {
                if (e.Dragging())
                    SetPosition(e.GetPosition() - startPos);
            }
            else
                e.Skip();
        });
	}
};

class NameClosePanel : public MovablePanel
{
private:
    wxButton* Close;
    wxStaticText* WinName;

    void CloseButtonInit()
    {
        Close = new wxButton(this, wxID_ANY, "x", wxDefaultPosition, { 20, 20 });
        Close->SetForegroundColour({ 0xFF, 0xFF, 0xFF });
        Close->SetBackgroundColour({ 0xFF, 0x00, 0x00 });
        Close->SetWindowStyle(wxNO_BORDER);

        Close->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
            Destroy();
        });
    }

    void LabelInit(wxString&& name)
    {
        WinName = new wxStaticText(this, wxID_ANY, name);
        WinName->Bind(wxEVT_LEFT_DOWN, dragstart);
        WinName->Bind(wxEVT_LEFT_UP, dragstop);
    }
protected:
    wxFlexGridSizer* HeaderSizer;
public:
    NameClosePanel(wxWindow* host, wxColor background, wxString name)
        : MovablePanel(host, background), Close()
    {
        CloseButtonInit();
        LabelInit(std::move(name));
        WinName->SetForegroundColour({ 0xFF, 0xFF, 0xFF });

        HeaderSizer = new wxFlexGridSizer(1, 2, 0,0);
        HeaderSizer->Add(Close, 0, wxALIGN_RIGHT | wxFIXED_MINSIZE);
        HeaderSizer->Add(WinName, 1, wxEXPAND);
    }
};

class ValuePanel : public NameClosePanel
{
private:
    wxStaticText* Units;
    wxTextCtrl* Value;
public:
    ValuePanel(wxWindow* host, wxColor background, wxString name, wxString&& units)
        : NameClosePanel(host, background, name)
    {
        Value = new wxTextCtrl(this, wxID_ANY, "");
        Value->SetWindowStyle(wxTE_READONLY);

        Units = new wxStaticText(this, wxID_ANY, units);
        Units->Bind(wxEVT_LEFT_DOWN, dragstart);
        Units->Bind(wxEVT_LEFT_UP, dragstop);
        Units->SetForegroundColour({ 0xFF, 0xFF, 0xFF });

        auto horizontal = new wxFlexGridSizer(1, 2, 0, 0);
        horizontal->Add(Value, 1, wxEXPAND);
        horizontal->Add(Units, 0, wxALIGN_RIGHT | wxFIXED_MINSIZE);

        auto box = new wxBoxSizer(wxVERTICAL);
        box->Add(HeaderSizer, 1, wxEXPAND);
        box->Add(horizontal, 1, wxEXPAND);
        
        SetSizer(box);
        Fit();
    }

    template<class type>
    void Update(type value) {
        Value->SetValue(std::to_string(value));
    }
};

// Image is inherited from independend wxFrame, because each refresh
//  on wxGLCanvas will cause image to refresh as well
class ImagePanel : public wxFrame
{
private:
    wxImage shot;
    wxPanel* imgpanel;
    wxPoint startPos;

    bool shouldUpdate = false;
    wxButton* Close;
    wxStaticText* WinName;

    void CloseButtonInit()
    {
        Close = new wxButton(this, wxID_ANY, "x", wxDefaultPosition, { 20, 20 });
        Close->SetForegroundColour({ 0xFF, 0xFF, 0xFF });
        Close->SetBackgroundColour({ 0xFF, 0x00, 0x00 });
        Close->SetWindowStyle(wxNO_BORDER);

        Close->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
            Destroy();
            });
    }

    void LabelInit(wxString&& name)
    {
        WinName = new wxStaticText(this, wxID_ANY, name);
        WinName->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent& e) {
            startPos = e.GetPosition();
        });
    }
public:
    ImagePanel(wxWindow* host, wxColor background, wxString name, int width, int height)
        : wxFrame(host, wxID_ANY, name), shot(wxSize(width, height))
    {
        SetBackgroundColour({ 0x42, 0x87,0xF5 });
        SetWindowStyle(wxSTAY_ON_TOP);
        CloseButtonInit();
        LabelInit(std::move(name));
        auto HeaderSizer = new wxFlexGridSizer(1, 2, 0, 0);
        HeaderSizer->Add(Close, 0, wxALIGN_RIGHT | wxFIXED_MINSIZE);
        HeaderSizer->Add(WinName, 1, wxEXPAND);


        Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent& e) {
            startPos = e.GetPosition();
        });
        Bind(wxEVT_MOTION, [this](wxMouseEvent& e) {
            auto pos = wxGetMousePosition();
            if (e.Dragging())
                SetPosition(pos - startPos);
        });

        imgpanel = new wxPanel(this, wxID_ANY);
        auto box = new wxBoxSizer(wxVERTICAL);
        box->Add(HeaderSizer);
        box->Add(imgpanel);

        SetSizer(box);
        Fit();
        SetSize(width, height);


        Bind(wxEVT_PAINT, [this](wxPaintEvent&) {
            wxPaintDC* dc = new wxPaintDC(this);
            dc->DrawBitmap(wxBitmap(shot), wxPoint(0, 20));
            delete dc;
        });
    }

    template<class type>
    void Update(type&& data) {
        shot = data;
        Refresh();
    }

    void Update() {
        Refresh();
    }

    void Set(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b) {
        shot.SetRGB(x, y, r, g, b);
    }
};