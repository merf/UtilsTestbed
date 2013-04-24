#pragma once

#include "cinder/Vector.h"
#include "cinder/Color.h"

// A helper macro to get a position
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


//***************************************************************************************
// VERTEX DECLARATIONS
//***************************************************************************************

//***************************************************************************************
// Position
#pragma pack(push, 1)
template <class P>
class vPosition
{
	vPosition() : m_Position(P::zero()) {}
	vPosition(P& p) : m_Position(p) {}

	P m_Position;

	static void Bind()
	{
		const GLuint P_ELEMENTS = sizeof(P)/sizeof(float);

		//Set the offsets of our interleaved data
		glVertexPointer(P_ELEMENTS, GL_FLOAT, sizeof(vPosition), BUFFER_OFFSET(0));

		//Enable client states
		glEnableClientState(GL_VERTEX_ARRAY);
	}

	static void UnBind()
	{
		//disable client state
		glDisableClientState(GL_VERTEX_ARRAY);
	}
};
#pragma pack(pop)
typedef vPosition<ci::Vec2f> vPosition2;
typedef vPosition<ci::Vec3f> vPosition3;
typedef vPosition<ci::Vec4f> vPosition4;

//***************************************************************************************
// Position & Colour
#pragma pack(push, 1)
template <class P, class C>
struct vPositionColor
{
	vPositionColor() : m_Position(P::zero()), m_Color(C::white()) {}
	vPositionColor(P& p, C& col) : m_Position(p), m_Color(col) {}

	P m_Position;
	C m_Color;

	static void Bind()
	{
		const GLuint P_ELEMENTS = sizeof(P)/sizeof(float);
		const GLuint C_ELEMENTS = sizeof(C)/sizeof(float);

		//Set the offsets of our interleaved data
		glColorPointer(	C_ELEMENTS, GL_FLOAT, sizeof(vPositionColor<P,C>), BUFFER_OFFSET(4 * P_ELEMENTS));
		//apparently you want to do the vertex pointer last as its faster!
		glVertexPointer(P_ELEMENTS, GL_FLOAT, sizeof(vPositionColor<P,C>), BUFFER_OFFSET(0));

		//Enable client states
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
	}

	static void UnBind()
	{
		//disable client state
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
};
#pragma pack(pop)
typedef vPositionColor<ci::Vec2f, ci::ColorA> vPosition2ColorA;
typedef vPositionColor<ci::Vec3f, ci::ColorA> vPosition3ColorA;
typedef vPositionColor<ci::Vec4f, ci::ColorA> vPosition4ColorA;

//***************************************************************************************
// Position & TexCoord
#pragma pack(push, 1)
template <class P, class TC>
struct vPositionTexcoord
{
	vPositionTexcoord() : m_Position(P::zero()), m_TexCoord(TC::zero()) {}
	vPositionTexcoord(P& p, TC& tc) : m_Position(p), m_TexCoord(tc) {}

	P m_Position;
	TC m_TexCoord;

