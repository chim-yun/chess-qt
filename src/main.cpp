#include <QApplication>
#include "login.h"
#include "chessgame.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        ChessGame game;
        game.show();
        return app.exec();
    }
    return 0;
}
