#ifndef UPDATETOKENWIDGET_H
#define UPDATETOKENWIDGET_H

#include <QtWidgets>

class UpdateTokenWidget : public QDialog
{
    Q_OBJECT
public:
    explicit UpdateTokenWidget(QWidget *parent = nullptr);
    QString getToken() const { return tokenLineEdit_->text(); }

private slots:
    void okBtn_clicked();

private:
    bool checkToken(const QString& str);

private:
    QLineEdit* tokenLineEdit_{};
    QPushButton* okBtn_{};
};

#endif // UPDATETOKENWIDGET_H
