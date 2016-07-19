/// GLSLProgram.h
/// Header of the structures Position, ColorRGBA8, UV, VertexData, GLTexture
#pragma once
#include <glew/glew.h>

namespace xixEngine
{
	///<summary>Stores a pair of x,y (2D Engine)</summary>
	struct Position {
		float x;
		float y;
	};
	///<summary>Structure to store RGBA data</summary>
	///We only need values between 0 to 255 so 
	///we use GLubytes (unsigned char)
	struct ColorRGBA8 { 
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
		ColorRGBA8() :r(255), g(255), b(255), a(255) {};
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
			r(R), g(G), b(B), a(A) {};
	};
	///<summary>Structure to save the U and V floats</summary>
	struct UV {
		float u;
		float v;
	};

	///<summary>Just data for vertices</summary>
	struct VertexData {
		Position position;//x,y -> 8 bytes
						  //float color[3];
						  //instead we pick a smaller datatype :4 bytes (rgb+alpha)
		ColorRGBA8 color;
		//UV Coordinates
		UV uv;

		//functions
		//Set position function
		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}


		///Sets the color vars
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
		///Set the UV vars
		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}


	}; //total = 12 bytes + padding -> GLubyte padding;
	
	///<summary>Structure to store info of textures and use it in the cache. ID, Width,Height</summary>
	struct GLTexture { 
		GLuint id;
		GLulong width, height;
	}; //12 bytes

	//some default colors
	const ColorRGBA8 COLOR_WHITE{ 255,255,255,255 };
	const ColorRGBA8 COLOR_BLUE{0,0,255,255};
	const ColorRGBA8 COLOR_RED{ 255,0,0,255 };
	const ColorRGBA8 COLOR_GREEN{ 0,0,255,255 };
	const ColorRGBA8 COLOR_BLACK{ 0,0,0,255 };
}
