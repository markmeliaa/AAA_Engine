#pragma once
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

	void LoadTexture(const char* image_file_name);

	DirectX::ScratchImage* GetImage();
	DirectX::TexMetadata GetImageMetadata();

private:
	DirectX::ScratchImage* loaded_image;
	DirectX::TexMetadata image_metadata;
};