#ifndef __Model_H__
#define __Model_H__

#include "Globals.h"
#include "Mesh.h"

#include <GL/glew.h>
#include <assimp/scene.h>

#include <vector>

class Model
{
public:
	Model(const char* file_name);
	~Model();

	void Draw();

private:
	void Load(const char* file_name);
	void LoadMaterials(aiMaterial** aiMaterial, const unsigned int& numMaterials);
	void LoadMeshes(aiMesh** aiMesh, const unsigned int& numMeshes);

	std::vector<GLuint> materials;
	std::vector<Mesh*> meshes;
};

#endif // __Model_H__