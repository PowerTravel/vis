#ifdef WIN32
#  define _CRT_SECURE_NO_WARNINGS
#  include <windows.h>
#endif
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>


extern "C"
{
#include <png.h>
}
   
#include <vr/PNGReader.h>

using namespace vr;

static jmp_buf png_setjmp_buffer;


void warn_callback(png_structp, png_const_charp pc)
{
   std::cerr << "[PNGReader::warning]: ";
  
   if(pc) 
      std::cerr << pc << std::endl;
}

void err_callback(png_structp, png_const_charp pc)
{
  std::cerr << "[PNGReader::error]: ";
  
  if(pc) 
     std::cerr << pc << std::endl;

	longjmp(png_setjmp_buffer, 1);
}


unsigned char *read_png_file(const char *filename, int *w, int *h, int *c)
{
	png_structp png_ptr;
	png_infop   info_ptr;
	png_uint_32 width, height;

	FILE *fp;
	unsigned char *buffer = NULL;
	int bpp, color_type, interlace_type;
	int bytes_per_row, number_passes, channels;

	if ((fp = fopen(filename, "rb")) == NULL) {
		std::cerr << "Could not read file " << filename << std::endl;
		return 0L;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, err_callback, warn_callback);

	if (!png_ptr) {
		std::cerr << "libpng error" << std::endl;
		fclose(fp);
		return 0L;
	}

	info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr) {
		std::cerr << "libpng error" << std::endl;
		fclose(fp);
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return 0L;
	}

	if (setjmp(png_setjmp_buffer)) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);

		std::cerr << "libpng error" << std::endl;

		if (buffer) {
			// agxAssert ( false ); // Should not happen?!
			delete[] buffer;
		}

		return 0L;
	}

	png_init_io(png_ptr, fp);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bpp, &color_type, &interlace_type, NULL, NULL);
	png_set_strip_16(png_ptr);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);

	if (color_type == PNG_COLOR_TYPE_GRAY && bpp < 8)
		png_set_expand(png_ptr);

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);

	png_read_update_info(png_ptr, info_ptr);

	number_passes = png_set_interlace_handling(png_ptr);
	channels = png_get_channels(png_ptr, info_ptr);

	bytes_per_row = (int)png_get_rowbytes(png_ptr, info_ptr);
	buffer = new unsigned char[bytes_per_row * height + 1];


	png_bytep * row_pointers = (png_bytep*)new unsigned char[height * sizeof(png_bytep)];

#if 0
	if (buffer) {
		unsigned char *dummy[1];

		for (int pass = 0; pass < number_passes; pass++) {
			for (int y = 0; (unsigned int)y < height; y++) {
				dummy[0] = &buffer[bytes_per_row * y]; //( height-1-y )];
				png_read_rows(png_ptr, dummy, NULL, 1);
			}
		}

		png_read_end(png_ptr, info_ptr);
	}
#else
	size_t i;
	for (i = 0; i < height; i++)
	{
		row_pointers[height - 1 - i] = buffer + i * bytes_per_row;
	}

	// read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

#endif
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	fclose(fp);

	if (buffer) {
		*w = (unsigned int)width;
		*h = (unsigned int)height;
		*c = (unsigned int)channels; //( format == 3 ) ? GL_RGB : GL_RGBA;
	}

	return buffer;
}



PNGReader::PNGReader() : ImageReader()
{
}

Image *PNGReader::readImage(const std::string &filename)
{
   int width;
   int height;
   int components;
   unsigned char *data;

   data = read_png_file(filename.c_str(), &width, &height, &components);

   if (data == 0)
      return 0;

   Image *newImage = new Image;
   newImage->setImage(width, height, 1, components, data);
   
   return newImage;
}

