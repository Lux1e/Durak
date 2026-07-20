#pragma once
#include "ListBox.h"



class Dropdown
{
public:
	Dropdown(UIElement& mainElement, ListBox& listBox) : m_mainElement(&mainElement), m_listBox(&listBox) {}


	void setMainElement(UIElement& mainElement) { m_mainElement = &mainElement; }

	UIElement& getMainElement() { return *m_mainElement; }
	ListBox& getListBox() { return *m_listBox; }

private:
	ListBox* m_listBox = nullptr;
	UIElement* m_mainElement = nullptr;
};