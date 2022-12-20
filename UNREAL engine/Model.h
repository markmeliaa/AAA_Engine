#ifndef __Model_H__
#define __Model_H__

#include "Globals.h"

#include <GL/glew.h>
#include <Geometry/Sphere.h>

#include <vector>
#include <string>

class Mesh;
struct aiMesh;
struct aiMaterial;

class Model
{
public:
	Model(const char* file_name);
	~Model();

	void Draw();

	std::vector<Mesh*> GetMeshes() const { return meshes; }
	int GetNumberVertices() const { return total_num_vertices; }
	int GetNumberIndices() const { return total_num_indices; }
	const Sphere& GetBaseModelBounds() const { return base_model_bounds; }
	const Sphere& GetCurrentModelBounds() const { return current_model_bounds; }
	void SetCurrentModelBounds(const Sphere& s) { current_model_bounds = s; }

	GLuint GetTexture() const { return current_texture; }
	const std::string& GetTextureName() const { return texture_name; }

private:
	void Load(const char* file_name);
	void LoadMaterials(aiMaterial** materials, unsigned int numMaterials, const char* file_name);
	void LoadMeshes(aiMesh** meshes, unsigned int numMeshes);
	void CreateModelSphere(aiMesh** meshes, unsigned int numMeshes);

	std::vector<GLuint> materials = {};
	std::vector<Mesh*> meshes = {};

	int total_num_vertices = 0;
	int total_num_indices = 0;
	Sphere base_model_bounds = Sphere();
	Sphere current_model_bounds = Sphere();

	GLuint current_texture = 0;
	std::string texture_name = "";
};

#endif // __Model_H__