#include "NfcSession.hpp"

#include <QDebug>

#include "MacAddressHandler.hpp"
#include "NfcSender.hpp"
#include "NfcShareHandler.hpp"

NfcSession::NfcSession(QDeclarativeContext *context, QObject *parent)
    : QObject(parent)
    , m_context(context)
    , m_activeModule(-1)
    , m_macAddressHandler(0)
    , m_nfcSender(0)
    , m_nfcShareHandler(0)
{
    setActiveModule(0);
}

NfcSession::~NfcSession()
{
    cleanupSession();
}

int NfcSession::activeModule() const
{
    return m_activeModule;
}

void NfcSession::setActiveModule(int module)
{
    if (m_activeModule == module)
        return;

    // clean up previous active module
    cleanupSession();

    // create new active module
    switch (module) {
        case 0:
            m_macAddressHandler = new MacAddressHandler;
            m_context->setContextProperty("_macAddressHandler", m_macAddressHandler);
            break;
        case 1:
            m_nfcSender = new NfcSender;
            m_context->setContextProperty("_nfcSender", m_nfcSender);
            break;
        case 3:
            m_nfcShareHandler = new NfcShareHandler;
            m_context->setContextProperty("_nfcShareHandler", m_nfcShareHandler);
            break;
        default:
            break;
    }

    m_activeModule = module;
    emit activeModuleChanged();
}

void NfcSession::cleanupSession()
{
    m_context->setContextProperty("_macAddressHandler", 0);
    m_context->setContextProperty("_nfcSender", 0);
    m_context->setContextProperty("_nfcShareHandler", 0);

    delete m_macAddressHandler;
    m_macAddressHandler = 0;

    delete m_nfcSender;
    m_nfcSender = 0;

    delete m_nfcShareHandler;
    m_nfcShareHandler = 0;
}
