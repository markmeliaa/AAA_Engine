#pragma once
#pragma warning( disable : 4267 )
#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"

#include "DirectXTex.h"
#include "GL/glew.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::CleanUp()
{
	delete loaded_image;
	return true;
}

GLuint ModuleTexture::LoadTexture(const char* image_file_name)
{
	loaded_image = new DirectX::ScratchImage;
	DirectX::ScratchImage* flip = new DirectX::ScratchImage;

	const size_t image_size = strlen(image_file_name) + 1;
	wchar_t* w_image_file_name = new wchar_t[image_size];
	mbstowcs(w_image_file_name, image_file_name, image_size);

	HRESULT loadResult = LoadFromDDSFile(w_image_file_name, DirectX::DDS_FLAGS_NONE, &image_metadata, *flip);
	if (FAILED(loadResult))
	{
		loadResult = DirectX::LoadFromTGAFile(w_image_file_name, &image_metadata, *flip);
		if (FAILED(loadResult))
		{
			loadResult = LoadFromWICFile(w_image_file_name, DirectX::WIC_FLAGS_NONE, &image_metadata, *flip);
			if (FAILED(loadResult))
			{
				flip = nullptr;
				D_LOG("WIC texture loading: %s FAILED", image_file_name);
			}
		}
	}

	if (flip != nullptr)
		DirectX::FlipRotate(flip->GetImages(), flip->GetImageCount(), flip->GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, *loaded_image);

	return CheckImageMetadata();
}

GLuint ModuleTexture::CheckImageMetadata() const
{
	GLuint texture_object;
	glGenTextures(1, &texture_object);
	glBindTexture(GL_TEXTURE_2D, texture_object);

	SetTextureOptions();

	int internalFormat, format, type;
	switch (image_metadata.format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image_metadata.width, image_metadata.height, 0, format, type, loaded_image->GetImage(0, 0, 0)->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture_object;
}

void ModuleTexture::SetTextureOptions() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}