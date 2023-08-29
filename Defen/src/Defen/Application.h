#pragma once

#include "Core.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "ImGui/ImGuiLayer.h"

#include "Defen/Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/VertexArray.h"
#include "Defen/Renderer/OrthographicCamera.h"

namespace Defen {
	class DEFEN_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	//To be defined in a client
	Application* CreateApplication();
}
