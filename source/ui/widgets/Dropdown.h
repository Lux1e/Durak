#pragma once
#include "ListBox.h"



class Dropdown
{
public:
	Dropdown() = default;
	Dropdown(UIElement& mainElement, ListBox& listBox) : m_mainElement(&mainElement), m_listBox(&listBox) {}


	void setMainElement(UIElement& mainElement) { m_mainElement = &mainElement; }
	void setListBox(ListBox& listBox) { m_listBox = &listBox; }

	UIElement& getMainElement() { assert(m_mainElement); return *m_mainElement; }
	ListBox& getListBox() { assert(m_listBox); return *m_listBox; }

private:
	UIElement* m_mainElement = nullptr;
	ListBox* m_listBox = nullptr;
};