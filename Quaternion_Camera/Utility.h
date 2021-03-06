#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <fstream>
#include <iostream>

using namespace std;
using namespace glm;

struct Utility
{
	string Read_Shader_Source(const string& filename)
	{
		ifstream file(filename, ios::in);
		string line = "";
		string result = "";
		while (!file.eof())
		{
			getline(file, line);
			result += line + "\n";
		}
		//cout << result;
		return result;
	}


	GLuint CreateShader(int shaderType, const string& ShaderPath)
	{
		GLint ShaderCompiled;
		string ShaderSource = Read_Shader_Source(ShaderPath);
		const char* source = ShaderSource.c_str();
		GLuint ShaderReference = glCreateShader(shaderType);
		glShaderSource(ShaderReference, 1, &source, NULL);

		glCompileShader(ShaderReference);
		CheckOpenGLError();
		glGetShaderiv(ShaderReference, GL_COMPILE_STATUS, &ShaderCompiled);

		if (ShaderCompiled != 1)
		{
			if (shaderType == 35633) cout << "Vertex ";
			if (shaderType == 36488) cout << "Tess Control ";
			if (shaderType == 36487) cout << "Tess Eval ";
			if (shaderType == 36313) cout << "Geometry ";
			if (shaderType == 35632) cout << "Fragment ";
			cout << "shader compilation error." << endl;
			PrintShaderLog(ShaderReference);
		}
		return ShaderReference;
	}

	GLuint CreateProgram(const string& vs_name, const string& fs_name)
	{
		//phan nay chuyen len CreateShader
		/*
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		const char* vs_source = Read_Shader_Source(vs_name).c_str();
		glShaderSource(vs, 1, &vs_source, NULL);
		glCompileShader(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fs_source = Read_Shader_Source(fs_name).c_str();
		glShaderSource(fs, 1, &fs_source, NULL);
		glCompileShader(fs);
		*/

		GLuint vs = CreateShader(GL_VERTEX_SHADER, vs_name);
		GLuint fs = CreateShader(GL_FRAGMENT_SHADER, fs_name);

		GLuint program = glCreateProgram();

		glAttachShader(program, vs);
		glAttachShader(program, fs);

		//glLinkProgram(program);

		program = LinkingProgram(program);

		return program;
	}

	//Check Errors
	bool CheckOpenGLError()
	{
		/*
		bool foundError = false;
		int glErr = glGetError();
		while (glErr != GL_NO_ERROR)
		{
		cout << "glErr: " << glErr << "\n";
		foundError = true;
		glErr = glGetError();
		}
		return foundError;
		*/

		bool foundError = false;
		int glErr = glGetError();
		if (glErr != GL_NO_ERROR)
		{
			//cout << "glErr: " << glErr << "\n";
			foundError = true;
			glErr = glGetError();
		}
		return foundError;
	}

	void PrintShaderLog(GLuint shader)
	{
		int len = 0;
		int char_Written = 0;
		char* log;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			log = (char*)malloc(len);
			glGetShaderInfoLog(shader, len, &char_Written, log);
			cout << "Shader Info Log " << log << "\n";
			free(log);
		}
	}

	void PrintProgramLog(GLuint program)
	{
		int len = 0;
		int char_Written = 0;
		char* log;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			log = (char*)malloc(len);
			glGetProgramInfoLog(program, len, &char_Written, log);
			cout << "Program Info Log " << log << "\n";
			free(log);
		}
	}

	GLuint LinkingProgram(GLuint program)
	{
		GLint linked;
		glLinkProgram(program);
		CheckOpenGLError();
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			cout << "linking failed!" << "\n";
			PrintProgramLog(program);
		}
		return program;
	}
};

#endif // !_UTILITY_H_

