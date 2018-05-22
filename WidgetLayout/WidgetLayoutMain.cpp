#include "pch.h"
#include "WidgetLayoutMain.h"
#include "Common\DirectXHelper.h"

#include "WidgetFactory.h"

#include <VerticalStackWidget.h>


using namespace WidgetLayout;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;





WidgetLayoutMain::WidgetLayoutMain()
{
	m_PageWidget = WidgetFactory::CreatePageWidget(DirectX::XMINT2(0, 0), WindowSize(1366, 696), DirectX::XMFLOAT4(0.31f, 0.3f, 0.3f, 1.0f));

	VerticalStackWidget *vertStack = WidgetFactory::CreateVerticalStackWidget(DirectX::XMINT2(30, 30), WindowSize(700, 450), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
	m_PageWidget->SetPageWidget(vertStack);

	// vertical children
	m_InternalWidget = WidgetFactory::CreateBoxWidget(5, WindowSize(500, 200), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.8f));
	vertStack->AddWidget(m_InternalWidget);

	vertStack->AddWidget(WidgetFactory::CreateBoxWidget(5, WindowSize(350, 150), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.8f)));

	vertStack->AddWidget(WidgetFactory::CreateBoxWidget(15, WindowSize(80, 50), DirectX::XMFLOAT4(1.0f, 0.5f, 0.0f, 0.8f)));

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
	


//	for (int i = 0, ei = _countof(m_Widgets); i < ei; ++i)
	{
		int d = int(sinf(float(m_timer.GetTotalSeconds())) * 20.0f);

		m_InternalWidget->SetSize(WindowSize(500+d, 200+d));
	}

	m_PageWidget->UpdateLayout(WindowRect(DirectX::XMINT2(0, 0), WindowSize(1366, 696)));

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
