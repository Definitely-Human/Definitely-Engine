#pragma once
#include "Application.h"

#ifdef DE_PLATFORM_WINDOWS

extern Defen::Application* Defen::CreateApplication();

int main(int argc, char** argv) {
	Defen::Log::Init();
	DE_CORE_WARN("Initialized Log!");
	DE_CORE_INFO("Initialized Log!");

	auto app = Defen::CreateApplication();
	app->Run();
	delete app;
}

#endif
