#ifndef MUSIC_H
#define MUSIC_H

#include <QMediaPlayer>

class Music
{
public:
    Music();
    QMediaPlayer player;
    int tot, current = 0;
    QStringList list;

    void init();
};

#endif // MUSIC_H
