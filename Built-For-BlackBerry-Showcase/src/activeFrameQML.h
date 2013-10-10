#ifndef ACTIVEFRAMEQML_H_
#define ACTIVEFRAMEQML_H_

#include <QObject>
#include <bb/cascades/SceneCover>
#include <bb/cascades/QmlDocument>

using namespace ::bb::cascades;

class ActiveFrameQML: public SceneCover {
Q_OBJECT

public:
	ActiveFrameQML(QObject *parent = 0);
	void setContextProperty(QString propertyName, QObject *object);
	Q_SIGNAL void updateChanged();

public slots:
	void update();
	void foregrounded();
	void backgrounded();

private:
	bb::cascades::QmlDocument *m_qml;
	bool isActiveFrame;
};

#endif /* ACTIVEFRAMEQML_H_ */
