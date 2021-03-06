#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QSpacerItem>
#include <qdebug.h>
#include <sstream>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QErrorMessage>

#define PARAMETER 14

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

    QHBoxLayout * input_layout = new QHBoxLayout;
    input_layout->addWidget(ui->lineEdit);
    ui->dodajDete->setFixedSize(70,30);
    input_layout->addWidget(ui->dodajDete);
    QPushButton* choose_file = new QPushButton;
    choose_file->setText("Učitaj");
    choose_file->setFixedSize(80,30);
    input_layout->addWidget(choose_file);

    QObject::connect(choose_file, SIGNAL(clicked(bool)), this, SLOT(chooseFileClicked()));
    QObject::connect(choose_file, SIGNAL(clicked(bool)), choose_file, SLOT(setDisabled(bool)));

    QVBoxLayout * btn_layout = new QVBoxLayout;
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
    btn_layout->addWidget(ui->falseE);
    btn_layout->addWidget(ui->trueI);
    btn_layout->addWidget(ui->ponisti);
    layout->addLayout(btn_layout);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(input_layout);
    main_layout->addLayout(layout);

    connect(ui->dodajDete, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(ui->ponisti, SIGNAL(clicked()), this, SLOT(ponistiClicked()));
    connect(ui->andI, SIGNAL(clicked()), this, SLOT(andIClicked()));
    connect(ui->orI1, SIGNAL(clicked()), this, SLOT(orI1Clicked()));
    connect(ui->orI2, SIGNAL(clicked()), this, SLOT(orI2Clicked()));
    connect(ui->orE, SIGNAL(clicked()), this, SLOT(orEClicked()));
    connect(ui->andE1, SIGNAL(clicked()), this, SLOT(andE1Clicked()));
    connect(ui->andE2, SIGNAL(clicked()), this, SLOT(andE2Clicked()));
    connect(ui->impI, SIGNAL(clicked()), this, SLOT(impIClicked()));
    connect(ui->impE, SIGNAL(clicked()), this, SLOT(impEClicked()));
    connect(ui->notE, SIGNAL(clicked()), this, SLOT(notEClicked()));
    connect(ui->notI, SIGNAL(clicked()), this, SLOT(notIClicked()));
    connect(ui->falseE, SIGNAL(clicked()), this, SLOT(falseEClicked()));
    connect(ui->trueI, SIGNAL(clicked()), this, SLOT(trueIClicked()));

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(selectedItemChanged()));

    QWidget *widget = new QWidget;
    widget->setLayout(main_layout);

    setCentralWidget(widget);
    setWindowTitle("Interaktivni dokazivač u prirodnoj dedukciji");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonClicked()
{
    ui->dodajDete->setDisabled(true);
    ui->lineEdit->setDisabled(true);
    if(depth == 0 ){
        std::string formula = ui->lineEdit->text().toUtf8().constData();
        formula += " ;";
        std::ostringstream stream;
        qDebug() << QString::fromStdString(formula);
       YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

                if(yyparse() == 1){
            qDebug() << "Pa to ti ne radi";
        }

        qreal rect_width = formula.length()*PARAMETER;
        qreal rect_height = 20;
        int rect_x = 85;
        int rect_y = 180;
        std::vector<Formula> tmp_vec = std::vector<Formula>();
        parsed_formula->getAtoms(tmp_vec);
        QVector<Formula> assumptions = QVector<Formula>::fromStdVector(tmp_vec);
        Node* item = new Node( parsed_formula, rect_width, rect_height, rect_x, rect_y, nullptr, assumptions);
        scene->addNode(item);
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
        selected->setRule("andI");
        selected->update();
        std::ostringstream stream;
        ((And*)(selected->getFormula().get()))->getOperand1()->printFormula(stream);
        qreal rect_width =  stream.str().length()*PARAMETER;
        qreal rect_height = 20;
        int rect_x = selected->getx() - 20 - depth/2;
        int rect_y = selected->gety() - 20 - depth/2;

        QVector<Formula> assumptions = selected->getAssumptions();


        Node* item = new Node( ((And*)(selected->getFormula().get()))->getOperand1(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
        scene->addNode(item);

        std::ostringstream stream1;
        ((And*)(selected->getFormula().get()))->getOperand2()->printFormula(stream1);
        rect_width =  stream1.str().length()*PARAMETER;
        rect_x = selected->getx() + 25  + depth/2;
        rect_y = selected->gety() - 20 - depth/2;

        Node* item1 = new Node( ((And*)(selected->getFormula().get()))->getOperand2(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
        scene->addNode(item1);


}

void MainWindow::orI1Clicked()
{


    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));
    selected->setRule("orI1");
    selected->update();
    std::ostringstream stream;
    ((Or*)(selected->getFormula().get()))->getOperand1()->printFormula(stream);
    qreal rect_width =  stream.str().length()*PARAMETER;
    qreal rect_height = 20;
    int rect_x = selected->getx();

    int rect_y = selected->gety() - 20 - depth/2;
    QVector<Formula> assumptions = selected->getAssumptions();

    Node* item = new Node(((Or*)(selected->getFormula().get()))->getOperand1(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item);



}


void MainWindow::orI2Clicked()
{
    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    selected->setRule("orI2");
    selected->update();

    std::ostringstream stream;
    ((Or*)(selected->getFormula().get()))->getOperand2()->printFormula(stream);
    qreal rect_width =  stream.str().length()*PARAMETER;
    qreal rect_height = 20;
    int rect_x = selected->getx() ;
    int rect_y = selected->gety() - 20 - depth/2;
    QVector<Formula> assumptions = selected->getAssumptions();
    Node* item = new Node(((Or*)(selected->getFormula().get()))->getOperand2(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item);



}

void MainWindow::orEClicked()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));
    selected->setRule("orE");
    selected->update();
    int rect_x;
    int rect_y;

    bool ok;
    while(1){
             QString tekst = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Unesite formulu:"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
             if (ok && !tekst.isEmpty()){
                qDebug() << "radi";
             }


            std::string formula = tekst.toUtf8().constData();
            formula += " ;";
            std::ostringstream stream;
            qDebug() << QString::fromStdString(formula);
            YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

            if(yyparse() == 1){
                qDebug() << "Pa to ti ne radi";
             }

            if(parsed_formula->getType() == BaseFormula::T_OR){
                break;
            }
    }

    std::ostringstream stream;
    parsed_formula->printFormula(stream);
    qreal rect_width =  stream.str().length()*PARAMETER;
    qreal rect_height = 20;

    rect_x = selected->getx() - 20 - depth/2;
    rect_y = selected->gety() - 20 - depth/2;

    QVector<Formula> assumptions = selected->getAssumptions();
    assumptions.push_back(parsed_formula);

    Node* item3 = new Node( parsed_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item3);

    Formula op1 = ((Or*)parsed_formula.get())->getOperand1();
    Formula op2 = ((Or*)parsed_formula.get())->getOperand2();

    rect_x = selected->getx() + 25 + depth/2;
    rect_y = selected->gety() - 20 - depth/2;


    assumptions.push_back(op1);
    assumptions.push_back(op2);
    op1->printFormula(stream);
    rect_width = stream.str().length()*PARAMETER;
    Node* item1 = new Node( selected->getFormula(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item1);

    rect_x = selected->getx() + 50 + depth/2;
    rect_y = selected->gety() - 20 - depth/2;

    op2->printFormula(stream);
    rect_width = stream.str().length()*PARAMETER;
    Node* item2 = new Node(selected->getFormula(), rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item2);



}

void MainWindow::andE1Clicked(){

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));
    selected->setRule("andE1");
    selected->update();
    Formula f1 = selected->getFormula();

    bool ok;
    QString tekst = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Unesite formulu:"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
    if (ok && !tekst.isEmpty()){
            qDebug() << "radi";
    }


    std::string formula = tekst.toUtf8().constData();
    formula += " ;";
    std::ostringstream stream;
    qDebug() << QString::fromStdString(formula);
    YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

    if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }

    Formula new_formula = Formula(new And(f1, parsed_formula));

    new_formula->printFormula(stream);
    tekst = QString::fromStdString(stream.str());
    qreal rect_width =  tekst.length()*PARAMETER;
    qreal rect_height = 20;

    int rect_x = selected->getx();
    int rect_y = selected->gety() - 20 - depth/2;
    QVector<Formula> assumptions = selected->getAssumptions();

    Node* item1 = new Node( new_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item1);
}

