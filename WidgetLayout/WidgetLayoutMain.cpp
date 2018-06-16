#include "pch.h"
#include "WidgetLayoutMain.h"
#include "Common\DirectXHelper.h"

#include "WidgetFactory.h"
#include <StackWidget.h>


using namespace WidgetLayout;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;





int m_NumberOfSizeCalculations;





WidgetLayoutMain::WidgetLayoutMain()
{
	m_PageWidget = WidgetFactory::CreatePageWidget(DirectX::XMFLOAT4(0.31f, 0.3f, 0.3f, 1.0f));

	Layout2();
}





void WidgetLayout::WidgetLayoutMain::Layout1()
{
	StackWidget* globVertStack = WidgetFactory::CreateStackWidget(StackWidget::Vertical, 15, WindowSize(/*ENVELOP_CHILD*/1000/*FIT_PARENT*/, FIT_PARENT), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.37f), WindowSize(1000/*ENVELOP_CHILD*/, ENVELOP_CHILD));
	m_PageWidget->SetPageWidget(globVertStack);

	m_VerticalStackWidget = WidgetFactory::CreateStackWidget(StackWidget::Vertical, 15, WindowSize(/*FIT_PARENT*//*800*/ENVELOP_CHILD, 350), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.37f), WindowSize(ENVELOP_CHILD, /*ENVELOP_CHILD*/95));

	globVertStack->AddWidget(m_VerticalStackWidget);

	// vertical children
	m_VerticalStackWidget->AddWidget(
		WidgetFactory::CreateAspectWrappedWidget(WindowSize(100, 100),
			WidgetFactory::CreateBoxWidget(10, WindowSize(120, FIT_PARENT/*80*/), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WIDGET_TEXTURE_BUTTON)
		)
	);
	m_VerticalStackWidget->AddWidget(
		WidgetFactory::CreateAspectWrappedWidget(WindowSize(100, 100),
			WidgetFactory::CreateBoxWidget(10, WindowSize(120, 30), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WIDGET_TEXTURE_BUTTON)
		)
	);

	// horizontal
	m_InternalWidget = WidgetFactory::CreateStackWidget(StackWidget::Horizontal, 10, WindowSize(ENVELOP_CHILD, ENVELOP_CHILD), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 0.37f), WindowSize(100/*ENVELOP_CHILD*/, 100/*ENVELOP_CHILD*/));
	m_InternalWidget->AddWidget(WidgetFactory::CreateBoxWidget(10, WindowSize(FIT_PARENT/*100*/, 80), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.8f), WIDGET_TEXTURE_FRAME));
	m_InternalWidget->AddWidget(
		WidgetFactory::CreateAspectWrappedWidget(WindowSize(100, 100),
			WidgetFactory::CreateBoxWidget(10, WindowSize(500, FIT_PARENT), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WIDGET_TEXTURE_BUTTON)
		)
	);
	m_InternalWidget->AddWidget(WidgetFactory::CreateBoxWidget(10, WindowSize(FIT_PARENT, 100), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.8f), WIDGET_TEXTURE_FRAME));
	m_VerticalStackWidget->AddWidget(m_InternalWidget);


	m_VerticalStackWidget->AddWidget(WidgetFactory::CreateBoxWidget(15, WindowSize(500, 140), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 0.8f), WIDGET_TEXTURE_EXACT));
	m_VerticalStackWidget->AddWidget(WidgetFactory::CreateBoxWidget(10, WindowSize(FIT_PARENT, 130), DirectX::XMFLOAT4(0.5f, 1.0f, 0.0f, 0.8f), WIDGET_TEXTURE_EXACT));
	m_VerticalStackWidget->AddWidget(WidgetFactory::CreateBoxWidget(20, WindowSize(200, FIT_PARENT/*not rendered if height not defined!*/), DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 0.8f), WIDGET_TEXTURE_EXACT));



	StackWidget* vertStack2 = WidgetFactory::CreateStackWidget(StackWidget::Horizontal, 15, WindowSize(ENVELOP_CHILD, ENVELOP_CHILD), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 0.37f), WindowSize(160/*ENVELOP_CHILD*/, ENVELOP_CHILD));
	globVertStack->AddWidget(vertStack2);
	vertStack2->AddWidget(WidgetFactory::CreateBoxWidget(5, WindowSize(100, 60), DirectX::XMFLOAT4(0.7f, 0.2f, 0.0f, 0.8f), WIDGET_TEXTURE_FRAME));
	vertStack2->AddWidget(WidgetFactory::CreateBoxWidget(5, WindowSize(FIT_PARENT, 60), DirectX::XMFLOAT4(0.6f, 0.3f, 0.0f, 0.8f), WIDGET_TEXTURE_FRAME));
}




