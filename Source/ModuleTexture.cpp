#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"

#include "DirectXTex.h"
#include <string>

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	D_LOG("Load the texture");
	App->editor->log.emplace_back("Load the texture");

	return true;
}

bool ModuleTexture::Start()
{
	return true;
}

update_status ModuleTexture::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	delete loaded_image;
	return true;
}

void ModuleTexture::LoadTexture(const char* image_file_name)
{
	loaded_image = new DirectX::ScratchImage;
	DirectX::ScratchImage* flip = new DirectX::ScratchImage;
	HRESULT loadResult;

	std::string string_file_name = std::string(image_file_name);
	std::string extension = string_file_name.substr(string_file_name.size() - 4, 4);

	std::wstring w_path = std::wstring(string_file_name.begin(), string_file_name.end());

	if (extension == ".dds")
	{
		loadResult = LoadFromDDSFile(w_path.c_str(), DirectX::DDS_FLAGS_NONE, &image_metadata, *flip);
		if (FAILED(loadResult))
		{
			flip = nullptr;
			D_LOG("DDS texture loading: %s FAILED", image_file_name);
		}
	}

	else if (extension == ".tga")
	{
		loadResult = DirectX::LoadFromTGAFile(w_path.c_str(), &image_metadata, *flip);
		if (FAILED(loadResult))
		{
			flip = nullptr;
			D_LOG("TGA texture loading: %s FAILED", image_file_name);
		}
	}

	else
	{
		loadResult = LoadFromWICFile(w_path.c_str(), DirectX::WIC_FLAGS_DEFAULT_SRGB, &image_metadata, *flip);
		if (FAILED(loadResult))
		{
			flip = nullptr;
			D_LOG("WIC texture loading: %s FAILED", image_file_name);
		}
	}

	DirectX::FlipRotate(flip->GetImages(), flip->GetImageCount(), flip->GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, *loaded_image);
}

DirectX::ScratchImage* ModuleTexture::GetImage()
{
	return loaded_image;
}

DirectX::TexMetadata ModuleTexture::GetImageMetadata()
{
	return image_metadata;
}
