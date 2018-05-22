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




void PageWidget::UpdateLayout(const WindowRect & _AvailableWindowRect)
{
	m_PageWidget->UpdateLayout(_AvailableWindowRect);
}




WindowSize PageWidget::GetRequiredSize(const WindowSize & _AvailableSize)
{
	WindowSize w = WindowSize::GetMin(m_AbsRect.GetSize(), _AvailableSize);

	return w;
}
