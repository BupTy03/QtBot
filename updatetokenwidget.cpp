#include "updatetokenwidget.h"

UpdateTokenWidget::UpdateTokenWidget(QWidget *parent)
    : QDialog(parent),
      tokenLineEdit_(new QLineEdit),
      okBtn_(new QPushButton("ОК"))
{
    this->setWindowTitle("Сменить токен");
    this->setMinimumSize(QSize(400, 110));
    this->setMaximumSize(QSize(400, 110));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    okBtn_->setSizePolicy(QSizePolicy::Policy::Fixed,
                          QSizePolicy::Policy::Fixed);

    QVBoxLayout* v_lay = new QVBoxLayout;

    QHBoxLayout* h_lay = new QHBoxLayout;
    h_lay->addWidget(new QLabel("<b>Новый токен:</b>"));
    h_lay->addWidget(tokenLineEdit_);

    v_lay->addLayout(h_lay);

    QHBoxLayout* h_lay_second = new QHBoxLayout;
    h_lay_second->addItem(new QSpacerItem(10000, 10000,
                          QSizePolicy::Preferred,
                          QSizePolicy::MinimumExpanding));

    h_lay_second->addWidget(okBtn_);

    v_lay->addLayout(h_lay_second);

    this->setLayout(v_lay);

    QObject::connect(okBtn_, &QPushButton::clicked,
                     this, &UpdateTokenWidget::okBtn_clicked);
}

void UpdateTokenWidget::okBtn_clicked()
{
    if(!checkToken(tokenLineEdit_->text()))
    {
        QMessageBox::critical(this,
                              tr("Ошибка"),
                              tr("Неверный токен!"));
        return;
    }

    this->accept();
}

bool UpdateTokenWidget::checkToken(const QString& str)
{
    if(str.isEmpty())
    {
        return false;
    }
    for(const auto& ch : str)
    {
        if(!ch.isLetterOrNumber())
        {
            return false;
        }
    }
    return true;
}
