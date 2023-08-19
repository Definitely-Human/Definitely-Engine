#include "Defen.h"

class Sandbox : public Defen::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

Defen::Application* Defen::CreateApplication()
{
	return new Sandbox();
}