#pragma once
#include "Application.h"

#ifdef DE_PLATFORM_WINDOWS

extern Defen::Application* Defen::CreateApplication();

int main(int argc, char** argv) {
	auto app = Defen::CreateApplication();
	app->Run();
	delete app;
}

#endif
