/// SpriteBatch.h
/// Header of the class Glyph, RenderBatch and SpriteBatch 
#pragma once

#include <glew/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "VertexData.h"

namespace xixEngine {

	///<summary>Determines how we should sort the glyphs</summary>
	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	///<summary>A glyph is a single quad. These are added via SpriteBatch::draw. Is a struct to minimize the costs of texture switching and #drawcalls. A single sprite with 6 vertices, depth and functions to do operations like rotations</summary>
	class Glyph {
	public:
		///<summary>Constructor</summary>
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);
		///<summary>Constructor</summary>
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);
		///<summary>Default constructor</summary>
		Glyph() {};
		///<summary>Binded texture</summary>
		GLuint texture;
		///<summary>Depth</summary>
		float depth;
		///<summary>Top Left Vertex</summary>
		VertexData topLeft;
		///<summary>Bottom Left Vertex</summary>
		VertexData bottomLeft;
		///<summary>Top Right Vertex</summary>
		VertexData topRight;
		///<summary>Bottom Right Vertex</summary>
		VertexData bottomRight;
	private:
		///<summary>Rotates a point about (0,0) by angle</summary>
		glm::vec2 rotatePoint(const glm::vec2& pos, float angle);
	};

	///<summary>Each render batch is used for a single draw call</summary>
	class RenderBatch {
	public:
		///<summary>RenderBatch constructor</summary>
		///<param>Offset</param>
		///<param>NumVertices</param>
		///<param>Texture</param>
		///<result>RenderBatch</result>
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : 
			offset(Offset),
			numVertices(NumVertices), 
			texture(Texture) 
		{
		}
		///<summary>offset</summary>
		GLuint offset;
		///<summary>number of vertices</summary>
		GLuint numVertices;
		///<summary>texture id from GLTexture</summary>
		GLuint texture;
	};

	///<summary>The SpriteBatch class is a more efficient way of drawing sprites</summary>
	class SpriteBatch
	{
	public:
		///<summary>Constructor</summary>
		SpriteBatch();
		///<summary>Destructor: delete memory</summary>
		~SpriteBatch();

		///<summary>Initializes the spritebatch</summary>
		void init();

		///<summary>Begins the spritebatch</summary>
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

		///<summary>Ends the spritebatch</summary>
		void end();

		///<summary>Adds a glyph to the spritebatch</summary>
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
		///<summary>Adds a glyph to the spritebatch with rotation</summary>
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
		///<summary>Adds a glyph to the spritebatch with rotation</summary>
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);

		///<summary>Renders the entire SpriteBatch to the screen</summary>
		void renderBatch();

	private:
		///<summary>Creates all the needed RenderBatches</summary>
		void createRenderBatches();

		///<summary>Generates our VAO and VBO</summary>
		void createVertexArray();

		///<summary>Sorts glyphs according to _sortType</summary>
		void sortGlyphs();

		///<summary>Comparator used by sortGlyphs()</summary>
		static bool compareFrontToBack(Glyph* a, Glyph* b);
		///<summary>Comparator used by sortGlyphs()</summary>
		static bool compareBackToFront(Glyph* a, Glyph* b);
		///<summary>Comparator used by sortGlyphs()</summary>
		static bool compareTexture(Glyph* a, Glyph* b);
		///<summary>Vertex Buffer Object</summary>
		GLuint _vbo;
		///<summary>All geometry is uploaded to IBO/VBO. We also wrap these up in a VAO: Vertex Array Object</summary>
		GLuint _vao;
		///<summary>Sort Type of Glyph</summary>
		GlyphSortType _sortType;
		///<summary>Pointers for sorting</summary>
		std::vector<Glyph*> _glyphPointers;
		///These are the actual glyphs</summary>
		std::vector<Glyph> _glyphs;
		///<summary>Array of Render Batches</summary>
		std::vector<RenderBatch> _renderBatches;
	};

}