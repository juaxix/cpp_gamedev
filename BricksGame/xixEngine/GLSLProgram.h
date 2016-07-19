/// GLSLProgram.h
/// Header of the class GLSLProgram
#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <glew/glew.h>

namespace xixEngine {
	///<summary>Class to load GLSL programs in the GPU</summary>
	class GLSLProgram
	{
	public:
		///<summary>Constructor: inits variables</summary>
		GLSLProgram();
		///<summary>Destructor</summary>
		~GLSLProgram();
		///<summary>Compile the glsl program and return the result</summary>
		///<param>const string</param>
		bool compileShader(const std::string& filePath, GLuint id);
		///<summary></summary>
		bool compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath); //as reference ,avoiding copy if the paths are really big
																				///<summary>Link shaders with the glsl program</summary>
		bool linkShaders();
		///<summary></summary>
		void addAttribute(const std::string& attributeName);
		///<summary></summary>
		void use();
		///<summary></summary>
		void unUse();
		///<summary></summary>
		GLint getUniformLocation(const std::string& uniformName);
	private:
		///<summary>ID of the glsl program</summary>
		GLuint _programID;
		///<summary>ID of the vertex shader program</summary>
		GLuint _vertexShaderID;
		///<summary>ID of the fragment shader program</summary>
		GLuint _fragmentShaderID;
		///<summary>Current number of attributes of the glslprogram</summary>
		int _numAttributes;
	};

}
