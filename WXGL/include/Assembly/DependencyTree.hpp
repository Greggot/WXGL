#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/treectrl.h>

#include "Core.hpp"

namespace Assembly
{
	class DependencyTree : public wxPanel
	{
	private:
		Core& core;
		wxTreeCtrl* tree;

		void RecursiveAppend(wxTreeItemId id, DependencyNode* node)
		{
			auto newid = tree->AppendItem(id, node->Name);
			for (auto child : node->getChildren())
				RecursiveAppend(newid, child);
		}
	public:
		DependencyTree(wxWindow* host, Core& core) 
			: wxPanel(host), core(core)
		{
			tree = new wxTreeCtrl(this, wxID_ANY, wxPoint{20, 10});
			auto root = tree->AddRoot("Model");

			tree->Bind(wxEVT_TREE_BEGIN_DRAG, [this](wxTreeEvent& e) {
				e.Allow();
			});

			// TODO: Add std::map or something else to associate wxTreeItemId with
			//		core model, so it would be possible to change model's 
			//		parent-children inside
			tree->Bind(wxEVT_TREE_END_DRAG, [this](wxTreeEvent& e) {
			});
		}

		void Update()
		{
			tree->DeleteAllItems();
			
			auto root = tree->AddRoot("Model");
			for (auto model : core)
				if (model->getParent() == nullptr)
					RecursiveAppend(root, model);
			tree->ExpandAll();
		}
	};
}