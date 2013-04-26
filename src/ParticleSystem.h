#pragma once

#include "VertexBuffer.h"

//TODO - AOS vs SOA and benchmark?
static float particle_speed = 200.0f;
static float particle_rotation = 5.0f;
static float particle_size = 20.0f;

template <class P>
struct CParticle
{
	void	Init(P pos)
	{
		m_TotalLifeTime = randFloat(0.5f, 2.0f);
		m_Life = m_TotalLifeTime;

		m_Pos = pos;

		m_Velocity = randVec2f() * particle_speed;
		
		m_Rotation = randFloat(-(float)M_PI, (float)M_PI);

		float f = (randInt(0, 2) - 0.5f) * 2.0f;
		m_AngularVelocity = randFloat((float)M_PI * 0.5f, (float)M_PI) * particle_rotation * f;
	}

	void	Update(float dt)
	{
		m_Pos += m_Velocity * dt;
		m_Velocity *= 0.99f;

		m_Rotation += m_AngularVelocity * dt;
		m_AngularVelocity *= 0.95f;

		m_Life -= dt;
	}

	float GetSize()
	{
		//return (1.0f - m_Life/m_TotalLifeTime) * particle_size;
		return (float)sin((m_Life/m_TotalLifeTime) * M_PI) * particle_size;
	}

	float GetAngle()
	{
		return m_Rotation;
	}

	ci::ColorA GetColor()
	{
		return lerp(ColorA(1.0f, 1.0f, 0.0f, 0.0f), ColorA(1.0f, 0.0f, 0.0f, 1.0f), m_Life/m_TotalLifeTime);
	}

	P		m_Pos;
	P		m_Velocity;

	float	m_Rotation;
	float	m_AngularVelocity;

	float	m_TotalLifeTime;
	float	m_Life;
};

template <class V, class P>
class CParticleSystem
{
public:
	CParticleSystem(P emitter_pos, int num_particles)
		:
	m_EmitterPosition(emitter_pos),
	m_NumParticles(num_particles)
	{
		mp_VertexBuffer = new CVertexBuffer<V>(num_particles * 6);
		mp_Particles = new CParticle<P>[num_particles];
	}

	void				Update(float dt)
	{
		for(int i=0; i<m_NumParticles; ++i)
		{
			//Update
			mp_Particles[i].Update(dt);

			//Reset if dead
			if(mp_Particles[i].m_Life <= 0.0f)
			{
				mp_Particles[i].Init(m_EmitterPosition);
			}

			//add geometry
			P centre = mp_Particles[i].m_Pos;
			ColorA col = mp_Particles[i].GetColor();
			float angle = mp_Particles[i].GetAngle();
			float size = mp_Particles[i].GetSize();

			mp_VertexBuffer->AddSprite(centre, size, Rectf(0,0,1,1), col, angle);
		}
	}

	void				Draw()
	{
		mp_VertexBuffer->Draw();
	}

private:
	CVertexBuffer<V>*	mp_VertexBuffer;
	P					m_EmitterPosition;

	int					m_NumParticles;
	CParticle<P>*		mp_Particles;
};