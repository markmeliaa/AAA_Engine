#pragma warning( disable : 4267 )
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"

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
	DirectX::TexMetadata this_image_metadata;

	const size_t image_name_size = strlen(image_file_name) + 1;
	wchar_t* w_image_file_name = new wchar_t[image_name_size];
	mbstowcs(w_image_file_name, image_file_name, image_name_size);

	HRESULT loadResult = LoadFromDDSFile(w_image_file_name, DirectX::DDS_FLAGS_NONE, &this_image_metadata, *flip);
	if (FAILED(loadResult))
	{
		loadResult = DirectX::LoadFromTGAFile(w_image_file_name, &this_image_metadata, *flip);
		if (FAILED(loadResult))
		{
			loadResult = LoadFromWICFile(w_image_file_name, DirectX::WIC_FLAGS_NONE, &this_image_metadata, *flip);
			if (FAILED(loadResult))
			{
				flip = nullptr;
				D_LOG("Loading texture FAILED with: %s", image_file_name);
				//App->editor->log.emplace_back("Loading texture FAILED with:");
				//App->editor->log.emplace_back(image_file_name);
			}
		}
	}

	if (flip != nullptr)
		DirectX::FlipRotate(flip->GetImages(), flip->GetImageCount(), flip->GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, *loaded_image);

	image_metadata = this_image_metadata;

	// Allocated memory on the heap must be deallocated
	delete flip;
	delete[] w_image_file_name;

	return CheckImageMetadata();
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

void ModuleTexture::SetMagFilter()
{
	if (mag_filter == GL_NEAREST)
		mag_filter = GL_LINEAR;

	else if (mag_filter == GL_LINEAR)
		mag_filter = GL_NEAREST;
}

void ModuleTexture::SetWrapMode()
{
	if (wrap_mode == GL_REPEAT)
		wrap_mode = GL_MIRRORED_REPEAT;

	else if (wrap_mode == GL_MIRRORED_REPEAT)
		wrap_mode = GL_CLAMP_TO_BORDER;

	else if (wrap_mode == GL_CLAMP_TO_BORDER)
		wrap_mode = GL_CLAMP_TO_EDGE;

	else if (wrap_mode == GL_CLAMP_TO_EDGE)
		wrap_mode = GL_REPEAT;
}

void ModuleTexture::SetTextureOptions() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetMinFilter());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetMagFilter());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapMode());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapMode());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GetWrapMode());
}

GLuint ModuleTexture::CheckImageMetadata() const
{
	GLuint texture_object;
	glGenTextures(1, &texture_object);
	glBindTexture(GL_TEXTURE_2D, texture_object);

	SetTextureOptions();

	int internalFormat, format, type = NULL;
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
		//assert(false && "Unsupported format");
		return 0;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image_metadata.width, image_metadata.height, 0, format, type, loaded_image->GetImage(0, 0, 0)->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture_object;
}