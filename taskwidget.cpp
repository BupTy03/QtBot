#include "taskwidget.h"

TaskWidget::TaskWidget(const Task& tsk, const MyVK* _vk, QWidget* parent) : QWidget(parent)
{
    this->setMinimumHeight(200);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QGridLayout* GLayout = new QGridLayout;
    QGroupBox* GBox = new QGroupBox;

    QHBoxLayout* HLayout = new QHBoxLayout;

    QVBoxLayout* _layout1 = new QVBoxLayout;

    _layout1->addWidget(new QLabel("<b>Группы:</b>"));

    this->groupsView = new QListView(this);
    this->grChModel = new GroupsChoiceModel(_vk->getGroupsResponse(), this);
    this->grChModel->setChecklist(tsk.getGroups());
    this->groupsView->setModel(this->grChModel);
    _layout1->addWidget(this->groupsView);

    HLayout->addLayout(_layout1);


    QVBoxLayout* _layout2 = new QVBoxLayout;

    _layout2->addWidget(new QLabel("<b>Текст сообщения:</b>"));
    this->messageEdit = new QTextEdit(this);
    this->messageEdit->setText(tsk.getMessage());
    _layout2->addWidget(this->messageEdit);

    HLayout->addLayout(_layout2);

    QVBoxLayout* _layout3 = new QVBoxLayout;

    _layout3->addWidget(new QLabel("<b>Задержка(в мс):</b>"));
    this->intervalSB = new QSpinBox(this);
    this->intervalSB->setValue(tsk.getInterval());
    _layout3->addWidget(this->intervalSB);
    _layout3->addWidget(new QLabel("<b>Период(в сек):</b>"));
    this->periodSB = new QSpinBox(this);
    this->periodSB->setValue(tsk.getPeriod());
    _layout3->addWidget(this->periodSB);
    this->startBtn = new QPushButton(this);
    this->startBtn->setText("Старт");
    this->stopBtn = new QPushButton(this);
    this->stopBtn->setText("Стоп");
    this->stopBtn->setDisabled(true);
    _layout3->addWidget(this->startBtn);
    _layout3->addWidget(this->stopBtn);
    _layout3->addItem(new QSpacerItem(0, 1, QSizePolicy::Preferred, QSizePolicy::Expanding));

    HLayout->addLayout(_layout3);

    GBox->setLayout(HLayout);
    GLayout->addWidget(GBox);

    this->setLayout(GLayout);

    connect(startBtn, &QPushButton::clicked, this, &TaskWidget::onStartBtnClick);
    connect(stopBtn, &QPushButton::clicked, this, &TaskWidget::onStopBtnClick);

    connect(grChModel, SIGNAL(dataChanged(int)), &tsk, SLOT(changeGroups(int)));
    connect(messageEdit, &QTextEdit::textChanged, this, [this](){ emit messageChanged(messageEdit->toPlainText()); });
    connect(this, SIGNAL(messageChanged(const QString&)), &tsk, SLOT(setMessage(const QString&)));
    connect(intervalSB, SIGNAL(valueChanged(int)), &tsk, SLOT(setInterval(int)));
    connect(periodSB, SIGNAL(valueChanged(int)), &tsk, SLOT(setPeriod(int)));
}

void TaskWidget::onStartBtnClick()
{
    this->startBtn->setDisabled(true);
    this->groupsView->setDisabled(true);
    this->messageEdit->setDisabled(true);
    this->intervalSB->setDisabled(true);
    this->periodSB->setDisabled(true);

    this->stopBtn->setEnabled(true);
    emit signalStart();
}

void TaskWidget::onStopBtnClick()
{
    this->stopBtn->setDisabled(true);
    this->groupsView->setEnabled(true);
    this->messageEdit->setEnabled(true);
    this->intervalSB->setEnabled(true);
    this->periodSB->setEnabled(true);

    this->startBtn->setEnabled(true);
    emit signalStop();
}
