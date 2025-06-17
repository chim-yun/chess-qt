#pragma once
#include <QDialog>

class QLineEdit;
class QSqlDatabase;

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget* parent = nullptr);
private slots:
    void onLogin();
    void onSignUp();
private:
    void initDatabase();
    QSqlDatabase* db;
    QLineEdit* userEdit;
    QLineEdit* passEdit;
};

