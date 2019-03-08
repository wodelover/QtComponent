/*************************************************************************/
/*                          Copyright Notice                             */
/* The code can not be copied or provided to other people without the    */
/* permission of Zhang Hao,otherwise intellectual property infringement  */
/* will be prosecuted.If you have any questions,please send me an email. */
/* My email is kingderzhang@foxmail.com. The final interpretation rights */
/* are interpreted by ZhangHao.                                          */
/*                  Copyright (C) ZhangHao All rights reserved           */
/*************************************************************************/

#ifndef SPEAKERCOM_H
#define SPEAKERCOM_H

#include "speakercom_global.h"
#include <QObject>
#include <QTextToSpeech>
#include <QStringList>

/**
 * @ClassName: SpeakerCom
 * @Description: Qt语音模块,播放字符串文本
 * @Autor: zhanghao kinderzhang@foxmail.com
 * @date: 2019-03-06 09:15:32
 * @Version: 1.0.0
 * @update_autor
 * @update_time
**/
class SPEAKERCOMSHARED_EXPORT SpeakerCom : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(double volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(double pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(QString engine READ engine WRITE setEngine NOTIFY engineChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString voice READ voice WRITE setVoice NOTIFY voiceChanged)

public:
    Q_ENUMS(STATUS)
    enum STATUS{
        Ready,
        Playing,
        Pause,
        Stop,
        Resume,
        Error,
    };
    SpeakerCom();

    // 开始、暂停、停止、重置播放字符串
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void resume();

    Q_INVOKABLE QString text();
    Q_INVOKABLE int status();
    Q_INVOKABLE double volume();
    Q_INVOKABLE double pitch();
    Q_INVOKABLE double rate();

    Q_INVOKABLE QString engine();
    // 获取当前系统可用的引擎,即输出设备
    Q_INVOKABLE QStringList getEngines();

    Q_INVOKABLE QString language();
    // 获取当前系统可用的语言
    Q_INVOKABLE QStringList getLanguages();

    Q_INVOKABLE QString voice();
    // 获取当前系统可用的声音源
    Q_INVOKABLE QStringList getVoices();

    // 设置待播放字符串
    Q_INVOKABLE void setText(QString text);
    // 设置播放状态
    Q_INVOKABLE void setStatus(int status);
    // 设置音量,范围在[0,1]之间，默认为系统设置音量大小
    Q_INVOKABLE void setVolume(double volume);
    // 设置播放音调高低,范围在[-1,1]之间，默认为0
    Q_INVOKABLE void setPitch(double pitch);
    // 设置播放速率,范围在[-1,1]之间，默认为0
    Q_INVOKABLE void setRate(double rate);
    // 设置播放引擎,即输出设备
    Q_INVOKABLE bool setEngine(QString engine);
    // 设置播放语言
    Q_INVOKABLE void setLanguage(QString language);
    // 设置系统播报声音
    Q_INVOKABLE void setVoice(QString voice);


signals:
    void textChanged(QString text);
    void statusChanged(int status);
    void volumeChanged(double volume);
    void pitchChanged(double pitch);
    void rateChanged(double rate);
    void engineChanged(QString engines);
    void languageChanged(QString language);
    void voiceChanged(QString voice);

private slots:
    void stateChanged(QTextToSpeech::State state);

private:
    QTextToSpeech *m_speech;
    QStringList m_voices;
    QString m_voice;
    QString m_text;
    int m_status;
    QStringList m_engines;
    QString m_engine="default";
    QStringList m_languages;
    QString m_language;
};

#endif // SPEAKERCOM_H
