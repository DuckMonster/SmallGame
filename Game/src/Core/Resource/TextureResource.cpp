
#include "TextureResource.h"

namespace
{
#pragma pack(1)
	struct TGA_Header
	{
		uint8 idLength;
		uint8 colorMapType;
		uint8 imageType;

		uint16 colorMapFirstIndex;
		uint16 colorMapLength;
		uint8 numBitsPerPixel;

		uint16 xOrigin;
		uint16 yOrigin;
		uint16 width;
		uint16 height;
		uint8 pixelDepth;
		uint8 imageDescriptor;
	};

	void SwizzleBGRAToRGBA(unsigned char* data, size_t size)
	{
		unsigned char temp = 0;

		for (int i = 0; i < size; i += 4)
		{
			temp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = temp;
		}
	}
}

bool TextureResource::LoadInternal(const char* path)
{
	// Open file
	FILE* file = fopen(path, "r");
	defer { fclose(file); };

	// Read TGA header
	TGA_Header header;
	Assert(sizeof(header) == 18);

	fread(&header, 1, sizeof(header), file);

	// These values are expected to be 0
	if (!Assert(header.idLength == 0 && header.colorMapLength == 0))
		return false;

	size_t bytesToRead = header.width * header.height * ( header.pixelDepth / 8 );

	// Read all color data
	unsigned char* pix = new unsigned char[bytesToRead];
	defer { delete[] pix; };

	size_t bytesRead = fread(pix, 1, bytesToRead, file);
	Assert(bytesRead == bytesToRead);

	// TGA is in BGRA format, so swizzle the R and B components
	SwizzleBGRAToRGBA(pix, bytesRead);

	// Create and upload texture data
	texture.Create();
	texture.width = header.width;
	texture.height = header.height;
	glBindTexture(GL_TEXTURE_2D, texture.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, header.width, header.width, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);

	return true;
}

void TextureResource::UnloadInternal()
{
	texture.Destroy();
}
