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

#ifndef CUSTOMRENDERER_H
#define CUSTOMRENDERER_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>

#include <bb/cascades/Event>
#include <bb/cascades/UiObject>
#include <bb/cascades/Control>

#include <screen/screen.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "IGLRenderer.h"

/*
 * Main container class for 3d application
 */
class CustomRenderer : public IGLRenderer
{

public:

	/*
	 * Parameterized Construction
	 */
	CustomRenderer(const QString &group, const QString id, int width,int height);

	/*
	 * Destructor
	 */
	~CustomRenderer();

	/*!
     * @see IGLRenderer::initialize
     */
    virtual void initialize();

    /*!
     * @see IGLRenderer::render
     */
    virtual void render();

    /*!
     * @see IGLRenderer::screenEvent
     */
    virtual void screenEvent(bps_event_t*);

    /*
     * Method that returns the window handle
     */
    screen_window_t getWindow();

private:

    /*!
     * @see IGLRenderer::shutdown
     */
    virtual void shutdown();

    /*
     * The group specifier of the window stored locally
     */
	QString m_group;

	/*
	 * The window ID of our ForeignWindow
	 */
	QString m_id;

	/*
	 * Local width member
	 */
	int m_width;

	/*
	 * Local height member
	 */
	int m_height;

	/*
	 * Our Screen Context used for OpenGL
	 */
	screen_context_t m_screen_cxt;

	/*
	 * Our Screen window specifier
	 */
	screen_window_t m_screen_win;

	/*
	 * Local member indicating if the renderer has completed initialization
	 */
	bool m_initialized;

	/*
	 * array containing indices for all textures
	 */
	GLuint m_textureindices[10];

};

#endif CUSTOMRENDERER_H
