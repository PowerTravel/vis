#ifndef __JPGREADER_H
#define __JPGREADER_H

#include <string>

#include <vr/Image.h>
#include <vr/ImageReader.h>

namespace vr
{
	class JPGReader : public ImageReader
   {
   public:
		 JPGReader();

      /// \return a pointer to a loaded image, YOU are responsible for deleting it when done with it.
      Image *readImage(const std::string &);   
   };
}

#endif
