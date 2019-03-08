#include "speakercom.h"
#include <QLoggingCategory>
#include <QLocale>
#include <QVariant>
#include <QVoice>

SpeakerCom::SpeakerCom()
{
    QLoggingCategory::setFilterRules(QStringLiteral("qt.speech.tts=true \n qt.speech.tts.*=true"));

    connect(m_speech,SIGNAL(stateChanged(QTextToSpeech::State)),this,SLOT(stateChanged(QTextToSpeech::State)));

    m_engines.append("default");
    foreach (QString engine, QTextToSpeech::availableEngines())
        m_engines.append(engine);

    m_speech = new QTextToSpeech(this);
    QVector<QLocale> locales = m_speech->availableLocales();

    foreach (const QLocale &locale, locales) {
        QString name(QString("%1 (%2)")
                     .arg(QLocale::languageToString(locale.language()))
                     .arg(QLocale::countryToString(locale.country())));
        m_languages.append(name);
    }

    QVector<QVoice> voices = m_speech->availableVoices();
    QVoice currentVoice = m_speech->voice();
    foreach (const QVoice &voice, voices) {
        QString name(QString("%1 - %2 - %3").arg(voice.name())
                          .arg(QVoice::genderName(voice.gender()))
                          .arg(QVoice::ageName(voice.age())));
        m_voices.append(name);
    }
}

void SpeakerCom::play()
{
    m_speech->say(m_text);
    setStatus(STATUS::Playing);
}

void SpeakerCom::pause()
{
    m_speech->pause();
    setStatus(STATUS::Pause);
}

void SpeakerCom::stop()
{
    m_speech->stop();
    setStatus(STATUS::Stop);
}

void SpeakerCom::resume()
{
    m_speech->resume();
    setStatus(STATUS::Resume);
}

bool SpeakerCom::setEngine(QString engine)
{
    if(engine==m_engine){
        return false;
    }
    m_engine = engine;
    emit engineChanged(m_engine);

    if(m_speech!=nullptr){
        delete m_speech;
        m_speech =nullptr;
    }
    if (m_engine == "default")
        m_speech = new QTextToSpeech(this);
    else
        m_speech = new QTextToSpeech(m_engine, this);

    if(m_speech==nullptr)
        return false;

    return true;
}

void SpeakerCom::setLanguage(QString language)
{
    if(language!=m_language){
        m_language = language;
        emit languageChanged(m_language);

        QVector<QLocale> locales = m_speech->availableLocales();
        QLocale current = m_speech->locale();
        foreach (const QLocale &locale, locales) {
            QString name(QString("%1 (%2)")
                         .arg(QLocale::languageToString(locale.language()))
                         .arg(QLocale::countryToString(locale.country())));

            if (name == m_language)
               {
                m_speech->setLocale(locale);
               }
        }
    }
}

void SpeakerCom::setVoice(QString voice)
{
    if(voice!=m_voice){
        m_voice = voice;
        emit voiceChanged(m_voice);

        QVector<QVoice> voices = m_speech->availableVoices();

        foreach (const QVoice &voice, voices) {
            QString name(QString("%1 - %2 - %3").arg(voice.name())
                              .arg(QVoice::genderName(voice.gender()))
                              .arg(QVoice::ageName(voice.age())));
            if(name == m_voice){
                m_speech->setVoice(voice);
            }
        }
    }
}

void SpeakerCom::stateChanged(QTextToSpeech::State state)
{
    switch (state) {
        case QTextToSpeech::Ready: setStatus(Ready); break;
        case QTextToSpeech::Speaking: setStatus(Playing); break;
        case QTextToSpeech::Paused: setStatus(Pause); break;
        case QTextToSpeech::BackendError: setStatus(Error); break;
    }
}

QString SpeakerCom::text()
{
    return m_text;
}

int SpeakerCom::status()
{
    return m_status;
}

double SpeakerCom::volume()
{
    return m_speech->volume();
}

double SpeakerCom::pitch()
{
    return m_speech->pitch();
}

double SpeakerCom::rate()
{
    return m_speech->rate();
}

QString SpeakerCom::engine()
{
    return m_engine;
}

QStringList SpeakerCom::getEngines()
{
    return m_engines;
}

QString SpeakerCom::language()
{
    return m_language;
}

QStringList SpeakerCom::getLanguages()
{
    return m_languages;
}

QString SpeakerCom::voice()
{
    return m_voice;
}

QStringList SpeakerCom::getVoices()
{
    return m_voices;
}

void SpeakerCom::setText(QString text)
{
    if(text!=m_text){
        m_text = text;
        emit textChanged(m_text);
    }
}

void SpeakerCom::setStatus(int status)
{
    if(status!=m_status){
        m_status = status;
        emit statusChanged(m_status);
    }
}

void SpeakerCom::setVolume(double volume)
{
    if(volume-m_speech->volume()>0.00||volume-m_speech->volume()<0.00){
        m_speech->setVolume(volume);
        emit volumeChanged(m_speech->volume());
    }
}

void SpeakerCom::setPitch(double pitch)
{
    if(pitch-m_speech->pitch()>0.00||pitch-m_speech->pitch()<0.00){
        m_speech->setPitch(pitch);
        emit pitchChanged(m_speech->pitch());
    }
}

void SpeakerCom::setRate(double rate)
{
    if(rate-m_speech->rate()>0.00||rate-m_speech->rate()<0.00){
        m_speech->setRate(rate);
        emit rateChanged(m_speech->rate());
    }
}

