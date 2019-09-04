#include "mainwindow.h"
#include "ui_mainwindow.h"


static const QString path = "TheMusicDataBase.db";


static const char geometryKey[] = "Geometry";


const int settingDBIndex_selected_index = 6;
const int settingDBIndex_volume = 5;
const int settingDBIndex_shuffle = 4;
const int settingDBIndex_repeatAll = 3;
const int settingDBIndex_repeatSingle = 2;
const int settingDBIndex_current_index = 1;
const int settingDBIndex_id = 0;


const int musicDBIndex_id = 0;
const int musicDBIndex_name = 1;
const int musicDBIndex_path = 2;
const int musicDBIndex_lenghtAsSecond = 3;
const int musicDBIndex_lenghtToShow = 4;



DbManager db(path);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //_activeSettings = new activeSettings();
    music = new Music();

    wmp_property_names = new WMP_Property_names();
    wmp_property_values = new WMP_Property_values();

    mSystemTrayIcon = new QSystemTrayIcon;
    mSystemTrayIcon->setIcon(QIcon(":/programIcon.ico"));
    mSystemTrayIcon->setVisible(true);


    if (db.isOpen())
    {
        //eğer yoksa database tablolarını oluşturur
        db.createMusicListTable(db.tableNamesMusicList);
        db.createSettingsTable(db.tableNamesSettings);
    }
    else
    {
        qDebug() << "Veritabanı açılamadı!";
    }

    getListe("");
    fnc_getSettings();
    fnc_setSettings();

    player = new QMediaPlayer(this);
    playerForDuration = new QMediaPlayer(this);
    tmrGetMusicDuration = new QTimer(this);

    tmrGetMusicDuration->setInterval(1000);



    setAcceptDrops(true);


    ui->lstMusic->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->lstMusic->setSelectionMode(QAbstractItemView::SingleSelection);

    player->setVolume(_activeSettings.volume);



    /*
        burada connect yaparken dikkat edilmnesi gereken mevzuu içinde değer gönderiyorsa aynı şekilde kendi fonksiyonumuzada yazmalıyız
    */

    connect(player,SIGNAL(positionChanged(qint64)) ,this,SLOT(on_positionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)) ,this,SLOT(on_durationChanged(qint64)));
    connect(player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(mediaStatuChngd(QMediaPlayer::MediaStatus)));

    connect(playerForDuration,SIGNAL(durationChanged(qint64)) ,this,SLOT(on_playerForDuration_durationChanged(qint64)));

    connect(ui->btnMute, &QPushButton::clicked, this, &MainWindow::fnc_btnMuteClick );
    connect(ui->btnPlay, &QPushButton::clicked, this, &MainWindow::fnc_btnPlay_clicked );
    connect(ui->btnNext, &QPushButton::clicked, this, &MainWindow::fnc_nextMusic );
    connect(ui->btnPrevious, &QPushButton::clicked, this, &MainWindow::fnc_previousMusic );

    connect(ui->chkShuffle, &QCheckBox::stateChanged, this, &MainWindow::fnc_setShuffle);
    connect(ui->chkRepeatCurrent, &QCheckBox::stateChanged, this, &MainWindow::fnc_setRepeatCurrent);
    connect(ui->chkRepeatList, &QCheckBox::stateChanged, this, &MainWindow::fnc_setRepeatList);

    connect(ui->pbVolume, &QSlider::valueChanged, this, &MainWindow::pbVolume_valueChanged);
    connect(ui->pbVolume, &QSlider::sliderReleased, this, &MainWindow::fnc_pbVolumeReleased);

    connect(ui->lstMusic, &QTableView::clicked, this, &MainWindow::fnc_lstMusic_singleClicked );

    connect(ui->txtSearchMusic, &QLineEdit::textChanged, this, &MainWindow::fnc_searchMusic);

    connect(ui->menuAddMusic, &QAction::triggered, this, &MainWindow::on_btnDosyaSec_clicked);
    connect(ui->menuShowMusicInformation, &QAction::triggered, this, &MainWindow::fnc_showMusicInf);
    connect(ui->menuImportPlaylist, &QAction::triggered, this, &MainWindow::fnc_importPlaylist);
    connect(ui->menuExportPlaylist, &QAction::triggered, this, &MainWindow::fnc_exportPlaylist);
    connect(ui->actionAboutMe, &QAction::triggered, this,&MainWindow::fnc_aboutMe);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::fnc_exitProgram);
    connect(ui->actionClearList, &QAction::triggered, this, &MainWindow::fnc_clearList);



    updateWindowTitle("The Music");

    QTimer::singleShot(1000, this, SLOT(fnc_getMusicDurationAsync()));


    //openMedia("D:/qtvideo_deneme.mp4");



}



