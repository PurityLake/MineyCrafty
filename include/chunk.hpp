#ifndef __HPP_CHUNK__
#define __HPP_CHUNK__

#include <memory>
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
    inline static std::unique_ptr<Shader> shader = nullptr;
    inline static std::unique_ptr<TextureAtlas> atlas = nullptr;
    GLuint vao, vbo, texcoord;
    int chunkX, chunkY;
    int xPos, yPos;
    std::vector<std::tuple<int, int, int>> blocks;
    std::vector<std::vector<std::vector<bool>>> bools;

    void blockBool();

public:
    Chunk();
    Chunk(int x, int y);
    virtual ~Chunk();

    static constexpr int l = 20, w = 20, h = 20;

    void init();
    void update(std::shared_ptr<Chunk> left, std::shared_ptr<Chunk> right, 
                std::shared_ptr<Chunk> forward, std::shared_ptr<Chunk> backward);
    void generateData();
    void draw(glm::mat4& trans);
    void addCube(int x, int y, int z);
    void finalise();

    bool isBlockAt(int x, int y, int z);

    std::pair<int, int> getPos();
};

} // namespace MineyCrafty

#endif /* __HPP_CHUNK__ */