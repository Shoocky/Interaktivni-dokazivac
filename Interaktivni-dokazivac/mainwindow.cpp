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
    btn_layout->addWidget(ui->andE);
    btn_layout->addWidget(ui->orI);
    btn_layout->addWidget(ui->orE);
    btn_layout->addWidget(ui->ponisti);
    layout->addLayout(btn_layout);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    connect(ui->dodajDete, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(ui->ponisti, SIGNAL(clicked()), this, SLOT(ponistiClicked()));

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

        qreal rect_width = ui->lineEdit->text().length()*10 -5;
        qreal rect_height = 20;
        int rect_x = 85;
        int rect_y = 180;
        Node* item = new Node(ui->lineEdit->text(), parsed_formula, rect_width, rect_height, rect_x, rect_y);
        scene->addItem(item);
        items.push_back(item);
    }
    else{

        QList<QGraphicsItem *> selected_list = scene->selectedItems();

        Node* selected = (Node*)(selected_list.at(0));
        qreal rect_width =  selected->getText().length()*10 - 5;
        qreal rect_height = 20;
        int rect_x = selected->getx() - 20 - depth/2;
        int rect_y = selected->gety() - 20 - depth/2;
        Node* item = new Node( selected->getText(), parsed_formula, rect_width, rect_height, rect_x, rect_y);
        scene->addItem(item);
        items.push_back(item);

        rect_x = selected->getx() + 25  + depth/2;
        rect_y = selected->gety() - 20 - depth/2;
        Node* item1 = new Node( selected->getText(), parsed_formula, rect_width, rect_height, rect_x, rect_y);
        scene->addItem(item1);
        items.push_back(item1);
    }
    depth += 15;
}

void MainWindow::ponistiClicked()
{
    qDebug() << "ponisti";
    scene->removeItem(items[items.size()-1]);
    items.pop_back();
    scene->update();
}
