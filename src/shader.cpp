#include <shader.hpp>

#include <iostream>
#include <fstream>

using namespace std;
using namespace MineyCrafty;

Shader::Shader() { }
Shader::Shader(string vertFilename, string fragFilename, 
	bool storePos, bool storeNormal, bool storeTexCoord,
	bool storeModel, bool storeView, bool storeProj)
    : vertFilename(vertFilename), fragFilename(fragFilename), storePos(storePos), storeNormal(storeNormal),
	storeTexCoord(storeTexCoord), storeModel(storeModel), storeView(storeView), storeProj(storeProj) { }
Shader::~Shader() { }

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
        cout << "Unable to compile vertex shader '" << vertFilename << '\'' << endl;
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
            cout << "Unable to compile fragment shader '" << fragFilename << '\'' << endl;
            printShaderLog(fragmentShader);
            return;
        } else {
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            GLint programSuccess = GL_TRUE;
            glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
            if (programSuccess != GL_TRUE) {
                cout << "Error linking program " << program << endl;
                printProgramLog(program);
                return;
            } else {
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
				if (storePos) {
					pos = glGetAttribLocation(program, "pos");
				}
				if (storeNormal) {
					normal = glGetAttribLocation(program, "normal");
				}
				if (storeTexCoord) {
					texcoord = glGetAttribLocation(program, "texcoord");
				}
				if (storeModel) {
					model = glGetUniformLocation(program, "model");
				}
				if (storeView) {
					view = glGetUniformLocation(program, "view");
				}
				if (storeProj) {
					proj = glGetUniformLocation(program, "proj");
				}
            }
        }
    }
    valid = true;
}

void Shader::activate(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) {
    if (valid) {
        glUseProgram(program);
		if (storeModel) {
			glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m));
		}
		if (storeView) {
			glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(v));
		}
		if (storeProj) {
			glUniformMatrix4fv(proj, 1, GL_FALSE, glm::value_ptr(p));
		}
    }
}

void Shader::deactivate() {
    glUseProgram(0);
}

void Shader::finalise() {
    glDeleteShader(program);
}

void Shader::printProgramLog(GLuint program) {
    int infoLogLength = 0;
    int maxLength = infoLogLength;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
    char* infoLog = new char[ maxLength ];
    glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
    if( infoLogLength > 0 ) {
        cout << infoLog << endl;
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
        cout << infoLog << endl;
    }
    delete[] infoLog;
}