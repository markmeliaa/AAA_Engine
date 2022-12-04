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
	inline DirectX::ScratchImage* GetLoadedImage() const { return loaded_image; }

	inline int GetMinFilter() const { return min_filter; }
	void SetMinFilter();
	inline int GetMagFilter() const { return mag_filter; }
	void SetMagFilter();
	inline int GetWrapMode() const { return wrap_mode; }
	void SetWrapMode();

	void SetTextureOptions() const;

private:
	GLuint CheckImageMetadata() const;

	DirectX::ScratchImage* loaded_image = nullptr;
	DirectX::TexMetadata image_metadata;

	unsigned int min_filter = GL_LINEAR_MIPMAP_LINEAR;
	unsigned int mag_filter = GL_LINEAR;
	unsigned int wrap_mode = GL_CLAMP_TO_EDGE;
};

#endif // __ModuleTexture_H__