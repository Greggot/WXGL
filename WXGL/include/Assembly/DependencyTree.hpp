#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/treectrl.h>

#include "Configurator.hpp"
#include "Core.hpp"
#include <map>

namespace Assembly
{
	class DependencyTree : public wxPanel
	{
	private:
		Core& core;
		wxTreeCtrl* tree;
		std::map<wxTreeItemId, DependencyNode*> items;

		void RecursiveAppend(wxTreeItemId id, DependencyNode* node)
		{
			auto newid = tree->AppendItem(id, node->Name);
			items.insert({ newid, node });
			for (auto child : node->getChildren())
				RecursiveAppend(newid, child);
		}

		DependencyNode* dragged = nullptr;
		void HierarchyDragAndDropChange()
		{
			tree->Bind(wxEVT_TREE_BEGIN_DRAG, [this](wxTreeEvent& e) {
				// Root isn't part of hierarchy, don't even allow to move it
				if (e.GetItem() == tree->GetRootItem())
					return;
				e.Allow();
				dragged = items[e.GetItem()];
			});

			tree->Bind(wxEVT_TREE_END_DRAG, [this](wxTreeEvent& e) {
				auto newnode = items[e.GetItem()];
				if (newnode == dragged)
					return;
				if (newnode == tree->GetRootItem())
					dragged->MakeIndependent();
				else
					dragged->LinkTo(newnode);
				Update();
			});
		}

		void ActiveLMBinit()
		{
			tree->Bind(wxEVT_TREE_SEL_CHANGED, [this](wxTreeEvent& e) {
				if (e.GetItem() == tree->GetRootItem())
					core.dropSelection();
				else
					core.setActive((DrawableModel*)items[e.GetItem()]);
			});
		}

		void ContexMenuOnRMBinit()
		{
			tree->Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, [this](wxTreeEvent& e) {
				if (e.GetItem() == tree->GetRootItem())
					return;
				auto model = items[e.GetItem()];
				const auto& it = std::find(core.begin(), core.end(), model);
				if (it == core.end())
					return;

				Configurator config(static_cast<uint16_t>(it - core.begin()), core);
				PopupMenu(&config, wxGetMousePosition());
			});
		}
	public:
		DependencyTree(wxWindow* host, Core& core) 
			: wxPanel(host), core(core)
		{
			tree = new wxTreeCtrl(this, wxID_ANY, wxPoint{20, 10});
			auto root = tree->AddRoot("Model");

			HierarchyDragAndDropChange();
			ActiveLMBinit();
			ContexMenuOnRMBinit();
		}

		void Update()
		{
			// TODO: change to managing already existing elements
			//		It's too stupid to remove all elements and refill 'em again
			//		if a single model is inserted/removed. BUT it works for now
			tree->DeleteAllItems();
			items.clear();
			
			auto root = tree->AddRoot("Model");
			for (auto model : core)
				if (model->getParent() == nullptr)
					RecursiveAppend(root, model);
			tree->ExpandAll();
		}
	};
}