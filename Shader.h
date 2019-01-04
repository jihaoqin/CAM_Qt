#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
		std::ifstream vertexShaderFile, fragmentShaderFile;
		std::string vStr, fStr;
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try{
			vertexShaderFile.open(vertexShaderPath);
			fragmentShaderFile.open(fragmentShaderPath);
			std::stringstream vss, fss;
			vss << vertexShaderFile.rdbuf();
			fss << fragmentShaderFile.rdbuf();
			vertexShaderFile.close();
			fragmentShaderFile.close();
			vStr = vss.str();
			fStr = fss.str();
		}
		catch(std::ifstream::failure e) {
			std::cerr << "Exception openning/reading/closing file\n";
		}
		const char* vCode = vStr.c_str();
		const char* fCode = fStr.c_str();
		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vs, 1, &vCode, NULL);
		glShaderSource(fs, 1, &fCode, NULL);
		glCompileShader(vs);
		glCompileShader(fs);
		checkCompilation(vs);
		checkCompilation(fs);
		program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		checkLink(program);
		glDeleteShader(vs);
		glDeleteShader(fs);
	}
	~Shader() {
		glDeleteProgram(program);
	}

	void use() {
		glUseProgram(program);
	}
	void setInt(const char* name, int data) {
		glUniform1i(glGetUniformLocation(program, name), data);
	}
	void setVec3(const char* name, glm::vec3 value) {
		glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
	}
	void setMat4(const char* name, const glm::mat4 &trans) {
		unsigned int transformLoc = glGetUniformLocation(program, name);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	}
	void setFloat(const char* name, float value) {
		GLint loc = glGetUniformLocation(program, name);
		glUniform1f(loc, value);
	}
	void setModel(glm::mat4 model) {
		setMat4("model", model);
	}
	GLint program;
private:
	void checkCompilation(GLint shaderID) {
		int success;
		char infoLog[512];
		GLint type;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		glGetShaderiv(shaderID, GL_SHADER_TYPE, &type);
		std::string typeStr;
		if (type == GL_VERTEX_SHADER) {
			typeStr = std::string("GL_VERTEX_SHADER");
		}
		else if (type == GL_FRAGMENT_SHADER) {
			typeStr = std::string("GL_FRAGMENT_SHADER");
		}
		else {
			std::cout << "SHADER_TYPE = "<<type<<std::endl;
		}

		if (!success) {
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::"<<typeStr<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	void checkLink(GLint program) {
		int success;
		char infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
};

