#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QtMultimedia/QMediaMetaData>
#include <QtMultimedia/QMediaObject>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QDesktopServices>
#include <QToolButton>
#include <QTime>
#include <QTextCodec>
#include <QBuffer>
#include <QPoint>
#include <QComboBox>
#include <taglib/fileref.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    general = new General;

    playerSaves = new QSettings(QDir::currentPath()+"/playerSaves.ini", QSettings::IniFormat);
    settings = new QSettings(QDir::currentPath()+"/settings.ini", QSettings::IniFormat);



    //заполнение combobox именами плейлистов
    outputPlaylists();

    ui->audioListWidget->setIconSize(QSize(50,50));

    createplaylist = new newplaylist;
    aboutProgramm = new About;
    browser = new MusicBrowser;
    tagsEdit = new TagsEdit;
    radio = new Radio;

    m_player = new QMediaPlayer(this);
    m_playlist = new QMediaPlaylist(m_player);
    play = 0;
    playlistsListcurrentIndexChanged();
    connect(ui->playlistsList, SIGNAL(currentIndexChanged(int)), this, SLOT(playlistsListcurrentIndexChanged()));

    m_player->setPlaylist(m_playlist);
    m_player->setVolume(0);
    m_player->setPlaybackRate(0);
    ui->TimelineSlider->setTracking(false);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    ui->audioListWidget->setAcceptDrops(true);
    ui->audioListWidget->setDragEnabled(true);
    ui->audioListWidget->setDragDropMode(QAbstractItemView::InternalMove);

//    metaDataPlayer = new QMediaPlayer(this);
//    connect(metaDataPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged1(QMediaPlayer::MediaStatus)));
//  Проверка статуса проигрывателя
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this,[=]
    {          
        if(m_player->mediaStatus() == QMediaPlayer::LoadedMedia)
        {
            onMediaStatusChanged();
            long long time(m_player->duration()/1000);
            ui->DurationLabel->setText(QDateTime::fromTime_t(time).toUTC().toString("mm:ss"));
            ui->TimelineSlider->setMaximum(m_player->duration());
        }
        if(m_player->mediaStatus() == QMediaPlayer::EndOfMedia)
        {
            ui->audioListWidget->setCurrentRow(m_playlist->currentIndex());
            ui->audioListWidget->currentItem()->setForeground(QColorConstants::White);
        }
    });
//  Действия при нажатии кнопки начать
    connect(ui->PlayButton, &QToolButton::clicked, this, [=]
    {
        play = 1;
        GetMetaData(m_player);
        m_player->play();
    });
//  Действия при нажатии кнопки пауза
    connect(ui->PauseButton, &QToolButton::clicked, m_player, &QMediaPlayer::pause);
