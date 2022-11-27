#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Module.h"
#include "Globals.h"

#include "DirectXTex.h"
#include "GL/glew.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool CleanUp() override;

	GLuint LoadTexture(const char* image_file_name);

private:
	GLuint CheckImageMetadata() const;
	void SetTextureOptions() const;

	DirectX::ScratchImage* loaded_image = nullptr;
	DirectX::TexMetadata image_metadata;
};

#endif // __ModuleTexture_H__