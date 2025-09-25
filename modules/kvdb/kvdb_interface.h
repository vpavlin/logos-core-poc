#pragma once

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include "../../core/interface.h"

class KeyValueDBInterface : public PluginInterface
{
public:
    virtual ~KeyValueDBInterface() {}
    
    // Core key/value operations
    Q_INVOKABLE virtual bool put(const QString &key, const QVariant &value) = 0;
    Q_INVOKABLE virtual QVariant get(const QString &key) = 0;
    Q_INVOKABLE virtual bool has(const QString &key) = 0;
    Q_INVOKABLE virtual bool remove(const QString &key) = 0;
    Q_INVOKABLE virtual QStringList keys() = 0;
    Q_INVOKABLE virtual int size() = 0;
    Q_INVOKABLE virtual bool clear() = 0;

signals:
    // Event signaling data changes
    void eventResponse(const QString& eventName, const QVariantList& data);
};

#define KeyValueDBInterface_iid "org.logos.KeyValueDBInterface"
Q_DECLARE_INTERFACE(KeyValueDBInterface, KeyValueDBInterface_iid)