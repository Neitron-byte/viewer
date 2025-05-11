#pragma once
#include <QWidget>

class QProgressBar;
class QTextEdit;

namespace View
{
/*!
 * \brief The StatusWidget class
 * Виджет отображения статуса загрузки файлов
 */
class StatusWidget: public QWidget
{
    Q_OBJECT
public:
    explicit StatusWidget(QWidget* parent = nullptr);

public slots:
    void setMaximum(int maximum);
    void clear();
    void setValue(int value);
    void pushMessage(const QString& message);
    void setOperationName(const QString& operation_name);

private:
    QProgressBar* _progress_bar;
    QTextEdit* _text_edit;
};
}
