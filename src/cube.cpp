#include <cube.hpp>

#include <iostream>

#include <util.hpp>

using namespace std;
using namespace MineyCrafty;

Cube::Cube() {

}

Cube::~Cube() {

}

void Cube::init() {
    shader = Shader(
        util::joinPath({"resources","vert.glsl"}),
        util::joinPath({"resources", "frag.glsl"}));
    shader.init();
    atlas.init();
    vector<GLfloat> coords = atlas.generateTexCoords(
        pair{0, 0}, pair{1, 0}, pair{1, 0}, pair{1, 0}, pair{1, 0}, pair{2, 0}
    );
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &texcoord);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), &coords[0], GL_STATIC_DRAW);
}

void Cube::draw(glm::mat4& trans) {
    glm::mat4 view = glm::lookAt(
        glm::vec3(3.0f, 3.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glm::mat4 proj = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    shader.activate(trans, view, proj);
    glBindVertexArray(vao);
    atlas.activate(shader.getProgram(), glGetUniformLocation(shader.getProgram(), "tex"));
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(shader.getVert());
    glVertexAttribPointer(shader.getVert(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord);
    glEnableVertexAttribArray(shader.getTexCoord());
    glVertexAttribPointer(shader.getTexCoord(), 2, GL_FLOAT, GL_TRUE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    shader.deactivate();
}

void Cube::finalise() {
    shader.finalise();
    atlas.finalise();
}