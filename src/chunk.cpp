#include <chunk.hpp>

#include <camera.hpp>
#include <util/util.hpp>

using namespace std;
using namespace MineyCrafty;

Chunk::Chunk() : Chunk(0, 0) { }

Chunk::Chunk(int x, int y) : chunkX(x), chunkY(y)
{
    xPos = chunkX * Chunk::w;
    yPos = chunkY * Chunk::l;
}

Chunk::~Chunk() { }

void Chunk::init() {
    if (shader == nullptr && atlas == nullptr)
    {
		shader = make_unique<Shader>(
			util::joinPath({ "resources", "vert.glsl" }),
			util::joinPath({ "resources", "frag.glsl" }),
			true, true, true,
			true, true, true
        );
		shadowShader = make_unique<Shader>(
			util::joinPath({ "resources", "shadowvert.glsl" }),
			util::joinPath({ "resources", "shadowfrag.glsl" }),
			true, false, false,
			true, false, false
		);
        atlas = make_unique<TextureAtlas>();
        shader->init();
		shadowShader->init();
        atlas->init();
    }
}

void Chunk::update(std::shared_ptr<Chunk> left, std::shared_ptr<Chunk> right, 
                  std::shared_ptr<Chunk> forward, std::shared_ptr<Chunk> backward)
{
    vector<GLfloat> coords = atlas->generateTexCoords(
        pair{0, 0}, pair{1, 0}, pair{1, 0}, pair{1, 0}, pair{1, 0}, pair{2, 0}
    );
    vector<GLfloat> coordsNoGrass = atlas->generateTexCoords(
        pair{2, 0}, pair{2, 0}, pair{2, 0}, pair{2, 0}, pair{2, 0}, pair{2, 0}
    );
    vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
    vector<glm::vec2> texcoords;
    float chunkModifierX = static_cast<float>(chunkX * Chunk::w * 2);
    float chunkModifierY = static_cast<float>(chunkY * Chunk::l * 2);
    for (const auto& pos : blocks)
    {
		auto&[x, y, z] = pos;

        bool blockAbove = y + 1 < Chunk::h && bools[z][y + 1][x];
        bool blockBelow = y == 0  ? true : bools[z][y - 1][x];

        bool blockRight = x + 1 < Chunk::w ? bools[z][y][x + 1] : true;
        if (x + 1 == Chunk::w && right != nullptr)
        {
            blockRight = right->isBlockAt(0, y, z);
        }
        bool blockLeft = x == 0 ? true : bools[z][y][x - 1];
        if (x == 0 && left != nullptr)
        {
            blockLeft = left->isBlockAt(Chunk::w - 1, y, z);
        }
        bool blockInfront = z + 1 < Chunk::l ? bools[z + 1][y][x] : true;
        if (z + 1 == Chunk::l && forward != nullptr)
        {
            blockInfront = forward->isBlockAt(x, y, 0);
        }
        bool blockBehind = z == 0 ? true : bools[z - 1][y][x];
        if (z == 0 && backward != nullptr)
        {
            blockBehind = backward->isBlockAt(x, y, Chunk::l - 1);
        }
        
		
		if (!(blockLeft && blockRight && blockInfront && blockBehind
			&& blockAbove && blockBelow))
        {
			for (int vert = 0; vert < sizeof(cubeVertices) / sizeof(GLfloat); vert += 3)
            {
				vertices.push_back(glm::vec3(
					cubeVertices[vert] + x * 2 + chunkModifierX,
					cubeVertices[vert + 1] + y * 2,
					cubeVertices[vert + 2] + z * 2 + chunkModifierY
				));
				normals.push_back(glm::vec3(
					cubeNormals[vert], cubeNormals[vert + 1], cubeNormals[vert + 2]
				));
			}
			if (blockAbove)
            {
				for (size_t c = 0; c < coordsNoGrass.size(); c += 2)
                {
					texcoords.push_back(glm::vec2(coordsNoGrass[c], coordsNoGrass[c + 1]));
				}
			}
			else
            {
				for (size_t c = 0; c < coords.size(); c += 2)
                {
					texcoords.push_back(glm::vec2(coords[c], coords[c + 1]));
				}
			}
		}
    }
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &texcoord);
	glGenBuffers(1, &normal);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), &texcoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, normal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	num_verts = static_cast<int>(vertices.size());
}

void Chunk::generateData()
{
    blockBool();
}

void Chunk::draw(glm::mat4& trans, glm::mat4& lightSpace, GLuint depthMap)
{
    auto cam = Camera::getMainCamera();
    shader->activate(trans, cam->getView(), cam->getProj());
	shader->setMat4("lightSpace", lightSpace);
	shader->setVec3("eye", cam->getPos());
	atlas->activate(shader->getProgram());
    glBindVertexArray(vao);
	glEnableVertexAttribArray(shader->getVert());
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(shader->getVert(), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, normal);
	glEnableVertexAttribArray(shader->getNormal());
	glVertexAttribPointer(shader->getNormal(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord);
    glEnableVertexAttribArray(shader->getTexCoord());
    glVertexAttribPointer(shader->getTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(glGetUniformLocation(shader->getProgram(), "shadowMap"), 1);
    glDrawArrays(GL_TRIANGLES, 0, num_verts);
	glDisableVertexAttribArray(shader->getVert());
	glDisableVertexAttribArray(shader->getNormal());
	glDisableVertexAttribArray(shader->getTexCoord());
	glBindVertexArray(0);
	atlas->deactivate();
	shader->deactivate();
}

void Chunk::drawShadow(glm::mat4& trans, glm::mat4& lightSpace)
{
	auto cam = Camera::getMainCamera();
	shadowShader->activate(trans, cam->getView(), cam->getProj());
	shadowShader->setMat4("lightSpace", lightSpace);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(shadowShader->getVert());
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(shadowShader->getVert(), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, num_verts);
	glDisableVertexAttribArray(shadowShader->getVert());
	glBindVertexArray(0);
	shadowShader->deactivate();
}

void Chunk::addCube(int x, int y, int z)
{
    blocks.push_back(make_tuple(x, y, z));
}

void Chunk::finalise()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &texcoord);
    glDeleteVertexArrays(1, &vao);
}

pair<int, int> Chunk::getPos()
{
    return pair{chunkX, chunkY};
}

void Chunk::blockBool()
{
    bools.reserve(Chunk::h);
    for (int z = 0; z < Chunk::w; ++z)
    {
        vector<vector<bool>> row;
        row.reserve(Chunk::w);
        for (int y = 0; y < Chunk::h; ++y)
        {
            vector<bool> col;
            col.reserve(Chunk::h);
            for (int x = 0; x < Chunk::l; ++x)
            {
                col.push_back(false);
            }
            row.push_back(col);
        }
        bools.push_back(row);
    }
    for (const auto& pos : blocks)
    {
        const auto& [x, y, z] = pos;
        bools[z][y][x] = true;
    }
}

bool Chunk::isBlockAt(int x, int y, int z)
{
    return bools[z][y][x];
}