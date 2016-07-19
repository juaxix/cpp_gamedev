#include "ParticleSystem.h"

namespace xixEngine {
	void ParticleBatch::init(int maxParticles,
		float decayRate,
		GLTexture texture,
		std::function<void(Particle&, float)> updateFunc /*=defaultParticleUpdate*/)
	{
		m_maxParticles = maxParticles;
		m_particles = new Particle[maxParticles];
		m_decayRate = decayRate;
		m_texture = texture;
		m_updateFunc = updateFunc;
	}

	void ParticleBatch::update(float deltaTime) {
		for (int i = 0; i < m_maxParticles; i++) {
			// Check if it is active
			if (m_particles[i].lifeTime > 0.0f) {
				// Update using function pointer
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].lifeTime -= m_decayRate * deltaTime;
			}
		}
	}

	void ParticleBatch::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_maxParticles; i++) {
			// Check if it is active
			auto& p = m_particles[i];
			if (p.lifeTime > 0.0f) {
				glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
				spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.color);
			}
		}
	}

	void ParticleBatch::addParticle(const glm::vec2& position,
		const glm::vec2& velocity,
		const ColorRGBA8& color,
		float width)
	{
		int particleIndex = findFreeParticle();

		auto& p = m_particles[particleIndex];

		p.lifeTime = 1.0f;
		p.position = position;
		p.velocity = velocity;
		p.color = color;
		p.width = width;
	}

	int ParticleBatch::findFreeParticle() {

		for (int i = m_lastFreeParticle; i < m_maxParticles; i++) {
			if (m_particles[i].lifeTime <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i < m_lastFreeParticle; i++)
		{
			if (m_particles[i].lifeTime <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		// No particles are free, overwrite first particle
		return 0;
	}


	ParticleSystem::ParticleSystem()
	{
	}


	ParticleSystem::~ParticleSystem()
	{
		for (auto& b : m_batches) {
			delete b;
		}
	}



	void ParticleSystem::addParticleBatch(ParticleBatch* particleBatch) {
		m_batches.push_back(particleBatch);
	}

	void ParticleSystem::update(float deltaTime) {
		for (auto& b : m_batches) {
			b->update(deltaTime);
		}
	}

	void ParticleSystem::draw(SpriteBatch* spriteBatch) {
		for (auto& b : m_batches) {
			b->draw(spriteBatch);
		}
	}


}
