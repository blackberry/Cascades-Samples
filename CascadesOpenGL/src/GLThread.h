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

#ifndef GLTHREAD_H_
#define GLTHREAD_H_
#include <assert.h>
#include <screen/screen.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <QWaitCondition>
#include <QMutex>
#include <QStack>
#include "IGLRenderer.h"

#include <QThread>
class GLThread :public QThread{

	enum RunState{Pause,Run};
public:

	/*
	 * Singleton accessor
	 */
	static GLThread* instance();

	/*
	 * Method used to initialize the instance
	 * @param width The width of the container
	 * @param height The height of the container
	 */
	static GLThread* initInstance(int width, int height);

	/*
	 * Routine that calls our renderer
	 */
	void run();

	/*
	 * Method that resizes the container
	 * @param width The width of the container
	 * @param height The height of the container
	 */
	int resize(int width, int height);

	/*
	 * Accessor for the screne context
	 * @return The screen context
	 */
	screen_context_t getContext();

	/*
	 * Setter for the renderer
	 * @param renderer The new renderer to be associated
	 */
	void setRenderer(IGLRenderer * renderer);

	/*
	 * Boolean used to determine if the thread is currently running.
	 */
	bool m_isRunning;

	/*
	 * This changes the current renderer to the previous one.
	 */
	void previousRenderer();

private:

	/*
	 * The singleton instance
	 */
	static GLThread* m_instance;

	/*
	 * Custom constructor
	 * @param width The width of the container
	 * @param height The height of the container
	 */
	GLThread(int width,int height);

	/*
	 * Destructor
	 */
	virtual ~GLThread();

private:

	/*
	 * Routine used to initialize the Blackberry Platform Service
	 * @return An error code if applicable
	 */
	int initBPS();

	/*
	 * The current running state of the thread
	 */
	RunState m_runState;

	/*
	 * The locally stored width of the application's container
	 */
	int m_width;

	/*
	 * The locally stored height of the application's container
	 */
	int m_height;

	/*
	 * the screen container
	 */
	screen_context_t m_screen_cxt;

	/*
	 * A thread waiting condition for the renderer
	 */
	QWaitCondition m_rendererReady;
	QWaitCondition m_rendererShutdown;
	QMutex m_mutex;

	/*
	 * A handle on our renderer
	 */
	IGLRenderer* m_renderer;
	QStack<IGLRenderer*> m_renderStack;
};

#endif /* GLTHREAD_H_ */