void WidgetLayout::WidgetLayoutMain::Layout2()
{
	m_VerticalStackWidget = WidgetFactory::CreateStackWidget(StackWidget::Vertical, 15, WindowSize(/*FIT_PARENT*//*800*/ENVELOP_CHILD, 350), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.37f), WindowSize(ENVELOP_CHILD, /*ENVELOP_CHILD*/95));
	m_PageWidget->SetPageWidget(m_VerticalStackWidget);

	m_VerticalStackWidget->AddWidget(
		WidgetFactory::CreateBoxWidget(10, WindowSize(120, FIT_PARENT/*120*//*fixed height should clip the rectangle if parent height is not enough*/), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WIDGET_TEXTURE_FRAME)
	);
	m_VerticalStackWidget->AddWidget(
		WidgetFactory::CreateBoxWidget(10, WindowSize(120, 100/*fixed height should clip the rectangle if parent height is not enough*/), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), WIDGET_TEXTURE_BUTTON)
	);
}





WidgetLayout::WidgetLayoutMain::~WidgetLayoutMain()
{
	WidgetFactory::DeleteWidget(m_PageWidget);
}






void WidgetLayoutMain::CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	// TODO: Replace this with your app's content initialization.
	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(deviceResources));

	OnWindowSizeChanged();
}








// Updates the application state once per frame.
void WidgetLayoutMain::Update()
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
	});
	

	// tweak some params by timer
	int d1 = int(sinf(float(m_timer.GetTotalSeconds())) * 100.0f);
	int d2 = int(sinf(float(m_timer.GetTotalSeconds() *1.8f)) * 20.0f);
	int d3 = int(sinf(float(m_timer.GetTotalSeconds() *2.1f)) * 50.0f);
	
	if (m_InternalWidget)
	{
		m_InternalWidget->SetElementSize(WindowSize(400+d3, /*ENVELOP_CHILD*/100+d3));
	}

	if (m_VerticalStackWidget)
	{
		m_VerticalStackWidget->SetElementSize(WindowSize(ENVELOP_CHILD, 110+d2));
	}

	m_NumberOfSizeCalculations = 0;

	// 1) create layout (define sizes)
	m_PageWidget->UpdateLayout( WindowSize(1366, 696) );
	// 2) build world positions (offset windows)
	m_PageWidget->BuildWorldPosition( WindowPos(0, 0) );



}








// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool WidgetLayoutMain::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	// push widgets to render
	m_sceneRenderer->ResetWidgetList();

	m_sceneRenderer->DisplayNumber(m_NumberOfSizeCalculations);

	// TODO: use visitor or iterator
	m_PageWidget->AddToRender(m_sceneRenderer.get());

	/*
	//	for (int i = 0, ei = _countof(m_Widgets); i < ei; ++i)
	{
	m_sceneRenderer->AddWidgetToList(m_PageWidget);
	}
	*/

	// Render the scene objects.
	return m_sceneRenderer->Render();
}








// Updates application state when the window's size changes (e.g. device orientation change)
void WidgetLayoutMain::OnWindowSizeChanged()
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// Notifies the app that it is being suspended.
void WidgetLayoutMain::OnSuspending()
{
	// TODO: Replace this with your app's suspending logic.

	// Process lifetime management may terminate suspended apps at any time, so it is
	// good practice to save any state that will allow the app to restart where it left off.

	m_sceneRenderer->SaveState();

	// If your application uses video memory allocations that are easy to re-create,
	// consider releasing that memory to make it available to other applications.
}

// Notifes the app that it is no longer suspended.
void WidgetLayoutMain::OnResuming()
{
	// TODO: Replace this with your app's resuming logic.
}

// Notifies renderers that device resources need to be released.
void WidgetLayoutMain::OnDeviceRemoved()
{
	// TODO: Save any necessary application or renderer state and release the renderer
	// and its resources which are no longer valid.
	m_sceneRenderer->SaveState();
	m_sceneRenderer = nullptr;
}


// handle mouse in imgui
extern LRESULT ImGui_ImplDX12_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void WidgetLayoutMain::OnMouseMoved(int x, int y)
{
	ImGui_ImplDX12_WndProcHandler(0, WM_MOUSEMOVE, 0, y << 16 | x);
}

void WidgetLayoutMain::OnMousePressedReleased(bool left, bool right)
{
	ImGui_ImplDX12_WndProcHandler(0, left ? WM_LBUTTONDOWN : WM_LBUTTONUP, 0, 0);
	ImGui_ImplDX12_WndProcHandler(0, right ? WM_RBUTTONDOWN : WM_RBUTTONUP, 0, 0);
}
