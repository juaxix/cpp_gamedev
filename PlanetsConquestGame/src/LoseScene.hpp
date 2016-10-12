#ifndef __opengl_sample__LoseScene__
#define __opengl_sample__LoseScene__

#include "IScene.hpp"
#include <vector>
#include <string>

class Plane;
class GUIText;
class Camera;

class LoseScene : public IScene
{
	Camera* _camera;
	Plane* _plane;
	GUIText* _text;

	float _loadTime;
	float _waittingTime;

public:
	~LoseScene();
	LoseScene();
	void load();

	void update();
	void render();
};

#endif /* defined(__opengl_sample__LoseScene__) */
