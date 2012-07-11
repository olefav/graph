#include "adjmatrdialog.h"
#include "ui_adjmatrdialog.h"

#include "adjmatrixmodel.h"

AdjMatrDialog::AdjMatrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdjMatrDialog)
{
    ui->setupUi(this);

    model = new AdjMatrixModel(2, this);

    adjustTableView();
}

AdjMatrDialog::AdjMatrDialog(bool **matrix, int n, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdjMatrDialog)
{
    ui->setupUi(this);

    model = new AdjMatrixModel(n, this);
    model->fromBoolMatrix(matrix, n);

    adjustTableView();
}

AdjMatrDialog::~AdjMatrDialog()
{
}

void AdjMatrDialog::adjustTableView()
{
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
}

void AdjMatrDialog::addRC()
{
    model->incrementN();
}

void AdjMatrDialog::removeRC()
{
    model->decrementN();
}
