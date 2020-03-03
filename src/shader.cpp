#include <shader.hpp>

#include <iostream>
#include <fstream>

using namespace std;
using namespace MineyCrafty;

GLint vert, col;

Shader::Shader() { }
Shader::Shader(string vertFilename, string fragFilename)
    : vertFilename(vertFilename), fragFilename(fragFilename) { }
Shader::~Shader() {
    glDeleteShader(program);
}

string Shader::readFile(string filename) {
    ifstream in(filename, ios::in);
    if (in) {
        string contents;
        in.seekg(0, ios::end);
        contents.reserve(in.tellg());
        in.seekg(0, ios::beg);
        contents.assign((istreambuf_iterator<char>(in)), (istreambuf_iterator<char>()));
        return contents;
    }
    throw("Error reading file '" + filename + "'");
}

void Shader::init() {
    valid = false;
    program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    string vertexShaderString = readFile(vertFilename);
    const char *vertexShaderSource = vertexShaderString.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE) {
        cerr << "Unable to compile vertex shader '" << vertFilename << '\'' << endl;
        printShaderLog(vertexShader);
        return;
    } else {
        glAttachShader(program, vertexShader);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        string fragShaderString = readFile(fragFilename);
        const char *fragmentShaderSource = fragShaderString.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompiled);
        if (shaderCompiled != GL_TRUE) {
            cerr << "Unable to compile fragment shader '" << fragFilename << '\'' << endl;
            printShaderLog(fragmentShader);
            return;
        } else {
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            GLint programSuccess = GL_TRUE;
            glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
            if (programSuccess != GL_TRUE) {
                cerr << "Error linking program " << program << endl;
                printProgramLog(program);
                return;
            } else {
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                vert = glGetAttribLocation(program, "vert");
                if (vert == -1) {
                    cerr << "vert is not a valid glsl program variable!" << endl;
                    return;
                } else {
                    col = glGetAttribLocation(program, "col");
                    if (col == -1) {
                        cerr << "col is not a valid glsl program variable!" << endl;
                        return;
                    } else {
                        model = glGetUniformLocation(program, "model");
                        view  = glGetUniformLocation(program, "view");
                        proj  = glGetUniformLocation(program, "proj");
                    }
                }
            }
        }
    }
    valid = true;
}

void Shader::activate(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) {
    if (valid) {
        glUseProgram(program);
        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(v));
        glUniformMatrix4fv(proj, 1, GL_FALSE, glm::value_ptr(p));
    }
}

void Shader::deactivate() {
    glUseProgram(0);
}

void Shader::printProgramLog(GLuint program) {
    int infoLogLength = 0;
    int maxLength = infoLogLength;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
    char* infoLog = new char[ maxLength ];
    glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
    if( infoLogLength > 0 ) {
        cerr << infoLog << endl;
    }
    delete[] infoLog;
}

void Shader::printShaderLog(GLuint shader) {
    int infoLogLength = 0;
    int maxLength = infoLogLength;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
    char* infoLog = new char[ maxLength ];
    glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
    if( infoLogLength > 0 ) {
        cerr << infoLog << endl;
    }
    delete[] infoLog;
}