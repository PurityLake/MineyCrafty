#ifndef __HPP_CUBE__
#define __HPP_CUBE__

#include <GL/glew.h>
#ifndef __APPLE__
# include <GL/glu.h>
#else
# include <OpenGL/glu.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>
#include <texture_atlas.hpp>

namespace MineyCrafty {

static inline GLfloat cubeVertices[] = {
    // top
        
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,

    // forward 
    // d ============== c
    // |                |
    // |                |
    // a ============== b
    -1.0f, -1.0f, -1.0f, // a
    -1.0f, -1.0f,  1.0f, // b
    -1.0f,  1.0f,  1.0f, // c

    -1.0f, -1.0f, -1.0f, // a
    -1.0f,  1.0f,  1.0f, // c
    -1.0f,  1.0f, -1.0f, // d

    // left
    // c ============== a
    // |                |
    // |                |
    // b ============== d
     1.0f,  1.0f, -1.0f, // a
    -1.0f, -1.0f, -1.0f, // b
    -1.0f,  1.0f, -1.0f, // c

     1.0f,  1.0f, -1.0f, // a
     1.0f, -1.0f, -1.0f, // d
    -1.0f, -1.0f, -1.0f, // b

    // backward
    // c ============== a
    // |                |
    // |                |
    // b ============== d
     1.0f,  1.0f,  1.0f, // a
     1.0f, -1.0f, -1.0f, // b
     1.0f,  1.0f, -1.0f, // c

     1.0f, -1.0f, -1.0f, // b
     1.0f,  1.0f,  1.0f, // a
     1.0f, -1.0f,  1.0f, // d

    // right
    // d ============== a
    // |                |
    // |                |
    // b ============== c
    -1.0f,  1.0f,  1.0f, // a
    -1.0f, -1.0f,  1.0f, // b
     1.0f, -1.0f,  1.0f, // c

     1.0f,  1.0f,  1.0f, // d
    -1.0f,  1.0f,  1.0f, // a
     1.0f, -1.0f,  1.0f, // c

        // bottom

     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
};

class Cube {
private:
    Shader shader;
    GLuint vao, vbo, texcoord;
    TextureAtlas atlas;
    int x, y, z;

public:
    Cube();
    Cube(int x, int y, int z);
    virtual ~Cube();

    void init();
    void draw(glm::mat4& trans);
    void finalise();

private:
};

} // namespace MineyCrafty

#endif /* __HPP_CUBE__ */