//  Действия при нажатии кнопки стоп
    connect(ui->StopButton, &QToolButton::clicked, this, [=]
    {
        play = 0;
        m_player->stop();
    });
    connect(ui->TimelineSlider, &QSlider::valueChanged, [this](long long time)
    {
        m_player->setPosition(time);
    });
    connect(m_player, &QMediaPlayer::positionChanged, this,[=]
    {
        long long time(m_player->position()/1000);
        ui->CurrentPosLabel->setText(QDateTime::fromTime_t(time).toUTC().toString("mm:ss"));
        if(!ui->TimelineSlider->isSliderDown())
        {
            ui->TimelineSlider->setSliderPosition(m_player->position());
        }
    });
    connect(ui->VolumeSlider, &QSlider::valueChanged, [this](int ind)
    {
        m_player->setVolume(ind);
        if(m_player->isMuted())
        {
            m_player->setMuted(!m_player->isMuted());
            ui->MuteButton->setIcon(QIcon(":/images/images/100%.svg"));
        }
    });
    connect(ui->TimelineSlider, &QSlider::valueChanged, [this](int time)
    {
        if(ui->TimelineSlider->isSliderDown())
        m_player->setPosition(time);
    });
    connect(ui->audioListWidget, &QListWidget::itemDoubleClicked, [this]()
    {
        play = 1;
        m_player->stop();
        int currentRow = ui->audioListWidget->currentRow();
        if(m_playlist->currentIndex() < (m_playlist->mediaCount()) && m_playlist->currentIndex() > -1)
        {
            ui->audioListWidget->setCurrentRow(m_playlist->currentIndex());
            ui->audioListWidget->currentItem()->setForeground(QColorConstants::White);
        }
        m_playlist->setCurrentIndex(currentRow);
        if(m_playlist->currentIndex() < (m_playlist->mediaCount()) && m_playlist->currentIndex() > -1)
        {
            ui->audioListWidget->setCurrentRow(currentRow);
            ui->audioListWidget->currentItem()->setForeground(QColorConstants::Green);
        }
    });
    connect(ui->NextButton, &QToolButton::clicked, this,[=]
    {
        play = 1;
        if(m_playlist->currentIndex() < (m_playlist->mediaCount()) && m_playlist->currentIndex() > -1)
        {
            ui->audioListWidget->setCurrentRow(m_playlist->currentIndex());
            ui->audioListWidget->currentItem()->setForeground(QColorConstants::White);
        }
        m_player->stop();
        m_playlist->next();
        if(m_playlist->currentIndex() < (m_playlist->mediaCount()) && m_playlist->currentIndex() > -1)
        {
            ui->audioListWidget->setCurrentRow(m_playlist->currentIndex());
            ui->audioListWidget->currentItem()->setForeground(QColorConstants::Green);
        }
    });
    connect(ui->PreviousButton, &QToolButton::clicked, this,[=]
    {
        play = 1;
        if(m_playlist->currentIndex() > -1)
        {
            ui->audioListWidget->setCurrentRow(m_playlist->currentIndex());
            ui->audioListWidget->currentItem()->setForeground(QColorConstants::White);
        }
        m_player->stop();
        m_playlist->previous();
        if(m_playlist->currentIndex() > -1)
        {
            ui->audioListWidget->setCurrentRow(m_playlist->currentIndex());
            ui->audioListWidget->currentItem()->setForeground(QColorConstants::Green);
        }
    });
    connect(createplaylist, &newplaylist::newplaylistsignal, this, &MainWindow::newplaylistslot);
    connect(ui->RepeatButton, &QToolButton::clicked, this, [=]
    {
        if(!isLooped)
        {
            ui->RepeatButton->setIcon(QIcon(":/images/images/repeaton.svg"));
            isLooped = true;
            m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        }
        else
        {
            ui->RepeatButton->setIcon(QIcon(":/images/images/repeat.svg"));
            isLooped = false;
            if(isRandomed)
                m_playlist->setPlaybackMode(QMediaPlaylist::Random);
            else
                m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
        }
    });
    connect(ui->RandomButton, &QToolButton::clicked, this, [=]
    {
        if(!isRandomed && !isLooped)
        {
            ui->RandomButton->setIcon(QIcon(":/images/images/random.svg"));
            isRandomed = true;
            m_playlist->setPlaybackMode(QMediaPlaylist::Random);
        }
        else
        {
            ui->RandomButton->setIcon(QIcon(":/images/images/randomoff.svg"));
            isRandomed = false;
            if(!isLooped)
                m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
            else
                m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        }
    });
    connect(ui->MuteButton, &QToolButton::clicked, this, [=]
    {
        m_player->setMuted(!m_player->isMuted());
        if(m_player->isMuted())
            ui->MuteButton->setIcon(QIcon(":/images/images/0%.svg"));
        else
            ui->MuteButton->setIcon(QIcon(":/images/images/100%.svg"));
    });
    connect(m_playlist, &QMediaPlaylist::currentMediaChanged, this, [=]
    {
        m_player->stop();
        if(m_playlist->currentIndex() < (m_playlist->mediaCount()) && m_playlist->currentIndex() > 0)
        {
            ui->audioListWidget->setCurrentRow(m_playlist->currentIndex()-1);
            ui->audioListWidget->currentItem()->setForeground(QColorConstants::White);
        }
        if(m_playlist->currentIndex() < (m_playlist->mediaCount()) && m_playlist->currentIndex() > -1)
        {
            ui->audioListWidget->setCurrentRow(m_playlist->currentIndex());
            ui->audioListWidget->currentItem()->setForeground(QColorConstants::Green);
        }
        ui->TimelineSlider->setValue(0);
    });
    connect(ui->audioListWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(browser, &MusicBrowser::mainWindowShowSignal, this, &MainWindow::show);
    connect(this, &MainWindow::urlSignal, browser, &MusicBrowser::urlSlot);
    //загрузка наcтроек
    loadSettings();
    refilling();
}


