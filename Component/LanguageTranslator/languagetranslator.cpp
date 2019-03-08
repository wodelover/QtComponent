/*************************************************************************/
/*                          Copyright Notice                             */
/* This code is only for use by Hangzhou Aoteng Electronics Co., Ltd.The */
/* code can not be copied or provided to other people without the        */
/* permission of Zhang Hao,otherwise intellectual property infringement  */
/* will be prosecuted.If you have any questions,please send me an email. */
/* My email is kingderzhang@foxmail.com. Note: the release of this code  */
/* version is also controlled by Hangzhou Aoteng Electronics Co., Ltd.   */
/* The final interpretation rights are interpreted by ZhangHao.          */
/*                  Copyright (C) ZhangHao All rights reserved           */
/*************************************************************************/
#include "languagetranslator.h"
#include <QDebug>
LanguageTranslator * LanguageTranslator::instance =nullptr;

LanguageTranslator::LanguageTranslator()
{

}

LanguageTranslator *LanguageTranslator::getInstance()
{
    if(instance==nullptr){
        instance = new LanguageTranslator;
    }
    return instance;
}

void LanguageTranslator::initLanguageTranslator(QGuiApplication &app, QQmlApplicationEngine &engine)
{
    m_app = &app;
    m_engine = &engine;
    m_appPath = m_app->applicationDirPath();
}

void LanguageTranslator::setLanguage(LanguageTranslator::LanguageType languageType)
{
    QString packetPath = m_appPath + packet_path;
    QDir dir(packetPath);
    if(!dir.exists()&&!dir.mkdir(packetPath)){
        qDebug()<<__FILE__<<__LINE__<<"mkdir false with file path-->" + packetPath;
    }
    QTranslator translator;
    bool flag = false;
    switch (languageType) {
    case  ZH_CN: flag = translator.load(packetPath + zh_CN_path); break;
    case  EN_US: flag = translator.load(packetPath + en_US_path); break;
    case  ZH_HK: flag = translator.load(packetPath + zh_HK_path); break;
    case  AR_AE: flag = translator.load(packetPath + ar_AE_path); break;
    case  AR_EG: flag = translator.load(packetPath + ar_EG_path); break;
    case  BE_BY: flag = translator.load(packetPath + be_BY_path); break;
    case  DA_DK: flag = translator.load(packetPath + da_DK_path); break;
    case  DE_AT: flag = translator.load(packetPath + de_AT_path); break;
    case  DE_DE: flag = translator.load(packetPath + de_DE_path); break;
    case  ES_ES: flag = translator.load(packetPath + es_ES_path); break;
    case  FR_FR: flag = translator.load(packetPath + fr_FR_path); break;
    case  IT_IT: flag = translator.load(packetPath + it_IT_path); break;
    case  JA_JA: flag = translator.load(packetPath + ja_JA_path); break;
    case  TH_TH: flag = translator.load(packetPath + th_TH_path); break;
    case  UK_UA: flag = translator.load(packetPath + uk_UA_path); break;
    default: break;
    }
    if(flag){
        if(m_app==nullptr){
            qDebug()<<__FILE__<<"line"<<__LINE__- 1 <<"Please call initLanguageTranslator() function to set Envirment.";
        }
        if(m_app->installTranslator(&translator)){
            m_engine->retranslate();
        }else{
            qDebug()<<__FILE__<<"line"<<__LINE__- 4 <<":Install Translator false,try again.";
        }
    }else{
        qDebug()<<packetPath + en_US_path;
        qDebug()<<__FILE__<<" line "<<__LINE__- 8 <<": load language packet failed,language packet is exists in your path?";
    }

}

QString LanguageTranslator::getAppPath() const
{
    return m_appPath;
}
