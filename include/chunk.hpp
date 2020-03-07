#ifndef __HPP_CHUNK__
#define __HPP_CHUNK__

#include <tuple>
#include <vector>

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

#include <cube.hpp>
#include <shader.hpp>
#include <texture_atlas.hpp>

namespace MineyCrafty {

class Chunk {
private:
    int num_verts;
    Shader shader;
    TextureAtlas atlas;
    GLuint vao, vbo, texcoord;
    int chunkX, chunkY;
    int xPos, yPos;
    std::vector<std::tuple<int, int, int>> blocks;

public:
    Chunk();
    Chunk(int x, int y);
    virtual ~Chunk();

    static constexpr int l = 10, w = 10, h = 10;

    void init();
    void update();
    void draw(glm::mat4& trans);
    void addCube(int x, int y, int z);
    void finalise();

    std::pair<int, int> getPos();
};

} // namespace MineyCrafty

#endif /* __HPP_CHUNK__ */