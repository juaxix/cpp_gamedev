/// GLSLProgram.cpp
/// Implementation of the class GLSLProgram

#include "GLSLProgram.h"

namespace xixEngine {
	GLSLProgram::GLSLProgram() : 
		_programID(0), 
		_vertexShaderID(0), 
		_fragmentShaderID(0),
		_numAttributes(0)
	{
                
	}

	bool GLSLProgram::compileShader(const std::string& filePath, GLuint id)
	{
		//open the shader program files
		std::ifstream readedFile(filePath);
		if (readedFile.fail())
		{
			//add the kind of error for output
			perror(filePath.c_str());
			//normal error message
			throw std::runtime_error("Failed to open " + filePath);
			
			return false;
		}

		//read
		std::string fileContents = "";
		std::string line;
		while (std::getline(readedFile, line))
		{
			fileContents += line + "\n"; //add the new line
		}
		//close (optional because when the var is destroyed it's closed too)
		readedFile.close();

		//build array for passing content
		const char* contentsPtr = fileContents.c_str();

		//compile (all the shader in one line!)
		glShaderSource(id, 1, &contentsPtr, nullptr /* only one */);
		glCompileShader(id);
		//Get result of the compilation
		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0; //size of the vector
			glGetShaderiv(id, GL_INFO_LOG_LENGTH /* info log from opengl */, &maxLength);
			//The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			//Provide the infolog in whatever manor you deem best
			//Exit with failure
			glDeleteShader(id); //Avoid leaking
			std::printf("%s\n", &(errorLog[0]));
			throw std::runtime_error("Shader " + filePath + " failed to compile");

			return false;
		}
		return true;
	}

	bool GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		//Create the program to bind shader in
		_programID = glCreateProgram();
		//Create shader programs:
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0)
		{
			throw std::runtime_error("Vertex shader failed to be created");
			return false;
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0)
		{
			throw std::runtime_error("Fragment shader failed to be created");
			return false;
		}

		//open, read and compile each program
		//compile vertex shader
		if (!compileShader(vertexShaderFilePath, _vertexShaderID))
		{
			return false;
		}
		//compile fragment shader
		if (!compileShader(fragmentShaderFilePath, _fragmentShaderID))
		{
			return false;
		}
		return true;
	}

	bool GLSLProgram::linkShaders()
	{
		//Vertex and fragment shaders are successfully compiled
		//Now it's time to link them together into a program (_programID)
		//First: attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Link our program
		glLinkProgram(_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
			//The maxLength includes the NULL char
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);

			//We dont need the program anymore
			glDeleteProgram(_programID);

			//Don't be leaking shaders
			glDetachShader(_programID, _vertexShaderID);
			glDetachShader(_programID, _fragmentShaderID);
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			//Use the info log to show error
			std::printf("%s\n", &(infoLog[0]));
			throw std::runtime_error("Shaders could not be linked");

			return false;
		}

		//Don't be leaking shaders,always dettach after link
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		return true;
	}

	void GLSLProgram::addAttribute(const std::string& attributeName)
	{
		//index position is associated to the number of in var in the shader (0)
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
		//is better than n = n + 1 because it uses ASM: INC

	}

	void GLSLProgram::use()
	{
		if (_programID != 0)
		{
			glUseProgram(_programID);
			for (int i = 0; i < _numAttributes; i++)
			{
				glEnableVertexAttribArray(i);
			}
		}
	}

	void GLSLProgram::unUse()
	{
		if (_programID != 0) {
			glUseProgram(0);
			for (int i = 0; i < _numAttributes; i++)
			{
				glDisableVertexAttribArray(i);
			}
		}
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
	{
		if (_programID == 0)
		{
			return 0;
		}
		else {
			GLint location = glGetUniformLocation(_programID, uniformName.c_str());
			if (location == GL_INVALID_INDEX)
			{
				throw std::runtime_error("Uniform " + uniformName + " not found in shader!");
				return 0;
			}
			else {
				return location;
			}

		}
	}

	GLSLProgram::~GLSLProgram()
	{
	}

}