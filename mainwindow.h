#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include <QApplication>
#include <QDesktopWidget>
#include <QVariant>
#include <QSettings>
#include <QCommandLineParser>

#include <QPushButton>

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

#include <QMimeData>
#include <QDragEnterEvent>
#include <QDrag>
#include <QDropEvent>
#include <QKeyEvent>

#include <QDir>
#include <QSlider>

#include <initguid.h>
#include "wmp.h"


#include <dbmanager.h>

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>

#include <QTimer>

#include <dialogaboutme.h>

#include <iostream>
#include <Windows.h>

#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



    struct activeSettings{
        bool paused = false;
        bool mute = false;
        int volume = 55;
        int currentId = 0;
        int current_index = 0;
        int selected_index = 0;
        int selected_id = -1;
        bool repeatCurrentMusic = false;
        bool repeatList = false;
        bool shuffle = false;
        QString dosya = "";
    };

    struct Music{
        int id = 0;
        QString name = "";
        QString path = "";
        int lenghtAsSecond = 0;
        QString lenghtToShow = "";
    };

    struct Duration{
        int musicId = 0;
        QString path = "";
        int lenghtAsSecond = 0;
        QString lenghtToShow = "";
    };


    //qDebug() << ui->mediaPlayer->propertyBag(); ile elde ettim
    struct WMP_Property_values{
       QString SAMIFilename = "";
       QString SAMILang = "";
       QString SAMIStyle = "";
       QString URL = "";
       int _cx = 2143;
       int _cy = 820;
       bool autoStart = true;
       int balance = 0;
       QString baseURL = "";
       QString captioningID = "";
       int currentMarker = 0;
       double currentPosition = 0;
       QString defaultFrame = 0;
       bool enableContextMenu = false;
       bool enableErrorDialogs = false;
       bool enabled = true;
       bool fullScreen = false;
       bool invokeURLs = true;
       bool mute = false;
       int playCount = 1;
       double rate = 1;
       bool stretchToFit = true;
       QString uiMode = "full";
       int volume = 50;
       bool windowlessVideo = false;
    };

    struct WMP_Property_names{
       QString SAMIFilename = "SAMIFilename";
       QString SAMILang = "SAMILang";
       QString SAMIStyle = "SAMIStyle";
       QString URL = "URL";
       QString _cx = "_cx";
       QString _cy = "_cy";
       QString autoStart = "autoStart";
       QString balance = "balance";
       QString baseURL = "baseURL";
       QString captioningID = "captioningID";
       QString currentMarker = "currentMarker";
       QString currentPosition = "currentPosition";
       QString defaultFrame = "defaultFrame";
       QString enableContextMenu = "enableContextMenu";
       QString enableErrorDialogs = "enableErrorDialogs";
       QString enabled = "enabled";
       QString fullScreen = "fullScreen";
       QString invokeURLs = "invokeURLs";
       QString mute = "mute";
       QString playCount = "playCount";
       QString rate = "rate";
       QString stretchToFit = "stretchToFit";
       QString uiMode = "uiMode";
       QString volume = "volume";
       QString windowlessVideo = "windowlessVideo";
    };



    enum PlayStateConstants {Stopped = 0, Paused = 1, Playing = 2};
    enum ReadyStateConstants {Uninitialized = 0, Loading = 1, Interactive = 3, Complete =4};

    Music *music;

    WMP_Property_names *wmp_property_names;
    WMP_Property_values *wmp_property_values;
    activeSettings _activeSettings;
    Duration _musicDuration;



private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlayer *playerForDuration;
    QTimer *tmrGetMusicDuration;

    dialogAboutMe *dlgAboutMe;

    QSystemTrayIcon *mSystemTrayIcon;





private slots:

    void mediaStatuChngd(QMediaPlayer::MediaStatus state);
    void on_positionChanged(qint64 position);
    void on_durationChanged(qint64 position);

    void on_playerForDuration_durationChanged(qint64 position);
    void fnc_getMusicDurationAsync();


    void pbVolume_valueChanged(int value);

    void on_btnDosyaSec_clicked();

    void on_lstMusic_doubleClicked(const QModelIndex &index);
    int getMusicDurationAsSecond(QString &filePath);

    void on_pbMusic_sliderMoved(int position);

    void updateWindowTitle(const QString &state);
    void fnc_btnMuteClick();
    void fnc_setShuffle();
    void fnc_setRepeatList();
    void fnc_setRepeatCurrent();
    void fnc_getSettings();
    void fnc_setSettings();
    void fnc_btnPlay_clicked();
    void fnc_lstMusic_singleClicked(const QModelIndex &index);
    void fnc_pbVolumeReleased();
    void fnc_nextMusic();
    void fnc_previousMusic();
    void fnc_changeMusic();
    void fnc_searchMusic(const QString &text);

    void fnc_aboutMe();
    void fnc_exitProgram();
    void fnc_showMusicInf();
    void fnc_exportPlaylist();
    void fnc_importPlaylist();
    void fnc_clearList();

    void on_pbMusic_sliderReleased();

public slots:


    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);

    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void getListe(QString searchWord);
    QString sarki_sure_getir(int sure_saniye);
    void PlayMusic(int rowindex);
    void fnc_getMusicDuration(QString musicPath);

signals:
    void musicAdded();



};


class MySlider : public QSlider
{

protected:
  void mousePressEvent ( QMouseEvent * event )
  {
    if (event->button() == Qt::LeftButton)
    {
        if (orientation() == Qt::Vertical)
            setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
        else
            setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() ) ;

        event->accept();
    }
    QSlider::mousePressEvent(event);
  }
};




#endif // MAINWINDOW_H
