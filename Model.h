#pragma once
#include <iostream>
#include <assimp/scene.h>
#include "Mesh.h"
#include "boundingbox.h"
#include <string>
#include <QOpenGLContext>
class Model
{
public:
	Model(const char* filePath);
	~Model();
	BoundingBox boundingBox();
    glm::mat4 modelMatrix();
    std::vector<Mesh> meshVec;
public:
    void bindGL(QOpenGLContext*);
	void draw(QOpenGLContext*);
private:
	std::string directory;
    glm::mat4 modelMat;
	BoundingBox box;
	void loadModel(const aiScene*);
	void processNode(const aiNode *, const aiScene *);
	Mesh processMesh(const unsigned int, const aiScene *);
};


