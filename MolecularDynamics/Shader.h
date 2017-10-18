#pragma once

#include <vector>
#include "OpenGLObject.h"

namespace OpenGL {

	class Executable : public OpenGLObject {
	protected:
		std::vector<char> ErrorMsg;
	public:
		virtual bool getStatus() const = 0;
		virtual const char* getStatusMessage() = 0;
	};

	class Shader : public Executable
	{
	public:
		Shader(GLuint type = GL_VERTEX_SHADER);
		virtual ~Shader();

		virtual bool getStatus() const;
		virtual const char* getStatusMessage();

		virtual void Bind(); // it's actually a compile, no need to be called, it's called automatically by setSource
		virtual void UnBind(); // does nothing for a shader

		void setSource(const char* src);
	};


	class VertexShader : public Shader {
	public:
		VertexShader();
		virtual GLuint getType() const;
	};


	class FragmentShader : public Shader {
	public:
		FragmentShader();
		virtual GLuint getType() const;
	};

	class GeometryShader : public Shader {
	public:
		GeometryShader();
		virtual GLuint getType() const;
	};

	class ComputeShader : public Shader {
	public:
		ComputeShader();
		virtual GLuint getType() const;
	};
}

