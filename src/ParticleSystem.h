#pragma once

#include "VertexBuffer.h"

//TODO - AOS vs SOA and benchmark?

template <class P>
struct CParticle
{
	void	Init(P pos)
	{
		m_TotalLifeTime = randFloat(0.5f, 2.0f);
		m_Life = m_TotalLifeTime;

		m_Pos = pos;

		float s = 100.f;
		m_Velocity = P(randFloat(-s, s), randFloat(-s, s));

		m_Rotation = randFloat(-(float)M_PI, (float)M_PI);
		m_AngularVelocity = randFloat(-(float)M_PI, (float)M_PI);
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
		return (float)sin((m_Life/m_TotalLifeTime) * M_PI);
	}

	ci::ColorA GetColor()
	{
		return lerp(ColorA(1.0f, 1.0f, 0.0f, 0.0f), ColorA(1.0f, 0.0f, 0.0f, 1.0f), m_Life/m_TotalLifeTime);
	}

	void GetVerts(P& tl, P& tr, P& bl, P& br, float size)
	{
		tl = m_Pos + P(cos(m_Rotation*1.0f), -sin(m_Rotation*1.0f)) * size;
		tr = m_Pos + P(cos(m_Rotation*2.0f), -sin(m_Rotation*2.0f)) * size;
		br = m_Pos + P(cos(m_Rotation*3.0f), -sin(m_Rotation*2.0f)) * size;
		bl = m_Pos + P(cos(m_Rotation*4.0f), -sin(m_Rotation*4.0f)) * size;
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
			float w = 10.0f * mp_Particles[i].GetSize();
			float h = 10.0f * mp_Particles[i].GetSize();

			P tl = centre + P(-w,-h);
			P tr = centre + P(-w,h);
			P bl = centre + P(w,-h);
			P br = centre + P(w,h);

			mp_Particles[i].GetVerts(tl, tr, bl, br, 10.0f * mp_Particles[i].GetSize());

			ColorA col = mp_Particles[i].GetColor();

			mp_VertexBuffer->AddQuad(
				V(tl, col, ci::Vec2f(0,0)),
				V(tr, col, ci::Vec2f(1,0)),
				V(bl, col, ci::Vec2f(0,1)),
				V(br, col, ci::Vec2f(1,1)));
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