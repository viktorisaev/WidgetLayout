#include "pch.h"

#include "PageWidget.h"
#include <WidgetFactory.h>



PageWidget::PageWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color) :
	Widget(_Position, _Size, _Color)
{
	
}


PageWidget::~PageWidget()
{
	WidgetFactory::DeleteWidget(m_PageWidget);
}




void PageWidget::AddToRender(Sample3DSceneRenderer * _Render)
{
	_Render->AddColoredRectToList(this->GetAbsRect(), this->GetColor());
	m_PageWidget->AddToRender(_Render);
}




void PageWidget::UpdateLayout(const WindowRect& _VisibleRect)
{
	m_PageWidget->UpdateLayout(_VisibleRect);
}




WindowSize PageWidget::GetEnvelopSize(const WindowSize & _MaxContentRect)
{
	WindowSize w = WindowSize::GetMin(m_AbsRect.GetSize(), _MaxContentRect);

	return w;
}
