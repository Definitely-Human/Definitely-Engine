#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

#include "Defen.h"
#include "Defen/Events/KeyEvent.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Defen::Layer
{
public:
	ExampleLayer() :Layer("Example"),
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0), m_SquarePosition(0)
	{
		m_VertexArray.reset(Defen::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Defen::Ref<Defen::VertexBuffer> vertexBuffer;
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
		Defen::Ref<Defen::IndexBuffer> indexBuffer;
		indexBuffer.reset(Defen::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Defen::VertexArray::Create());

		float verticesSquare[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0, 0.0,
			 0.5f, -0.5f, 0.0f, 1.0, 0.0,
			 0.5f,  0.5f, 0.0f, 1.0, 1.0,
			-0.5f,  0.5f, 0.0f, 0.0, 1.0
		};

		Defen::Ref<Defen::VertexBuffer> squareVB;
		squareVB.reset(Defen::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));
		squareVB->SetLayout({
				{Defen::ShaderDataType::Float3, "a_Position"},
				{Defen::ShaderDataType::Float2, "a_TexCoord"}
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };
		Defen::Ref<Defen::IndexBuffer> squareIB;
		squareIB.reset(Defen::IndexBuffer::Create(squareIndices, 6));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader.reset(Defen::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string flatShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}

		)";

		m_FlatShader.reset(Defen::Shader::Create(flatShaderVertexSrc, flatShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}

		)";

		m_TextureShader.reset(Defen::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = Defen::Texture2D::Create("assets/textures/Checkerboard.png");
		m_TextureTransparent = Defen::Texture2D::Create("assets/textures/transparent.png");

		std::dynamic_pointer_cast<Defen::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Defen::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
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
		MoveCamera(ts);
		Defen::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.15f, 1 });
		Defen::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Defen::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Defen::OpenGLShader>(m_FlatShader)->Bind();
		std::dynamic_pointer_cast<Defen::OpenGLShader>(m_FlatShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Defen::Renderer::Submit(m_FlatShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Defen::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_TextureTransparent->Bind();
		Defen::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//Defen::Renderer::Submit(m_Shader, m_VertexArray);

		Defen::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
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
	Defen::Ref<Defen::Shader> m_Shader;
	Defen::Ref<Defen::VertexArray> m_VertexArray;

	Defen::Ref<Defen::Shader> m_FlatShader;
	Defen::Ref<Defen::Shader> m_TextureShader;

	Defen::Ref<Defen::Texture2D> m_Texture, m_TextureTransparent;

	Defen::Ref<Defen::VertexArray> m_SquareVA;
	glm::vec3 m_SquarePosition;

	Defen::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0;

	float m_CameraRotationSpeed = 50.0f;
	float m_CameraMoveSpeed = 3.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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