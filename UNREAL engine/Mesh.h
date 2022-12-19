#ifndef __Mesh_H__
#define __Mesh_H__

#include <assimp/mesh.h>
#include <GL/glew.h>

#include <vector>

class Mesh
{
public:
	Mesh(const aiMesh* mesh);
	~Mesh();

	void Draw(const std::vector<GLuint>& model_textures);

	int GetNumVertices() const { return num_vertices; }
	int GetNumIndices() const { return num_indices; }

private:
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();

	GLuint vbo = 0;
	GLuint ebo = 0;
	GLuint vao = 0;
	int num_vertices = 0;
	int num_indices = 0;
	int material_index = 0;
};

#endif // __Mesh_H__