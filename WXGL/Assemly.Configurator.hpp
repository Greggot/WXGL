#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


namespace Assembly
{
	class Configurator : public wxMenu
	{
	private:
		inline void AppendItem(wxString Name);
	public:
		Configurator(wxString ModelName);
	};
}