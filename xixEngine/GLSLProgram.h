#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include "Errors.h"
namespace xixEngine {
	class GLSLProgram
	{
	private:
		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
		int _numAttributes;

	public:
		GLSLProgram();
		~GLSLProgram();
		bool compileShader(const std::string& filePath, GLuint id);
		bool compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath); //as reference ,avoiding copy if the paths are really big
		bool linkShaders(); //link with the program
		void addAttribute(const std::string& attributeName);
		void use();
		void unUse();
		GLint getUniformLocation(const std::string& uniformName);
	};

}
