#include "ModelReader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include <string>

using std::vector;
ModelReader::ModelReader(const char* filePath)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	std::string path = filePath;
	directory = path.substr(0, path.find_last_of('/'));
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	if (!scene) {
		std::cout << "Failed to import the model.\n";
	}
	else {
		loadModel(scene);
        unsigned int vSum = 0;
        vector<Vertex> vertexAll;
        vector<unsigned int> indexAll;
        for(auto& m:meshVec){
            auto&& vs = m.vertexs();
            vertexAll.insert(vertexAll.end(), vs.begin(), vs.end());
            auto&& inds = m.indexs();
            for(auto i:inds){
                indexAll.push_back(vSum+i);
            }
            vSum += vs.size();
        }
        m_mesh.setData(vertexAll, indexAll);
	}
}


ModelReader::~ModelReader()
{
}



void ModelReader::loadModel(const aiScene *s) {
	if (!s) {
		std::cout << "Pointer pointed to model is NULL";
	}
	else {
		const aiNode *n = s->mRootNode;
		if (!n) {
			std::cout << "RootNode is NULL.\n";
		}
		else {
			processNode(n, s);
		}
	}
}

void ModelReader::processNode(const aiNode* n, const aiScene* s) {
	for (int i = 0; i < n->mNumMeshes; i++) {
		meshVec.push_back(processMesh(n->mMeshes[i], s));
	}
	unsigned int num = n->mNumChildren;
	if (0 == num) {
		//do nothing
		return;
	}
	else {
		for (int i = 0; i < num; i++) {
			processNode(n->mChildren[i], s);
		}
	}
}

Mesh ModelReader::processMesh(const unsigned int index, const aiScene *s) {
	const aiMesh* m = s->mMeshes[index];
	//process position
	vector<Vertex> verVec;

	for (int i = 0; i < m->mNumVertices; i++) {
		Vertex ver;
		auto& v = m->mVertices[i];
		auto& n = m->mNormals[i];
		ver.vertex = glm::vec3(v.x, v.y, v.z);
		ver.normal = glm::vec3(n.x, n.y, n.z);

		if (m->mTextureCoords[0]) {
			glm::vec2 tex;
			tex.x = m->mTextureCoords[0][i].x;
			tex.y = m->mTextureCoords[0][i].y;
			ver.coordinate = tex;
		}
		verVec.push_back(ver);
	}
	vector<unsigned int> indexs;
	for (int i = 0; i < m->mNumFaces; i++) {
		aiFace& face = m->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indexs.push_back(face.mIndices[j]);
		}
	}

    return Mesh(verVec, indexs);
}


Mesh& ModelReader::modelMesh(){
    return m_mesh;
}
