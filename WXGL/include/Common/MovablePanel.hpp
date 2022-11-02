#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MovablePanel : public wxPanel
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
		: wxPanel(host, wxID_ANY)
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

public:
    NameClosePanel(wxWindow* host, wxColor background, wxString name)
        : MovablePanel(host, background), Close()
    {
        CloseButtonInit();
        LabelInit(std::move(name));

        auto horizontal = new wxFlexGridSizer(1, 2, 0,0);
        horizontal->Add(WinName, 1, wxEXPAND);
        horizontal->Add(Close,   0, wxALIGN_RIGHT | wxFIXED_MINSIZE);
        SetSizer(horizontal);

        Fit();
    }
};