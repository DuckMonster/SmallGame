
#include "TextureResource.h"

namespace
{
#pragma pack(1)
	struct TGA_Header
	{
		uint8 id_length;
		uint8 color_map_type;
		uint8 image_type;

		uint16 color_map_first_index;
		uint16 color_map_length;
		uint8 num_bits_per_pixel;

		uint16 origin_x;
		uint16 origin_y;
		uint16 width;
		uint16 height;
		uint8 pixel_depth;
		uint8 image_descriptor;
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
	if (!Assert(header.id_length == 0 && header.color_map_length == 0))
		return false;

	size_t bytes_to_read = header.width * header.height * ( header.pixel_depth / 8 );

	// Read all color data
	unsigned char* pix = new unsigned char[bytes_to_read];
	defer { delete[] pix; };

	size_t bytes_read = fread(pix, 1, bytes_to_read, file);
	Assert(bytes_read == bytes_to_read);

	// TGA is in BGRA format, so swizzle the R and B components
	SwizzleBGRAToRGBA(pix, bytes_read);

	// Create and upload texture data
	texture.Create();
	texture.width = header.width;
	texture.height = header.height;

	texture.Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, header.width, header.width, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);

	return true;
}

void TextureResource::UnloadInternal()
{
	texture.Destroy();
}
