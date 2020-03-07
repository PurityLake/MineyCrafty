#include <chunk.hpp>

#include <camera.hpp>
#include <util/util.hpp>

using namespace std;
using namespace MineyCrafty;

Chunk::Chunk() : Chunk(0, 0) { }

Chunk::Chunk(int x, int y) : chunkX(x), chunkY(y) {
    xPos = chunkX * w;
    yPos = chunkY * l;
}

Chunk::~Chunk() { }

void Chunk::init() {
    shader = Shader(
        util::joinPath({"resources", "vert.glsl"}),
        util::joinPath({"resources", "frag.glsl"})
    );
    shader.init();
    atlas.init();
}

void Chunk::update() {
    vector<GLfloat> coords = atlas.generateTexCoords(
        pair{0, 0}, pair{1, 0}, pair{1, 0}, pair{1, 0}, pair{1, 0}, pair{2, 0}
    );
    vector<glm::vec3> vertices;
    vector<glm::vec2> texcoords;
    for (const auto& pos : blocks) {
        auto& [x, y, z] = pos;
        for (int vert = 0; vert < sizeof(cubeVertices) / sizeof(GLfloat); vert += 3) {
            vertices.push_back(glm::vec3(
                cubeVertices[vert] + x * 2 + chunkX * w,
                cubeVertices[vert + 1] + y * 2,
                cubeVertices[vert + 2] + z  * 2 + chunkY * l
            ));
        }
        for (int c = 0; c < coords.size(); c += 2) {
            texcoords.push_back(glm::vec2(coords[c], coords[c+1]));
        }
    }
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &texcoord);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), &texcoords[0], GL_STATIC_DRAW);
    num_verts = vertices.size();
}

void Chunk::draw(glm::mat4& trans) {
    auto cam = Camera::getMainCamera();
    shader.activate(trans, cam->getView(), cam->getProj());
    glBindVertexArray(vao);
    atlas.activate(shader.getProgram(), glGetUniformLocation(shader.getProgram(), "tex"));
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(shader.getVert());
    glVertexAttribPointer(shader.getVert(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord);
    glEnableVertexAttribArray(shader.getTexCoord());
    glVertexAttribPointer(shader.getTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, num_verts);
    shader.deactivate();
}

void Chunk::addCube(int x, int y, int z) {
    blocks.push_back(make_tuple(x, y, z));
}

void Chunk::finalise() {
    shader.finalise();
    atlas.finalise();
}

pair<int, int> Chunk::getPos() {
    return pair{chunkX, chunkY};
}