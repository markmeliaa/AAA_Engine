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

private:
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();

	GLuint vbo;
	GLuint ebo;
	GLuint vao;
	int num_vertices;
	int num_indices;
	int material_index;
};

#endif // __Mesh_H__