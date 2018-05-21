#include "pch.h"
#include "BoxWidget.h"


BoxWidget::BoxWidget()
{
}


BoxWidget::~BoxWidget()
{
}

void BoxWidget::AddToRender(Sample3DSceneRenderer * _Render)
{
	_Render->AddColoredRectToList(this->GetRect(), this->GetColor());
}

void BoxWidget::UpdateLayout(const WindowRect & _ParentWindowRect)
{
}
