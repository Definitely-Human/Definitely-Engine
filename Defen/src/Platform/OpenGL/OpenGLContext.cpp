#include "depch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Defen
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle)
	{
		DE_CORE_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DE_CORE_ASSERT(status, "Failed to initialize Glad.");

		DE_CORE_INFO("OpenGL Info: ");
		DE_CORE_INFO("  Vendor: {0}: ", (const char*)glGetString(GL_VENDOR));
		DE_CORE_INFO("  Renderer: {0} ", (const char*)glGetString(GL_RENDERER));
		DE_CORE_INFO("  Version: {0} ", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}