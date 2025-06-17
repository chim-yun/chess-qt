#include "login.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QtSql>

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent), db(nullptr) {
    setWindowTitle("Login");
    auto* layout = new QVBoxLayout(this);

    auto* form = new QHBoxLayout;
    form->addWidget(new QLabel("User:"));
    userEdit = new QLineEdit;
    form->addWidget(userEdit);
    layout->addLayout(form);

    form = new QHBoxLayout;
    form->addWidget(new QLabel("Password:"));
    passEdit = new QLineEdit;
    passEdit->setEchoMode(QLineEdit::Password);
    form->addWidget(passEdit);
    layout->addLayout(form);

    auto* buttons = new QHBoxLayout;
    auto* loginBtn = new QPushButton("Login");
    auto* signBtn = new QPushButton("Sign Up");
    buttons->addWidget(loginBtn);
    buttons->addWidget(signBtn);
    layout->addLayout(buttons);

    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(signBtn, &QPushButton::clicked, this, &LoginDialog::onSignUp);

    initDatabase();
}

void LoginDialog::initDatabase() {
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName("chess_users.db");
    if (db->open()) {
        QSqlQuery q("CREATE TABLE IF NOT EXISTS users (name TEXT PRIMARY KEY, password TEXT)");
        q.exec();
    }
}

void LoginDialog::onLogin() {
    if (!db || !db->isOpen()) return;
    QString name = userEdit->text();
    QString pw = passEdit->text();
    QSqlQuery q;
    q.prepare("SELECT password FROM users WHERE name=:name");
    q.bindValue(":name", name);
    if (q.exec() && q.next()) {
        if (q.value(0).toString() == pw) {
            accept();
        }
    }
}

void LoginDialog::onSignUp() {
    if (!db || !db->isOpen()) return;
    QString name = userEdit->text();
    QString pw = passEdit->text();
    QSqlQuery q;
    q.prepare("INSERT OR IGNORE INTO users VALUES(:name,:pw)");
    q.bindValue(":name", name);
    q.bindValue(":pw", pw);
    q.exec();
}

