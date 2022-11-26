#pragma once
#pragma warning( disable : 4267 )
#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"

#include "DirectXTex.h"
#include "GL/glew.h"
#include <string>

ModuleTexture::ModuleTexture()
{
	min_filter = GL_NEAREST_MIPMAP_LINEAR;
	mag_filter = GL_NEAREST;
	tex_wrap = GL_CLAMP_TO_EDGE;
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	return true;
}

bool ModuleTexture::Start()
{
	D_LOG("Load the texture");
	App->editor->log.emplace_back("Load the texture");
	App->texture->LoadTexture("image-Baboon.ppm");

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetMinFilter());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetMagFilter());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapMode());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapMode());
}

DirectX::ScratchImage* ModuleTexture::GetImage()
{
	return loaded_image;
}

DirectX::TexMetadata ModuleTexture::GetImageMetadata()
{
	return image_metadata;
}

int ModuleTexture::GetMinFilter() const
{
	return min_filter;
}

void ModuleTexture::SetMinFilter()
{
	if (min_filter == GL_NEAREST_MIPMAP_NEAREST)
		min_filter = GL_LINEAR_MIPMAP_NEAREST;

	else if (min_filter == GL_LINEAR_MIPMAP_NEAREST)
		min_filter = GL_NEAREST_MIPMAP_LINEAR;

	else if (min_filter == GL_NEAREST_MIPMAP_LINEAR)
		min_filter = GL_LINEAR_MIPMAP_LINEAR;

	else if (min_filter == GL_LINEAR_MIPMAP_LINEAR)
		min_filter = GL_NEAREST_MIPMAP_NEAREST;
}

int ModuleTexture::GetMagFilter() const
{
	return mag_filter;
}

void ModuleTexture::SetMagFilter()
{
	if (mag_filter == GL_NEAREST)
		mag_filter = GL_LINEAR;

	else if (mag_filter == GL_LINEAR)
		mag_filter = GL_NEAREST;
}

int ModuleTexture::GetWrapMode() const
{
	return tex_wrap;
}

void ModuleTexture::SetWrapMode()
{
	if (tex_wrap == GL_REPEAT)
		tex_wrap = GL_MIRRORED_REPEAT;

	else if (tex_wrap == GL_MIRRORED_REPEAT)
		tex_wrap = GL_CLAMP_TO_BORDER;

	else if (tex_wrap == GL_CLAMP_TO_BORDER)
		tex_wrap = GL_CLAMP_TO_EDGE;

	else if (tex_wrap == GL_CLAMP_TO_EDGE)
		tex_wrap = GL_REPEAT;
}