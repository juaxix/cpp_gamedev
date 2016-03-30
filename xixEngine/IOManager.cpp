#include "IOManager.h"
namespace xixEngine {

	/**
	@brief Read a file and save the contents in the buffer
	@param
	*/
	bool IOManager::readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary /* we read all the file at a time, in binary mode */);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		//read the file into the buffer, first make space in mem for it
		//how big the file is? seekg and tellg helps
		//seek to the end
		file.seekg(0, std::ios::end); //go to the ending
		//and now get the file size
		int fileSize = file.tellg(); //how many bytes has we passed with seek?
		file.seekg(0, std::ios::beg); //go to the beginning of  the file again
		//Reduce the file size by any header bytes that might be present
		fileSize -= file.tellg(); //optional, just check the file size
		//Set the size of the buffer now
		buffer.resize(fileSize);
		//read the number of bytes into the buffer
		file.read(
			(char*) //cast from unsigned char 
			&(buffer[0]), //address of the first element, beginning!
			fileSize
			);

		file.close(); //(it will close it anyway when the var is destroyed
		return true;
	}


}
