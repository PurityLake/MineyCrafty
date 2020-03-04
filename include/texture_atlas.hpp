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
    SDL_Surface *texture;
    int width, height;
    int cellWidth = 32, cellHeight = 32;
    std::vector<std::vector<GLuint>> textures;
public:
    TextureAtlas();
    TextureAtlas(std::string atlasFilename);
    virtual ~TextureAtlas();

    void init();
    void activate(GLuint program, GLint texLoc, unsigned int x, unsigned int y);
    void finalise();

    void setCellDims(int w, int h);
};

} // namespace MineyCrafty

#endif /* __HPP_TEXTURE__ */