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
{
    _table_view = new QTableView(this);
    _table_view->setModel(_controller->getModel());

    setCentralWidget(_table_view);

    QMenuBar* menu_bar = menuBar();

    QMenu* menu = menu_bar->addMenu(tr("Файл"));

    QAction* import_action = menu->addAction(tr("Импорт"));

    connect(import_action,&QAction::triggered,this,&MainWindow::onImportActionTriggered);

    QAction* clear_action = menu->addAction(tr("Очистить"));
    connect(clear_action,&QAction::triggered,_controller.get(),&Controller::clearData);
    connect(_controller.get(),&Controller::message,[this](const QString& msg){
        statusBar()->showMessage(msg);
    });

    QTimer::singleShot(10,[this](){
        _controller->loadData();
    });

    _status_widget = new StatusWidget(this);
    _status_widget->setWindowFlag(Qt::Tool,true);

    connect(_controller.get(),&Controller::statusLoadedMessage,_status_widget,&StatusWidget::pushMessage);
    connect(_controller.get(),&Controller::filesCount,_status_widget,&StatusWidget::setMaximum);
    connect(_controller.get(),&Controller::progress,_status_widget,&StatusWidget::setValue);
    connect(_controller.get(),&Controller::operationChanged,_status_widget,&StatusWidget::setOperationName);
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
