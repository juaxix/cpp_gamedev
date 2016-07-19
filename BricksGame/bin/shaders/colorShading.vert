#version 330 core
//avoid <2008 devices (<OpenGL3) 
//Vertex shader operates on each vertex
//input data from the _vbo, each vertex is 2 floats
in vec2 vertexPosition; //x,y
in vec4 vertexColor; //r,g,b,a
in vec2 vertexUV;

out vec4 fragmentColor; //must have the same name in the fragment shader
//to avoid the interpolation we can use "flat" before out
out vec2 fragmentPosition;
out vec2 fragmentUV;

uniform mat4 camPosition; //to get normalized position in the camera

void main()
{
	//set the x,y position on the screen
	gl_Position.xy = //swiz , we can do 2 times the position for example
		 (camPosition *vec4(vertexPosition, 0.0, 1.0)).xy
	; 
	//The z position is 0 because we are in 2D mode
	gl_Position.z = 0.0;
	//indicate that the coordinates are normalized
	gl_Position.w = 1.0;
	
	//copy positions
	fragmentPosition = vertexPosition;
	fragmentUV = vec2(vertexUV.x,1-vertexUV.y); //flip Y
	
	//set the fragment color
	fragmentColor = vertexColor;
}