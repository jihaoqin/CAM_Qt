#pragma once
#include <iostream>
#include <assimp/scene.h>
#include "Mesh.h"
#include "boundingbox.h"
#include <string>
#include <QOpenGLContext>
#include "Color.h"
class ModelReader
{
public:
    ModelReader(const char* filePath);
    Mesh& modelMesh();
    ~ModelReader();
private:
    Mesh m_mesh;
    std::vector<Mesh> meshVec;
    std::string directory;
	void loadModel(const aiScene*);
	void processNode(const aiNode *, const aiScene *);
	Mesh processMesh(const unsigned int, const aiScene *);
};


