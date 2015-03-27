#ifndef __IMAGEREADER_H
#define __IMAGEREADER_H

#include <string>

#include <vr/Image.h>

namespace vr
{
   class ImageReader
   {
   public:
		 ImageReader() {}

      /// \return a pointer to a loaded image, YOU are responsible for deleting it when done with it.
			virtual Image *readImage(const std::string &) = 0;
   };
}

#endif