void MainWindow::andE2Clicked(){



    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));
    selected->setRule("andE2");
    selected->update();
    Formula f1 = selected->getFormula();
    bool ok;
    QString tekst = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Unesite formulu:"), QLineEdit::Normal,
                                             "", &ok);
    if (ok && !tekst.isEmpty()){
            qDebug() << "radi";
    }


    std::string formula = tekst.toUtf8().constData();
    formula += " ;";
    std::ostringstream stream;
    qDebug() << QString::fromStdString(formula);
    YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

            if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }

    Formula new_formula = Formula(new And(parsed_formula, f1));

    new_formula->printFormula(stream);
    tekst = QString::fromStdString(stream.str());
    qreal rect_width =  tekst.length()*PARAMETER;
    qreal rect_height = 20;

    int rect_x = selected->getx();
    int rect_y = selected->gety() - 20 - depth/2;
    QVector<Formula> assumptions = selected->getAssumptions();

    Node* item1 = new Node( new_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item1);
}

void MainWindow::impIClicked()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    selected->setRule("impI");
    selected->update();

    Formula f = selected->getFormula();
    Formula op1 = ((Imp*)f.get())->getOperand1();
    Formula op2 = ((Imp*)f.get())->getOperand2();


    std::ostringstream stream;
    op2->printFormula(stream);
    qreal rect_width =  stream.str().length()*PARAMETER;
    qreal rect_height = 20;

    int rect_x = selected->getx() ;
    int rect_y = selected->gety() - 20 - depth/2;

    QVector<Formula> assumptions = selected->getAssumptions();
    assumptions.push_back(op1);
    Node* item = new Node( op2, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item);

}

