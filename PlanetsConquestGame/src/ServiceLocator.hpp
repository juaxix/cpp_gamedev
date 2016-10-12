#ifndef __opengl_sample__ServiceLocator__
#define __opengl_sample__ServiceLocator__

class Input;
class Audio;
class TextureManager;
class WindowInfo;
class SceneManager;
class IScene;

class ServiceLocator
{
    static ServiceLocator* _instance;
    
    Input* _input;
    TextureManager* _textureManager;
	WindowInfo* _windowInfo;
	SceneManager* _sceneMng;
	Audio* _audio;
private:
    ServiceLocator();
    void load();
	void doDestroy();
    
public:
	static ServiceLocator& get();
	static void destroy();
    
    Input& getInput();
	Audio& getAudio();
    TextureManager& getTextureManager();
    IScene& getCurrentScene();
	WindowInfo& getWindowInfo();
	SceneManager& getSceneManager();
};

#endif /* defined(__opengl_sample__ServiceLocator__) */
