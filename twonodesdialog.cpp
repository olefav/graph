#include "twonodesdialog.h"
#include "ui_twonodesdialog.h"

TwoNodesDialog::TwoNodesDialog(int n, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TwoNodesDialog)
{
    ui->setupUi(this);
    ui->sbStart->setMaximum(n);
    ui->sbEnd->setMaximum(n);
}

int TwoNodesDialog::getStart() const
{
    return ui->sbStart->value() - 1;
}

int TwoNodesDialog::getEnd() const
{
    return ui->sbEnd->value() - 1;
}

TwoNodesDialog::~TwoNodesDialog()
{
    delete ui;
}
