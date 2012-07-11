#ifndef SIZEDIALOG_H
#define SIZEDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
class SizeDialog;
}

class SizeDialog : public QDialog
{
    Q_OBJECT
    
public:
    QString getSize() const;
    explicit SizeDialog(double currentScale = 1.0, QWidget *parent = 0);
    ~SizeDialog();
    
private:
    Ui::SizeDialog *ui;
};

#endif // SIZEDIALOG_H
