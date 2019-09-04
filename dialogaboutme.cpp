#include "dialogaboutme.h"
#include "ui_dialogaboutme.h"

dialogAboutMe::dialogAboutMe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogAboutMe)
{
    ui->setupUi(this);
}

dialogAboutMe::~dialogAboutMe()
{
    delete ui;
}
