#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Widget.h"

// Renders Direct3D content on the screen.
namespace WidgetLayout
{
	class WidgetLayoutMain
	{
	public:
		WidgetLayoutMain();
		void CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void Update();
		bool Render();

		void OnWindowSizeChanged();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();

		void OnMouseMoved(int x, int y);
		void OnMousePressedReleased(bool left, bool right);

	private:
		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;

		// Rendering loop timer.
		DX::StepTimer m_timer;


		Widget m_Widgets[3];

	};
}