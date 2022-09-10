#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "BaseModel.hpp"

namespace Assembly
{
	class Configurator : public wxMenu
	{
	private:
		inline void AppendItem(wxString Name);
		BaseModel& model;
	public:
		Configurator(BaseModel& model);
	};
}