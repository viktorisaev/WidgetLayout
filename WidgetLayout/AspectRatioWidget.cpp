#include "pch.h"
#include "AspectRatioWidget.h"


AspectRatioWidget::AspectRatioWidget()
{
}

AspectRatioWidget::AspectRatioWidget(WindowSize _RefenerceSize, Widget * const _ContentWidget) :
  m_RefenerceSize(_RefenerceSize)
, m_ContentWidget(_ContentWidget)
{
}


AspectRatioWidget::~AspectRatioWidget()
{
}

WindowSize AspectRatioWidget::GetEnvelopSize(const WindowSize & _ParentContentRect)
{
	WindowSize winSize = m_ContentWidget->GetEnvelopSize(_ParentContentRect);
	return GetAspectRatioSize(winSize);
}



void AspectRatioWidget::AddToRender(Sample3DSceneRenderer * _Render)
{
	m_ContentWidget->AddToRender(_Render);
}



WindowSize AspectRatioWidget::GetAspectRatioSize(const WindowSize & _QueriedContentSize)
{
	WindowSize heightControlsWidth = WindowSize((_QueriedContentSize.GetHeight() * m_RefenerceSize.GetWidth()) / m_RefenerceSize.GetHeight(), _QueriedContentSize.GetHeight());
	WindowSize widthControlsHeight = WindowSize(_QueriedContentSize.GetWidth(), (_QueriedContentSize.GetWidth() * m_RefenerceSize.GetHeight()) / m_RefenerceSize.GetWidth());		// TODO: use this size also

	// TODO: choose proper size
	return heightControlsWidth;
}




void AspectRatioWidget::UpdateLayout(const WindowSize& _ParentSize)
{
	WindowSize aspectedSize = GetAspectRatioSize(_ParentSize);
	m_ContentWidget->UpdateLayout(aspectedSize);
}


void AspectRatioWidget::BuildWorldPosition(const WindowPos & _ParentPos)
{
	m_ContentWidget->BuildWorldPosition(_ParentPos);
}
