#include <Dialog/Value.Change.hpp>

DialogValue::DialogValue(wxFrame* Host, wxPoint&& mousepos, std::initializer_list<changeValue> values)
	: wxFrame(Host, wxID_ANY, wxT("Redact"), mousepos, wxSize(270, 150))
{
	SetBackgroundColour({ 0xFF, 0xFF, 0xFF });
	auto sizer = new wxGridSizer(values.size(), 2, 10, 10);
	for (auto cv : values)
	{
		sizer->Add(new wxStaticText(this, wxID_ANY, cv.description), 1, wxEXPAND);
		sizer->Add(TextField(std::move(cv)));
	}
	SetSizer(sizer);
	SetCorrectWindowSize();

	CloseOnEnterInit();
}

wxTextCtrl* DialogValue::TextField(changeValue&& cv)
{
	auto text = new wxTextCtrl(this, wxID_ANY, std::to_string(*cv.value));
	text->SetWindowStyle(wxTE_PROCESS_ENTER);
	text->Bind(wxEVT_TEXT, [cv, text](wxCommandEvent&) {
		try { *cv.value = std::atof(text->GetValue().mb_str().data()); }
		catch (const std::exception&) {}
	});
	text->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&) { Close(); });
	
	return text;
}


void DialogValue::SetCorrectWindowSize()
{
	Fit();
	auto size = GetSize();
	SetSize({ size.x + 20, size.y + 20 });
}

void DialogValue::CloseOnEnterInit()
{
	Bind(wxEVT_KEY_DOWN, [this](wxKeyEvent& evt) {
		if (evt.GetKeyCode() == WXK_RETURN)
			Close();
	});
}

