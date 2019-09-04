#include <dbmanager.h>
#include <QDebug>

DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    m_db.setDatabaseName(path);

    allTables = new tablesAndColumns();

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::createTable()
{
    bool success = false;

    return success;
}

bool DbManager::createMusicListTable(const QString &tableName)
{
    bool success = true;

    QSqlQuery query;
    query.prepare("CREATE TABLE " + tableName + "("
                  + allTables->_tableMusic.id       + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                  + allTables->_tableMusic.name     + " TEXT, "
                  + allTables->_tableMusic.path     + " TEXT,"
                  + allTables->_tableMusic.lenghtAsSecond   + " INTEGER "
                  + allTables->_tableMusic.lenghtToShow + " TEXT );");


    if (!query.exec())
    {
        qDebug() << "müzik listesi tablosu zaten oluşturulmuş durumda gardaşım ";
        success = false;
    }

    return success;
}

bool DbManager::createSettingsTable(const QString &tableName)
{
    bool success = true;

    QSqlQuery query;
    query.prepare("CREATE TABLE " + tableName +
                  "(" + allTables->_tableSettings.id + " INTEGER PRIMARY KEY, "
                      + allTables->_tableSettings.current_index  + " INTEGER, "
                      + allTables->_tableSettings.repeatSingle + " INTEGER, "
                      + allTables->_tableSettings.repeatAll + " INTEGER, "
                      + allTables->_tableSettings.shuffle + " INTEGER, "
                      + allTables->_tableSettings.volume + " INTEGER);");

    if (!query.exec())
    {
        qDebug() << "Ayarlar tablosu zaten oluşturulmuş durumda.";
        success = false;
    }

    if (success == true)
    {
        QSqlQuery queryValues;
        queryValues.prepare("INSERT INTO " + tableName
                            + "("
                                + allTables->_tableSettings.id + ", "
                                + allTables->_tableSettings.current_index + ", "
                                + allTables->_tableSettings.repeatSingle + ", "
                                + allTables->_tableSettings.repeatAll + ", "
                                + allTables->_tableSettings.shuffle + ", "
                                + allTables->_tableSettings.volume
                            + ") VALUES(1,0,0,0,0,50);");

        if (!queryValues.exec())
        {
            qDebug() << "Ayarlar Tablosunun değerleri eklenemedi.";
            success = false;
        }
    }

    return success;
}

bool DbManager::changeSetting(QString columnName, QString value )
{

    bool success = false;

    if(columnName != "" && value != "")
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("UPDATE " + tableNamesSettings + " SET " + columnName + "=:value where id=1");
        queryAdd.bindValue(":value", value);

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "ayar değiştirilemedi: " << queryAdd.lastError();
        }
    }
     return success;

}

bool DbManager::changeShuffleMode(int value )
{
    bool success = false;

    if( value > -1)
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("UPDATE " + tableNamesSettings + " SET shuffle=" + QString::number(value) + " where id=1");
        qDebug() << "UPDATE " + tableNamesSettings + " SET shuffle=" + QString::number(value) + " where id=1";

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "ayar değiştirilemedi: " << queryAdd.lastError();
        }
    }
     return success;

}


bool DbManager::insertMusic(const QString tableName,const QString &name, const QString &path, const int &lenghtAsSecond, QString lenghtToShow)
{
    bool success = false;

    if (!name.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO " + tableName + " (name,path,lenghtAsSecond,lenghtToShow) VALUES (:name,:path,:lenghtAsSecond,:lenghtToShow)");
        queryAdd.bindValue(":name", name);
        queryAdd.bindValue(":path", path);
        queryAdd.bindValue(":lenghtAsSecond", lenghtAsSecond);
        queryAdd.bindValue(":lenghtToShow", lenghtToShow);

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "müzik eklenemedi: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "Ekleme yapılamsı için veri gerekmektedir.";
    }

    return success;
}

