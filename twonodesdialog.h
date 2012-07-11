#ifndef TWONODESDIALOG_H
#define TWONODESDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
class TwoNodesDialog;
}

class TwoNodesDialog : public QDialog
{
    Q_OBJECT
    
public:
    int getStart() const;
    int getEnd() const;
    explicit TwoNodesDialog(int n = 1, QWidget *parent = 0);
    ~TwoNodesDialog();
    
private:
    Ui::TwoNodesDialog *ui;
};

#endif // TWONODESDIALOG_H
