#ifndef ADJMATRDIALOG_H
#define ADJMATRDIALOG_H

#include <QtGui/QDialog>

class AdjMatrixModel;

namespace Ui {
class AdjMatrDialog;
}

class AdjMatrDialog : public QDialog
{
    Q_OBJECT
    
public:
    //void setMatrixForModel(bool ** matrix, int n);
    AdjMatrixModel * getModel() {return model;}
    AdjMatrDialog(QWidget *parent = 0);
    AdjMatrDialog(bool ** matrix, int n, QWidget *parent = 0);
    ~AdjMatrDialog();
public slots:
    void addRC();
    void removeRC();

private:
    void adjustTableView();
    Ui::AdjMatrDialog *ui;
    AdjMatrixModel *model;

};

#endif // ADJMATRDIALOG_H
