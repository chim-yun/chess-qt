#pragma once
#include <QWidget>
#include <QVector>
#include <QPair>
#include <QTimer>
#include <QTime>


enum class PieceType { None, Pawn, Knight, Bishop, Rook, Queen, King };
enum class Color { White, Black };

struct Piece {
    PieceType type;
    Color color;
};

class ChessGame : public QWidget {
    Q_OBJECT
public:
    explicit ChessGame(bool vsAI = false, Color aiColor = Color::Black, QWidget* parent = nullptr);
    QSize sizeHint() const override { return QSize(512,512); }


signals:
    void gameOver();


private:
    QVector<Piece> board;
    Color turn;
    bool vsAI;
    Color aiColor;

    QTime whiteTime;
    QTime blackTime;
    QTimer tick;
    int selected;
    QVector<int> highlights;

    void setupBoard();
    void loadAI();
    QVector<int> validMoves(int idx) const;
    void movePiece(int from, int to);
    void checkTimers();
    void endGame(const QString& winner);
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
};

