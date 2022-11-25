#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Module.h"
#include "Globals.h"

#include "DirectXTex.h"

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

	void LoadTexture(const wchar_t* image_file_name);

	DirectX::ScratchImage* GetImage();
	DirectX::TexMetadata GetImageMetadata();

private:
	DirectX::ScratchImage* loaded_image = nullptr;
	DirectX::TexMetadata image_metadata;
};

#endif // __ModuleTexture_H__