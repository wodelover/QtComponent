/*************************************************************************/
/*                          Copyright Notice                             */
/* The code can not be copied or provided to other people without the    */
/* permission of Zhang Hao,otherwise intellectual property infringement  */
/* will be prosecuted.If you have any questions,please send me an email. */
/* My email is kingderzhang@foxmail.com. The final interpretation rights */
/* are interpreted by ZhangHao.                                          */
/*                  Copyright (C) ZhangHao All rights reserved           */
/*************************************************************************/
#include "httpcom.h"
#include <QDebug>

#ifdef SET_SINGLE_MODE
HttpCom *HttpCom::instance=nullptr;
QNetworkAccessManager *HttpCom::m_manager=nullptr;
QNetworkRequest *HttpCom::m_request=nullptr;
QNetworkReply *HttpCom::m_reply=nullptr;
#endif

void HttpCom::requestFinished()
{
    emit hasDataComeFromServer(m_reply->readAll());
}

void HttpCom::slotError(QNetworkReply::NetworkError errCode)
{
    emit requestError(static_cast<int>(errCode));
}

#ifdef SET_SINGLE_MODE
HttpCom *HttpCom::getinstance()
{
#ifdef SET_SINGLE_MODE
    if(nullptr==instance){
        instance = new HttpCom();
        m_manager = new QNetworkAccessManager ;
        m_request = new QNetworkRequest;
    }
    return instance;
#endif
}

QObject *HttpCom::HttpCom_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    return getinstance();
}
#endif

void HttpCom::sendDataToServerByPost(QUrl url, QVariant data)
{
    m_request->setUrl(url);
    try {
        m_reply = m_manager->post(*m_request,data.toByteArray());
        throw 0;
    } catch (int) {
        connect(m_reply, SIGNAL(readyRead()), this, SLOT(requestFinished()));
        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(slotError(QNetworkReply::NetworkError)));
    }
}

void HttpCom::sendDataToServerByPost(QString url, QVariant data)
{
    QUrl address(url);
    sendDataToServerByPost(address,data);
}

void HttpCom::getDataFromServerByGet(QUrl url)
{
    m_request->setUrl(url);
    try {
        m_reply = m_manager->get(*m_request);
        throw 0;
    } catch (int) {
        connect(m_reply, SIGNAL(readyRead()), this, SLOT(requestFinished()));
        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(slotError(QNetworkReply::NetworkError)));
    }
}

void HttpCom::getDataFromServerByGet(QString url)
{
    QUrl address(url);
    getDataFromServerByGet(address);
}

void HttpCom::setRequestRawHeader(QByteArray key,QByteArray value)
{
    m_request->setRawHeader(key,value);
}

void HttpCom::setRequestHeader(QNetworkRequest::KnownHeaders header, QVariant value)
{
    m_request->setHeader(header,value);
}

HttpCom::HttpCom()
{
#ifndef SET_SINGLE_MODE
    m_manager = new QNetworkAccessManager ;
    m_request = new QNetworkRequest;
#endif
}

HttpCom::~HttpCom()
{
    if(m_manager!=nullptr){
        delete m_manager;
        m_manager = nullptr;
    }

    if(m_request!=nullptr){
        delete m_request;
        m_request = nullptr;
    }
}
