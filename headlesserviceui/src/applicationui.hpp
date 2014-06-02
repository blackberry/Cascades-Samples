/* Copyright (c) 2013 BlackBerry Limited.
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
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QFileSystemWatcher>

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;

/*!
 * @brief Class which represents the headless service UI
 * 	      that communicates with the headless service via
 * 	      QSettings to retrieve service values such as, status
 * 	      and current Led values.
 *
 */
//! [0]
class ApplicationHeadless: public QObject
{
    Q_OBJECT
    // property holding value for remaining flash count
    Q_PROPERTY(int remainingFlashCount READ remainingFlashCount WRITE setRemainingFlashCount NOTIFY remainingFlashCountChanged)

public:
    ApplicationHeadless();
    virtual ~ApplicationHeadless() {}
    Q_INVOKABLE void resetLED();

Q_SIGNALS:
    // Emitted when the remaining flash count value has changed
    void remainingFlashCountChanged();

public Q_SLOTS:
    /**
     * Method that can be invoked from within qml
     * to read QSettings key/value for ServiceStatus,
     * in order to decipher if service is running (active).
     */
    bool isServiceRunning();

    /**
     * Method hooked into the signal/slots mechanism, which
     * gets invoked upon receiving fileChanged() signal
     * from the settingsWatcher instance.
     */
    void settingsChanged(const QString & path);

    /**
     * Method to retrieve the number of times the Led
     * was set to flash by reading this from the QSettings.
     */
    int flashCount();

private:
    static const QString m_author; // for creating settings
    static const QString m_appName; // for creating settings
    static const QString m_flashNumber;
    static const QString m_remainingCount;
    static const QString m_reset;
    static const QString m_serviceStatus;

    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    // QTimer used to periodically read QSettings to retrieve
    // new remaining flash count value
    int m_remainingFlashCount;
    // setter to store the new remaining flash count value
    void setRemainingFlashCount(int x);
    // Convenience method to retrieve remaining flash count.
    int remainingFlashCount();
    // Watcher for qsettigns file changes
    QFileSystemWatcher* settingsWatcher;
};
//! [0]
#endif /* ApplicationUI_HPP_ */
