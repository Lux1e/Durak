#pragma once
#include "../../ui/core/UIInteractive.h"



class InputCapture
{
public:
	void set(UIInteractive& element)
	{
		if (m_capturedElement == &element)
			return;

		if (m_capturedElement)
			m_capturedElement->onCaptureEnd();

		m_capturedElement = &element;
	}

	void release()
	{
		if (m_capturedElement)
		{
			auto* prev = m_capturedElement;
			m_capturedElement = nullptr;
			prev->onCaptureEnd();
		}
	}

	UIInteractive* get() const { return m_capturedElement; }
	bool active() const { return m_capturedElement != nullptr; }

private:
	UIInteractive* m_capturedElement = nullptr;
};