MainWindow::~MainWindow()
{
    savePlayer();
    delete ui;
}



void MainWindow::onMediaStatusChanged()
{
        GetMetaData(m_player);
        if(play == 1)
            m_player->play();
        else
        {
            m_player->stop();
            ui->cover->setPixmap(QPixmap(":/images/images/Aua(logo night).png"));
        }

}

void MainWindow::GetMetaData(QMediaPlayer *player)//получение метаданных
{
    QStringList metadatalist = player->availableMetaData();
    QString metadata_key;
    QVariant var_data;
    // Get the size of the list
//    for(int i = 0; i<(playlist->mediaCount()); i++)
//    {
    QString file;
    QString titleName,titleAuthor,titleSampleRate,titleBitRate,titleFormat,titleOutput;
        titleName = player->metaData(QMediaMetaData::Title).value<QString>();
        if(titleName == "")
            titleName = QFileInfo(m_playlist->currentMedia().request().url().path()).completeBaseName();
        titleAuthor = player->metaData(QMediaMetaData::Author).value<QString>();
        file = player->currentMedia().request().url().path();
        titleFormat = QFileInfo(file).suffix().toUpper();
        titleSampleRate = QString::number(player->metaData(QMediaMetaData::SampleRate).value<int>()/1000);
        titleBitRate = QString::number(player->metaData(QMediaMetaData::AudioBitRate).value<int>()/1000);
        if(player->metaData(QMediaMetaData::ChannelCount).value<int>() == 1)
        {
            titleOutput = "Mono";
        }
        else
        {
            titleOutput = "Stereo";
        }
//        titleSize = titlePath.size();
//        titleCover = player->metaData(QMediaMetaData::ThumbnailImage).value<QPixmap>();
//        item->setText(titleName+"\n"+titleFormat+"::"+titleSampleRate+titleBitRate);
//        item->setIcon(titleCover);

//        player->playlist()->next();
//    }

    // Define variables to store metadata key and value
    QPixmap pixmap(player->metaData(QMediaMetaData::ThumbnailImage).value<QPixmap>());
    if(pixmap.isNull())
    {
        ui->cover->setPixmap(QPixmap(":/images/images/Aua(logo night).png"));
    }
    else
    {
        ui->cover->setPixmap(pixmap);
    }
    if(titleName != "")
    ui->TrackName->setText(titleName);
    if(titleAuthor != "")
    ui->AuthorLabel->setText(titleAuthor);
    if(titleFormat != "")
    ui->TrackInfoLabel->setText(titleFormat+", "+titleSampleRate+" kHz, "+titleBitRate+" kbps, "+titleOutput);

//    for (int indx = 0; indx < list_size; indx++)
//    {

//        // Get the key from the list
//        metadata_key = metadatalist.at(indx);
//        // Get the value for the key
//        var_data = player->metaData(metadata_key);
//        qDebug() << metadata_key << var_data.value<QString>();
//    }
}

