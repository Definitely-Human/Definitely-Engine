#include "depch.h"
#include "Application.h"
#include "Defen/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>

namespace Defen {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application() {}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.2039f, 0.3843f, 0.247f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}