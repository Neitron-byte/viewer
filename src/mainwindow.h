#pragma once
#include <QMainWindow>
#include <QScopedPointer>

#include "widgets/status_widget.h"
#include "engine/controller.h"

class QTableView;

namespace View
{
/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onImportActionTriggered();

private:
    QScopedPointer<Controller> _controller;
    QTableView* _table_view;
    QScopedPointer<StatusWidget> _status_widget;
};
}

