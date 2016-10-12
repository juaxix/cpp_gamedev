#ifndef __opengl_sample__WinScene__
#define __opengl_sample__WinScene__

#include "IScene.hpp"
#include <vector>
#include <string>

class Plane;
class GUIText;
class Camera;

class WinScene : public IScene
{
	Camera* _camera;
	Plane* _plane;
	GUIText* _text;

	float _loadTime;
	float _waittingTime;

public:
	~WinScene();
	WinScene();
	void load();

	void update();
	void render();
};

#endif /* defined(__opengl_sample__WinScene__) */
