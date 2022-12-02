#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Module.h"
#include "Globals.h"

#include <GL/glew.h>
#include <DirectXTex.h>

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool CleanUp() override;

	GLuint LoadTexture(const char* image_file_name);

	inline DirectX::TexMetadata GetImageMetadata() const { return image_metadata; }

private:
	GLuint CheckImageMetadata() const;
	void SetTextureOptions() const;

	DirectX::ScratchImage* loaded_image = nullptr;
	DirectX::TexMetadata image_metadata;
};

#endif // __ModuleTexture_H__