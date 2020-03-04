#ifndef __HPP_SHADER__
#define __HPP_SHADER__

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

namespace MineyCrafty {

class Shader {
private:
    bool valid = false;
    std::string vertFilename, fragFilename;
    GLuint program = 0;
    GLint vert, texcoord;
    GLint model, view, proj;

    std::string readFile(std::string filename);
    void printProgramLog(GLuint program);
    void printShaderLog(GLuint shader);
    
public:
    Shader();
    Shader(std::string vertFilename, std::string fragFilename);
    virtual ~Shader();

    void init();
    void activate(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p);
    void deactivate();
    void finalise();

    const GLuint getProgram() const {
        return program;
    }

    GLint getVert() const {
        return vert;
    }
    GLint getTexCoord() const {
        return texcoord;
    }
};

} // namespace MineyCrafty

#endif /* __HPP_SHADER__ */