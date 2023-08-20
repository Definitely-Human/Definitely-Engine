#include "Application.h"
#include "Defen/Events/ApplicationEvent.h"
#include "Defen/Log.h"

namespace Defen {
	Application::Application() {}
	Application::~Application() {}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		DE_TRACE(e);

		while (true);
	}
}