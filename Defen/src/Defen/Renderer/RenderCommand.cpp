#include "depch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Defen
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGlRendererAPI();
}