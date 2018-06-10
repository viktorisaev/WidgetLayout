#pragma once

#include "BoxWidget.h"



class AspectRatioWidget :	public Widget
{
public:
	AspectRatioWidget();

	AspectRatioWidget(WindowSize _RefenerceSize, Widget * const _ContentWidget);

	virtual ~AspectRatioWidget();


// IWidget
	virtual void UpdateLayout(const WindowRect & _VisibleRect) override;
	virtual WindowSize GetEnvelopSize(const WindowSize& _ParentContentRect) override;
	virtual void AddToRender(Sample3DSceneRenderer* _Render) override;

protected:
	WindowSize GetAspectRatioSize(const WindowSize& _QueriedContentSize);

private:
	Widget *m_ContentWidget;
	WindowSize m_RefenerceSize;

};