void MainWindow::addAudioItems(QStringList fileInfoString, QColor itemColor)
{
    QListWidgetItem *item = new QListWidgetItem;
    QString titleName;
    if(fileInfoString[3].isEmpty())
        titleName = QFileInfo(fileInfoString[0]).baseName();
    else
        titleName = fileInfoString[3].toLocal8Bit();
    int kHz = fileInfoString[10].toInt()/1000;
    int sizeMb = QFileInfo(fileInfoString[0]).size()/1024/1024;
    int sizeKb = (QFileInfo(fileInfoString[0]).size()/1024-sizeMb*1024)/10;
    item->setForeground(QColorConstants::White);
    item->setBackground(QColor(itemColor));
    item->setText(QString::number(m_playlist->mediaCount()+1)+". "+titleName.toLocal8Bit()+"\n"+fileInfoString[1].toUpper()+":: "+QString::number(kHz)+" kHz, "+fileInfoString[9]+" kbps, "+QString::number(sizeMb)+","+QString::number(sizeKb)+" МБ");
    ui->audioListWidget->addItem(item);

}

void MainWindow::loadSettings()
{
    QFile styleSheetFile(settings->value("Theme",":/themes/themes/Cstartpage.qss").toString());
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    qApp->setStyleSheet(styleSheet);


    ui->playlistsList->setCurrentText(playerSaves->value("Current Playlist", ui->playlistsList->currentText()).toString());
    ui->VolumeSlider->setValue(playerSaves->value("Volume", 0).toInt());
//    if(m_playlist->mediaCount()!=0)
//    {
        m_playlist->setCurrentIndex(playerSaves->value("Current Audio",0).toInt());
        ui->TimelineSlider->setMaximum(playerSaves->value("Max Time",0).toInt());
        ui->TimelineSlider->setValue(playerSaves->value("Current Time").toInt());
        ui->CurrentPosLabel->setText(playerSaves->value("Label Current Time", "0:00").toString());
        ui->DurationLabel->setText(playerSaves->value("Duration time", "0:00").toString());
        ui->TrackName->setText(playerSaves->value("Track Name", "Имя аудио").toString());
        ui->AuthorLabel->setText(playerSaves->value("Author Name", "Исполнитель").toString());
        ui->TrackInfoLabel->setText(playerSaves->value("Track Info", "Информация").toString());
        GetMetaData(m_player);
//    }
}

void MainWindow::on_AddPlaylistButton_clicked() //Открытие листа
{
    createplaylist->show();
}

void MainWindow::newplaylistslot(QString setPlaylist) //Слот для сигнала из newplaylist
{
    outputPlaylists();
    ui->playlistsList->setCurrentText(setPlaylist);
}

void MainWindow::radioSlot()
{
    m_player->pause();
}



void MainWindow::on_delitPlaylistButton_clicked() //Удалить плейлист
{
    QString delPlaylist(ui->playlistsList->currentText());
    if(delPlaylist != NULL)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удалить плейлист", "Вы действительно хотите удалить плейлист?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
         {
             QFile file("Playlists/" + delPlaylist + ".txt");
             file.remove();
             outputPlaylists();
        }
    }
}


void MainWindow::outputPlaylists() //функция заполнения combobox именами плейлистов
{
    ui->playlistsList->clear();
    QDir dir("Playlists/");

    foreach(QFileInfo item, dir.entryInfoList())
    {
        if(item.isFile())
            ui->playlistsList->addItem(item.baseName());
    }
}

void MainWindow::outputAudio() //заполнение listwidget аудио треками
{
    ui->audioListWidget->clear(); 
}

