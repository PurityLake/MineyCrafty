#include <cube.hpp>

#include <iostream>

using namespace std;
using namespace MineyCrafty;

Cube::Cube() {

}

Cube::~Cube() {
    glDeleteProgram(program);
}

void Cube::init() {
    program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE) {
        cerr << "Unable to compile vertex shader " << vertexShader << endl;
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        glGetShaderiv( vertexShader, GL_INFO_LOG_LENGTH, &maxLength );
        char* infoLog = new char[ maxLength ];
        glGetShaderInfoLog( vertexShader, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            cerr << infoLog << endl;
        }
        delete[] infoLog;
    } else {
        glAttachShader(program, vertexShader);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompiled);
        if (shaderCompiled != GL_TRUE) {
            cerr << "Unable to compile fragmentshader " << fragmentShader << endl;
            int infoLogLength = 0;
            int maxLength = infoLogLength;
            glGetShaderiv( fragmentShader, GL_INFO_LOG_LENGTH, &maxLength );
            char* infoLog = new char[ maxLength ];
            glGetShaderInfoLog( fragmentShader, maxLength, &infoLogLength, infoLog );
            if( infoLogLength > 0 )
            {
                cerr << infoLog << endl;
            }
            delete[] infoLog;
        } else {
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            GLint programSuccess = GL_TRUE;
            glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
            if (programSuccess != GL_TRUE) {
                cerr << "Error linking program " << program << endl;
            } else {
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                glGenVertexArrays(1, &vao);
                glGenBuffers(1, &vbo);
                glGenBuffers(1, &color);
                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, color);
                glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

                uniTrans = glGetUniformLocation(program, "trans");

                posAttrib = glGetAttribLocation(program, "position");
                colAttrib = glGetAttribLocation(program, "color");
            }
        }
    }
}

void Cube::draw(glm::mat4& trans) {
    glUseProgram(program);
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));
    glm::mat4 view = glm::lookAt(
        glm::vec3(6.0f, 6.0f, 6.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    GLint uniView = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    GLint uniProj = glGetUniformLocation(program, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, color);
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glUseProgram(NULL);
}