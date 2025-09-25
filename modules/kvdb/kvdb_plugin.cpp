#include "kvdb_plugin.h"
#include <QDebug>
#include <QVariant>
#include <QDateTime>

KVDBPlugin::KVDBPlugin()
{
    qDebug() << "KVDBPlugin: Initializing...";
    m_logosAPI = nullptr;
    qDebug() << "KVDBPlugin: Initialized successfully";
}

KVDBPlugin::~KVDBPlugin() 
{
    // Clean up resources
    if (m_logosAPI) {
        delete m_logosAPI;
        m_logosAPI = nullptr;
    }
}

bool KVDBPlugin::put(const QString &key, const QVariant &value)
{
    qDebug() << "KVDBPlugin::put called with key:" << key << "value:" << value;
    
    if (key.isEmpty()) {
        qWarning() << "KVDBPlugin::put: Cannot store with empty key";
        return false;
    }
    
    // Store the value
    m_storage[key] = value;
    
    // Create event data
    QVariantList eventData;
    eventData << key << value << QDateTime::currentDateTime().toString(Qt::ISODate);
    
    // Trigger event
    if (m_logosAPI) {
        qDebug() << "KVDBPlugin: Triggering event 'keyStored' with data:" << eventData;
        m_logosAPI->getClient("core_manager")->onEventResponse(this, "keyStored", eventData);
        qDebug() << "KVDBPlugin: Event 'keyStored' triggered with data:" << eventData;
    } else {
        qWarning() << "KVDBPlugin: LogosAPI not available, cannot trigger event";
    }
    
    return true;
}

QVariant KVDBPlugin::get(const QString &key)
{
    qDebug() << "KVDBPlugin::get called with key:" << key;
    
    if (key.isEmpty()) {
        qWarning() << "KVDBPlugin::get: Cannot retrieve with empty key";
        return QVariant();
    }
    
    if (!m_storage.contains(key)) {
        qDebug() << "KVDBPlugin::get: Key not found:" << key;
        return QVariant();
    }
    
    QVariant value = m_storage[key];
    qDebug() << "KVDBPlugin::get: Found value:" << value;
    
    // Create event data
    QVariantList eventData;
    eventData << key << value << QDateTime::currentDateTime().toString(Qt::ISODate);
    
    // Trigger event
    if (m_logosAPI) {
        qDebug() << "KVDBPlugin: Triggering event 'keyRetrieved' with data:" << eventData;
        m_logosAPI->getClient("core_manager")->onEventResponse(this, "keyRetrieved", eventData);
        qDebug() << "KVDBPlugin: Event 'keyRetrieved' triggered with data:" << eventData;
    } else {
        qWarning() << "KVDBPlugin: LogosAPI not available, cannot trigger event";
    }
    
    return value;
}

bool KVDBPlugin::has(const QString &key)
{
    qDebug() << "KVDBPlugin::has called with key:" << key;
    
    if (key.isEmpty()) {
        qWarning() << "KVDBPlugin::has: Cannot check empty key";
        return false;
    }
    
    bool exists = m_storage.contains(key);
    qDebug() << "KVDBPlugin::has: Key" << (exists ? "exists" : "does not exist");
    
    // Create event data
    QVariantList eventData;
    eventData << key << exists << QDateTime::currentDateTime().toString(Qt::ISODate);
    
    // Trigger event
    if (m_logosAPI) {
        qDebug() << "KVDBPlugin: Triggering event 'keyChecked' with data:" << eventData;
        m_logosAPI->getClient("core_manager")->onEventResponse(this, "keyChecked", eventData);
        qDebug() << "KVDBPlugin: Event 'keyChecked' triggered with data:" << eventData;
    } else {
        qWarning() << "KVDBPlugin: LogosAPI not available, cannot trigger event";
    }
    
    return exists;
}

