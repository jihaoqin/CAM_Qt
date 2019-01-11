#pragma once
#include <iostream>
#include <assimp/scene.h>
#include "Mesh.h"
#include "boundingbox.h"
#include <string>
#include <QOpenGLContext>
#include "GLBinder.h"
class Model: public GLBinder
{
public:
	Model(const char* filePath);
	~Model();
	BoundingBox boundingBox();
    glm::mat4 modelMatrix();
public:
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
private:
    std::vector<Mesh> meshVec;
    std::string directory;
    glm::mat4 modelMat;
	BoundingBox box;
	void loadModel(const aiScene*);
	void processNode(const aiNode *, const aiScene *);
	Mesh processMesh(const unsigned int, const aiScene *);
};


