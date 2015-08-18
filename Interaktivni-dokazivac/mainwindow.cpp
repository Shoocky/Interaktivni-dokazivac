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


        if(parsed_formula.get() != 0){
               ((Or*)parsed_formula.get())->getOperand2()->printFormula(stream);
                qDebug() << "ovo je nesto: " << QString::fromStdString(stream.str());
        }
        else {
            qDebug() << " pa dobro";
        }

        qreal rect_width = (ui->lineEdit->text().length()*10 -5)*2;
        qreal rect_height = 20;
        int rect_x = 85;
        int rect_y = 180;
        Node* item = new Node(ui->lineEdit->text(), parsed_formula, rect_width, rect_height, rect_x, rect_y);
        scene->addNode(item);
        //scene->addItem(item);
        //items.push_back(item);
    }
    else{

        QList<QGraphicsItem *> selected_list = scene->selectedItems();

        Node* selected = (Node*)(selected_list.at(0));
        qreal rect_width =  (selected->getText().length()*10 - 5)*2;
        qreal rect_height = 20;
        int rect_x = selected->getx() - 20 - depth/2;
        int rect_y = selected->gety() - 20 - depth/2;
        Node* item = new Node( selected->getText(), parsed_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
        scene->addNode(item);
        //scene->addItem(item);
        //items.push_back(item);

        rect_x = selected->getx() + 25  + depth/2;
        rect_y = selected->gety() - 20 - depth/2;
        Node* item1 = new Node( selected->getText(), parsed_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
        scene->addNode(item1);
        //scene->addItem(item1);
        //items.push_back(item1);
    }
    depth += 15;
}

void MainWindow::ponistiClicked()
{
    QList<QGraphicsItem*> selected_list = scene->selectedItems();
    if(selected_list.isEmpty()){
        return;
    }
    //scene->removeItem(selected_list.at(0));
    scene->removeNode(selected_list.at(0));
    /*
    scene->removeItem(items[items.size()-1]);
    items.pop_back();
    */
    scene->update();
}

void MainWindow::andIClicked()
{

        qDebug() << "AndI " ;
        QList<QGraphicsItem *> selected_list = scene->selectedItems();

        /*
        std::ostringstream stream;
        ((And*)parsed_formula.get())->getOperand1()->printFormula(stream);
         QString tekst = QString::fromStdString(stream.str());

*/
        Node* selected = (Node*)(selected_list.at(0));
        qDebug() << selected->getText();

        std::ostringstream stream;
        qDebug() << "lalal";
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

            Node* item = new Node( selected->getText(), ((And*)(selected->getFormula().get()))->getOperand1(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
            scene->addNode(item);
            //scene->addItem(item);
            //items.push_back(item);
            /*

        ((And*)parsed_formula.get())->getOperand2()->printFormula(stream);
        tekst = QString::fromStdString(stream.str());*/
            rect_x = selected->getx() + 25  + depth/2;
            rect_y = selected->gety() - 20 - depth/2;
            Node* item1 = new Node( selected->getText(),  ((And*)(selected->getFormula().get()))->getOperand2(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
            scene->addNode(item1);

        }

}

void MainWindow::orI1Clicked()
{
    if(parsed_formula->getType() != BaseFormula::T_OR){

        QMessageBox msgBox;
        msgBox.setText("Ne moze se primeniti pravilo orI1 ");
        msgBox.exec();
    }
    else{

        QList<QGraphicsItem *> selected_list = scene->selectedItems();

        std::ostringstream stream;
        ((Or*)parsed_formula.get())->getOperand1()->printFormula(stream);
         QString tekst = QString::fromStdString(stream.str());


        Node* selected = (Node*)(selected_list.at(0));
        qreal rect_width =  (selected->getText().length()*10 - 5)*2;
        qreal rect_height = 20;
        int rect_x = selected->getx() - 20 - depth/2;
        int rect_y = selected->gety() - 20 - depth/2;
        Node* item = new Node( tekst, ((Or*)parsed_formula.get())->getOperand1(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
        scene->addNode(item);
        //scene->addItem(item);
        //items.push_back(item);



    }

}


void MainWindow::orI2Clicked()
{
    if(parsed_formula->getType() != BaseFormula::T_OR){

        QMessageBox msgBox;
        msgBox.setText("Ne moze se primeniti pravilo andI ");
        msgBox.exec();
    }
    else{

        QList<QGraphicsItem *> selected_list = scene->selectedItems();

        std::ostringstream stream;

        ((Or*)parsed_formula.get())->getOperand2()->printFormula(stream);
        QString tekst = QString::fromStdString(stream.str());
        Node* selected = (Node*)(selected_list.at(0));
        qreal rect_width =  selected->getText().length()*10 - 5;
        qreal rect_height = 20;

        int rect_x = selected->getx() + 25  + depth/2;
        int rect_y = selected->gety() - 20 - depth/2;
        Node* item1 = new Node( tekst, ((Or*)parsed_formula.get())->getOperand2(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
        scene->addNode(item1);

    }

}

void MainWindow::andE1Clicked(){



    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    QString tekst = selected->getText();
    qDebug() << "lalalalalla";
    std::string formula = tekst.toUtf8().constData();
    formula += ";";
    std::ostringstream stream;
    std::string tmp;
    qDebug() << QString::fromStdString(formula);
   YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());
            if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }


    if(parsed_formula.get() != 0){
           ((Or*)parsed_formula.get())->getOperand2()->printFormula(stream);
            qDebug() << "ovo je nesto: " << QString::fromStdString(stream.str());
    }
    else {
        qDebug() << " pa dobro";
    }
     Formula f1 = parsed_formula;


    qDebug() << "lalalalalla";
   formula = ui->constLineEdit->text().toUtf8().constData();
    formula += ";";
    qDebug() << QString::fromStdString(formula);
    buffer = yy_scan_string(formula.c_str());
            if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }


    if(parsed_formula.get() != 0){
           qDebug() << "ne znam sta";
    }
    else {
        qDebug() << " pa dobro";
    }

    Formula new_formula = Formula(new And(f1, parsed_formula));

    new_formula->printFormula(stream);
   tekst = QString::fromStdString(stream.str());
    selected = (Node*)(selected_list.at(0));
    qreal rect_width =  (selected->getText().length()*10 - 5)*3;
    qreal rect_height = 20;

    int rect_x = selected->getx() + 25  + depth/2;
    int rect_y = selected->gety() - 20 - depth/2;
    Node* item1 = new Node( tekst, new_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0));
    scene->addNode(item1);
}
