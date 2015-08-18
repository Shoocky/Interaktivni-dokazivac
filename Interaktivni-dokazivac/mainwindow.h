#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <treescene.h>
#include <node.h>
#include <vector>
#include "fol.hpp"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void buttonClicked();
    void ponistiClicked();
    void andIClicked();
    void orI1Clicked();
    void orI2Clicked();
    void andE1Clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsView *view;
    TreeScene *scene;
    int depth;
    std::vector<QGraphicsItem *> items;
};

#endif // MAINWINDOW_H
