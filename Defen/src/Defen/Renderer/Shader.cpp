#include "depch.h"
#include "Defen/Core.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Defen {
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			DE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(filepath);
		}

		DE_CORE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrt)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			DE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrt);
		}

		DE_CORE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}
}