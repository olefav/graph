#include "formmatrixgrid.h"
#include "ui_formmatrixgrid.h"

FormMatrixGrid::FormMatrixGrid(bool **matr, int n, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMatrixGrid)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    ui->tableWidget->setRowCount(n);
    ui->tableWidget->setColumnCount(n);
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(matr[i][j])));
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);
}

FormMatrixGrid::~FormMatrixGrid()
{
    delete ui;
}
