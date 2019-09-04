#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>



/**
 * \class DbManager
 *
 * \brief SQL Database Manager class
 *
 * DbManager sets up the connection with SQL database
 * and performs some basics queries. The class requires
 * existing SQL database. You can create it with sqlite:
 * 1. $ sqlite3 people.db
 * 2. sqilte> CREATE TABLE people(ids integer primary key, name text);
 * 3. sqlite> .quit
 */
class DbManager
{
public:
    /**
     * @brief Constructor
     *
     * Constructor sets up connection with db and opens it
     * @param path - absolute path to db file
     */
    DbManager(const QString& path );

    /**
     * @brief Destructor
     *
     * Close the db connection
     */
    ~DbManager();

    bool isOpen() const;

    /**
     * @brief Creates a new 'people' table if it doesn't already exist
     * @return true - 'people' table created successfully, false - table not created
     */
    bool createTable();

    /**
     * @brief Music Listesi için gereken tabloyu oluşturur
     * @param tableName - eklenecek tablonun adı
     * @return true - true dönerseişlem başarılı false dönerse başarısız
     */
    bool createMusicListTable(const QString &tableName);

    /**
     * @brief Program için gereken ayarların tablosunu oluşturur
     * @param tableName - eklenecek tablonun adı
     * @return true - true dönerseişlem başarılı false dönerse başarısız
     */
    bool createSettingsTable(const QString &tableName);



    /**
     * @brief Müzik eklemek için gereken metod
     * @param tableName - müziklerin tutulduğu tablonun adı
     * @param name - name müziğin adı
     * @param path - ilgili dosyanın yolu
     * @param lenght - müziğin uzunluğu
     * @return true - müzik eklendi, false - müzik eklenmediğinde
     */
    bool insertMusic(const QString tableName, const QString &name, const QString &path, const int &lenghtAsSecond, QString lenghtToShow);

    /**
     * @brief Remove person data from db
     * @param name - name of person to remove.
     * @return true - person removed successfully, false - person not removed
     */
    bool removePerson(const QString& name);

    /**
     * @brief Check if person of name "name" exists in db
     * @param name - name of person to check.
     * @return true - person exists, false - person does not exist
     */
    bool personExists(const QString& name) const;

    /**
     * @brief Print names of all persons in db
     */
    void printAllPersons() const;


    /**
     * @brief Remove all persons from db
     * @return true - all persons removed successfully, false - not removed
     */
    bool removeAllPersons();

    /**
     * @brief changeSetting Programın veritabanına kayıtlı ayarlarından birini değiştirmek için kullanılır. Bütün değerler int değerinde dir.
     * @param columnName Değişecek olan ayarın tutulduğu kolonun adı
     * @param value değişecek olan kolonun değeridir.
     * @return true işlem başarılı ise, false işlem başarısız ise
     */
    bool changeSetting(  QString columnName, QString value );


    /**
     * @brief getAndSetSettings program içerisinde veritabanından ayarları getirip MainWindow içerisindeki  ayarlar yapısına verileri yazar
     */
    int getSetting(QString whichOne);

    /**
     * @brief updateMusic
     * @param musicId
     * @param name
     * @param path
     * @param lenghtAsSecond
     * @param lenghtToShow
     * @return
     */
    bool updateMusic(const int &musicId,const QString &name, const QString &path, const int &lenghtAsSecond, QString lenghtToShow);

    /**
     * @brief updateMusicDuration
     * @param musicId
     * @param lenghtAsSecond
     * @param lenghtToShow
     * @return
     */
    bool updateMusicDuration(const int &musicId , const int &lenghtAsSecond , QString lenghtToShow );

    /**
     * @brief removeMusic
     * @param musicId
     * @return
     */
    bool removeMusic(const int &musicId);

    bool changeShuffleMode(int value );
    bool deleteAllList();


    QString tableNamesMusicList = "tblMusicList";
    QString tableNamesSettings = "tblSettings";

    typedef struct {
         QString id = "id";
         QString current_index = "current_index";
         QString selected_index = "selected_index";
         QString repeatSingle = "repeatSingle";
         QString repeatAll = "repeatAll";
         QString shuffle = "shuffle";
         QString volume = "volume";
     } table_settings ;

    typedef struct{
        QString id = "id";
        QString name = "name";
        QString path = "path";
        QString lenghtAsSecond = "lenghtAsSecond";
        QString lenghtToShow = "lenghtToShow";
    } table_music;


    /**
     * @brief Bir üst alanda oluşturulan struct yapılarının toplanıp aynı yerden çağürıldığı yerdir.
     */
    struct tablesAndColumns{

        table_settings _tableSettings;
        table_music _tableMusic;

    };




    const tablesAndColumns *allTables;




private:
    QSqlDatabase m_db;


};

#endif // DBMANAGER_H
