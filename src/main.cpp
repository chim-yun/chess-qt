#include <QApplication>
#include "login.h"
#include "chessgame.h"
#include "startdialog.h"
#include <memory>
#include <functional>


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    LoginDialog login;

    if (login.exec() != QDialog::Accepted)
        return 0;

    std::function<void()> startGame;
    std::shared_ptr<ChessGame> game;
    startGame = [&]() {
        StartDialog dlg;
        if (dlg.exec() != QDialog::Accepted) { app.quit(); return; }
        bool vsAI = dlg.mode() == GameMode::VsAI;
        game.reset(new ChessGame(vsAI, dlg.aiColor()));
        QObject::connect(game.get(), &ChessGame::gameOver, startGame);
        game->show();
    };

    startGame();
    return app.exec();

    if (login.exec() == QDialog::Accepted) {
        ChessGame game;
        game.show();
        return app.exec();
    }
    return 0;
}
