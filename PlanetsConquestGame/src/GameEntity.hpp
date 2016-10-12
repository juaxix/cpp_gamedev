#ifndef __opengl_sample__GameEntity__
#define __opengl_sample__GameEntity__

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Tags.hpp"
#include <iostream>
#include <vector>


class GameEntity
{
	static int entityCounter;
    
    glm::vec3 _pos;
    glm::vec3 _scale;
	glm::quat _rotation;
    
	glm::mat4 _model;
    
	int _id;
	Tag _tag;
    
	std::string _name;
    
public:
    GameEntity();
	GameEntity(Tag tag);
    virtual ~GameEntity();
    
	virtual void load(){};
    virtual void update(){};
	virtual void render(){};
    virtual void onDestroy(){};
    
	void setTag(Tag tag);
	Tag getTag();
    
    int getId();
    
    void setPosition(const glm::vec3 &newPos);
    glm::vec3 getPosition() const;
    
    void setScale(const glm::vec3 &newScale);
    const glm::vec3& getScale() const;
    
	void setRotation(const glm::quat &newRot);
	const glm::quat& getRotation() const;
    
	void setName(const std::string& name);
	const std::string& getName();
    
	const glm::mat4& getModel();
    
    void move(const glm::vec3 &pos);

    
private:
	void buildModel();
};

#endif /* defined(__opengl_sample__GameEntity__) */
