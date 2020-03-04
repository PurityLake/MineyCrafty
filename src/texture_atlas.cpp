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

void TextureAtlas::activate(GLuint program, GLint texLoc) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas);
    //glUniform1i(texLoc, atlas);
}

void TextureAtlas::finalise() {
    glDeleteTextures(1, &atlas);
}

vector<GLfloat> TextureAtlas::generateTexCoords(pair<int, int> top,
            pair<int, int> forward, pair<int, int> left, pair<int, int> backward,
            pair<int, int> right, pair<int, int> bottom) {
    vector<GLfloat> coords;
    coords.reserve(36 * 2);
    if (const auto& [_x, _y] = top; _x >= 0 && _y >= 0) {
        GLfloat xleft = (GLfloat)(_x * cellWidth)  / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight) + cellHeight) / height; 

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
        GLfloat xleft = (GLfloat)(_x * cellWidth)  / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight) + cellHeight) / height; 

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
        GLfloat xleft = (GLfloat)(_x * cellWidth)  / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight) + cellHeight) / height; 

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
        GLfloat xleft = (GLfloat)(_x * cellWidth)  / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight) + cellHeight) / height; 
        
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
        GLfloat xleft = (GLfloat)(_x * cellWidth)  / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight) + cellHeight) / height; 

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
        GLfloat xleft = (GLfloat)(_x * cellWidth)  / width;
        GLfloat ytop = (GLfloat)(_y * cellHeight) / height;
        GLfloat xright = ((GLfloat)(_x * cellWidth) + cellWidth) / width; 
        GLfloat ybottom = (GLfloat)((_y * cellHeight) + cellHeight) / height; 

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