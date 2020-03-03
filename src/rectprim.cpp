#include <rectprim.hpp>

#include <iostream>

using namespace std;
using namespace MineyCrafty;

RectPrim::RectPrim() {

}

RectPrim::~RectPrim() {
    glDeleteProgram(program);
}

void RectPrim::init() {
    program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE) {
        cerr << "Unable to compile vertex shader " << vertexShader << endl;
    } else {
        glAttachShader(program, vertexShader);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompiled);
        if (shaderCompiled != GL_TRUE) {
            cerr << "Unable to compile fragmentshader " << fragmentShader << endl;
        } else {
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            GLint programSuccess = GL_TRUE;
            glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
            if (programSuccess != GL_TRUE) {
                cerr << "Error linking program " << program << endl;
            } else {
                vertexPosLoc = glGetAttribLocation(program, "LVertexPos2D");
                if (vertexPosLoc == -1) {
                    cerr << "LVertexPos2D is not a valid glsl program variable!" << endl;
                } else {
                    glGenVertexArrays(1, &vao);
                    glGenBuffers(1, &vbo);
                    glBindVertexArray(vao);
                    glBindBuffer(GL_ARRAY_BUFFER, vbo);
                    glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
                    glDeleteShader(vertexShader);
                    glDeleteShader(fragmentShader);
                }
            }
        }
    }
}

void RectPrim::draw() {
    glUseProgram(program);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vertexPosLoc);
    glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glDisableVertexAttribArray(vertexPosLoc);
    glUseProgram(NULL);
}