#include "Defen.h"
#include "Defen/Events/KeyEvent.h"

class ExampleLayer : public Defen::Layer
{
public:
	ExampleLayer() :Layer("Example"),
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0)
	{
		m_VertexArray.reset(Defen::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Defen::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Defen::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Defen::BufferLayout layout = {
				{Defen::ShaderDataType::Float3, "a_Position"},
				{Defen::ShaderDataType::Float4, "a_Color"}
			};

			vertexBuffer->SetLayout(layout);
		}

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		std::shared_ptr<Defen::IndexBuffer> indexBuffer;
		indexBuffer.reset(Defen::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Defen::VertexArray::Create());

		float verticesSquare[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Defen::VertexBuffer> squareVB;
		squareVB.reset(Defen::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));
		squareVB->SetLayout({
				{Defen::ShaderDataType::Float3, "a_Position"}
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<Defen::IndexBuffer> squareIB;
		squareIB.reset(Defen::IndexBuffer::Create(squareIndices, 6));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}

		)";

		m_Shader.reset(new Defen::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}

		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}

		)";

		m_BlueShader.reset(new Defen::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void MoveCamera(Defen::Timestep ts)
	{
		if (Defen::Input::IsKeyPressed(DE_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Defen::Input::IsKeyPressed(DE_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Defen::Input::IsKeyPressed(DE_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Defen::Input::IsKeyPressed(DE_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Defen::Input::IsKeyPressed(DE_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Defen::Input::IsKeyPressed(DE_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
	}

	void OnUpdate(Defen::Timestep ts) override
	{
		DE_TRACE("Delta time: {0} ({1}ms)", ts.GetSeconsds(), ts.GetMiliseconds());

		MoveCamera(ts);
		Defen::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.15f, 1 });
		Defen::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Defen::Renderer::BeginScene(m_Camera);
		Defen::Renderer::Submit(m_BlueShader, m_SquareVA);
		Defen::Renderer::Submit(m_Shader, m_VertexArray);

		Defen::Renderer::EndScene();
	}

	void OnEvent(Defen::Event& event) override
	{
		Defen::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Defen::KeyPressedEvent>(DE_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Defen::KeyPressedEvent& event)
	{
		return false;
	}
private:
	std::shared_ptr<Defen::Shader> m_Shader;
	std::shared_ptr<Defen::VertexArray> m_VertexArray;

	std::shared_ptr<Defen::Shader> m_BlueShader;
	std::shared_ptr<Defen::VertexArray> m_SquareVA;

	Defen::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0;
	float m_CameraRotationSpeed = 50.0f;

	float m_CameraMoveSpeed = 3.0f;
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