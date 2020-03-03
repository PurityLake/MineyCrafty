#ifndef __HPP_SHADER__
#define __HPP_SHADER__

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#ifdef _WIN32
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
    GLint vert, col;
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

    GLint getVert() const {
        return vert;
    }

    GLint getCol() const {
        return col;
    }
};

} // namespace MineyCrafty

#endif /* __HPP_SHADER__ */