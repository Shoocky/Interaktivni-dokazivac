#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <qdebug.h>
#include <sstream>
#include <QMessageBox>


typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern void yy_switch_to_buffer (YY_BUFFER_STATE new_buffer  );
extern Formula parsed_formula;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    scene = new TreeScene(this);
    depth = 0;


    QHBoxLayout * layout = new QHBoxLayout;
    QVBoxLayout * btn_layout = new QVBoxLayout;
    ui->dodajDete->setFixedSize(50,30);
    btn_layout->addWidget(ui->lineEdit);
    btn_layout->addWidget(ui->dodajDete);
    btn_layout->addWidget(ui->andI);
    btn_layout->addWidget(ui->andE1);
    btn_layout->addWidget(ui->andE2);
    btn_layout->addWidget(ui->orI1);
    btn_layout->addWidget(ui->orI2);
    btn_layout->addWidget(ui->orE);
    btn_layout->addWidget(ui->notI);
    btn_layout->addWidget(ui->notE);
    btn_layout->addWidget(ui->impI);
    btn_layout->addWidget(ui->impE);
    btn_layout->addWidget(ui->constLineEdit);

    btn_layout->addWidget(ui->ponisti);
    layout->addLayout(btn_layout);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    connect(ui->dodajDete, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(ui->ponisti, SIGNAL(clicked()), this, SLOT(ponistiClicked()));
    connect(ui->andI, SIGNAL(clicked()), this, SLOT(andIClicked()));
    connect(ui->orI1, SIGNAL(clicked()), this, SLOT(orI1Clicked()));
    connect(ui->orI2, SIGNAL(clicked()), this, SLOT(orI2Clicked()));
    connect(ui->andE1, SIGNAL(clicked()), this, SLOT(andE1Clicked()));
    connect(ui->andE2, SIGNAL(clicked()), this, SLOT(andE2Clicked()));
    connect(ui->impI, SIGNAL(clicked()), this, SLOT(impIClicked()));
    connect(ui->impE, SIGNAL(clicked()), this, SLOT(impEClicked()));
    connect(ui->notE, SIGNAL(clicked()), this, SLOT(notEClicked()));
    connect(ui->notI, SIGNAL(clicked()), this, SLOT(notIClicked()));



    connect(scene, SIGNAL(selectionChanged()), this, SLOT(selectedItemChanged()));

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle("nase drvce");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonClicked()
{
    if(depth == 0 ){
        qDebug() << "lalalalalla";
        std::string formula = ui->lineEdit->text().toUtf8().constData();
        formula += " ;";
        std::ostringstream stream;
        std::string tmp;
        qDebug() << QString::fromStdString(formula);
       YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

                if(yyparse() == 1){
            qDebug() << "Pa to ti ne radi";
        }

/*
        if(parsed_formula.get() != 0){
               ((Or*)parsed_formula.get())->getOperand2()->printFormula(stream);
                qDebug() << "ovo je nesto: " << QString::fromStdString(stream.str());
        }
        else {
            qDebug() << " pa dobro";
        }
*/       qreal rect_width = (ui->lineEdit->text().length()*10 -5)*2;
        qreal rect_height = 20;
        int rect_x = 85;
        int rect_y = 180;

        Node* item = new Node( parsed_formula, rect_width, rect_height, rect_x, rect_y);
        scene->addNode(item);
    }
    else{

        QList<QGraphicsItem *> selected_list = scene->selectedItems();

        Node* selected = (Node*)(selected_list.at(0));
        qreal rect_width =  (selected->getText().length()*10 - 5)*2;
        qreal rect_height = 20;
        int rect_x = selected->getx() - 20 - depth/2;
        int rect_y = selected->gety() - 20 - depth/2;

        Node* item = new Node( selected->getFormula(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
        scene->addNode(item);

        rect_x = selected->getx() + 25  + depth/2;
        rect_y = selected->gety() - 20 - depth/2;
        Node* item1 = new Node( selected->getFormula(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
        scene->addNode(item1);

    }
    depth += 15;
}

void MainWindow::ponistiClicked()
{
    QList<QGraphicsItem*> selected_list = scene->selectedItems();

    if(selected_list.isEmpty()){
        return;
    }

    scene->removeNode(selected_list.at(0));

    scene->update();
}

void MainWindow::andIClicked()
{

        QList<QGraphicsItem *> selected_list = scene->selectedItems();
        Node* selected = (Node*)(selected_list.at(0));

        if(selected->getFormula()->getType() != BaseFormula::T_AND){

            QMessageBox msgBox;
            msgBox.setText("Ne moze se primeniti pravilo andI ");
            msgBox.exec();
        }
        else{
            qreal rect_width =  (selected->getText().length()*10 - 5)*2;
            qreal rect_height = 20;
            int rect_x = selected->getx() - 20 - depth/2;
            int rect_y = selected->gety() - 20 - depth/2;

            Node* item = new Node( ((And*)(selected->getFormula().get()))->getOperand1(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
            scene->addNode(item);

            rect_x = selected->getx() + 25  + depth/2;
            rect_y = selected->gety() - 20 - depth/2;

            Node* item1 = new Node( ((And*)(selected->getFormula().get()))->getOperand2(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
            scene->addNode(item1);

        }

}

void MainWindow::orI1Clicked()
{


    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    if(selected->getFormula()->getType() != BaseFormula::T_OR){

        QMessageBox msgBox;
        msgBox.setText("Ne moze se primeniti pravilo andI ");
        msgBox.exec();
    }

    else{

        qreal rect_width =  (selected->getText().length()*10 - 5)*2;
        qreal rect_height = 20;
        int rect_x = selected->getx() - 20 - depth/2;
        int rect_y = selected->gety() - 20 - depth/2;
        Node* item = new Node(((Or*)(selected->getFormula().get()))->getOperand1(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
        scene->addNode(item);

    }

}


void MainWindow::orI2Clicked()
{
    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    if(selected->getFormula()->getType() != BaseFormula::T_OR){

        QMessageBox msgBox;
        msgBox.setText("Ne moze se primeniti pravilo andI ");
        msgBox.exec();
    }

    else{

        qreal rect_width =  (selected->getText().length()*10 - 5)*2;
        qreal rect_height = 20;
        int rect_x = selected->getx() - 20 - depth/2;
        int rect_y = selected->gety() - 20 - depth/2;
        Node* item = new Node(((Or*)(selected->getFormula().get()))->getOperand2(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
        scene->addNode(item);

    }

}

void MainWindow::andE1Clicked(){



    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    Formula f1 = selected->getFormula();

    std::string formula = ui->constLineEdit->text().toUtf8().constData();
    formula += " ;";
    std::ostringstream stream;
    QString tekst;
    qDebug() << QString::fromStdString(formula);
    YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

            if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }

    Formula new_formula = Formula(new And(f1, parsed_formula));

    new_formula->printFormula(stream);
    tekst = QString::fromStdString(stream.str());
    qreal rect_width =  (tekst.length()*10 - 5)*3;
    qreal rect_height = 20;

    int rect_x = selected->getx() + 25  + depth/2;
    int rect_y = selected->gety() - 20 - depth/2;
    Node* item1 = new Node( new_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
    scene->addNode(item1);
}

void MainWindow::andE2Clicked(){



    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    Formula f1 = selected->getFormula();

    std::string formula = ui->constLineEdit->text().toUtf8().constData();
    formula += " ;";
    std::ostringstream stream;
    QString tekst;
    qDebug() << QString::fromStdString(formula);
    YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

            if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }

    Formula new_formula = Formula(new And(parsed_formula, f1));

    new_formula->printFormula(stream);
    tekst = QString::fromStdString(stream.str());
    qreal rect_width =  (tekst.length()*10 - 5)*3;
    qreal rect_height = 20;

    int rect_x = selected->getx() + 25  + depth/2;
    int rect_y = selected->gety() - 20 - depth/2;
    Node* item1 = new Node( new_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
    scene->addNode(item1);
}

void MainWindow::impIClicked()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));
    Formula f = selected->getFormula();
    Formula op1 = ((Imp*)f.get())->getOperand1();
    Formula op2 = ((Imp*)f.get())->getOperand2();

    m_pretpostavke.push_back(op1);
    qreal rect_width =  (selected->getText().length()*10 - 5)*3;
    qreal rect_height = 20;

    int rect_x = selected->getx() + 25  + depth/2;
    int rect_y = selected->gety() - 20 - depth/2;
    Node* item = new Node( op2, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
    scene->addNode(item);

}

void MainWindow::impEClicked()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    Formula f1 = selected->getFormula();

    std::string formula = ui->constLineEdit->text().toUtf8().constData();
    formula += " ;";
    std::ostringstream stream;
    QString tekst;
    qDebug() << QString::fromStdString(formula);
    YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

            if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }

    qreal rect_width =  (selected->getText().length()*10 - 5)*3;
    qreal rect_height = 20;

    int rect_x = selected->getx() -20  - depth/2;
    int rect_y = selected->gety() - 20 - depth/2;
    Node* item1 = new Node( parsed_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
    scene->addNode(item1);

    Formula f2 = Formula(new Imp(parsed_formula, f1));

    rect_x = selected->getx() + 25  + depth/2;
    rect_y = selected->gety() - 20 - depth/2;
    Node* item2 = new Node( f2, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
    scene->addNode(item2);

}

void MainWindow::notEClicked()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    std::string formula = ui->constLineEdit->text().toUtf8().constData();
    formula += " ;";
    std::ostringstream stream;
    QString tekst;
    qDebug() << QString::fromStdString(formula);
    YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

            if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }
    qreal rect_width =  (selected->getText().length()*10 - 5)*3;
    qreal rect_height = 20;

    int rect_x = selected->getx() -20  - depth/2;
    int rect_y = selected->gety() - 20 - depth/2;
    Node* item1 = new Node( parsed_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
    scene->addNode(item1);

    Formula f2 = Formula(new Not(parsed_formula));

    rect_x = selected->getx() + 25  + depth/2;
    rect_y = selected->gety() - 20 - depth/2;
    Node* item2 = new Node( f2, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
    scene->addNode(item2);

}

void MainWindow::notIClicked()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));
    m_pretpostavke.push_back(((Not*)selected->getFormula().get())->getOperand());

    Formula false_f = Formula(new False());

    qreal rect_width =  (selected->getText().length()*10 - 5)*3;
    qreal rect_height = 20;

    int rect_x = selected->getx() -20  - depth/2;
    int rect_y = selected->gety() - 20 - depth/2;

    Node* item = new Node( false_f, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
    scene->addNode(item);
}



void MainWindow::selectedItemChanged()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));
    if(selected->getFormula()->getType() == BaseFormula::T_AND){
        ui->orI1->setDisabled(true);
        ui->orI2->setDisabled(true);
        ui->impI->setDisabled(true);
        ui->impE->setDisabled(false);
        ui->orE->setDisabled(false);
        ui->notI->setDisabled(true);
        ui->notE->setDisabled(true);
        ui->andE1->setDisabled(false);
        ui->andE2->setDisabled(false);
        ui->andI->setDisabled(false);

    }
    else if(selected->getFormula()->getType() == BaseFormula::T_OR){
        ui->andE1->setDisabled(false);
        ui->andE2->setDisabled(false);
        ui->impI->setDisabled(true);
        ui->impE->setDisabled(false);
        ui->andI->setDisabled(true);
        ui->notI->setDisabled(true);
        ui->notE->setDisabled(false);
        ui->orE->setDisabled(false);
        ui->orI1->setDisabled(false);
        ui->orI2->setDisabled(false);
    }
    else if(selected->getFormula()->getType() == BaseFormula::T_IMP){
        ui->andE1->setDisabled(false);
        ui->andE2->setDisabled(false);
        ui->orI1->setDisabled(true);
        ui->orI2->setDisabled(true);
        ui->andI->setDisabled(true);
        ui->orE->setDisabled(false);
        ui->notI->setDisabled(true);
        ui->notE->setDisabled(true);
        ui->impE->setDisabled(false);
        ui->impI->setDisabled(false);
    }
    else if(selected->getFormula()->getType() == BaseFormula::T_NOT){

        ui->andE1->setDisabled(true);
        ui->andE2->setDisabled(true);
        ui->orI1->setDisabled(true);
        ui->orI2->setDisabled(true);
        ui->andI->setDisabled(true);
        ui->orE->setDisabled(true);
        ui->impE->setDisabled(true);
        ui->impI->setDisabled(true);
        ui->notE->setDisabled(true);
        ui->notI->setDisabled(false);
    }
    else if(selected->getFormula()->getType() == BaseFormula::T_FALSE){

        ui->andE1->setDisabled(true);
        ui->andE2->setDisabled(true);
        ui->orI1->setDisabled(true);
        ui->orI2->setDisabled(true);
        ui->andI->setDisabled(true);
        ui->orE->setDisabled(true);
        ui->impE->setDisabled(true);
        ui->impI->setDisabled(true);
        ui->notI->setDisabled(true);
        ui->notE->setDisabled(false);
    }
}