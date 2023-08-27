#include "Defen.h"

class ExampleLayer : public Defen::Layer
{
public:
	ExampleLayer() :Layer("Example")
	{
	}

	void OnUpdate() override
	{

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
		PushOverlay(new Defen::ImGuiLayer());
	}

	~Sandbox()
	{
	}
};

Defen::Application* Defen::CreateApplication()
{
	return new Sandbox();
}