void MainWindow::impEClicked()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    selected->setRule("impE");
    selected->update();

    Formula f1 = selected->getFormula();
    bool ok;
    QString tekst = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Unesite formulu:"), QLineEdit::Normal,
                                             "", &ok);
    if (ok && !tekst.isEmpty()){
            qDebug() << "radi";
    }


    std::string formula = tekst.toUtf8().constData();
    formula += " ;";
    std::ostringstream stream;
    qDebug() << QString::fromStdString(formula);
    YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

            if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }
    std::ostringstream stream1;
    parsed_formula->printFormula(stream1);
    qreal rect_width =  stream1.str().length()*PARAMETER;

    qreal rect_height = 20;

    int rect_x = selected->getx() -20  - depth/2;
    int rect_y = selected->gety() - 20 - depth/2;

    QVector<Formula> assumptions = selected->getAssumptions();
    Node* item1 = new Node( parsed_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item1);

    Formula f2 = Formula(new Imp(parsed_formula, f1));
    f2->printFormula(stream1);
    rect_width = stream1.str().length()*PARAMETER;
    rect_x = selected->getx() + 25  + depth/2;
    rect_y = selected->gety() - 20 - depth/2;
    Node* item2 = new Node( f2, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item2);

}

void MainWindow::notEClicked()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    selected->setRule("notE");
    selected->update();

    bool ok;
    QString tekst = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Unesite formulu:"), QLineEdit::Normal,
                                             "", &ok);
    if (ok && !tekst.isEmpty()){
            qDebug() << "radi";
    }


    std::string formula = tekst.toUtf8().constData();
    formula += " ;";
    std::ostringstream stream;
    qDebug() << QString::fromStdString(formula);
    YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

            if(yyparse() == 1){
        qDebug() << "Pa to ti ne radi";
    }


    std::ostringstream stream1;
    parsed_formula->printFormula(stream1);
    qreal rect_width =  stream1.str().length()*PARAMETER;
    qreal rect_height = 20;

    int rect_x = selected->getx() -20  - depth/2;
    int rect_y = selected->gety() - 20 - depth/2;
    QVector<Formula> assumptions = selected->getAssumptions();
    Node* item1 = new Node( parsed_formula, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item1);

    Formula f2 = Formula(new Not(parsed_formula));

    rect_x = selected->getx() + 25  + depth/2;
    rect_y = selected->gety() - 20 - depth/2;
    f2->printFormula(stream1);
    rect_width = stream1.str().length()*PARAMETER;
    Node* item2 = new Node( f2, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item2);

}

