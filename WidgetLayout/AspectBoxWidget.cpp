#include "pch.h"
#include "AspectBoxWidget.h"


AspectBoxWidget::AspectBoxWidget()
{
}


AspectBoxWidget::~AspectBoxWidget()
{
}

WindowSize AspectBoxWidget::GetEnvelopSize(const WindowSize & _MaxContentRect)
{
	WindowSize winSize = BoxWidget::GetEnvelopSize(_MaxContentRect);
	WindowSize defaultSize = m_LayoutData.GetInternalSize();

	WindowSize heightControlsWidth = WindowSize((winSize.GetHeight() * defaultSize.GetWidth()) / defaultSize.GetHeight(), winSize.GetHeight());
	WindowSize widthControlsHeight = WindowSize(winSize.GetWidth(), (winSize.GetWidth() * defaultSize.GetHeight()) / defaultSize.GetWidth() );

	return heightControlsWidth;
}
