#include "Sprite.h"

namespace xixEngine 
{
	Sprite::Sprite() : _vboID(0)
	{

	}


	Sprite::~Sprite()
	{
		//delete the buffer only if it exists
		if (_vboID != 0)
		{
			glDeleteBuffers(1, &_vboID); //just 1
		}
	}


	void Sprite::init(float x, float y, float width, float height, std::string texturePath)
	{
		this->_x = x;
		this->_y = y;
		this->_width = width;
		this->_height = height;

		//load texture from cache
		if (!texturePath.empty())
		{
			//load the player texture
			_texture = ResourceManager::getTexture(texturePath);
		}

		//we only create the buffer if it is not created already
		if (_vboID == 0)
		{
			//we want one buffer by reference
			glGenBuffers(1, &_vboID);
		}

		//first construct the definitions
		//3 vertices for each triangle
		//make space in mem for the array:
		//6 vertices * 2 floats = 12 -> with VertexData 6
		VertexData vertexData[6];
		//now set these in order
		//First triangle
		vertexData[0].setPosition(/*right up*/x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height /*up*/);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x /*origin*/, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//Second triangle:
		vertexData[3].setPosition(x /* origin */, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width /*bottom right*/, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width /*right up*/, y + height);
		vertexData[5].setUV(1.0f, 1.0f);


		/*//With floats:
		//First triangle
		vertexData[0] = x + width; //right up
		vertexData[1] = y + height;

		vertexData[2] = x;		  //up
		vertexData[3] = y + height;

		vertexData[4] = x;		 //origin
		vertexData[5] = y;

		//Second triangle:
		vertexData[6] = x;		//origin
		vertexData[7] = y;

		vertexData[8] = x + width; //bottom right
		vertexData[9] = y;

		vertexData[10] = x + width; //right up
		vertexData[11] = y + height;
		*/


		//Set the colors with a loop
		for (int i = 0; i < 6; i++)
		{
			vertexData[i].color.r = 255;
			vertexData[i].color.g = 0;
			vertexData[i].color.b = 255;
			vertexData[i].color.a = 255;
		}
		//Testing: let's change the color of the top-left vertex
		vertexData[1].setColor(255, 0, 255, 255);

		//change bottom right
		vertexData[4].setColor(0, 255, 0, 255);

		//Bind the buffer:
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		//Upload the data of the buffer to the gpu
		glBufferData(
			GL_ARRAY_BUFFER,
			/* we know the size */
			sizeof(vertexData), //it's a static array
			vertexData,
			GL_STATIC_DRAW /* we will change this later to draw again */
			);
		//end with the binding
		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}


	void Sprite::draw()
	{
		//bind the texture
		glBindTexture(GL_TEXTURE_2D, _texture.id);
		//we do not want to unbind the texture, we do it 

		//instead of using the direct method to draw all the vertices passing one by one to the gpu ,we use a buffer to do the drawing, an array of vertices to build triangles. but we need memory!
		//all we have to do is let the gpu know what to draw ,and we have not to send it every frame by binding with opengl the buffer
		//First bind the buffer again
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		//1 array of positions in the 0 index
		glEnableVertexAttribArray(0);
		//same for others
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//where is the vertex positions array? maybe we only want a part of the array
		glVertexAttribPointer(
			0, //index: this is the position of the start of the struct VertexData
			2, /* the entire array,of 2 floats */
			GL_FLOAT,
			GL_FALSE,  //don't normalize
			sizeof(VertexData), //stride?
			(GLvoid *)offsetof(VertexData, position) //8 bytes after we have the color
			//(GLvoid *)0 //no pointer right now
			);
		//Now the vertex color part of the array
		glVertexAttribPointer(
			1, //index: this is the position of the start of the struct VertexData
			4, /* the entire array,of 4 bytes */
			GL_UNSIGNED_BYTE, //type for the color
			GL_TRUE,  //normalize colors from 0-255 to 0-1
			sizeof(VertexData), //stride
			(GLvoid *)offsetof(VertexData, color)
			);
		//Finally, the UV part
		glVertexAttribPointer(
			2, //second attr
			2, /* 2 floats */
			GL_FLOAT, //type for the coordinates (float)
			GL_FALSE,  //no norm
			sizeof(VertexData), //stride
			(GLvoid *)offsetof(VertexData, uv)
			);

		//Draw all the array of 6 vertices:
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//disable vertex attrib arrays after using them
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		//unbind the buffer at the end of drawing
		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}
}
