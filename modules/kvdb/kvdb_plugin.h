#pragma once

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include "kvdb_interface.h"
#include "../../SDK/cpp/logos_api.h"
#include "../../SDK/cpp/logos_api_client.h"

class KVDBPlugin : public QObject, public KeyValueDBInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID KeyValueDBInterface_iid FILE "metadata.json")
    Q_INTERFACES(KeyValueDBInterface PluginInterface)

public:
    KVDBPlugin();
    ~KVDBPlugin();

    // KeyValueDBInterface implementation
    Q_INVOKABLE bool put(const QString &key, const QVariant &value) override;
    Q_INVOKABLE QVariant get(const QString &key) override;
    Q_INVOKABLE bool has(const QString &key) override;
    Q_INVOKABLE bool remove(const QString &key) override;
    Q_INVOKABLE QStringList keys() override;
    Q_INVOKABLE int size() override;
    Q_INVOKABLE bool clear() override;
    
    // PluginInterface implementation
    QString name() const override { return "kvdb"; }
    QString version() const override { return "1.0.0"; }

    // LogosAPI initialization
    Q_INVOKABLE void initLogos(LogosAPI* logosAPIInstance);

signals:
    // Event signaling data changes
    void eventResponse(const QString& eventName, const QVariantList& data);

private:
    QMap<QString, QVariant> m_storage;
    LogosAPI* m_logosAPI;
};