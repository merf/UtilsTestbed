#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIO.h"

#include "VertexBuffer.h"
#include "ParticleSystem.h"

using namespace ci;
using namespace ci::app;

#include <list>
using namespace std;

GLuint vao;



//typedef vPosition2Texcoord2 VERTEX_t;
typedef vPosition2ColorATexcoord2 VERTEX_t;

// We'll create a new Cinder Application by deriving from the BasicApp class
class BasicApp : public AppBasic {
 public:
	void setup();
	// Cinder will always call this function whenever the user drags the mouse
	void mouseDrag( MouseEvent event );
	void keyDown( KeyEvent event );
	// Cinder calls this function 30 times per second by default
	void draw();

	// This will maintain a list of points which we will draw line segments between
	list<Vec2f>		mPoints;

	//CVertexBuffer<VERTEX_t>*	mp_Buffer;
	CParticleSystem<VERTEX_t, Vec2f>*	mp_Particles;
	gl::Texture					m_Texture;
};

void BasicApp::setup()
{
	glGenVertexArrays(1, & vao);	
	glBindVertexArray(vao);

	mp_Particles = new CParticleSystem<VERTEX_t, Vec2f>(Vec2f(320, 240), 100);
	//mp_Buffer = new CVertexBuffer<VERTEX_t>(6);

	gl::Texture::Format format;
	format.enableMipmapping(true);
	format.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	m_Texture = gl::Texture(loadImage(loadAsset("LH.bmp")), format);
}

void BasicApp::mouseDrag( MouseEvent event )
{
	// add wherever the user drags to the end of our list of points
	mPoints.push_back( event.getPos() );
}

void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );

	if(event.getCode() == KeyEvent::KEY_ESCAPE)
	{
		quit();
	}
}

void BasicApp::draw()
{
	gl::enableAlphaBlending();
	gl::enableAdditiveBlending();

	//m_Texture.enableAndBind();

	gl::setMatricesWindow( getWindowSize() );
	// this pair of lines is the standard way to clear the screen in OpenGL
	gl::clear( Color( 0.1f, 0.1f, 0.1f ) );

	gl::color(ColorA(1.0, 1.0, 1.0, 1.0));

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	//mp_Buffer->AddTriangle(vPosition(Vec3f(0.0f, 0.0f, 0.0f)), vPosition(Vec3f(100.0f, 0.0f, 0.0f)), vPosition(Vec3f(0.0f, 100.0f, 0.0f)));
	//mp_Buffer->AddTriangle(vPosition(Vec3f(0.0f, 100.0f, 0.0f)), vPosition(Vec3f(100.0f, 100.0f, 0.0f)), vPosition(Vec3f(0.0f, 200.0f, 0.0f)));
	
	//mp_Buffer->AddQuad(	VERTEX_t(Vec2f(0.0f,	0.0f),		ColorA(1.0f, 0.0f, 0.0f, 1.0f)), 
	//	VERTEX_t(Vec2f(100.0f,	0.0f),		ColorA(0.0f, 1.0f, 0.0f, 1.0f)), 
	//	VERTEX_t(Vec2f(0.0f,	100.0f),	ColorA(0.0f, 0.0f, 1.0f, 1.0f)), 
	//	VERTEX_t(Vec2f(100.0f,	100.0f),	ColorA(1.0f, 1.0f, 1.0f, 1.0f)));

	/*
	float f = 0.5f + 0.5f * (float)sin(getElapsedSeconds());
	float w = m_Texture.getWidth() * f;
	float h = m_Texture.getHeight() * f;

	w = 100.0f;
	h = 100.0f;

	mp_Buffer->AddQuad(	VERTEX_t(Vec2f(0.0f,	0.0f),		Vec2f(0.0f, 0.0f)), 
						VERTEX_t(Vec2f(w,		0.0f),		Vec2f(1.0f, 0.0f)), 
						VERTEX_t(Vec2f(0.0f,	h),			Vec2f(0.0f, 1.0f)), 
						VERTEX_t(Vec2f(w,		h),			Vec2f(1.0f, 1.0f)));

 	mp_Buffer->Draw();
	*/

	mp_Particles->Update(1.0f / 60.0f);
	mp_Particles->Draw();
}

// This line tells Flint to actually create the application
CINDER_APP_BASIC( BasicApp, RendererGl )