bool DbManager::removeMusic(const int &musicId)
{
    bool success = false;

    if (musicId > -1)
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM " + tableNamesMusicList + " WHERE id=" + QString::number(musicId) );
        success = queryDelete.exec();

        if(!success)
        {
            qDebug() << "MusicSilindi: " << queryDelete.lastError();
        }
    }
    else
    {
        qDebug() << "MusicSilinemedi";
    }

    return success;
}


bool DbManager::updateMusicDuration(const int &musicId , const int &lenghtAsSecond , QString lenghtToShow )
{
    bool success = false;



    if (musicId > -1)
    {
        QString query_str = "UPDATE " + tableNamesMusicList +
                " SET "
                + "lenghtAsSecond= " + QString::number(lenghtAsSecond) + " , "
                + "lenghtToShow='"+ lenghtToShow + "' "
                + "where id=" + QString::number(musicId);

        qDebug() << query_str;

        QSqlQuery queryAdd;
        queryAdd.prepare(query_str);


        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "müzik eklenemedi: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "Güncelleme için müzik id gelmelidir...";
    }

    return success;
}

bool DbManager::updateMusic(const int &musicId = -1,const QString &name = "", const QString &path = "", const int &lenghtAsSecond = 0, QString lenghtToShow = "00:00")
{
    bool success = false;

    QString query_str = "UPDATE " + tableNamesMusicList + " SET ";
    if(!name.isEmpty()) query_str.append(" name=':name', ");
    if(!path.isEmpty()) query_str.append(" path=':path', ");
    if(!lenghtAsSecond  != 0) query_str.append(" lenghtAsSecond=:lenghtAsSecond, ");
    if(!lenghtToShow.isEmpty()) query_str.append(" lenghtToShow=:lenghtToShow ");
    query_str.append(" where id=" + musicId);

    qDebug() << query_str;

    if (musicId > -1)
    {

        QSqlQuery queryAdd;
        queryAdd.prepare(query_str);
        if(!name.isEmpty()) queryAdd.bindValue(":name", name);
        if(!path.isEmpty()) queryAdd.bindValue(":path", path);
        if(!lenghtAsSecond  != 0) queryAdd.bindValue(":lenghtAsSecond", lenghtAsSecond);
        if(!lenghtToShow.isEmpty()) queryAdd.bindValue(":lenghtToShow", lenghtToShow);

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "müzik eklenemedi: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "Ekleme yapılamsı için veri gerekmektedir.";
    }

    return success;
}

bool DbManager::removePerson(const QString& name)
{
    bool success = false;

    if (personExists(name))
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM people WHERE name = (:name)");
        queryDelete.bindValue(":name", name);
        success = queryDelete.exec();

        if(!success)
        {
            qDebug() << "remove person failed: " << queryDelete.lastError();
        }
    }
    else
    {
        qDebug() << "remove person failed: person doesnt exist";
    }

    return success;
}

int DbManager::getSetting(QString whichOne)
{
    qDebug() << "Veritabanından ayar alınıyor:";
    QSqlQuery query("SELECT " + whichOne + " FROM " + tableNamesSettings + " where id=1" );
    int result = 0;
    return result;
}

bool DbManager::deleteAllList()
{
    bool success = false;

    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM " + tableNamesMusicList + " ");
    success = queryDelete.exec();

    if(!success)
    {
        qDebug() << "remove person failed: " << queryDelete.lastError();
        return false;
    }
    else return true;

}

void DbManager::printAllPersons() const
{
    qDebug() << "Persons in db:";
    QSqlQuery query("SELECT * FROM people");
    int idName = query.record().indexOf("name");
    while (query.next())
    {
        QString name = query.value(idName).toString();
        qDebug() << "===" << name;
    }
}

bool DbManager::personExists(const QString& name) const
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT name FROM people WHERE name = (:name)");
    checkQuery.bindValue(":name", name);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else
    {
        qDebug() << "person exists failed: " << checkQuery.lastError();
    }

    return exists;
}

bool DbManager::removeAllPersons()
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM people");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all persons failed: " << removeQuery.lastError();
    }

    return success;
}