void MainWindow::fnc_getMusicDuration(QString musicPath)
{
    FILE *fp = NULL;
    fp = fopen(QString(musicPath).toLocal8Bit().data() ,"r");

    char type[2];
    fread(type, sizeof(char), 2, fp);

    qDebug() << type ;
}



void MainWindow::mediaStatuChngd(QMediaPlayer::MediaStatus state)
{



    if(state == QMediaPlayer::EndOfMedia)
    {


        if(_activeSettings.repeatCurrentMusic == true)
        {
            PlayMusic(_activeSettings.current_index);
            db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
        }

        else if(_activeSettings.shuffle == true)
        {
            _activeSettings.current_index = qrand() % ((ui->lstMusic->verticalHeader()->count()) - 0) + 0;
            PlayMusic(_activeSettings.current_index);
            db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
        }

        else
        {

            if(_activeSettings.current_index >= ui->lstMusic->verticalHeader()->count() -1 )
            {
                if( _activeSettings.repeatList){
                    _activeSettings.current_index = 0;
                    PlayMusic(_activeSettings.current_index);
                    db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
                }
            }
            else
            {
                _activeSettings.current_index +=1;
                PlayMusic(_activeSettings.current_index);
                db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
            }

        }
    }
}

void MainWindow::fnc_getMusicDurationAsync()
{
   //
    QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT * FROM " + db.tableNamesMusicList + " where lenghtAsSecond=0");

        int rowCount = model->rowCount();
        if(rowCount > 0)
        {
            int _id = model->index(0 ,musicDBIndex_id).data().toInt();
            QString path = model->index(0,musicDBIndex_path).data().toString();
            playerForDuration->setMedia(QUrl::fromLocalFile(path));

            int lenghtAsSecond   = 0;
            QString lenghtToShow     = "";


            while(playerForDuration->duration() != 0)
            {
                db.updateMusic( _id,"","",playerForDuration->duration(), sarki_sure_getir(playerForDuration->duration()) );
            }
        }



}

void MainWindow::on_playerForDuration_durationChanged(qint64 position)
{

}

void MainWindow::on_positionChanged(qint64 position)
{
    ui->pbMusic->setValue(position);
    //ui->lblEstimatedDuration->setText(sarki_sure_getir(position / 1000));

    ui->lblCurrentMusicDuration->setText( sarki_sure_getir(position / 1000).append("/").append( sarki_sure_getir( (ui->pbMusic->maximum())/1000 ) ) );
}

void MainWindow::on_durationChanged(qint64 position)
{
    ui->pbMusic->setMaximum(position);
    //ui->lblCurrentMusicDuration->setText( sarki_sure_getir( position/1000 ) );

    if(music->lenghtAsSecond == 0){
        db.updateMusicDuration(music->id, position /1000, sarki_sure_getir( position/1000 ) );
        getListe("");
    }

}




void MainWindow::pbVolume_valueChanged(int value)
{
    player->setVolume(value);
}


void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{

    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }

}

void MainWindow::dropEvent(QDropEvent *e)
{

    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString filePath = url.toLocalFile();

        QFileInfo fileInfo( filePath );
        db.insertMusic(db.tableNamesMusicList,fileInfo.fileName(),filePath, 0, "00:00");
        fnc_getMusicDuration(filePath);

        getListe("");
        //QMessageBox::information(this,"atılan",fileName);
    }


}

