#include "status_widget.h"

#include <QProgressBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

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
}

void View::StatusWidget::setRange(QPair<int, int> range)
{
    _progress_bar->setRange(range.first,range.second);
}

void View::StatusWidget::clear()
{
    _progress_bar->reset();
    _text_edit->clear();
}

void View::StatusWidget::setValue(int value)
{
    _progress_bar->setValue(value);
}

void View::StatusWidget::pushMessage(const QString &message)
{
    _text_edit->append(message);
}
