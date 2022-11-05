#pragma once
#include <Assembly/DependencyTree.hpp>
#include <Context/Model.hpp>
using namespace Assembly;

DependencyTree::DependencyTree(wxWindow* host)
	: wxPanel(host)
{
	auto sizer = new wxBoxSizer(wxHORIZONTAL);
	tree = new wxTreeCtrl(this, wxID_ANY);
	sizer->AddSpacer(10);
	sizer->Add(tree, 1, wxEXPAND);
	sizer->AddSpacer(10);
	SetSizer(sizer);

	auto root = tree->AddRoot("Model");

	HierarchyDragAndDropChange();
	ActiveLMBinit();
	ContexMenuOnRMBinit();
}

void DependencyTree::Update()
{
	// TODO: change to managing already existing elements
	//		It's too stupid to remove all elements and refill 'em again
	//		if a single model is inserted/removed. BUT it works for now
	tree->DeleteAllItems();
	items.clear();

	auto root = tree->AddRoot("Model");
	for (auto model : Models)
		if (model->getParent() == nullptr)
			RecursiveAppend(root, model);
	tree->ExpandAll();
}

void DependencyTree::RecursiveAppend(wxTreeItemId id, DependencyNode* node)
{
	auto newid = tree->AppendItem(id, node->Name);
	items.insert({ newid, node });
	for (auto child : node->getChildren())
		RecursiveAppend(newid, child);
}

DependencyNode* dragged = nullptr;
void DependencyTree::HierarchyDragAndDropChange()
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
		if (newnode == tree->GetRootItem())
			dragged->MakeIndependent();
		else
			dragged->LinkTo(newnode);
		Update();
		});
}

void DependencyTree::ActiveLMBinit()
{
	tree->Bind(wxEVT_TREE_SEL_CHANGED, [this](wxTreeEvent& e) {
		if (e.GetItem() == tree->GetRootItem())
			dropSelection();
		else
			setActive((DrawableModel*)items[e.GetItem()]);
		});
}

void DependencyTree::ContexMenuOnRMBinit()
{
	tree->Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, [this](wxTreeEvent& e) {
		if (e.GetItem() == tree->GetRootItem())
			return;
		auto model = items[e.GetItem()];
		const auto& it = std::find(begin(), end(), model);
		if (it == end())
			return;

		Context::Model config(static_cast<uint16_t>(it - begin()), *this);
		PopupMenu(&config, e.GetPoint());
	});
}
