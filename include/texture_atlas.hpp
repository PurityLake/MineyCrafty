#ifndef __HPP_TEXTURE__
#define __HPP_TEXTURE__

#include <string>
#include <vector>

#include <GL/glew.h>
#ifndef __APPLE__
# include <GL/glu.h>
#else
# include <OpenGL//glu.h>
#endif

#include <SDL.h>
#include <SDL_image.h>

namespace MineyCrafty {

class TextureAtlas {
private:
    std::string filename;
    GLuint atlas;
    int width, height;
    int cellWidth = 32, cellHeight = 32;
public:
    TextureAtlas();
    TextureAtlas(std::string atlasFilename);
    virtual ~TextureAtlas();

    void init();
    void activate(GLuint program);
    void finalise();

    std::vector<GLfloat> generateTexCoords(std::pair<int, int> top,
            std::pair<int, int> forward, std::pair<int, int> left,
            std::pair<int, int> backward, std::pair<int, int> right,
            std::pair<int, int> bottom);

    void setCellDims(int w, int h);
};

} // namespace MineyCrafty

#endif /* __HPP_TEXTURE__ */