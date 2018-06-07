#pragma once

#include "BoxWidget.h"



class AspectBoxWidget :
	public BoxWidget
{
public:
	AspectBoxWidget();

	AspectBoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color) :
		BoxWidget(_Margin, _Size, _Color)
	{ }

	virtual ~AspectBoxWidget();


	WindowSize GetEnvelopSize(const WindowSize& _MaxContentRect) override;

};