void MainWindow::keyPressEvent(QKeyEvent* ke)
{

}

void MainWindow::keyReleaseEvent(QKeyEvent* ke)
{
    switch (ke->key()) {
    case (Qt::Key_Up):
        //QMessageBox::information(this,"bas","yukarı bastın");
        break;

    case(Qt::Key_Down):
        //QMessageBox::information(this,"bas" ,"aşşa bastın");
        break;

    case(Qt::Key_Enter):


        break;


    case(Qt::Key_Delete):
        if(_activeSettings.selected_index > -1){
            db.removeMusic(_activeSettings.selected_id);
             _activeSettings.selected_id = -1;
             getListe("");
        }

        break;

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnDosyaSec_clicked()
{
    _activeSettings.dosya = QFileDialog::getOpenFileName(this,"Dosya Seçin");
    _activeSettings.current_index = ui->lstMusic->verticalHeader()->count();
    if(_activeSettings.dosya != ""){
        QFileInfo fileInfo( _activeSettings.dosya );

        //bu arkadaş signal olusturuyr onu bir slota bağla ve oradan deam et veya meta datayı takip et
        playerForDuration->setMedia(QUrl::fromLocalFile(_activeSettings.dosya));


        db.insertMusic(db.tableNamesMusicList,fileInfo.fileName(),_activeSettings.dosya, 0, "00:00");

        getListe("");
        PlayMusic(_activeSettings.current_index);

    }
}



void MainWindow::getListe(QString searchWord)
{

    QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT * FROM " + db.tableNamesMusicList + " where name LIKE '%" + searchWord.toLower() + "%' LIMIT 1000");

        //tablodaki bütün verileri çekebilmek için
        while (model->canFetchMore())
            model->fetchMore();

        ui->lstMusic->setModel(model);
        ui->lstMusic->setColumnHidden(0, true);
        ui->lstMusic->setColumnHidden(2, true);
        ui->lstMusic->setColumnHidden(3, true);
        ui->lstMusic->setColumnHidden(4, false);
        ui->lstMusic->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //    ui->lstMusic->selectRow(_activeSettings.selected_index);
}



void MainWindow::on_lstMusic_doubleClicked(const QModelIndex &index)
{
    int rowidx = index.row();

    PlayMusic(rowidx);

}

int MainWindow::getMusicDurationAsSecond(QString &filePath)
{


    return 0;
}

void MainWindow::on_pbMusic_sliderMoved(int position)
{
    player->setPosition( (qint64)position );
}

void MainWindow::updateWindowTitle(const QString &state)
{
    QString appName = QCoreApplication::applicationName();
    QString title = state.isEmpty() ? appName :
                    QString("%1 (%2)").arg(appName, state);
    setWindowTitle(title);
}

void MainWindow::fnc_btnMuteClick()
{
    if(_activeSettings.mute == false){
        _activeSettings.mute = true;
        player->setMuted(true);
        ui->btnMute->setIcon(QIcon(":/imgVoiceOff"));

    }
    else if(_activeSettings.mute == true){
        _activeSettings.mute = false;
        player->setMuted(false);
        ui->btnMute->setIcon(QIcon(":/imgVoiceOn"));
    }
}

QString MainWindow::sarki_sure_getir(int sure_saniye)
{

    int saniye = sure_saniye;
    QString dk = QString::number( (saniye) / 60);
    int _tmp_dk = dk.toInt() * 60;
    QString sn = QString::number( (sure_saniye - _tmp_dk ) % 60 );

    return (dk.length() == 1 ? "0" + dk : dk ) + ":" + (sn.length() == 1 ? "0" + sn : sn );
}

void MainWindow::PlayMusic(int rowindex)
{
    _activeSettings.current_index = rowindex;
    qDebug() << _activeSettings.current_index;


    music->id               = ui->lstMusic->model()->index(rowindex,0).data().toInt();
    music->name             = ui->lstMusic->model()->index(rowindex,1).data().toString();
    music->path             = ui->lstMusic->model()->index(rowindex,2).data().toString();
    music->lenghtAsSecond   = ui->lstMusic->model()->index(rowindex,3).data().toInt();
    music->lenghtToShow     = ui->lstMusic->model()->index(rowindex,4).data().toString();

    _activeSettings.dosya = QDir::toNativeSeparators( music->path );

    ui->lstMusic->selectRow(_activeSettings.current_index);
    ui->lblCurrentMusicDuration->setText( music->lenghtToShow );
    db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
    db.changeSetting( db.allTables->_tableSettings.selected_index, QString::number(_activeSettings.selected_index) );

    player->setMedia(QUrl::fromLocalFile(_activeSettings.dosya));
    player->play();
}

void MainWindow::fnc_setShuffle()
{
    bool p_success;

    if(ui->chkShuffle->isChecked() == true)
    {
        _activeSettings.shuffle = true;
        p_success =  db.changeShuffleMode(1);//  db.changeSetting(db.allTables->_tableSettings.shuffle, "1");
    }
    else
    {
        _activeSettings.shuffle = false;
        p_success =  db.changeShuffleMode(0);//  db.changeSetting(db.allTables->_tableSettings.shuffle, "1");
    }

    qDebug() << p_success;
}

void MainWindow::fnc_setRepeatList()
{
    bool p_success;
    if(ui->chkRepeatList->isChecked() == true)
    {
        _activeSettings.repeatList = true;
       p_success = db.changeSetting(db.allTables->_tableSettings.repeatAll, "1");
    }
    else
    {
        _activeSettings.repeatList = false;
       p_success = db.changeSetting(db.allTables->_tableSettings.repeatAll, "0");
    }
    qDebug() << p_success;
}

void MainWindow::fnc_setRepeatCurrent()
{
    bool p_success = false;

    if(ui->chkRepeatCurrent->isChecked() == true)
    {
        _activeSettings.repeatCurrentMusic = true;
      p_success= db.changeSetting(db.allTables->_tableSettings.repeatSingle, "1");
    }
    else
    {
        _activeSettings.repeatCurrentMusic = false;
      p_success= db.changeSetting(db.allTables->_tableSettings.repeatSingle, "0");
    }
    qDebug() << p_success;
}

void MainWindow::fnc_getSettings()
{

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM " + db.tableNamesSettings + " where id=1") ;
    _activeSettings.volume = model->index(0, settingDBIndex_volume).data().toInt();
    _activeSettings.repeatList = model->index(0, settingDBIndex_repeatAll).data().toInt() == 1 ? true : false;
    _activeSettings.repeatCurrentMusic = model->index(0, settingDBIndex_repeatSingle).data().toInt() == 1 ? true : false;
    _activeSettings.shuffle = model->index(0, settingDBIndex_shuffle).data().toInt() == 1 ? true : false;
    _activeSettings.current_index = model->index(0, settingDBIndex_current_index).data().toInt();
    _activeSettings.selected_index = model->index(0, settingDBIndex_selected_index).data().toInt();

}

void MainWindow::fnc_setSettings()
{
    ui->pbVolume->setValue( _activeSettings.volume );
 //   ui->lstMusic->selectRow( _activeSettings.current_index );
    ui->chkShuffle->setChecked( _activeSettings.shuffle  );
    ui->chkRepeatCurrent->setChecked( _activeSettings.repeatCurrentMusic  );
    ui->chkRepeatList->setChecked( _activeSettings.repeatList );
}

void MainWindow::fnc_btnPlay_clicked()
{

    if(_activeSettings.paused == false){
            _activeSettings.paused = true;
            player->pause();
            ui->btnPlay->setIcon(QIcon(":/imgOynat"));
        }
        else if(_activeSettings.paused == true){
            if(_activeSettings.selected_index > -1 )
            {
                _activeSettings.current_index = _activeSettings.selected_index;
                _activeSettings.paused = false;
                player->play();
                ui->btnPlay->setIcon(QIcon(":/imgDuraklat"));
                //PlayMusic(_activeSettings.current_index);
            }
        }
}

void MainWindow::fnc_lstMusic_singleClicked(const QModelIndex &index)
{
    _activeSettings.selected_index = index.row();
    _activeSettings.selected_id = ui->lstMusic->model()->index(_activeSettings.selected_index, musicDBIndex_id).data().toInt();
    db.changeSetting( db.allTables->_tableSettings.selected_index, QString::number(_activeSettings.selected_index) );
}

void MainWindow::fnc_pbVolumeReleased()
{
    _activeSettings.volume = ui->pbVolume->value();
    db.changeSetting( db.allTables->_tableSettings.volume, QString::number(_activeSettings.volume) );
}

void MainWindow::fnc_nextMusic()
{
    if(_activeSettings.current_index < ui->lstMusic->verticalHeader()->count() -1 )
    {
        _activeSettings.current_index++;
    }
    else
    {
        _activeSettings.current_index = 0;
    }



    if(_activeSettings.repeatCurrentMusic == true)
    {
        PlayMusic(_activeSettings.current_index);
        db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
    }

    else if(_activeSettings.shuffle == true)
    {
        _activeSettings.current_index = qrand() % ((ui->lstMusic->verticalHeader()->count()) - 0) + 0;
        PlayMusic(_activeSettings.current_index);
        db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
    }

    else
    {

        if(_activeSettings.current_index >= ui->lstMusic->verticalHeader()->count() -1 )
        {
            if( _activeSettings.repeatList){
                _activeSettings.current_index = 0;
                PlayMusic(_activeSettings.current_index);
                db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
            }
        }
        else
        {
            PlayMusic(_activeSettings.current_index);
            db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
        }

    }

}

void MainWindow::fnc_previousMusic()
{

    if(_activeSettings.current_index > 0 )
    {
        _activeSettings.current_index--;
    }
    else
    {
        _activeSettings.current_index = ui->lstMusic->verticalHeader()->count() -1;
    }

    if(_activeSettings.repeatCurrentMusic == true)
    {
        PlayMusic(_activeSettings.current_index);
        db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
    }

    else if(_activeSettings.shuffle == true)
    {
        _activeSettings.current_index = qrand() % ((ui->lstMusic->verticalHeader()->count()) - 0) + 0;
        PlayMusic(_activeSettings.current_index);
        db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
    }

    else
    {

        if(_activeSettings.current_index >= ui->lstMusic->verticalHeader()->count() -1 )
        {
            if( _activeSettings.repeatList){
                _activeSettings.current_index = 0;
                PlayMusic(_activeSettings.current_index);
                db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
            }
        }
        if(_activeSettings.current_index <= 0)
        {
            if( _activeSettings.repeatList){
                _activeSettings.current_index = _activeSettings.current_index >= ui->lstMusic->verticalHeader()->count() -1;
                PlayMusic(_activeSettings.current_index);
                db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
            }
        }
        else
        {
            PlayMusic(_activeSettings.current_index);
            db.changeSetting( db.allTables->_tableSettings.current_index, QString::number(_activeSettings.current_index) );
        }

    }
}

void MainWindow::fnc_changeMusic()
{

}

void MainWindow::fnc_searchMusic(const QString &text)
{
    getListe(text);
}

void MainWindow::fnc_aboutMe()
{
    dlgAboutMe = new dialogAboutMe();
    dlgAboutMe->exec();
}

void MainWindow::fnc_exitProgram()
{
    qApp->exit(0);
    qDebug() << "çıkış tıklaması";
}

void MainWindow::fnc_clearList()
{
    QMessageBox::StandardButton _dialog;
      _dialog = QMessageBox::question(this, tr("Listeyi Temizle"), tr("Bak bütün listeyi sileceğim siliyorum ? "),
                                    QMessageBox::Yes|QMessageBox::No);
      if (_dialog == QMessageBox::Yes)
      {
          bool clearList =  db.deleteAllList();
          if(clearList)
          {
              getListe("");
          }
      }
      else
      {

      }
}

void MainWindow::fnc_showMusicInf()
{

}

void MainWindow::fnc_exportPlaylist()
{

}

void MainWindow::fnc_importPlaylist()
{

}

void MainWindow::on_pbMusic_sliderReleased()
{

}
