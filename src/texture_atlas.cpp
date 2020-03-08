#include <texture_atlas.hpp>

#include <util/util.hpp>

#include <algorithm>
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
    SDL_Surface *texture = IMG_Load(filename.c_str());
    width = texture->w;
    height = texture->h;
    numRows = height / cellHeight;
    numCols = width / cellWidth;
    glGenTextures(1, &atlas);
    glBindTexture(GL_TEXTURE_2D, atlas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->w, texture->h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);
	SDL_FreeSurface(texture);
}

void TextureAtlas::activate() {
    glBindTexture(GL_TEXTURE_2D, atlas);
}

void TextureAtlas::deactivate() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureAtlas::finalise() {
    glDeleteTextures(1, &atlas);
}

vector<GLfloat> TextureAtlas::generateTexCoords(pair<int, int> top,
            pair<int, int> forward, pair<int, int> left, pair<int, int> backward,
            pair<int, int> right, pair<int, int> bottom) {
    vector<GLfloat> coords;
    GLfloat pixel = 1.0f / width;
    coords.reserve(36 * 2);
    if (const auto& [_x, _y] = top; _x >= 0 && _y >= 0) {
        GLfloat xleft = (GLfloat)(_x * cellWidth + 1.0f) / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight + 1.0f) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth - 1.0f) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight - 1.0f) + cellHeight) / height; 

        coords.push_back(xleft);
        coords.push_back(ytop);
        
        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xright);
        coords.push_back(ybottom);

        coords.push_back(xright);
        coords.push_back(ybottom);

        coords.push_back(xleft);
        coords.push_back(ytop);
        
        coords.push_back(xleft);
        coords.push_back(ybottom);
    }

    if (const auto& [_x, _y] = forward; _x >= 0 && _y >= 0) {
        GLfloat xleft = (GLfloat)(_x * cellWidth + 1.0f) / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight + 1.0f) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth - 1.0f) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight - 1.0f) + cellHeight) / height; 

        coords.push_back(xleft);
        coords.push_back(ybottom);
        
        coords.push_back(xright);
        coords.push_back(ybottom);

        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xleft);
        coords.push_back(ybottom);

        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xleft);
        coords.push_back(ytop);
    }

    if (const auto& [_x, _y] = left; _x >= 0 && _y >= 0) {
        GLfloat xleft = (GLfloat)(_x * cellWidth + 1.0f) / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight + 1.0f) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth - 1.0f) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight - 1.0f) + cellHeight) / height; 

        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xleft);
        coords.push_back(ybottom);

        coords.push_back(xleft);
        coords.push_back(ytop);

        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xright);
        coords.push_back(ybottom);

        coords.push_back(xleft);
        coords.push_back(ybottom);
    }

    if (const auto& [_x, _y] = backward; _x >= 0 && _y >= 0) {
        GLfloat xleft = (GLfloat)(_x * cellWidth + 1.0f) / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight + 1.0f) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth - 1.0f) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight - 1.0f) + cellHeight) / height; 
        
        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xleft);
        coords.push_back(ybottom);

        coords.push_back(xleft);
        coords.push_back(ytop);

        coords.push_back(xleft);
        coords.push_back(ybottom);

        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xright);
        coords.push_back(ybottom);
    }

    if (const auto& [_x, _y] = right; _x >= 0 && _y >= 0) {
        GLfloat xleft = (GLfloat)(_x * cellWidth + 1.0f) / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight + 1.0f) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth - 1.0f) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight - 1.0f) + cellHeight) / height;

        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xleft);
        coords.push_back(ybottom);

        coords.push_back(xright);
        coords.push_back(ybottom);

        coords.push_back(xleft);
        coords.push_back(ytop);

        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xright);
        coords.push_back(ybottom);
    }

    if (const auto& [_x, _y] = bottom; _x >= 0 && _y >= 0) {
        GLfloat xleft = (GLfloat)(_x * cellWidth + 1.0f) / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight + 1.0f) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth - 1.0f) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight - 1.0f) + cellHeight) / height; 

        coords.push_back(xleft);
        coords.push_back(ytop);
        
        coords.push_back(xright);
        coords.push_back(ytop);

        coords.push_back(xright);
        coords.push_back(ybottom);

        coords.push_back(xright);
        coords.push_back(ybottom);

        coords.push_back(xleft);
        coords.push_back(ytop);
        
        coords.push_back(xleft);
        coords.push_back(ybottom);
    }
    return coords;
}

void TextureAtlas::setCellDims(int w, int h) {
    cellWidth = w;
    cellHeight = h;
}