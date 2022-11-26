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

	bool Init() override;
	bool Start() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	GLuint LoadTexture(const char* image_file_name);
	GLuint CheckImageMetadata() const;
	void SetTextureOptions() const;

	DirectX::ScratchImage* GetImage();
	DirectX::TexMetadata GetImageMetadata();

	int GetMinFilter() const;
	void SetMinFilter();

	int GetMagFilter() const;
	void SetMagFilter();

	int GetWrapMode() const;
	void SetWrapMode();

private:
	DirectX::ScratchImage* loaded_image = nullptr;
	DirectX::TexMetadata image_metadata;

	int min_filter;
	int mag_filter;
	int tex_wrap;
};

#endif // __ModuleTexture_H__