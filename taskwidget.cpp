#include "taskwidget.h"

TaskWidget::TaskWidget(Task* task, const QStringList& groups_list, QWidget* parent) : QWidget(parent)
{
    this->setMinimumHeight(210);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QGridLayout* GLayout = new QGridLayout;
    QGroupBox* GBox = new QGroupBox;

    QHBoxLayout* HLayout = new QHBoxLayout;

    QVBoxLayout* _layout1 = new QVBoxLayout;

    _layout1->addWidget(new QLabel("<b>Группы:</b>"));

    groupsView_ = new QListView(this);

    grChModel_ = new QStandardItemModel(this);

    for(const auto& group : groups_list)
    {
        QStandardItem* tmp = new QStandardItem(group);
        tmp->setEditable(false);
        grChModel_->appendRow(tmp);
    }

    groupsView_->setModel(grChModel_);
    _layout1->addWidget(groupsView_);

    HLayout->addLayout(_layout1);


    QVBoxLayout* _layout2 = new QVBoxLayout;

    _layout2->addWidget(new QLabel("<b>Текст сообщения:</b>"));
    messageEdit_ = new QTextEdit(this);
    messageEdit_->setText(task->getMessage());
    messageEdit_->setReadOnly(true);
    _layout2->addWidget(messageEdit_);

    HLayout->addLayout(_layout2);

    QVBoxLayout* _layout3 = new QVBoxLayout;

    _layout3->addWidget(new QLabel("<b>Задержка(в мс):</b>"));
    intervalSB_ = new QSpinBox;
    intervalSB_->setRange(0, 10000000);
    intervalSB_->setValue(task->getInterval());
    intervalSB_->setSuffix(" мс.");
    intervalSB_->setReadOnly(true);
    _layout3->addWidget(intervalSB_);
    _layout3->addWidget(new QLabel("<b>Период(в сек):</b>"));
    periodSB_ = new QSpinBox;
    periodSB_->setRange(0, 10000000);
    periodSB_->setValue(task->getPeriod());
    periodSB_->setSuffix(" сек.");
    periodSB_->setReadOnly(true);
    _layout3->addWidget(periodSB_);
    startBtn_ = new QPushButton;
    startBtn_->setText("Старт");
    startBtn_->setDisabled(true);
    stopBtn_ = new QPushButton;
    stopBtn_->setText("Стоп");
    stopBtn_->setEnabled(true);
    removeBtn_ = new QPushButton;
    removeBtn_->setText("Удалить");
    removeBtn_->setEnabled(true);
    _layout3->addWidget(startBtn_);
    _layout3->addWidget(stopBtn_);
    _layout3->addWidget(removeBtn_);
    _layout3->addItem(new QSpacerItem(0, 1, QSizePolicy::Preferred, QSizePolicy::Expanding));

    HLayout->addLayout(_layout3);

    GBox->setLayout(HLayout);
    GLayout->addWidget(GBox);

    this->setLayout(GLayout);

    QObject::connect(startBtn_, &QPushButton::clicked, this, &TaskWidget::onStartBtnClick);
    QObject::connect(stopBtn_, &QPushButton::clicked, this, &TaskWidget::onStopBtnClick);
    QObject::connect(removeBtn_, &QPushButton::clicked, this, &TaskWidget::onRemoveBtnClick);

    QObject::connect(this, &TaskWidget::signalStart, task, &Task::start);
    QObject::connect(this, &TaskWidget::signalStop, task, &Task::stop);
    QObject::connect(this, &TaskWidget::destroyed, task, &Task::deleteLater);
}

void TaskWidget::onStartBtnClick()
{
    startBtn_->setDisabled(true);
    stopBtn_->setEnabled(true);
    emit signalStart();
}

void TaskWidget::onStopBtnClick()
{
    stopBtn_->setDisabled(true);
    startBtn_->setEnabled(true);
    emit signalStop();
}

void TaskWidget::onRemoveBtnClick()
{
    stopBtn_->setDisabled(true);
    startBtn_->setDisabled(true);
    this->deleteLater();
}
