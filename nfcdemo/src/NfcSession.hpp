#ifndef NFCSESSION_HPP
#define NFCSESSION_HPP

#include <QObject>

class MacAddressHandler;
class NfcSender;
class NfcShareHandler;
class QDeclarativeContext;

class NfcSession : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int activeModule READ activeModule WRITE setActiveModule NOTIFY activeModuleChanged)

public:
    NfcSession(QDeclarativeContext *context, QObject *parent = 0);
    ~NfcSession();

Q_SIGNALS:
    void activeModuleChanged();

private:
    int activeModule() const;
    void setActiveModule(int module);

    void cleanupSession();

    QDeclarativeContext* m_context;
    int m_activeModule;

    MacAddressHandler *m_macAddressHandler;
    NfcSender *m_nfcSender;
    NfcShareHandler *m_nfcShareHandler;
};

#endif
