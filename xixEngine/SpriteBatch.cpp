#include "SpriteBatch.h"
namespace xixEngine 
{
	SpriteBatch::SpriteBatch() :
		_vbo(0),
		_vao(0),
		_sortType(GlyphSortType::TEXTURE)
	{
	}

	SpriteBatch::~SpriteBatch()
	{	
		for (int i = 0; i < _glyphs.size(); i++)
		{
			delete _glyphs[i];
		}
	}

	void SpriteBatch::createRenderBatches()
	{
		int cv = 0; //current vertex
		int offset = 0; //current offset
		//reserve some memory while keeping actual size:
		std::vector<VertexData> verticesDataArray;
		verticesDataArray.resize(_glyphs.size() * 6); //6 vertices per Glyph: allocate all the mem needed
		//if it's a new texture, create a new batch
		if (_glyphs.empty()) {
			return; //no batches to create
		}
		//Create the first batch for the first glyph
		//
		//RenderBatch newBatch(0, 
		//	6, // just a plane 
		//	_glyphs[0]->texture
		//);
		//_renderBatches.push_back(); //is worst because it does extra checkings
		//

		//Better than a copy, this method does not need a man in the middle ,less mem consumption:
		_renderBatches.emplace_back(0, 6, _glyphs[0]->texture);
		//add the 6 vertices in order:
		
		verticesDataArray[cv++] = _glyphs[0]->topLeft;
		verticesDataArray[cv++] = _glyphs[0]->bottomLeft;
		verticesDataArray[cv++] = _glyphs[0]->bottomRight;
		verticesDataArray[cv++] = _glyphs[0]->bottomRight; //again to connect
		verticesDataArray[cv++] = _glyphs[0]->topRight;
		verticesDataArray[cv++] = _glyphs[0]->topLeft;

		//cg = current Glyph
		for (int cg = 1; cg < _glyphs.size(); cg++)
		{
			//Check the previous texture:
			if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture)
			{
				_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
			}
			else {
				_renderBatches.back().numVertices += 6;
			}
			//add the 6 vertices in order:
			verticesDataArray[cv++] = _glyphs[cg]->topLeft;
			verticesDataArray[cv++] = _glyphs[cg]->bottomLeft;
			verticesDataArray[cv++] = _glyphs[cg]->bottomRight;
			verticesDataArray[cv++] = _glyphs[cg]->bottomRight; 
			verticesDataArray[cv++] = _glyphs[cg]->topRight;
			verticesDataArray[cv++] = _glyphs[cg]->topLeft;
			offset += 6;
		}
		//bind
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//upload can be done as usual:
		
		//glBufferData(
		//	GL_ARRAY_BUFFER, 
		//	verticesDataArray.size()*sizeof(VertexData), 
		//	verticesDataArray.data(), //address for the first element
		//	GL_STREAM_DRAW //binding dynamic allow changes
		//);
		
		//or upload the buffer with a trick make it faster, passing nullptr instead orphan the buffer first and upload later
		glBufferData(GL_ARRAY_BUFFER, verticesDataArray.size()*sizeof(VertexData), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesDataArray.size()*sizeof(VertexData), verticesDataArray.data());
		//unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//smart function to avoid doing buffer assignments each draw
	void SpriteBatch::createVertexArray()
	{
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		//now if anything changes inside it will be stored
		glBindVertexArray(_vao);

		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		//anytime we rebind vertexatt it will bind this buffer forward so we dont need to do this again in the draw function
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//1 array of positions in the 0 index
		glEnableVertexAttribArray(0);
		//same for others
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//where is the vertex positions array? maybe we only want a part of the array
		glVertexAttribPointer(
			0, //index: this is the position of the start of the struct VertexData
			2, //the entire array,of 2 floats
			GL_FLOAT,
			GL_FALSE,  //don't normalize
			sizeof(VertexData), //stride?
			(GLvoid *)offsetof(VertexData, position) //8 bytes after we have the color
													 //(GLvoid *)0 //no pointer right now
			);
		//Now the vertex color part of the array
		glVertexAttribPointer(
			1, //index: this is the position of the start of the struct VertexData
			4, //the entire array,of 4 bytes
			GL_UNSIGNED_BYTE, //type for the color
			GL_TRUE,  //normalize colors from 0-255 to 0-1
			sizeof(VertexData), //stride
			(GLvoid *)offsetof(VertexData, color)
			);
		//Finally, the UV part
		glVertexAttribPointer(
			2, //second attr
			2, // 2 floats
			GL_FLOAT, //type for the coordinates (float)
			GL_FALSE,  //no norm
			sizeof(VertexData), //stride
			(GLvoid *)offsetof(VertexData, uv)
			);
		//unbind
		glBindVertexArray(0);

	}


	//Initialize
	void SpriteBatch::init()
	{
		createVertexArray();

	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) 
	{
		return a->depth < b->depth;
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) 
	{
		return a->depth > b->depth;
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return a->texture < b->texture; //using ascii
	}

	void SpriteBatch::sortGlyphs()
	{
		//from algorithm,safer for us -> std::stable_sort
		switch (_sortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
			break;
		}
	}

	//Set up: sort the glyph and generate batches
	void SpriteBatch::begin(GlyphSortType sortType //= GlyphSortType::TEXTURE default 
		)
	{
		//how are we going to sort? with our enum sort op
		_sortType = sortType;
		//any time we call begin, we clean vectors (they add a lot of vertexdata!)
		_renderBatches.clear(); //just change the size but not free the mem
		for (int i = 0; i < _glyphs.size(); i++) {
			delete _glyphs[i];
		}
		_glyphs.clear(); 
	}

	//Add them to the batch
	void SpriteBatch::draw(const glm::vec4& destintyRectangle, const glm::vec4& uvRectangle, GLuint texture, const float depth, const Color& color)
	{
		Glyph* newGlyph = new Glyph();
		newGlyph->texture = texture;
		newGlyph->depth = depth;
		//1
		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(
			destintyRectangle.x,
			destintyRectangle.y + destintyRectangle.w // or destintyRectangle[3] --> add the height 
			);
		newGlyph->topLeft.setUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);
		//2
		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destintyRectangle.x, destintyRectangle.y);
		newGlyph->bottomLeft.setUV(uvRectangle.x, uvRectangle.y);
		//3
		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destintyRectangle.x
			+ destintyRectangle.z // or [3] -> add the width 
			, destintyRectangle.y
			);
		newGlyph->bottomRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);
		//4
		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destintyRectangle.x + destintyRectangle.z, destintyRectangle.y + destintyRectangle.w);
		newGlyph->topRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);
		//add it
		_glyphs.push_back(newGlyph);
	}
	//
	void SpriteBatch::end()
	{
		sortGlyphs();
		createRenderBatches();
	}

	//Render to the screen
	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			//draw
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);

		}

		glBindVertexArray(0);
	}
}
