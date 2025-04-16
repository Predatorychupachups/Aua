#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "musicbrowser.h"
#include "qmediaplayer.h"
#include "newplaylist.h"
#include "about.h"
#include "options.h"
#include "tagsedit.h"
#include <QSettings>
#include <QDir>
#include <general.h>
#include "radio.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AddPlaylistButton_clicked();

    void on_addAudioBtn_clicked();

    void on_delitPlaylistButton_clicked();

    void onMediaStatusChanged();

//    void onMediaStatusChanged1(QMediaPlayer::MediaStatus status); 

    void showContextMenu(const QPoint&);

    void on_audioListWidget_customContextMenuRequested(QPoint);

    void playlistsListcurrentIndexChanged();

    void deleteAudio();

    void editTags();

    void on_AboutButton_clicked();

    void on_spotifyButton_clicked();

    void on_yMusicButton_clicked();

    void on_vkMusicButton_clicked();

    void savePlayer();

//    void on_TimelineSlider_sliderPressed();

//    void on_TimelineSlider_sliderReleased();
    void on_optionsButton_clicked();

    void refilling();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    MusicBrowser *browser;
    newplaylist *createplaylist;
    About *aboutProgramm;
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;
    TagsEdit *tagsEdit;
    Options *options;
    QSettings *settings;
    QSettings *playerSaves;
    General *general;
    Radio *radio;

    void outputPlaylists();
    void outputAudio();
    void GetMetaData(QMediaPlayer *player);
    QStringList GetAllMetaData (QString audioPath);
    void addAudioItems(QStringList fileInfoString, QColor itemColor);
    int play;
    int currentPlaylistColor;
    bool isLooped = false;
    bool isRandomed = false;
    void loadSettings();


public slots:
    void newplaylistslot(QString selPlaylist);
    void radioSlot();

signals:
    void newtracksignal(QString,QString,QString);
    void urlSignal(QString urlString);
    void audioPathSignal(QString audioPath);
};
#endif // MAINWINDOW_H
