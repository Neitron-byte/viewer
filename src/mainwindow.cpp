#include "mainwindow.h"

#include <QTableView>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QTimer>
#include <QStatusBar>

#include "engine/controller.h"
#include "engine/records_table_model.h"

#include "widgets/status_widget.h"

View::MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}
    , _controller{new Controller}
    ,_status_widget{new StatusWidget(this)}
{
    _table_view = new QTableView(this);
    _table_view->setModel(_controller->getModel());

    setCentralWidget(_table_view);

    QMenuBar* menu_bar = menuBar();

    QMenu* menu = menu_bar->addMenu(tr("Файл"));

    QAction* import_action = menu->addAction(tr("Импорт"));

    connect(import_action,&QAction::triggered,this,&MainWindow::onImportActionTriggered);


    connect(_controller.get(),&Controller::statusLoadedMessage,_status_widget.get(),&StatusWidget::pushMessage);
    connect(_controller.get(),&Controller::filesCount,[this](int count){
        _status_widget->setRange({0,count});
    });
    connect(_controller.get(),&Controller::filesCount,_status_widget.get(),&StatusWidget::setValue);


    connect(_controller.get(),&Controller::message,[this](const QString& msg){
        statusBar()->showMessage(msg);
    });

    QTimer::singleShot(10,[this](){
        _controller->loadData();
    });
}

void View::MainWindow::onImportActionTriggered()
{
    QString folder_path = QFileDialog::getExistingDirectory(this, tr("Выберите директорию"), "/home");
    if(folder_path.isEmpty())
        return;

    _status_widget->clear();
    _status_widget->show();
    _controller->loadFiles(folder_path);
}
