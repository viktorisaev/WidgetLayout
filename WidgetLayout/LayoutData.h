#pragma once

#include <WindowSize.h>


const int32_t FIT_PARENT = 0;
const int32_t ENVELOP_CHILD = INT_MAX;


class LayoutData
{
public:
	LayoutData();
	LayoutData(int32_t _Margin, WindowSize _Size);
	~LayoutData();

	WindowSize GetMinimumSize();	// size + margin
	void SetSize(const WindowSize& _Size);
	WindowSize GetContentSize(const WindowSize & _EnvelopSize);
	WindowSize GetRequiredSizeWithParent(const WindowSize& _ParentContentRect);
	WindowSize GetRequiredSizeWithContent(const WindowSize& _ContentRect);
	WindowSize GetRequiredSizeWithContentAndParent(const WindowSize& _ContentRect, const WindowSize& _ParentContentRect);
	int32_t GetMargin();
	WindowSize GetInternalSize();
	WindowSize GetEnvelopSize(const WindowSize & _ContentSize);


private:
	int32_t GetDoubleMargin() { return m_Margin * 2; }

	int32_t GetMeasureWithEnvelop(int32_t _DefaultMeasure, int32_t _EnvelopMeasure);

private:
	int32_t m_Margin;
	WindowSize m_Size;
};

