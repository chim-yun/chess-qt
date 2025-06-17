#include "chessgame.h"
#include <QPainter>

ChessGame::ChessGame(bool vsAI, Color aiColor, QWidget* parent)
    : QWidget(parent), board(64), turn(Color::White), vsAI(vsAI), aiColor(aiColor) {
    setupBoard();
    whiteTimer.start(1000*60*10);
    blackTimer.start(1000*60*10);
}

void ChessGame::setupBoard() {
    // Simplified starting position
    // Pawns
    for (int i=8;i<16;i++) board[i] = {PieceType::Pawn, Color::White};
    for (int i=48;i<56;i++) board[i] = {PieceType::Pawn, Color::Black};
    board[0] = board[7] = {PieceType::Rook, Color::White};
    board[56] = board[63] = {PieceType::Rook, Color::Black};
    board[1] = board[6] = {PieceType::Knight, Color::White};
    board[57] = board[62] = {PieceType::Knight, Color::Black};
    board[2] = board[5] = {PieceType::Bishop, Color::White};
    board[58] = board[61] = {PieceType::Bishop, Color::Black};
    board[3] = {PieceType::Queen, Color::White};
    board[59] = {PieceType::Queen, Color::Black};
    board[4] = {PieceType::King, Color::White};
    board[60] = {PieceType::King, Color::Black};
}

void ChessGame::loadAI() {
    // TODO: connect to Stockfish using QProcess
}

void ChessGame::paintEvent(QPaintEvent*) {
    QPainter p(this);
    int squareSize = width() / 8;
    for (int r=0; r<8; ++r) {
        for (int c=0; c<8; ++c) {
            bool dark = (r+c)%2;
            p.fillRect(c*squareSize, r*squareSize, squareSize, squareSize,
                       dark ? QColor(118,150,86) : QColor(238,238,210));
            const Piece& piece = board[r*8+c];
            if (piece.type != PieceType::None) {
                QString filename;
                QString colorPrefix = piece.color == Color::White ? "_w" : "_b";
                switch(piece.type) {
                    case PieceType::Pawn: filename = "pawn"; break;
                    case PieceType::Knight: filename = "knight"; break;
                    case PieceType::Bishop: filename = "bishop"; break;
                    case PieceType::Rook: filename = "rook"; break;
                    case PieceType::Queen: filename = "queen"; break;
                    case PieceType::King: filename = "king"; break;
                    default: break;
                }
                if (!filename.isEmpty()) {
                    QPixmap pix(":/images/" + filename + colorPrefix + ".png");
                    p.drawPixmap(c*squareSize, r*squareSize, squareSize, squareSize, pix);
                }
            }
        }
    }
}

void ChessGame::mousePressEvent(QMouseEvent* ev) {
    // TODO: handle piece selection and move highlighting
}

