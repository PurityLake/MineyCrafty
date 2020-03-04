#ifndef __HPP_CHUNK__
#define __HPP_CHUNK__

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
    static constexpr int l = 5, w = 5, h = 5;
    int num_verts;
    Shader shader;
    TextureAtlas atlas;
    GLuint vao, vbo, texcoord;
    std::vector<std::vector<std::vector<glm::vec3>>> chunk;
    std::vector<std::vector<std::vector<glm::vec2>>> texcoords;

public:
    Chunk();
    virtual ~Chunk();

    void init();
    void draw(glm::mat4& trans);
    void addCube(int x, int y, int z);
    void finalise();
};

} // namespace MineyCrafty

#endif /* __HPP_CHUNK__ */