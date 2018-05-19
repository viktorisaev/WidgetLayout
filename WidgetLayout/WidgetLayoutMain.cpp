#include "pch.h"
#include "WidgetLayoutMain.h"
#include "Common\DirectXHelper.h"

using namespace WidgetLayout;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// The DirectX 12 Application template is documented at https://go.microsoft.com/fwlink/?LinkID=613670&clcid=0x409




// Loads and initializes application assets when the application is loaded.
WidgetLayoutMain::WidgetLayoutMain()
{
	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/


	m_Widgets[0] = Widget(DirectX::XMINT2(30, 30), DirectX::XMINT2(500, 200), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_Widgets[1] = Widget(DirectX::XMINT2(400,50), DirectX::XMINT2(300,150), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f));
}






// Creates and initializes the renderers.
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



	for (int i = 0, ei = _countof(m_Widgets); i < ei; ++i)
	{
		float d = sinf(m_timer.GetTotalSeconds()) * 20;

		m_Widgets[0] = Widget(DirectX::XMINT2(30, 30), DirectX::XMINT2(500+d, 200+d), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));

	}


	// push widgets to render
	m_sceneRenderer->ResetWidgetList();
	for (int i = 0, ei = _countof(m_Widgets); i < ei; ++i)
	{
		m_sceneRenderer->AddWidgetToList(m_Widgets[i]);
	}

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

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
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
