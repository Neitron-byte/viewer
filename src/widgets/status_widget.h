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

    void setRange(QPair<int,int> range);

public slots:
    void clear();
    void setValue(int value);
    void pushMessage(const QString& message);

private:
    QProgressBar* _progress_bar;
    QTextEdit* _text_edit;
};
}
