#include "Defen.h"

class ExampleLayer : public Defen::Layer
{
public:
	ExampleLayer() :Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Defen::Input::IsKeyPressed(DE_KEY_TAB))
			DE_TRACE("Tab is pressed~!");
	}

	void OnEvent(Defen::Event& event) override
	{
		DE_TRACE("{0}", event);
	}
};

class Sandbox : public Defen::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Defen::Application* Defen::CreateApplication()
{
	return new Sandbox();
}