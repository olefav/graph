#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QFile>

class GraphScene;
class QActionGroup;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    bool isGraphEmpty() const;
    QString findStrComp();
    void makeActionGroup();
    bool okToContinue();
    void exportToMatrix(bool **matr, int n);
    void loadLinksFromBoolMatr(bool **matrix, int n);

    void sceneScaleChanged(const QString &scale);

    QActionGroup *ag;
    GraphScene *scene;
    Ui::MainWindow *ui;
    double curScale;

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void newGraph();
    void deleteSelected();
    void changeScale();
    void loadFromFile();
    bool saveToFile();
    void changeMode(QAction *);
    void checkDelete();
    void showAdjMatrix();
    void buildReachMatrix();
    void checkRouteTwoNodes();
    void setWModifTrue();
    void setWModifFalse();
};

#endif // MAINWINDOW_H
