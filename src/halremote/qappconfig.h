/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
#ifndef QAPPCONFIG_H
#define QAPPCONFIG_H

#include <QQuickItem>
#include <QQmlListProperty>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>
#include <nzmqt/nzmqt.hpp>
#include "qappconfigitem.h"
#include "qappconfigfilter.h"
#include "qservicediscovery.h"
#include "message.pb.h"
#include "types.pb.h"
#include <google/protobuf/text_format.h>

namespace gpb = google::protobuf;

using namespace nzmqt;

class QAppConfig : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(bool ready READ isReady WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QAppConfigItem *selectedConfig READ selectedConfig NOTIFY selectedConfigChanged)
    Q_PROPERTY(QQmlListProperty<QAppConfigItem> appConfigs READ appConfigs NOTIFY appConfigsChanged)
    Q_PROPERTY(QQmlListProperty<QAppConfigFilter> filters READ filters)
    Q_PROPERTY(QQmlListProperty<QService> services READ services)
    Q_PROPERTY(QQmlListProperty<QService> receivedServices READ receivedServices NOTIFY receivedServicesChanged)

public:
    explicit QAppConfig(QQuickItem *parent = 0);

    virtual void componentComplete();

    QString uri() const
    {
        return m_uri;
    }

    int timeout() const
    {
        return m_timeout;
    }

    bool isReady() const
    {
        return m_ready;
    }

    QAppConfigItem * selectedConfig() const
    {
        return m_selectedConfig;
    }

    QQmlListProperty<QAppConfigItem> appConfigs();
    int appConfigCount() const;
    QAppConfigItem *appConfig(int index) const;

    QQmlListProperty<QAppConfigFilter> filters();
    int filterCount() const;
    QAppConfigFilter* filter(int index) const;

    QQmlListProperty<QService> services();
    int serviceCount() const;
    QService *service(int index) const;

    QQmlListProperty<QService> receivedServices();
    int replieCount() const;
    QService *replie(int index) const;

public slots:

    void selectAppConfig(QString name);
    void unselectAppConfig();
    void updateServices();

    void setUri(QString arg)
    {
        if (m_uri != arg) {
            m_uri = arg;
            emit uriChanged(arg);
        }
    }

    void setTimeout(int arg)
    {
        if (m_timeout != arg) {
            m_timeout = arg;
            emit timeoutChanged(arg);
        }
    }

    void setReady(bool arg);

    void setSelectedConfig(QAppConfigItem * arg)
    {
        if (m_selectedConfig != arg) {
            m_selectedConfig = arg;
            emit selectedConfigChanged(arg);
        }
    }

private:
    QString m_uri;
    int m_timeout;
    bool m_ready;
    QAppConfigItem * m_selectedConfig;
    QList<QAppConfigItem*> m_appConfigs;
    QList<QAppConfigFilter*> m_filters;
    QList<QService*> m_services;
    QList<QService*> m_receivedServices;

    ZMQContext *m_context;
    ZMQSocket *m_configSocket;
    // more efficient to reuse a protobuf Message
    pb::Container m_rx;
    pb::Container m_tx;

    QTimer *m_timeoutTimer;
    bool m_componentCompleted;

    void start();
    void stop();

private slots:
    void timeoutTimerTick();
    void connectSocket();
    void disconnectSocket();
    void configMessageReceived(QList<QByteArray> messageList);
    void request(pb::ContainerType type);

signals:
    void uriChanged(QString arg);
    void timeoutChanged(int arg);
    void readyChanged(bool arg);
    void selectedConfigChanged(QAppConfigItem * arg);
    void appConfigsChanged(QQmlListProperty<QAppConfigItem> arg);
    void receivedServicesChanged(QQmlListProperty<QService> arg);
};

#endif // QAPPCONFIG_H
