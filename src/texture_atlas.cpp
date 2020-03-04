#include <texture_atlas.hpp>

#include <util.hpp>

#include <iostream>

using namespace std;
using namespace MineyCrafty;

TextureAtlas::TextureAtlas()
    : TextureAtlas(util::joinPath(
        {"resources", "textures", "atlas.png"})) { }

TextureAtlas::TextureAtlas(std::string atlasFilename)
    : filename(atlasFilename) { }

TextureAtlas::~TextureAtlas() { }

void TextureAtlas::init() {
    texture = IMG_Load(filename.c_str());
    width = texture->w;
    height = texture->h;
}

void TextureAtlas::activate(GLuint program, GLint texLoc) {

}

void TextureAtlas::finalise() {
    SDL_FreeSurface(texture);
}

void TextureAtlas::setCellDims(int w, int h) {
    cellWidth = w;
    cellHeight = h;
}