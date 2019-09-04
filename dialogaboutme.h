#ifndef DIALOGABOUTME_H
#define DIALOGABOUTME_H

#include <QDialog>

namespace Ui {
class dialogAboutMe;
}

class dialogAboutMe : public QDialog
{
    Q_OBJECT

public:
    explicit dialogAboutMe(QWidget *parent = 0);
    ~dialogAboutMe();

private:
    Ui::dialogAboutMe *ui;
};

#endif // DIALOGABOUTME_H
