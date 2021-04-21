#ifndef __HPP_SHADER__
#define __HPP_SHADER__

#include <iostream>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#ifndef __APPLE__
# include <GL/glu.h>
#else
# include <OpenGL//glu.h>
#endif

namespace MineyCrafty
{

class Shader
{
private:
    bool valid = false;
    std::string vertFilename, fragFilename;
    GLuint program = 0;
    GLint pos, normal, texcoord;
    GLint model, view, proj;

    std::string readFile(std::string filename);
    void printProgramLog(GLuint program);
    void printShaderLog(GLuint shader);
	bool storePos, storeNormal, storeTexCoord;
	bool storeModel, storeView, storeProj;
    
public:
    Shader();
    Shader(std::string vertFilename, std::string fragFilename, 
		bool storePos, bool storeNormal, bool storeTexCoord,
		bool storeModel, bool storeView, bool storeProj);
    virtual ~Shader();

    void init();
    void activate(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p);
    void deactivate();
    void finalise();

	void setMat4(const std::string& name, glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void setVec3(const std::string& name, glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z);
	}

    const GLuint getProgram() const
	{
        return program;
    }

    GLint getVert() const
	{
        return pos;
    }
    GLint getTexCoord() const
	{
        return texcoord;
    }
	GLint getNormal() const
	{
		return normal;
	}

	bool isStorePos() const
	{
		return storePos;
	}
	bool isStoreNormal() const
	{
		return storeNormal;
	}
	bool isStoreTexCoord() const
	{
		return storeTexCoord;
	}

	bool isStoreModel() const
	{
		return storeModel;
	}
	bool isStoreView() const
	{
		return storeView;
	}
	bool isStoreProj() const
	{
		return storeProj;
	}
};

} // namespace MineyCrafty

#endif /* __HPP_SHADER__ */