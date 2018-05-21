#include "pch.h"

#include "PageWidget.h"
#include <WidgetFactory.h>



PageWidget::PageWidget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color) :
	Widget(_Position, _Size, _Color)
{
	
}


PageWidget::~PageWidget()
{
	WidgetFactory::DeleteWidget(m_PageWidget);
}


void PageWidget::AddToRender(Sample3DSceneRenderer * _Render)
{
	_Render->AddColoredRectToList(this->GetRect(), this->GetColor());
	m_PageWidget->AddToRender(_Render);
}
