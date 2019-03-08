#ifndef CAUDIOLYRICS_H
#define CAUDIOLYRICS_H

#include "caudiolyrics_global.h"
#include <QObject>
#include <QList>
#include <QMap>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <qdebug.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QDataStream>

class CAUDIOLYRICSSHARED_EXPORT CAudioLyrics  : public QObject
{
    Q_OBJECT

public:
    CAudioLyrics();    //QObject *parent = 0

    /* 属性取消，因为处理歌词索引操作在C++中，无需开放变量到qml中
     * Q_PROPERTY(QVector<int> timesLst READ timesLst NOTIFY timesLstChanged)
     * QVector<int>    timesLst();
     * signal:
     * void        timesLstChanged(QVector<int> _vector);
     * */

    // 设置歌词list属性，歌词列表
    Q_PROPERTY(QStringList lyricsLst READ lyricsLst NOTIFY lyricsLstChanged) // read only property

    // 设置歌词json list属性，json数据 ["lyrics:" ****]
    Q_PROPERTY(QJsonArray lyricsJsonLst READ lyricsJsonLst NOTIFY lyricsJsonLstChanged)


    // 设置歌词完整路径，qml中实例化时，注意要component加载完成，再传值
    // 因为c++要比qml先加载起来，qml收不到信号
    Q_PROPERTY(QString lyricsName READ lyricsName WRITE setLyricsName NOTIFY lyricsNameChanged)

    QStringList     lyricsLst();
    QJsonArray      lyricsJsonLst();
    QString         lyricsName();

     /* qml中获取当前歌词索引
     * 输入参数：qml中audio的position属性，当前播放位置，单位ms
     * 返回值：当前歌词索引
     * */
    Q_INVOKABLE     int     getLyricsIndex(int _currentMSPositon);

    /* qml中设置歌曲结束时间搓
     * 必须要传递最后一个时间参数  ！！！！！
     * 输入参数：qml中audio的duration属性，总时长，单位ms
     * */
    Q_INVOKABLE     void    setLyricEndTime(int _duration);

    // c++内部使用时，设置歌词路径，获取歌词
    void            setLyricsName(QString _name);

signals:
    void        lyricsLstChanged(QStringList _lyricsLst);
    void        lyricsJsonLstChanged(QJsonArray _jsonArray);
    void        lyricsNameChanged(QString _name);

private:
    // 获取所有歌词，并触发lyricsLstChanged, lyricsJsonLstChanged信号
    void        _getLyrics();

    // 获取所有歌词后，转为json数据格式["lyrics:" ****]
    void        _convertToJsonData();

    // 判断歌词文件是否为UTF格式，根据二进制范围判断，实测无法分辨UTF格式 -- 暂时废弃
    bool        _isUTF8Format(QByteArray* _pbyteArray, int _iSize);

    // 判断歌词文件是否为GBK格式，使用而进度读取，然后再转为unicode是否成功判断
    bool        _isGBKCodeFormat(QFile&  _file);

    // 快进处理，包括正常播放，快进，快进到最后一项三种情况
    void        _dealFoward(int _currretTime);

    // 快退处理， 只有快退和快退到第一项两种情况
    void        _dealBackward(int _currretTime);

    // 调试用，打印所有的歌词时间信息
    void        _printAllDates();

private:
    // 返回歌词索引
    int         m_lyricsIndex;

    // 获取当前歌词的最大行数
    int         m_lyricsMaxIndex;

    // 保存上次时间搓，判断是快进还是快退
    int         m_lyricsTimePre;

    // 完整路径的歌词
    QString     m_strFileName;

    // 获取到的歌词列表，为了和时间搓对应，第一句歌词置空
    QStringList m_LyricsLst;

    // 获取到的歌词时间搓数组，注意要包含最后一个时间总长信息
    QVector<int> m_vectTimes;

    QJsonArray  m_jsonArray;
};

#endif // CAUDIOLYRICS_H
