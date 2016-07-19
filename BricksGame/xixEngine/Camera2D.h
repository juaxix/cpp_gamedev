/// Camera2D.h
/// Header of the class Camera2D
#pragma once
#include <glm/glm.hpp>
//tools to transform world to camera coordinates:
#include <glm/gtc/matrix_transform.hpp>

namespace xixEngine {
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();
		//Setters
		void setPosition(const glm::vec2& position) {
			_position = position;
			_needsMatrixUpdate = true;
		}
		void setCamMatrix(const glm::mat4& camMatrix) {
			_camMatrix = camMatrix;
		}
		void setScale(float scale) {
			_scale = scale;
			_needsMatrixUpdate = true;
		}

		//Getters
		const glm::vec2 getPosition() const {
			return _position;
		}
		const glm::mat4 getCamMatrix() const {
			return _camMatrix;
		}
		const float getScale() const {
			return _scale;
		}
		void init(int screenWidth, int screenHeight);
		//Camera update
		void update();
		glm::vec2 getScreenToWorldCoords(const glm::vec2 screenCoords);
	private:
		glm::vec2 _position; //x,y
		glm::mat4 _camMatrix; //4x4 matrix
		float _scale;
		bool _needsMatrixUpdate; //do we need to update the matrix?
		int _screenWidth, _screenHeight;
		glm::mat4 _orthoMatrix;
	};

}
