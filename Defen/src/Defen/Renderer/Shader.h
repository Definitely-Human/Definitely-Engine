#pragma once

#include <string>

namespace Defen {
	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrt);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;
	};
}