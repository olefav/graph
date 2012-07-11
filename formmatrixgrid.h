#ifndef FORMMATRIXGRID_H
#define FORMMATRIXGRID_H

#include <QtGui/QWidget>

namespace Ui {
class FormMatrixGrid;
}

class FormMatrixGrid : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormMatrixGrid(bool ** matr, int n, QWidget *parent = 0);
    ~FormMatrixGrid();
    
private:
    Ui::FormMatrixGrid *ui;
};

#endif // FORMMATRIXGRID_H
