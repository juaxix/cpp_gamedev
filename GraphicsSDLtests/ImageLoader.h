#pragma once
#include "GLTexture.h"
#include <string>
#include "picopng.h"
#include "IOManager.h"
#include "Errors.h"
class ImageLoader
{
public:
	static GLTexture loadPNG(std::string filePath);
};

