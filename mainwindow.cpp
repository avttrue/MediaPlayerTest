#include "mainwindow.h"

#include <QDebug>
#include <QBuffer>
#include <QFile>
#include <QAudioOutput>
#include <QMediaPlayer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QFile file("../test/Bankrobber.mp3");
    if(!file.open(QIODevice::ReadOnly))
        qDebug() << "File not opened";
    qDebug() << "File size:" << file.size(); // File size: 11181085

    QByteArray ba = file.readAll();
    qDebug() << "ByteArray size:" << ba.size(); // ByteArray size: 11181085

    QBuffer* buffer = new QBuffer(this);
    buffer->setData(ba);
    if(!buffer->open(QIODevice::ReadOnly))
        qDebug() << "MediaPlayer: Device not opened";
    qDebug() << "Buffer size:" << buffer->size(); // Buffer size: 11181085

    buffer->seek(qint64(0));

    auto audioOutput = new QAudioOutput(this);
    auto player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    player->setSourceDevice(buffer);
    qDebug() << "Device:" << player->sourceDevice(); // Device: QBuffer(0x563180493020)

    QObject::connect(player, &QMediaPlayer::mediaStatusChanged,
                     [=](QMediaPlayer::MediaStatus status)
    { qDebug() << "MediaStatus:" << player->mediaStatus() << "|" << status; });

    QObject::connect(player, &QMediaPlayer::errorOccurred,
                     [=](QMediaPlayer::Error error)
    { qDebug() << "Error:" << player->errorString() << "|" << error; });

    QObject::connect(player, &QMediaPlayer::playbackStateChanged,
                     [=](QMediaPlayer::PlaybackState state)
    { qDebug() << "PlaybackState:" << player->playbackState() << "|" << state; });

    player->play();
    qDebug() << "MediaStatus:" << player->mediaStatus(); // MediaStatus: QMediaPlayer::NoMedia
}

MainWindow::~MainWindow()
{
}

