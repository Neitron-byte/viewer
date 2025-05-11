#include "status_widget.h"

#include <QProgressBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

View::StatusWidget::StatusWidget(QWidget *parent): QWidget(parent), _progress_bar(new QProgressBar(this)), _text_edit(new QTextEdit(this))
{
    setWindowTitle("Status");
    QVBoxLayout* v_l = new QVBoxLayout(this);
    v_l->addWidget(_progress_bar);
    v_l->addWidget(_text_edit);
    QHBoxLayout* h_l = new QHBoxLayout;
    h_l->addStretch();
    QPushButton* ok_btn = new QPushButton(tr("OK"),this);
    h_l->addWidget(ok_btn);
    connect(ok_btn,&QPushButton::clicked,this,&StatusWidget::hide);
    v_l->addLayout(h_l);

    _progress_bar->setMinimum(0);
}

void View::StatusWidget::setMaximum(int maximum)
{
    qDebug() << "Set Max: " << maximum;
    _progress_bar->setMaximum(maximum);
    _progress_bar->setMinimum(0);
}


void View::StatusWidget::clear()
{
    _progress_bar->reset();
    _text_edit->clear();
    _progress_bar->resetFormat();
}

void View::StatusWidget::setValue(int value)
{
    qDebug() << "SetValue: " << value;
    _progress_bar->setValue(value);
}

void View::StatusWidget::pushMessage(const QString &message)
{
    _text_edit->append(message);
}

void View::StatusWidget::setOperationName(const QString &operation_name)
{
    _progress_bar->setFormat(operation_name + "%p%");
}
