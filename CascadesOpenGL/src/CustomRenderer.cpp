/*
 * Copyright (c) 2011-2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <QThread>

#include "GLThread.h"
#include "bbutil.h"

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "CustomRenderer.h"

CustomRenderer::CustomRenderer(const QString &group, const QString id, int width,int height):
m_group(group), m_id(id), m_width(width), m_height(height),
m_initialized(false)
{

}

CustomRenderer::~CustomRenderer()
{
	this->shutdown();
}

void CustomRenderer::initialize()
{
	if (m_initialized) return;

	GLThread::instance()->resize(m_width,m_height);

	// we call this here because run() method is too late...
	m_screen_win = bbutil_get_window();

	// We want to handle openGL Events in the foreign window. So bring the
	// in front of the cascades window (z > 0).
	//m_screen_win = bbutil_get_window();
	int z = 1;
	screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_ZORDER, &z);
	screen_join_window_group(m_screen_win, m_group.toAscii().constData());

	//Load and save a texture index
	float tx = 1.0f;
	float ty = 1.0f;

	//Image source: http://commons.wikimedia.org/wiki/File%3AMixed-forest.jpg
	//By Oliver Herold (Own work) [CC-BY-3.0 (http://creativecommons.org/licenses/by/3.0)], via Wikimedia Commons
	int result = bbutil_load_texture("app/native/assets/data/textures/Forest-2.png",NULL,NULL,&tx,&ty,&m_textureindices[0]);

	m_initialized = true;
}

void CustomRenderer::shutdown()
{
    int vis = 0;
    screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_VISIBLE, &vis);

}

void CustomRenderer::render()
{

	//setup projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glFrustumf(-0.005, 0.005, -0.01, 0.01, 0.01, 2);

	//setup modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//clear our buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//create and draw our triangle
	static GLfloat triangleVertices[9] { -1.0, 0.0, -1.0,     0.0, 1.0, -1.0,     1.0, 0.0, -1.0 };
	static GLfloat triangleTexCoords[9] { 0.0, 0.0,         0.5, 0.75,         1.0, 0.0 };

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glBindTexture(GL_TEXTURE_2D, m_textureindices[0]);

	glVertexPointer(3, GL_FLOAT, 0, triangleVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, triangleTexCoords);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

	glDisable(GL_TEXTURE_2D);

	//pop matrices
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

}

void CustomRenderer::screenEvent(bps_event_t*)
{
	//empty
}

screen_window_t CustomRenderer::getWindow()
{
	return m_screen_win;
}