void MainWindow::on_addAudioBtn_clicked() //Добавление аудио трека в плейлист
{
    QFile defaultFile("Playlists/Default.txt");
    if(!QDir("Playlists").exists() || QDir("Playlists").isEmpty())
    {
        QDir().mkdir("Playlists");
        defaultFile.open(QIODevice::Append);
        defaultFile.close();
        ui->playlistsList->addItem("Default");
    }

    QString curAudioplaylist(ui->playlistsList->currentText());
    if(curAudioplaylist != NULL)
    {
        QStringList audioFiles;
        QString playlistPath;
        audioFiles = QFileDialog::getOpenFileNames(this, "Выберите аудио файл", "/", "Аудио (*.3gp *.aa *.aac *.aax *.act *.aiff *.alac *.amr *.ape *.au *.awd *.dss *.dvf *.flac *.gsm *.iklax *.ivs *.m4a *.m4d *.m4p *.mmf *.mp3 *.mp4 *.mpc *.msv *.nmf *.ogg *.oga *.mogg *.opus *.ra *.rm *.raw *.rf64 *.sln *.tta *.voc *.vox *.wav *.wma *.wv *.webm *.8svx *.cda)");
        playlistPath = "Playlists/" + curAudioplaylist + ".txt";
        QFile playlistFile(playlistPath);

        playlistFile.open(QIODevice::Append);
        QStringList allMetaData;
        QTextStream out(&playlistFile);
        foreach (QString audioPath, audioFiles)
        {
            out << audioPath+"|"+QFileInfo(audioPath).suffix()+"|"+QString::number(QFileInfo(audioPath).size());
            allMetaData.append(audioPath);
            allMetaData.append(QFileInfo(audioPath).suffix());
            allMetaData.append(QString::number(QFileInfo(audioPath).size()));
            foreach(QString tag,GetAllMetaData(audioPath))
            {
                out<<"|"+tag;
                allMetaData.append(tag);
            }
            out << Qt::endl;
            addAudioItems(allMetaData,QColor("46,46,46,1"));
            m_playlist->addMedia(QUrl::fromLocalFile(QFileInfo(audioPath).absoluteFilePath()));
            allMetaData.clear();
        }
        playlistFile.close();
    }
}


QStringList MainWindow::GetAllMetaData(QString audioPath)
{
    QStringList trackMetaData;
    TagLib::FileRef fileRef(QFile::encodeName(audioPath).constData());
        trackMetaData.append(QString::fromStdWString(fileRef.tag()->title().toWString()));
        trackMetaData.append(QString::fromStdWString(fileRef.tag()->artist().toWString()));
        trackMetaData.append(QString::fromStdWString(fileRef.tag()->album().toWString()));
        trackMetaData.append(QString::fromStdWString(fileRef.tag()->genre().toWString()));
        trackMetaData.append(QString::number(fileRef.tag()->year()));
        trackMetaData.append(QString::fromStdWString(fileRef.tag()->comment().toWString()));
        trackMetaData.append(QString::number(fileRef.audioProperties()->bitrate()));
        trackMetaData.append(QString::number(fileRef.audioProperties()->sampleRate()));
        trackMetaData.append(QString::number(fileRef.audioProperties()->lengthInSeconds()));
        trackMetaData.append(QString::number(fileRef.audioProperties()->channels()));
    return trackMetaData;
}

void MainWindow::playlistsListcurrentIndexChanged()
{
    m_player->stop();
    m_playlist->clear();
    ui->audioListWidget->clear();
    if(QDir("Playlists").exists() && !QDir("Playlists").isEmpty())
    {
        QStringList splitedString;
        QString curAudioplaylist(ui->playlistsList->currentText());
        QFile playlistFile("Playlists/" + curAudioplaylist + ".txt");
        QString audioFile;
        currentPlaylistColor=0;
        if (playlistFile.open(QIODevice::ReadOnly))
        {
            QTextStream playlistFileStream(&playlistFile);
            playlistFileStream.setCodec("Unicode");
            QUrl audioFileUrl;
            while(!playlistFileStream.atEnd())
            {
                audioFile = playlistFileStream.readLine();
                splitedString = audioFile.split('|');
                if(currentPlaylistColor == 0)
                {
                    addAudioItems(splitedString,QColor("77,77,77,1"));
                    currentPlaylistColor++;
                }
                else
                {
                    addAudioItems(splitedString,("46,46,46,1"));
                    currentPlaylistColor--;
                }
                audioFileUrl = QUrl::fromLocalFile(QFileInfo(splitedString[0]).absoluteFilePath());
                m_playlist->addMedia(audioFileUrl);
            }
            play = 0;
            m_player->setPlaylist(m_playlist);
            m_playlist->setCurrentIndex(0);
            playlistFile.close();
            audioFileUrl.clear();
        }
    }
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->audioListWidget->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Удалить", this, SLOT(deleteAudio()));
    myMenu.addAction("Редактировать теги", this, SLOT(editTags()));

    myMenu.exec(globalPos);
}

