#pragma once
#include <functional>
#include <glm/glm.hpp>
#include "VertexData.h"
#include "SpriteBatch.h"


namespace xixEngine 
{
	///Class to use in ParticleBatch as reference to draw particles
	class Particle {
	public:
		///Position of the particle
		glm::vec2 position = glm::vec2(0.0f);
		///Velocity to apply in each particle update
		glm::vec2 velocity = glm::vec2(0.0f);
		///Color of this particle
		ColorRGBA8 color;
		///Time in loops for the particle to live
		float lifeTime = 0.0f;
		///Width of the particle
		float width = 0.0f;
	};

	/// Default function pointer for the particle (just adds the velocity to the position)
	inline void defaultParticleUpdate(Particle& particle, float deltaTime) 
	{
		particle.position += particle.velocity * deltaTime;
	}
	///<sumary>Particle Batch to render particles with a <c>SpriteBatch</c></summary>
	class ParticleBatch {
	public:
		///Particle Batch constructor (empty)
		ParticleBatch() {}
		
		///Avoid copies of this class (default is not valid)
		ParticleBatch(ParticleBatch&) = delete; //tell c++11 to remove this 

		///Avoid assigments of this class (default is not valid)
		ParticleBatch& operator=(const ParticleBatch&) = delete;


		///ParticleBatch destructor will delete all the array of Particle pointers (delete[])
		~ParticleBatch() {
			//important: use of delete[] when we do new da
			delete[] m_particles;
		};


		///initializes the ParticleBatch 
		void init(int maxParticles,
			float decayRate,
			GLTexture texture,
			std::function<void(Particle&, float)> updateFunc = defaultParticleUpdate);
		///Updates all the active particles with a deltaTime using their particle update function
		void update(float deltaTime);
		///Draw all the particles using the given SpriteBatch
		void draw(SpriteBatch* spriteBatch);
		///Adds a particle if there is space for it,with the given parms: position, velocity,color and width
		void addParticle(const glm::vec2& position,
			const glm::vec2& velocity,
			const ColorRGBA8& color,
			float width);

	private:
		///search for a dead or empty particle to use and return its index in the array
		int findFreeParticle();
		///Function pointer for custom updates of each Particle
		std::function<void(Particle&, float)> m_updateFunc;
		///Amount to substract to the Particle.lifeTime on each loop of the update function
		float m_decayRate = 0.1f;
		///Array to store Particles
		Particle* m_particles = nullptr;
		///Max num of Particles
		int m_maxParticles = 0;
		///Index of the last particle used 
		int m_lastFreeParticle = 0;
		///Shared GLTexture for this Particle Batch
		GLTexture m_texture;
	};

	///ParticleSystem stores an array of ParticleBatch to update and draw them as we add them
	class ParticleSystem
	{
	public:
		///Default constructor
		ParticleSystem();
		///Destructor: it will delete all the SpriteBatch in the array
		~ParticleSystem();
		///avoid copy construct
		ParticleSystem(ParticleSystem&) = delete;
		///Avoid assigments of this class 
		ParticleSystem& operator=(const ParticleSystem&) = delete;
		///Adds a particle batch 
		void addParticleBatch(ParticleBatch* particleBatch);
		///call all the particlebatch update functions
		void update(float deltaTime);
		///Draw all the spritebatches
		void draw(SpriteBatch* spriteBatch);

	private:
		///array of ParticleBatch
		std::vector<ParticleBatch*> m_batches;
	};
}
