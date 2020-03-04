#include <chunk.hpp>

#include <util.hpp>

using namespace std;
using namespace MineyCrafty;

Chunk::Chunk() { }
Chunk::~Chunk() { }

template<typename T>
void reshape(vector<T> &out, vector<vector<vector<T>>> v, int num_items) {
    out.reserve(num_items);
    for (const auto& z : v) {
        for (const auto& y : z) {
            for (const auto& x : y) {
                out.push_back(x);
            }
        }
    }
}

void Chunk::init() {
    shader = Shader(
        util::joinPath({"resources", "vert.glsl"}),
        util::joinPath({"resources", "frag.glsl"})
    );
    shader.init();
    atlas.init();
    vector<GLfloat> coords = atlas.generateTexCoords(
        pair{0, 0}, pair{1, 0}, pair{1, 0}, pair{1, 0}, pair{1, 0}, pair{2, 0}
    );
    int num_chunks = 0;
    int num_coords = 0;
    chunk = vector<vector<vector<glm::vec3>>>();;
    chunk.reserve(h);
    texcoords = vector<vector<vector<glm::vec2>>>();;
    texcoords.reserve(h);
    for (int i = 0; i < h; ++i) {
        vector<vector<glm::vec3>> y;
        y.reserve(w);
        vector<vector<glm::vec2>> texcoordy;
        texcoordy.reserve(w);
        for (int j = 0; j < w; ++j) {
            vector<glm::vec3> x;
            x.reserve(sizeof(cubeVertices) / sizeof(GLfloat));
            vector<glm::vec2> texcoordx;
            texcoordx.reserve(coords.size());
            for (int k = 0; k < l; ++k) {
                for (int vert = 0; vert < sizeof(cubeVertices) / sizeof(GLfloat); vert += 3) {
                    x.push_back(glm::vec3(
                        cubeVertices[vert] + i * 2,
                        cubeVertices[vert + 1] + j * 2,
                        cubeVertices[vert + 2] + k * 2
                    ));
                    ++num_chunks;
                }
                for (int c = 0; c < coords.size(); c += 2) {
                    texcoordx.push_back(glm::vec2(coords[c], coords[c+1]));
                    ++num_coords;
                }
            }
            y.push_back(x);
            texcoordy.push_back(texcoordx);
        }
        chunk.push_back(y);
        texcoords.push_back(texcoordy);
    }
    vector<glm::vec3> vertices;
    vector<glm::vec2> _coords;
    reshape(vertices, chunk, num_chunks);
    reshape(_coords, texcoords, num_coords);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &texcoord);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord);
    glBufferData(GL_ARRAY_BUFFER, _coords.size() * sizeof(glm::vec2), &_coords[0], GL_STATIC_DRAW);
    num_verts = num_chunks;
}

void Chunk::draw(glm::mat4& trans) {
    glm::mat4 view = glm::lookAt(
        glm::vec3(20.0f, 20.0f, 20.0f),
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
    glVertexAttribPointer(shader.getTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, num_verts);
    shader.deactivate();
}

void Chunk::addCube(int x, int y, int z) {

}
void Chunk::finalise() {
    shader.finalise();
    atlas.finalise();
}