void MainWindow::deleteAudio()
{
    m_playlist->removeMedia(ui->audioListWidget->currentRow());
    QListWidgetItem *item = ui->audioListWidget->takeItem(ui->audioListWidget->currentRow());
    delete item;
    if(QDir("Playlists").exists() && !QDir("Playlists").isEmpty())
    {
        QString curAudioplaylist(ui->playlistsList->currentText());
        QString file("Playlists/" + curAudioplaylist + ".txt");
        QStringList allText;
        QFile inputFile(file);
        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream edit(&inputFile);
            edit.setCodec("UTF-8");
            while(!edit.atEnd())
                allText.push_back(edit.readLine());
        }
        inputFile.close();

        allText.removeAt(ui->audioListWidget->currentRow());

        QFile outputFile(file);
        if(outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream edit(&outputFile);
            edit.setCodec("UTF-8");
            for(int i=0; i<allText.count(); i++)
                edit << allText[i] << Qt::endl;
        }
    }
    playlistsListcurrentIndexChanged();
}

void MainWindow::editTags()
{
    tagsEdit->show(); 
    QStringList allText;
    if(QDir("Playlists").exists() && !QDir("Playlists").isEmpty())
    {
        QString file("Playlists/" + ui->playlistsList->currentText() + ".txt");
        QFile inputFile(file);
        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream inputFil(&inputFile);
            inputFil.setCodec("Windows-1251");
            while(!inputFil.atEnd())
                allText << inputFil.readLine();
        }
        inputFile.close();
    }
    connect(this, &MainWindow::audioPathSignal, tagsEdit, &TagsEdit::audioPathSlot);
    QString line = allText.at(ui->audioListWidget->currentRow());
    emit audioPathSignal(line.left(line.indexOf("|")));
}

void MainWindow::on_audioListWidget_customContextMenuRequested(QPoint)
{

}

void MainWindow::on_AboutButton_clicked()
{
    aboutProgramm->show();
}



void MainWindow::on_spotifyButton_clicked()
{
    browser->show();
    emit urlSignal("https://open.spotify.com/");
    play = 0;
    m_player->pause();
    this->hide();
}

void MainWindow::on_yMusicButton_clicked()
{

    browser->show();
    emit urlSignal("https://music.yandex.ru/home?=");
    play = 0;
    m_player->pause();
    this->hide();
}

void MainWindow::on_vkMusicButton_clicked()
{
    browser->show();
    emit urlSignal("https://vk.com/audios586023621");
    play = 0;
    m_player->pause();
    this->hide();
}

void MainWindow::savePlayer()
{
    playerSaves->setValue("Current Playlist", ui->playlistsList->currentText());
    if(m_playlist->mediaCount()!=0)
    {
        playerSaves->setValue("Current Audio", m_playlist->currentIndex());
        playerSaves->setValue("Max Time", ui->TimelineSlider->maximum());
        playerSaves->setValue("Current Time", ui->TimelineSlider->value());
        playerSaves->setValue("Label Current Time", ui->CurrentPosLabel->text());
        playerSaves->setValue("Duration time", ui->DurationLabel->text());
        playerSaves->setValue("Track Name", ui->TrackName->text());
        playerSaves->setValue("Author Name", ui->AuthorLabel->text());
        playerSaves->setValue("Track Info", ui->TrackInfoLabel->text());
    }
    playerSaves->setValue("Volume", ui->VolumeSlider->value());
    playerSaves->sync();
}

void MainWindow::on_optionsButton_clicked()
{
    options = new Options;
    options->show();
}

void MainWindow::refilling()
{

}



void MainWindow::on_pushButton_clicked()
{
    radio->show();
    connect(radio, &Radio::radioSinganl, this, &MainWindow::radioSlot);
}

