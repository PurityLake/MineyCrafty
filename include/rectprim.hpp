#ifndef __HPP_RECTPRIM__
#define __HPP_RECTPRIM__

#include <GL/glew.h>
#ifdef _WIN32
# include <GL/glu.h>
#else
# include <OpenGL//glu.h>
#endif

namespace MineyCrafty {

class RectPrim {
private:
    GLuint program = 0;
    GLint vertexPosLoc = -1;
    GLuint vao = 0;
    GLuint vbo = 0;

public:
    RectPrim();
    virtual ~RectPrim();

    void init();
    void draw();

private:
    static constexpr GLfloat vertexData[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };
    static constexpr GLchar* vertexShaderSource[] = {
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};
    static constexpr GLchar* fragmentShaderSource[] = {
        "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
    };
};

} // namespace MineyCrafty

#endif /* __HPP_RECTPRIM__ */