#include "caudiolyrics.h"

CAudioLyrics::CAudioLyrics()  //QObject *parent
{
    m_lyricsIndex = 0;
    m_lyricsMaxIndex = 0;
    m_lyricsTimePre = 0;
    m_strFileName = "";
}

QStringList CAudioLyrics::lyricsLst()
{
    return m_LyricsLst;
}

QString CAudioLyrics::lyricsName()
{
    return m_strFileName;
}

QJsonArray CAudioLyrics::lyricsJsonLst()
{
    return m_jsonArray;
}

int CAudioLyrics::getLyricsIndex(int _currentMSPositon)
{
    if(m_lyricsTimePre > _currentMSPositon)
    {// backward
        _dealBackward(_currentMSPositon);
    }
    else
    {// forward
        _dealFoward(_currentMSPositon);
    }

    m_lyricsTimePre = _currentMSPositon;
    return m_lyricsIndex;
}

void CAudioLyrics::setLyricEndTime(int _duration)
{
    m_vectTimes.append(_duration);
}

void CAudioLyrics::setLyricsName(QString _name)
{
    m_strFileName = _name;
    _getLyrics();
}

void CAudioLyrics::_getLyrics()
{
    m_lyricsIndex = 0;
    m_lyricsTimePre = 0;

    if(m_strFileName.isEmpty()){
        m_LyricsLst.append("歌词文件未定义");
    }
    else {
        QFile _file(m_strFileName);
        if(!_file.exists()){
            m_LyricsLst.append("歌词文件不存在");
        }
        else{// QFile::ReadOnly | QFile::Text
            if(!_file.open(QFile::ReadWrite)){
              m_LyricsLst.append("歌词文件打开失败");
            }
            else {
                QTextCodec* _txtCodec = QTextCodec::codecForName("UTF-8");
                if(_isGBKCodeFormat(_file))
                    _txtCodec = QTextCodec::codecForName("GB2312");


                QTextStream _txtStream(&_file);
                _txtStream.setCodec(_txtCodec);   // UTF-8  GB2312
                //_txtStream.setCodec("UTF-8");   // UTF-8  GB2312


                QString _txtLines;
                bool toIntOk = false;
                int itotalTimeinMs = 0;
                m_LyricsLst.append("");     //第一句一定要加上空，否则会快一句
                while (!_txtStream.atEnd()) {
                    _txtLines = _txtStream.readLine();
                    //qDebug() << _txtLines;
                    _txtLines.mid(1,1).toInt(&toIntOk);
                    if(!toIntOk){
                        continue;
                    }
                    else {
                        itotalTimeinMs = _txtLines.mid(1,2).toInt(&toIntOk)*60*1000 + \
                                _txtLines.mid(4,2).toInt(&toIntOk)*1000 + \
                                _txtLines.mid(7,2).toInt(&toIntOk);
                        m_LyricsLst.append(_txtLines.mid(10));
                        m_vectTimes.append(itotalTimeinMs);
                    }
                }
            }

        }
    }

    m_lyricsMaxIndex = m_LyricsLst.length()-1;
    _convertToJsonData();
    //_printAllDates();
    //qDebug() <<"emit lyricsLstChanged";
    emit lyricsLstChanged(m_LyricsLst);
    emit lyricsJsonLstChanged(m_jsonArray);
    return;
}

void CAudioLyrics::_convertToJsonData()
{
    QJsonObject _jsonobj;
    foreach(auto itemlyric, m_LyricsLst)
    {
       _jsonobj.insert("lyrics", itemlyric);
       m_jsonArray.append(_jsonobj);
    }
    return;
}

bool CAudioLyrics::_isUTF8Format(QByteArray* _pbyteArray, int _iSize)
{
    qDebug() <<"size: " << _iSize;
    bool bRetUTF8 = true;
    unsigned char* start = (unsigned char*)_pbyteArray;
    unsigned char* end = (unsigned char*)_pbyteArray + _iSize;
    while (start < end)
    {
        if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符
        {
           start++;
        }
        else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
        {
           bRetUTF8 = false;
           qDebug() << "1111";
           break;
        }
        else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符
        {
           if (start >= end - 1)
               break;
           if ((start[1] & (0xC0)) != 0x80)
           {
               qDebug() << "22222";
               bRetUTF8 = false;
               break;
           }
           start += 2;
        }
        else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符
        {
           if (start >= end - 2)
               break;
           if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
           {
               bRetUTF8 = false;
               qDebug() << "33333";
               break;
           }
           start += 3;
        }
        else
        {
           bRetUTF8 = false;
           qDebug() << "4444";
           break;
        }
   }
    return bRetUTF8;
}

bool CAudioLyrics::_isGBKCodeFormat(QFile& _file)
{
    bool bRetBGK = true;
    qDebug() << "start code format check by datastream read." ;
    QDataStream _dataStreamIn(&_file);
    QByteArray _byteArray;
    qint8 _date;
    while (!_dataStreamIn.atEnd()) {
        _dataStreamIn >> _date;
        _byteArray.append(_date);
    }
    //qDebug() << _byteArray.size() << _byteArray.length();

    QTextCodec::ConverterState state;
    QTextCodec* _txtCodec = QTextCodec::codecForName("GB2312");

    QString _tmp = _txtCodec->toUnicode(_byteArray, _byteArray.size(), &state);
    //qDebug() << "unicode: " << _tmp;

    if(state.invalidChars > 0)
    {
        bRetBGK = false;
        //qDebug() << "utf format..." ;
    }
    _file.seek(0);
    return  bRetBGK;
}

void CAudioLyrics::_dealFoward(int _currretTime)
{// 可能性3种：正常运行[index++]；存在快进；到最后一句[do nothing]
    bool bNormalNext = false;
    if( m_lyricsIndex < m_lyricsMaxIndex)
    {
        if(_currretTime > m_vectTimes[m_lyricsIndex])
        {
            //qDebug() << "大于index的时间搓,正常播放,触发下一帧";
            bNormalNext = true;

            while (_currretTime > m_vectTimes[m_lyricsIndex+1]) {
                //qDebug() << "大于index的下一个时间搓，有快进";
                bNormalNext = false;
                m_lyricsIndex++;

                if(m_lyricsIndex == m_lyricsMaxIndex)
                {
                    break;
                }
            }
        }

    }

    if(bNormalNext)
    {// 正常播放，只要大于下一个时间搓，index++即可
        m_lyricsIndex++;
    }
    return;
}

void CAudioLyrics::_dealBackward(int _currretTime)
{//存在的情况2个： 1存在快退； 2到第一句[do nothing]
    if( m_lyricsIndex >0 )
    {
        if(_currretTime < m_vectTimes[m_lyricsIndex])
        {// 本句歌词上的后退，do nothing
            while (_currretTime < m_vectTimes[m_lyricsIndex-1])
            {// 超过本句歌词上的后退，拿到最新索引
                m_lyricsIndex--;
                if(m_lyricsIndex == 0)
                {
                    break;
                }
            }
        }
    }
    return;
}


void CAudioLyrics::_printAllDates()
{
    int i=0;
    for(; i<m_lyricsMaxIndex; i++)
    {
        qDebug() << i << m_vectTimes[i] << m_LyricsLst[i];
    }
    qDebug() << i << m_LyricsLst[i];
    qDebug() << m_lyricsMaxIndex;
}

