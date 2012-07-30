#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphscene.h"
#include "sizedialog.h"
#include "adjmatrdialog.h"
#include "adjmatrixmodel.h"
#include "formmatrixgrid.h"
#include "twonodesdialog.h"

#include "link.h"
#include "node.h"

#include <QFileDialog>
#include <QTimer>

//--------------------------------------------
Node* findNode(QList<Node*> nlist, int number)
{
    int n = nlist.count();
    for(int i = 0; i < n; ++i) {
        if(nlist.at(i)->getNumber() - 1 == number) {
            return nlist.at(i);
        }
    }
    return 0;
}

//--------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new GraphScene(this);

    curScale = 1.0;

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(QRectF(0, 0, ui->graphicsView->width(), ui->graphicsView->height()));

    makeActionGroup();

    connect(scene, SIGNAL(selectionChanged()), SLOT(checkDelete()));

    connect(scene, SIGNAL(changed(QList<QRectF>)), SLOT(setWModifTrue()));
    QTimer::singleShot(0, this, SLOT(setWModifFalse()));
}

void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = ui->graphicsView->matrix();
    ui->graphicsView->resetMatrix();
    ui->graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
    ui->graphicsView->scale(newScale, newScale);
    curScale = newScale;
}

void MainWindow::changeScale()
{
    SizeDialog sd(curScale);
    if (sd.exec() == QDialog::Accepted) {
        sceneScaleChanged(sd.getSize());
    }
}

void MainWindow::deleteSelected()
{
    scene->deleteSelectedItems();
}

void MainWindow::loadFromFile()
{
    if (!okToContinue())
        return;

    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), QApplication::applicationDirPath(), tr("Text files (*.txt)"));
    if (filename.isEmpty())
        return;
    QFile file(filename);
    file.open(QFile::ReadOnly | QFile::Text);
    if(!file.isOpen()) {
        QMessageBox::warning(0, tr("Error"), tr("Unable to open file!"));
        return;
    }

    QString txt;
    while(!file.atEnd()) {
        char c;
        file.getChar(&c);
        if((c != '0')&&(c != '1')&&(c != '\n')) {
            QMessageBox::warning(0, tr("Error"), tr("Wrong file contents!"));
            return;
        } else if(c != '\n')
            txt.append(c);
    }

    double nt = sqrt(static_cast<double>(txt.size()));
    double fnt = static_cast<int> (nt);

    if(!qFuzzyCompare(fnt, nt)) {
        QMessageBox::warning(0, tr("Error"), tr("Matrix dimension error."));
        return;
    }

    scene->clearScene();

    int n = fnt;

    bool **matrix = new bool * [n];
    for(int i = 0; i < n; ++i)
        matrix[i] = new bool [n];

    int k = 0;

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            matrix[i][j] = txt.at(k++).digitValue();
    //matrix is ready, add items to scene
    scene->insertNodes(n);
    //add links
    loadLinksFromBoolMatr(matrix, n);
}

bool MainWindow::saveToFile()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), QApplication::applicationDirPath(), tr("Text files (*.txt)"));
    if (filename.isEmpty())
        return false;
    int n = scene->nodesCount();

    bool ** matr = new bool * [n];
    for(uint i = 0; i < n; ++i)
        matr[i] = new bool[n];

    QList<QGraphicsItem *> allSceneItems = scene->items();
    foreach(QGraphicsItem *curItem, allSceneItems) {
        Node *curNode = qgraphicsitem_cast<Node *> (curItem);
        if(curNode != 0) {
            QList<Node *> cNodes = curNode->linkedNodes();
            int curI = curNode->getNumber() - 1; //current i for matrix
            foreach(Node * ccNode, cNodes) {
                int curJ = ccNode->getNumber() - 1;
                matr[curI][curJ] = true;
            }
        } //curNode != 0
    } //foreach allSceneItems

    QFile file(filename);
    file.open(QFile::WriteOnly | QFile::Text);
    if(!file.isOpen()) {
        QMessageBox::warning(0, tr("Error"), tr("Unable to open file for writing!"));
        return false;
    }
    QTextStream stream(&file);
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j) {
            stream << ((matr[i][j] == true) ? 1 : 0) << ' ';
        }
        stream << endl;
    }
    setWindowModified(false);
    return true;
}

void MainWindow::exportToMatrix(bool **matr, int n)
{
    int nn = scene->nodesCount();

    if(nn != n)
        return;

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            matr[i][j] = false;

    QList<QGraphicsItem *> allSceneItems = scene->items();
    foreach(QGraphicsItem *curItem, allSceneItems) {
        Node *curNode = qgraphicsitem_cast<Node *> (curItem);
        if(curNode != 0) {
            QList<Node *> cNodes = curNode->linkedNodes();
            int curI = curNode->getNumber() - 1; //current i for matrix
            foreach(Node * ccNode, cNodes) {
                int curJ = ccNode->getNumber() - 1;
                matr[curI][curJ] = true;
            }
        } //curNode != 0
    } //foreach allSceneItems
}

void MainWindow::buildReachMatrix()
{
    if(isGraphEmpty()) {
        QMessageBox::warning(0, tr("Error"), tr("The graph has no vertexes!!"));
        return;
    }
    int n = scene->nodesCount();
    bool ** mm = new bool * [n];
    for(int i = 0; i < n; ++i)
        mm[i] = new bool [n];
    exportToMatrix(mm, n);

    for(int k = 0; k < n; ++k)
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                mm[i][j] = mm[i][j] || (mm[i][k] && mm[k][j]);
    FormMatrixGrid * fmg = new FormMatrixGrid(mm, n, this);
    fmg->show();
}

