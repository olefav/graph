#include "sizedialog.h"
#include "ui_sizedialog.h"

#include <QRegExpValidator>

SizeDialog::SizeDialog(double currentScale, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SizeDialog)
{
    ui->setupUi(this);

    QString regExp = "^\\d+%?$";
    QRegExpValidator *val = new QRegExpValidator(QRegExp(regExp), this);

    ui->comboBox->setValidator(val);

    QString scaleText = QString::number(currentScale * 100.0) + "%";
    ui->comboBox->addItems(QStringList() << "50%" << "75%" << "100%" << "125%" << "150%" << "175%" << "200%");
    ui->comboBox->addItem(scaleText);
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(scaleText));
}

QString SizeDialog::getSize() const
{
    return ui->comboBox->currentText();
}

SizeDialog::~SizeDialog()
{
    delete ui;
}