bool KVDBPlugin::remove(const QString &key)
{
    qDebug() << "KVDBPlugin::remove called with key:" << key;
    
    if (key.isEmpty()) {
        qWarning() << "KVDBPlugin::remove: Cannot remove empty key";
        return false;
    }
    
    if (!m_storage.contains(key)) {
        qDebug() << "KVDBPlugin::remove: Key not found:" << key;
        return false;
    }
    
    // Get the value before removing
    QVariant value = m_storage[key];
    m_storage.remove(key);
    
    // Create event data
    QVariantList eventData;
    eventData << key << value << QDateTime::currentDateTime().toString(Qt::ISODate);
    
    // Trigger event
    if (m_logosAPI) {
        qDebug() << "KVDBPlugin: Triggering event 'keyRemoved' with data:" << eventData;
        m_logosAPI->getClient("core_manager")->onEventResponse(this, "keyRemoved", eventData);
        qDebug() << "KVDBPlugin: Event 'keyRemoved' triggered with data:" << eventData;
    } else {
        qWarning() << "KVDBPlugin: LogosAPI not available, cannot trigger event";
    }
    
    return true;
}

QStringList KVDBPlugin::keys()
{
    qDebug() << "KVDBPlugin::keys called";
    
    QStringList keyList = m_storage.keys();
    qDebug() << "KVDBPlugin::keys: Found" << keyList.size() << "keys";
    
    // Create event data
    QVariantList eventData;
    eventData << QVariant::fromValue(keyList) << keyList.size() << QDateTime::currentDateTime().toString(Qt::ISODate);
    
    // Trigger event
    if (m_logosAPI) {
        qDebug() << "KVDBPlugin: Triggering event 'keysRetrieved' with data:" << eventData;
        m_logosAPI->getClient("core_manager")->onEventResponse(this, "keysRetrieved", eventData);
        qDebug() << "KVDBPlugin: Event 'keysRetrieved' triggered with data:" << eventData;
    } else {
        qWarning() << "KVDBPlugin: LogosAPI not available, cannot trigger event";
    }
    
    return keyList;
}

int KVDBPlugin::size()
{
    qDebug() << "KVDBPlugin::size called";
    
    int count = m_storage.size();
    qDebug() << "KVDBPlugin::size: Current size is" << count;
    
    // Create event data
    QVariantList eventData;
    eventData << count << QDateTime::currentDateTime().toString(Qt::ISODate);
    
    // Trigger event
    if (m_logosAPI) {
        qDebug() << "KVDBPlugin: Triggering event 'sizeRetrieved' with data:" << eventData;
        m_logosAPI->getClient("core_manager")->onEventResponse(this, "sizeRetrieved", eventData);
        qDebug() << "KVDBPlugin: Event 'sizeRetrieved' triggered with data:" << eventData;
    } else {
        qWarning() << "KVDBPlugin: LogosAPI not available, cannot trigger event";
    }
    
    return count;
}

bool KVDBPlugin::clear()
{
    qDebug() << "KVDBPlugin::clear called";
    
    int count = m_storage.size();
    if (count == 0) {
        qDebug() << "KVDBPlugin::clear: No keys to clear";
        return true;
    }
    
    m_storage.clear();
    qDebug() << "KVDBPlugin::clear: Cleared" << count << "keys";
    
    // Create event data
    QVariantList eventData;
    eventData << count << QDateTime::currentDateTime().toString(Qt::ISODate);
    
    // Trigger event
    if (m_logosAPI) {
        qDebug() << "KVDBPlugin: Triggering event 'cleared' with data:" << eventData;
        m_logosAPI->getClient("core_manager")->onEventResponse(this, "cleared", eventData);
        qDebug() << "KVDBPlugin: Event 'cleared' triggered with data:" << eventData;
    } else {
        qWarning() << "KVDBPlugin: LogosAPI not available, cannot trigger event";
    }
    
    return true;
}

void KVDBPlugin::initLogos(LogosAPI* logosAPIInstance) {
    qDebug() << "KVDBPlugin: initLogos called with LogosAPI instance";
    m_logosAPI = logosAPIInstance;
    qDebug() << "KVDBPlugin: LogosAPI instance stored successfully";
}