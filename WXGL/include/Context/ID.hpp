#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

namespace Context
{
	enum Mod
	{
		Delete = 7,
		Coordinates,
		Angles,
		Scale,
		Properties,

		CameraWin,
		Amount
	};

	enum APIid
	{
		Update = Mod::Amount,
	};
}