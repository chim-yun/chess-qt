#pragma once
#include <QWidget>
#include <QVector>
#include <QPair>
#include <QTimer>

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

private:
    QVector<Piece> board;
    Color turn;
    bool vsAI;
    Color aiColor;
    QTimer whiteTimer;
    QTimer blackTimer;

    void setupBoard();
    void loadAI();
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
};

