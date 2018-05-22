#include "pch.h"
#include "AspectBoxWidget.h"


AspectBoxWidget::AspectBoxWidget()
{
}


AspectBoxWidget::~AspectBoxWidget()
{
}

WindowSize AspectBoxWidget::GetRequiredSize(const WindowSize & _AvailableSize)
{
	WindowSize winSize = BoxWidget::GetRequiredSize(_AvailableSize);

	int32_t nX = (winSize.GetHeight() * m_DefaultSize.GetWidth()) / m_DefaultSize.GetHeight();

	return WindowSize(nX, winSize.GetHeight());
}