	static void Bind()
	{
		const GLuint P_ELEMENTS = sizeof(P)/sizeof(float);
		const GLuint TC_ELEMENTS = sizeof(TC)/sizeof(float);

		//Set the offsets of our interleaved data
		glTexCoordPointer(TC_ELEMENTS, GL_FLOAT, sizeof(vPositionTexcoord<P,TC>), BUFFER_OFFSET(4 * P_ELEMENTS));
		//apparently you want to do the vertex pointer last as its faster!
		glVertexPointer(P_ELEMENTS, GL_FLOAT, sizeof(vPositionTexcoord<P,TC>), BUFFER_OFFSET(0));

		//Enable client states
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	static void UnBind()
	{
		//disable client state
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
};
#pragma pack(pop)
typedef vPositionTexcoord<ci::Vec2f, ci::Vec2f> vPosition2Texcoord2;
typedef vPositionTexcoord<ci::Vec3f, ci::Vec2f> vPosition3Texcoord2;

//***************************************************************************************
// Position & TexCoord
#pragma pack(push, 1)
template <class P, class C, class TC>
struct vPositionColorTexcoord
{
	vPositionColorTexcoord() : m_Position(P::zero()), m_TexCoord(TC::zero()), m_Color(ColorA::white()) {}
	vPositionColorTexcoord(P& p, C& c, TC& tc) : m_Position(p), m_Color(c), m_TexCoord(tc) {}

	P m_Position;
	C m_Color;
	TC m_TexCoord;

	static void Bind()
	{
		const GLuint P_ELEMENTS = sizeof(P)/sizeof(float);
		const GLuint C_ELEMENTS = sizeof(C)/sizeof(float);
		const GLuint TC_ELEMENTS = sizeof(TC)/sizeof(float);

		//Set the offsets of our interleaved data
		glTexCoordPointer(TC_ELEMENTS, GL_FLOAT, sizeof(vPositionColorTexcoord<P,C,TC>), BUFFER_OFFSET(4 * (P_ELEMENTS+C_ELEMENTS)));
		glColorPointer(C_ELEMENTS, GL_FLOAT, sizeof(vPositionColorTexcoord<P,C,TC>), BUFFER_OFFSET(4 * P_ELEMENTS));
		//apparently you want to do the vertex pointer last as its faster!
		glVertexPointer(P_ELEMENTS, GL_FLOAT, sizeof(vPositionColorTexcoord<P,C,TC>), BUFFER_OFFSET(0));

		//Enable client states
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	static void UnBind()
	{
		//disable client state
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
};
#pragma pack(pop)
typedef vPositionColorTexcoord<ci::Vec2f, ci::ColorA, ci::Vec2f> vPosition2ColorATexcoord2;
typedef vPositionColorTexcoord<ci::Vec3f, ci::ColorA, ci::Vec2f> vPosition3ColorATexcoord2;

//***************************************************************************************
// VERTEX BUFFER
//
// Deals with buffering up geometry to be drawn, copying it to the card, binding buffers
// and setting vertex attributes, double buffering and actually drawing the geometry.
//***************************************************************************************
template <class T>
class CVertexBuffer
{
public:
	CVertexBuffer(GLuint max_verts)
		:
	m_CurrVertex(0),
		m_CurrVertexBuffer(0),
		m_MaxVertices(max_verts)
	{
		//generate the buffer on the GPU
		glGenBuffers(2, &m_VertexBufferID);

		//and also new up the vertex array in system memory
		mp_Vertices = new T[max_verts];
	}

	void Draw()
	{
		//Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		//copy the data over
		glBufferData(GL_ARRAY_BUFFER, m_CurrVertex * sizeof(T), mp_Vertices, GL_STREAM_DRAW);
		//Bind for the vertex type
		T::Bind();
		//and draw
		glDrawArrays(GL_TRIANGLES, 0, m_CurrVertex);
		//Unbind
		T::UnBind();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//flip buffers
		m_CurrVertex = 0;
		m_CurrVertexBuffer = (m_CurrVertexBuffer==0) ? 1 : 0;
	}

	void AddTriangle(T v0, T v1, T v2)
	{
		//if there isn't room in the buffer then draw what we've got
		if(m_CurrVertex + 3 > m_MaxVertices)
		{
			Draw();
		}

		mp_Vertices[m_CurrVertex++] = v0;
		mp_Vertices[m_CurrVertex++] = v1;
		mp_Vertices[m_CurrVertex++] = v2;
	}

	void AddQuad(T v0, T v1, T v2, T v3)
	{
		//if there isn't room in the buffer then draw what we've got
		if(m_CurrVertex + 6 > m_MaxVertices)
		{
			Draw();
		}

		AddTriangle(v0, v1, v2);
		AddTriangle(v2, v1, v3);
	}

private:
	GLuint		m_VertexBufferID;
	GLuint		m_CurrVertexBuffer;

	GLuint		m_MaxVertices;
	GLuint		m_CurrVertex;

	T*			mp_Vertices;
};
