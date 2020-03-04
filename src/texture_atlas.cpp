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
    int numRows, numCols;
    texture = IMG_Load(filename.c_str());
    width = texture->w;
    height = texture->h;
    numRows = height / cellHeight;
    numCols = width / cellWidth;
    textures.reserve(numRows);
    for (int y = 0; y < numRows; ++y) {
        int x = 0;
        vector<GLuint> row;
        row.reserve(numCols);
        glGenTextures(numCols, &row[0]);
        for (const auto& col : row) {
            glBindTexture(GL_TEXTURE_2D, col);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            SDL_Rect source;
            source.x = x * cellWidth;
            source.y = y * cellHeight;
            source.w = cellWidth;
            source.h = cellHeight;

            SDL_Surface *cell;

            SDL_BlitSurface(texture, &source, cell, NULL);

            int mode = GL_RGB;
            if (cell->format->BytesPerPixel == 4) {
                mode = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, mode, cellWidth, cellHeight, 0, mode, 
                GL_UNSIGNED_BYTE, cell->pixels);
            ++x;
            SDL_FreeSurface(cell);
        }
        textures.push_back(row);
    }
}

void TextureAtlas::activate(GLuint program, GLint texLoc, unsigned int x, unsigned int y) {
    glBindTexture(GL_TEXTURE_2D, textures[y][x]);
}

void TextureAtlas::finalise() {
    SDL_FreeSurface(texture);
    for (auto& tex : textures) {
        glDeleteTextures(width / cellWidth, &tex[0]);
    }
}

void TextureAtlas::setCellDims(int w, int h) {
    cellWidth = w;
    cellHeight = h;
}