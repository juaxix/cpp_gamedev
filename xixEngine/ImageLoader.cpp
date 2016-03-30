#include "ImageLoader.h"
namespace xixEngine {
	GLTexture ImageLoader::loadPNG(std::string filePath)
	{
		GLTexture texture = {}; //initialize the structure
		std::vector<unsigned char> out_image;

		std::vector<unsigned char> in_data;
		if (IOManager::readFileToBuffer(filePath, in_data) == false) {
			fatalError("Failed to load PNG file to buffer!");
			return texture;
		}
		int errorCode = decodePNG(
			out_image, texture.width, texture.height, &(in_data[0]), in_data.size()
			);

		if (errorCode != 0) {
			fatalError("decodePNG failed with error: " + std::to_string(errorCode)); //convert int to string
			return texture;
		}
		//Create the texture
		glGenTextures(1, &(texture.id));
		//Bind the texture
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA,
			texture.width,
			texture.height,
			0, GL_RGBA, GL_UNSIGNED_BYTE,
			out_image.data()//&(out_image[0])
			); //we have a head in the png ,we need to specify the position[0]

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//mipmapping: filter the texture with a linear interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		//unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;
	}

}
