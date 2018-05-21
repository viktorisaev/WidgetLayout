#include "pch.h"
#include "PageWidget.h"


PageWidget::PageWidget() :
	Widget(DirectX::XMINT2(0, 0), DirectX::XMINT2(1366, 696), DirectX::XMFLOAT4(0.31f, 0.3f, 0.3f, 1.0f))
{
	
}


PageWidget::~PageWidget()
{
}


void PageWidget::AddToRender(Sample3DSceneRenderer * _Render)
{
	_Render->AddColoredRectToList(this->GetRect(), this->GetColor());
	m_PageWidget->AddToRender(_Render);
}
