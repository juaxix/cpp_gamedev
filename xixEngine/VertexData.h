#pragma once
#include <GL/glew.h>
namespace xixEngine {
	struct Position {
		float x;
		float y;
	};

	struct Color {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
		void set(GLubyte R, GLubyte G, GLubyte B, GLubyte A) {
			r = R; g = G; b = B; a = A;
		}
	};

	struct UV {
		float u;
		float v;
	};

	//Just data for vertices
	struct VertexData {
		Position position;//x,y -> 8 bytes
		//float color[3];
		//instead we pick a smaller datatype :4 bytes (rgb+alpha)
		Color color;
		//UV Coordinates
		UV uv;

		//functions
		//Set position function
		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}


		//Set the color function
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.set(r, g, b, a);
		}
		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}


	}; //total = 12 bytes + padding -> GLubyte padding;
}
