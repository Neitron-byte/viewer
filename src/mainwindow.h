#pragma once
#include <QMainWindow>
#include <QScopedPointer>

#include "engine/controller.h"

class QTableView;

namespace View
{
class StatusWidget;
class RecordsTableModel;
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
    void onRemoveRecords();

private:
    void createTableViewActions();

private:
    QSharedPointer<RecordsTableModel> _records_model;
    QScopedPointer<Controller> _controller;
    QTableView* _table_view;
    StatusWidget* _status_widget;
};
}

