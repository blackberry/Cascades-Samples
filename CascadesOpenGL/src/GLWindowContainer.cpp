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
#include <bb/cascades/CustomControl>
#include <bb/cascades/ForeignWindow>
#include <bb/cascades/Container>
#include <bb/cascades/AbsoluteLayout>
#include "GLWindowContainer.h"
#include "qdebug.h"
#include "GLThread.h"
#include <QDebug>

#include "CustomRenderer.h"

using namespace bb::cascades;

GLWindowContainer::GLWindowContainer(QObject* parent) : bb::cascades::CustomControl()
{
	Q_UNUSED(parent);

	QObject::connect(this, SIGNAL(creationCompleted()), this, SLOT(onCreationCompleted()));

}

GLWindowContainer::~GLWindowContainer()
{
    if (m_renderer)
    {
    	delete m_renderer;
    }
}

void GLWindowContainer::onWindowAttached(unsigned long handle, const QString &group, const QString &id)
{
	int ert=0;
	ert++;

	qDebug() << "onWindowAttached!";

	screen_flush_context(GLThread::instance()->getContext(), SCREEN_WAIT_IDLE);

}

void GLWindowContainer::onCreationCompleted()
{
	//overridden by UIObject, called when created.

	//Create our foreign window and set it as the root of this application.
	float prefWidth = preferredWidth();
	float prefHeight = preferredHeight();
	m_foreignWindow = ForeignWindow::create().preferredSize(prefWidth, prefHeight);

	bool result = QObject::connect( m_foreignWindow,
									SIGNAL(windowAttached(unsigned long, const QString&, const QString&)),
									this,
									SLOT(onWindowAttached(unsigned long, const QString&, const QString&)));

	if (result)
		qWarning() << "connected.";
	else
		qWarning() << "NOT connected.";

	setRoot(m_foreignWindow);

	//Build our renderer
	m_renderer = new CustomRenderer(m_foreignWindow->windowGroup(), "GLRendererThread", prefWidth, prefHeight);

	GLThread::instance()->setRenderer(m_renderer);

}


