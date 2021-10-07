#include "mainwindow.h"

#include <QDebug>
#include <QBuffer>
#include <QFile>
#include <QAudioOutput>
#include <QMediaPlayer>
//#include <QTemporaryFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto audioOutput = new QAudioOutput(this);
    auto player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);

    QObject::connect(player, &QMediaPlayer::mediaStatusChanged,
                     [=](QMediaPlayer::MediaStatus status)
    { qDebug() << "MediaStatus:" << status; });

    QObject::connect(player, &QMediaPlayer::errorOccurred,
                     [=](QMediaPlayer::Error error)
    { qDebug() << "Error:" << player->errorString() << "|" << error; });

    QObject::connect(player, &QMediaPlayer::playbackStateChanged,
                     [=](QMediaPlayer::PlaybackState state)
    { qDebug() << "PlaybackState:" << state; });

    QFile file("../test/Bankrobber.mp3");
    if(!file.open(QIODevice::ReadOnly)) qDebug() << "File not opened";
    qDebug() << "File size:" << file.size();

    QByteArray ba = file.readAll();
    qDebug() << "ByteArray size:" << ba.size();

    // ByteArray Buffer variant
    QBuffer* buffer = new QBuffer(this);
    buffer->setData(ba);
    if(!buffer->open(QIODevice::ReadOnly)) qDebug() << "Buffer not opened";
    else
    {
        qDebug() << "Buffer size:" << buffer->size();
        buffer->seek(0);
        player->setSourceDevice(buffer, QUrl("bla-bla.mp3")); // file extension makes clear file format
        player->play();
    }

    /* // TemporaryFile variant
    QTemporaryFile tfile;
    if (!tfile.open()) qDebug() << "TemporaryFile not opened";
    else
    {
        qDebug() << "TemporaryFile writed:" << tfile.write(ba);
        if(tfile.size() != ba.size())
            qDebug() << "TemporaryFile not complited";
        else
        {
            player->setSource(QUrl::fromLocalFile(tfile.fileName()));
            player->play();
        }
    }
    */
}

MainWindow::~MainWindow()
{
}

