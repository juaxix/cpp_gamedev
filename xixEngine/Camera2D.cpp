#include "Camera2D.h"

namespace xixEngine 
{
	Camera2D::Camera2D() :
		_position(0.0f, 0.0f),
		_camMatrix(1.0f),
		_scale(1.0f),
		_needsMatrixUpdate(true), //the first time we runs it needs to update the matrix
		_screenWidth(800),
		_screenHeight(640),
		_orthoMatrix(1.0f)
	{
	}


	Camera2D::~Camera2D()
	{
	}


	void Camera2D::init(int screenWidth, int screenHeight) {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		//each time the screen res changes, the camera ortho matrix too
		_orthoMatrix = glm::ortho(
			0.0f, //left side of the window
			(float)_screenWidth,//right
			0.0f, //bottom
			(float)_screenHeight//top
			);
	}

	void Camera2D::update() 
	{
		if (_needsMatrixUpdate)
		{
			//Fix for position before scale
			glm::vec3 translate(
				-_position.x + _screenWidth/2, //fix pos
				-_position.y + _screenHeight/2,
				0.0f //only 2D
			);
			
			_camMatrix = glm::translate(_orthoMatrix, translate);

			//Scale: (translate-scale order does not matter in this case)
			//glm::vec3 scale(_scale, _scale, 0.0f);
			//_camMatrix = glm::scale(
			//	//identity matrix with scale * camMatrix
			//	glm::scale(glm::mat4(1.0f), scale)*
			//	_camMatrix, scale
			//);
			//Camera Scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			_camMatrix = glm::scale(glm::mat4(1.0f), scale) * _camMatrix;

			_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::getScreenToWorldCoords(const glm::vec2 screenCoords)
	{
		//top -> -width/2 , -height/2
		glm::vec2 result(screenCoords);
		result -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		//make sure that the scale is applied to the distance calculation. Make it so that 0 is the center
		//Scale coordinates:
		result /= _scale; 
		
		//translate with the camera position
		result += _position;

		return result;
	}	

}