QString MainWindow::findStrComp()
{

    int n = scene->nodesCount();
    bool ** mm = new bool * [n];
    for(int i = 0; i < n; ++i)
        mm[i] = new bool [n];
    exportToMatrix(mm, n);
    //building R
    for(int k = 0; k < n; ++k)
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                mm[i][j] = mm[i][j] || (mm[i][k] && mm[k][j]);
    //building Q*R
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            mm[i][j] = mm[i][j] && mm[j][i];

    int * sk = new int[n];
    for(int i = 0; i < n; ++i)
        sk[i] = 0;
    int nComp = 1;
    for(int i = 0; i < n; ++i) {
        if(!sk[i]) {
            for(int j = 0; j < n; ++j)
                if(mm[i][j])
                    sk[j] = nComp;
            ++nComp;
        }
    }

    QString strCompStr = "";
    for(int i = 0; i <= nComp; ++i) {
        QString tempStr = "";
        for(int j = 0; j < n; ++j)
            if(sk[j] == i) {
            tempStr.append(QString::number(j + 1) + ',');
            }
        if(!tempStr.isEmpty()) {
            tempStr.remove(tempStr.length() - 1, 1); // removing unnecessary coms
            strCompStr.append("{" + tempStr + "}");
        }
    }
    return strCompStr;
}

bool MainWindow::isGraphEmpty() const
{
    return !scene->nodesCount();
}

void MainWindow::checkRouteTwoNodes()
{
    TwoNodesDialog tnd(scene->nodesCount());
    if(tnd.exec() == QDialog::Accepted) {
        int k = tnd.getStart();
        int l = tnd.getEnd();

        int n = scene->nodesCount();
        bool ** mm = new bool * [n];
        for(int i = 0; i < n; ++i)
            mm[i] = new bool [n];
        exportToMatrix(mm, n);

        for(int k = 0; k < n; ++k)
            for(int i = 0; i < n; ++i)
                for(int j = 0; j < n; ++j)
                    mm[i][j] = mm[i][j] || (mm[i][k] && mm[k][j]);

        if(mm[k][l]) {
            QMessageBox::information(0, tr("Result"), tr("There is a way"));
        } else {
            QMessageBox::information(0, tr("Result"), tr("There are no ways"));
        }

    }
}

//--------------------------------------------------------------------

void MainWindow::makeActionGroup()
{
    ag = new QActionGroup(this);
    ag->addAction(ui->actionMove);
    ag->addAction(ui->actionAddVertexes);
    ag->addAction(ui->actionAddLines);
    ag->setExclusive(true);

    ui->actionMove->setChecked(true);

    connect(ag, SIGNAL(triggered(QAction*)), SLOT(changeMode(QAction*)));
}

void MainWindow::changeMode(QAction * act)
{
    if(!act)
        return;

    if(act->objectName().endsWith("move", Qt::CaseInsensitive)) {
        scene->setMode(GraphScene::MoveItem);
        ui->actionMove->setChecked(true);
    } else
        if (act->objectName().endsWith("vertexes", Qt::CaseInsensitive)) {
        scene->setMode(GraphScene::InsertNode);
        ui->actionAddVertexes->setChecked(true);
    } else
        if (act->objectName().endsWith("lines", Qt::CaseInsensitive)) {
        scene->setMode(GraphScene::InsertLine);
        ui->actionAddLines->setChecked(true);
    } else {
        qDebug() << "Unknown action";
        return;
        }
}

void MainWindow::checkDelete()
{
    QList<QGraphicsItem *> allSceneItems = scene->selectedItems();
    if (allSceneItems.isEmpty())
        ui->actionDelete->setEnabled(false);
    else
        ui->actionDelete->setEnabled(true);
}

void MainWindow::showAdjMatrix()
{
    int n = scene->nodesCount();

    bool ** matrix = new bool * [n];
    for(int i = 0; i < n; ++i)
        matrix[i] = new bool [n];
    exportToMatrix(matrix, n);

    AdjMatrDialog amd(matrix, n);
    if (amd.exec() == QDialog::Accepted) {
        AdjMatrixModel *newModel = amd.getModel();
        int new_n = newModel->dimension();
        if (new_n != n) {
            //new graph will be made
            scene->clearScene();

            for(int i = 0; i < n; ++i)
                delete[] matrix[i];
            delete[] matrix;

            bool ** matrix = new bool * [new_n];
            for(int i = 0; i < new_n; ++i)
                matrix[i] = new bool [new_n];

            newModel->toBoolMatrix(matrix);

            scene->insertNodes(new_n);

            loadLinksFromBoolMatr(matrix, new_n);
        } else {
            //modifying existing matrix

            scene->deleteAllLinks();

            newModel->toBoolMatrix(matrix);

            loadLinksFromBoolMatr(matrix, n);
        }

    }

}

void MainWindow::loadLinksFromBoolMatr(bool **matrix, int n)
{
    QList<Node*> ls = scene->getAllNodes();

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j) {
            if(i == j)
                continue;
            if(matrix[i][j] == true)
            {
                Node* a = findNode(ls, i);
                Node* b = findNode(ls, j);
                Link *l = new Link(a, b);
                a->addLink(l);
                b->addLink(l);
                l->setZValue(-1000.0);
                scene->addItem(l);
                l->updatePosition();
            }
        }
}

void MainWindow::newGraph()
{
    if (okToContinue()) {
        scene->clearScene();
        setWindowModified(false);
    }
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Graph"),
                        tr("The graph was changed.\n"
                           "Do you want to save it?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return saveToFile();
        } else if (r == QMessageBox::Cancel) {
            setWindowModified(false);
            return false;
        }
    }
    return true;
}

void MainWindow::setWModifTrue()
{
    setWindowModified(true);
}

void MainWindow::setWModifFalse()
{
    setWindowModified(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
