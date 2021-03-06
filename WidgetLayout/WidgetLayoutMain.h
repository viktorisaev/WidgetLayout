﻿#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"

#include <PageWidget.h>
#include <BoxWidget.h>
#include <StackWidget.h>



// Renders Direct3D content on the screen.
namespace WidgetLayout
{
	class WidgetLayoutMain
	{
	public:
		WidgetLayoutMain();
		~WidgetLayoutMain();
		void CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void Update();
		bool Render();

		void OnWindowSizeChanged();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();

		void OnMouseMoved(int x, int y);
		void OnMousePressedReleased(bool left, bool right);

	private:	// func
		void Layout1();
		void Layout2();
		void Layout3();


	private:
		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;

		// Rendering loop timer.
		DX::StepTimer m_timer;


		PageWidget* m_PageWidget = nullptr;
		StackWidget* m_InternalWidget = nullptr;
//		BoxWidget* m_InternalWidget = nullptr;
		StackWidget* m_VerticalStackWidget = nullptr;
		StackWidget* m_HorizontalStackWidget = nullptr;

	};
}