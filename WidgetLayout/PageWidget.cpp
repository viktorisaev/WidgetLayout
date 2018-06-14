#include "pch.h"

#include "PageWidget.h"
#include <WidgetFactory.h>



PageWidget::PageWidget(const DirectX::XMFLOAT4& _Color) :
	Widget(_Color)
{
}


PageWidget::~PageWidget()
{
	WidgetFactory::DeleteWidget(m_PageWidget);
}




void PageWidget::AddToRender(Sample3DSceneRenderer * _Render)
{
	_Render->AddColoredRectToList(this->GetWorldRect(), this->GetDebugColor());
	m_PageWidget->AddToRender(_Render);
}




void PageWidget::UpdateLayout(const WindowSize& _ParentSize)
{
	this->SetSize(_ParentSize);
	m_PageWidget->UpdateLayout(_ParentSize);
}



void PageWidget::BuildWorldPosition(const WindowPos & _ParentPos)
{
	this->SetPosition(_ParentPos);
	m_PageWidget->BuildWorldPosition(_ParentPos);
}



WindowSize PageWidget::GetEnvelopSize(const WindowSize & _MaxContentRect)
{
	WindowSize w = WindowSize::GetMin(this->GetRect().GetSize(), _MaxContentRect);

	return w;
}

