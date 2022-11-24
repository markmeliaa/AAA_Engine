#pragma once
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

	LoadTexture(L"image-Baboon.ppm");

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

void ModuleTexture::LoadTexture(const wchar_t* image_file_name)
{
	loaded_image = new DirectX::ScratchImage;
	DirectX::ScratchImage* flip = new DirectX::ScratchImage;

	HRESULT loadResult = LoadFromDDSFile(image_file_name, DirectX::DDS_FLAGS_NONE, &image_metadata, *flip);
	if (FAILED(loadResult))
	{
		loadResult = DirectX::LoadFromTGAFile(image_file_name, &image_metadata, *flip);
		if (FAILED(loadResult))
		{
			loadResult = LoadFromWICFile(image_file_name, DirectX::WIC_FLAGS_NONE, &image_metadata, *flip);
			if (FAILED(loadResult))
			{
				flip = nullptr;
				D_LOG("WIC texture loading: %s FAILED", image_file_name);
			}
		}
	}

	if (flip != nullptr)
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