void MainWindow::notIClicked()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    selected->setRule("notI");
    selected->update();

    Formula false_f = Formula(new False());
    std::ostringstream stream;
    false_f->printFormula(stream);
    qreal rect_width =  stream.str().length()*PARAMETER;
    qreal rect_height = 20;
    int rect_x = selected->getx();
    int rect_y = selected->gety() - 20;
    QVector<Formula> assumptions = selected->getAssumptions();
    assumptions.push_back(((Not*)selected->getFormula().get())->getOperand());
    Node* item = new Node( false_f, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item);
}

void MainWindow::falseEClicked()
{
    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    Node* selected = (Node*)(selected_list.at(0));

    selected->setRule("false");
    selected->update();

    Formula false_f = Formula(new False());
    std::ostringstream stream;
    false_f->printFormula(stream);
    qreal rect_width =  stream.str().length()*PARAMETER;
    qreal rect_height = 20;
    int rect_x = selected->getx();
    int rect_y = selected->gety() - 20 - depth/2;
    QVector<Formula> assumptions = selected->getAssumptions();
    Node* item = new Node( false_f, rect_width, rect_height, rect_x, rect_y, selected_list.at(0), assumptions);
    scene->addNode(item);
}

void MainWindow::trueIClicked()
{


}



void MainWindow::selectedItemChanged()
{

    QList<QGraphicsItem *> selected_list = scene->selectedItems();
    if(selected_list.empty()){
        return;
    }
    Node* selected = (Node*)(selected_list.at(0));


    qDebug() <<  selected->getFormula()->formulaDepth();
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
        ui->trueI->setDisabled(true);
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
        ui->trueI->setDisabled(true);
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
        ui->trueI->setDisabled(true);
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
        ui->trueI->setDisabled(true);
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
        ui->trueI->setDisabled(true);
    }
    else if(selected->getFormula()->getType() == BaseFormula::T_ATOM){

        ui->andE1->setDisabled(false);
        ui->andE2->setDisabled(false);
        ui->orI1->setDisabled(true);
        ui->orI2->setDisabled(true);
        ui->andI->setDisabled(true);
        ui->orE->setDisabled(false);
        ui->impE->setDisabled(false);
        ui->impI->setDisabled(true);
        ui->notI->setDisabled(true);
        ui->notE->setDisabled(true);
        ui->trueI->setDisabled(true);
    }
    else if(selected->getFormula()->getType() == BaseFormula::T_TRUE){

        ui->andE1->setDisabled(true);
        ui->andE2->setDisabled(true);
        ui->orI1->setDisabled(true);
        ui->orI2->setDisabled(true);
        ui->andI->setDisabled(true);
        ui->orE->setDisabled(true);
        ui->impE->setDisabled(true);
        ui->impI->setDisabled(true);
        ui->notI->setDisabled(true);
        ui->notE->setDisabled(true);
        ui->falseE->setDisabled(true);
        ui->trueI->setDisabled(false);
    }
}

void MainWindow::chooseFileClicked()
{
    ui->dodajDete->setDisabled(true);
    ui->lineEdit->setDisabled(true);

    std::ostringstream stream;
    if(depth == 0 ){
        QString file_name = QFileDialog::getOpenFileName(this, tr("Učitaj iz fajla"), QDir::currentPath(), tr("*.for"));
        qDebug() << file_name;
        QFile f(file_name);
            if (!f.open(QFile::ReadOnly | QFile::Text)){
                QMessageBox msgBox;
                 msgBox.setWindowTitle("Error!");
                 msgBox.setText("Can`t open file");
                 msgBox.setDetailedText("Unable to open file for reading");
                 msgBox.exec();
            }
            QTextStream in(&f);
            stream << in.readAll().toUtf8().constData();



        std::string formula = stream.str();
        formula += " ;";
        qDebug() << QString::fromStdString(formula);
       YY_BUFFER_STATE buffer = yy_scan_string(formula.c_str());

                if(yyparse() == 1){
            qDebug() << "Pa to ti ne radi";
        }

        qreal rect_width = formula.length()*PARAMETER;
        qreal rect_height = 20;
        int rect_x = 85;
        int rect_y = 180;
        std::vector<Formula> tmp_vec = std::vector<Formula>();
        parsed_formula->getAtoms(tmp_vec);
        QVector<Formula> assumptions = QVector<Formula>::fromStdVector(tmp_vec);
        Node* item = new Node( parsed_formula, rect_width, rect_height, rect_x, rect_y, nullptr, assumptions);
        scene->addNode(item);
   }

}
