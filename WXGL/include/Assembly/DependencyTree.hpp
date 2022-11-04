#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/treectrl.h>
#include <map>

#include "Core.hpp"
#include <Context/Model.hpp>

namespace Assembly
{
	class DependencyTree : public wxPanel
	{
	private:
		wxTreeCtrl* tree;
		std::map<wxTreeItemId, DependencyNode*> items;
		DependencyNode* dragged = nullptr;

		void RecursiveAppend(wxTreeItemId id, DependencyNode* node);
		void HierarchyDragAndDropChange();
		void ActiveLMBinit();
		void ContexMenuOnRMBinit();
	public:
		Core& core;
		DependencyTree(wxWindow* host, Core& core);

		void Update();
	};
}