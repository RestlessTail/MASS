#include "GLSLLoader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void GLSLLoader::loadGLSLProgram(std::string vertexPath, std::string fragmentPath)
{

    int success;
    char infoLog[512];

	std::ifstream file_v(vertexPath);
	std::ifstream file_f(fragmentPath);

	std::string content_v((std::istreambuf_iterator<char>(file_v)), std::istreambuf_iterator<char>());
	std::string content_f((std::istreambuf_iterator<char>(file_f)), std::istreambuf_iterator<char>());

    const char* cp_v = content_v.c_str();
    const char* cp_f = content_f.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &cp_v, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        goto end;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &cp_f, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ActiveProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        goto end;
    }

    ActiveProgramID = glCreateProgram();
    glAttachShader(ActiveProgramID, vertexShader);
    glAttachShader(ActiveProgramID, fragmentShader);
    glLinkProgram(ActiveProgramID);

    glGetProgramiv(ActiveProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ActiveProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        goto end;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLSLPrograms.push_back(ActiveProgramID);
end:;
}

void GLSLLoader::SetActiveProgram(int index) {
    ActiveProgramID = GLSLPrograms[index];
    glUseProgram(ActiveProgramID);
}

void GLSLLoader::setUniformData(std::string key, int value) {
    int vertexColorLocation = glGetUniformLocation(ActiveProgramID, key.c_str());
    glUniform1i(vertexColorLocation, value);
}

void GLSLLoader::setUniformData(std::string key, float value) {
    int vertexColorLocation = glGetUniformLocation(ActiveProgramID, key.c_str());
    glUniform1f(vertexColorLocation, value);
}


void GLSLLoader::setUniformData(std::string key, bool value) {
    int vertexColorLocation = glGetUniformLocation(ActiveProgramID, key.c_str());
    glUniform1i(vertexColorLocation, value);
}

void GLSLLoader::setUniformData(std::string key, float value1, float value2, float value3)
{
    int vertexColorLocation = glGetUniformLocation(ActiveProgramID, key.c_str());
    glUniform3f(vertexColorLocation, value1, value2, value3);
}

void GLSLLoader::setUniformData(std::string key, float value1, float value2, float value3, float value4) {
    int vertexColorLocation = glGetUniformLocation(ActiveProgramID, key.c_str());
    glUniform4f(vertexColorLocation, value1, value2, value3, value4);
}

void GLSLLoader::setUniformData(std::string key, glm::mat4 value) {
    unsigned int transformLoc = glGetUniformLocation(ActiveProgramID, key.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}

