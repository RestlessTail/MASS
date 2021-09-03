//GLSLLoader.h : 此文件用于从磁盘加载GLSL程序、设置uniform信息等

#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class GLSLLoader{
public:

	void loadGLSLProgram(std::string vertexPath, std::string fragmentPath);

	std::vector<unsigned int> GLSLPrograms;

	void SetActiveProgram(int index);

	inline unsigned int getShaderID() {
		return ActiveProgramID;
	}

	void setUniformData(std::string key, int value);

	void setUniformData(std::string key, float value);

	void setUniformData(std::string key, bool value);

	void setUniformData(std::string key, float value1, float value2, float value3);

	void setUniformData(std::string key, float value1, float value2, float value3, float value4);

	void setUniformData(std::string key, glm::mat4 value);

private:

	unsigned int ActiveProgramID;
};

