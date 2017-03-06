#ifndef PARTICLE_H
#define PARTICLE_H

#include <mge/core/GameObject.hpp>
#include "Content/GameObjects/ParticleSystem.hpp"
#include <glm.hpp>
#include <cassert>
#include <iostream>

class Particle : public GameObject
{
public:
	Particle(glm::vec3 pPosition, std::string pName, glm::vec3 pDirection, float pDuration, ParticleSystem * pOwner, int pIndex);
	void ResetDuration(float pDuration);
	float getDuration();
	float getScale();
	void setDuration(float pDuration);
	virtual ~Particle();
	void update(float pStep);
	bool updateParticle(float pStep);
	void SetStartEndScale(float pStartscale, float pEndScale);

private:
	int _index = 0;
	ParticleSystem * _owner;
	glm::vec3 direction;
	glm::vec3 originalDirection;
	float startScaleSize = 1.0f;
	float endScaleSize = 1.0f;
	float _OriginalDuration;
	float _scale;
	float _duration;
	float _currentscale = 1.0f;
};

#endif // GAMEOBJECT_H
