#include "chessgame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <algorithm>
#include <array>

QVector<Piece> ChessGame::createInitialBoard() {
    QVector<Piece> b(64, {PieceType::None, Color::White});
    auto place = [&](int idx, PieceType t, Color c) { b[idx] = {t, c}; };
    for (int i = 8; i < 16; ++i) place(i, PieceType::Pawn, Color::White);
    for (int i = 48; i < 56; ++i) place(i, PieceType::Pawn, Color::Black);
    for (int i : {0,7}) place(i, PieceType::Rook, Color::White);
    for (int i : {56,63}) place(i, PieceType::Rook, Color::Black);
    for (int i : {1,6}) place(i, PieceType::Knight, Color::White);
    for (int i : {57,62}) place(i, PieceType::Knight, Color::Black);
    for (int i : {2,5}) place(i, PieceType::Bishop, Color::White);
    for (int i : {58,61}) place(i, PieceType::Bishop, Color::Black);
    place(3, PieceType::Queen, Color::White);
    place(59, PieceType::Queen, Color::Black);
    place(4, PieceType::King, Color::White);
    place(60, PieceType::King, Color::Black);
    return b;
}

ChessGame::ChessGame(bool vsAI, Color aiColor, QWidget* parent)
    : QWidget(parent), board(createInitialBoard()), turn(Color::White), vsAI(vsAI), aiColor(aiColor),
      whiteTime(0,10,0), blackTime(0,10,0), selected(-1) {
    tick.setInterval(1000);
    connect(&tick, &QTimer::timeout, this, &ChessGame::checkTimers);
    tick.start();
    if(vsAI) loadAI();

}

void ChessGame::setupBoard() {
    board = createInitialBoard();
}

void ChessGame::loadAI() {

    // Placeholder for AI connection
    // In a full implementation we would start Stockfish and communicate via UCI
}

QVector<int> ChessGame::validMoves(int idx) const {
    QVector<int> moves;
    const Piece& p = board[idx];
    if (p.type == PieceType::None) return moves;
    int r = idx/8, c = idx%8;
    auto add=[&](int rr,int cc){
        if(rr<0||rr>=8||cc<0||cc>=8) return;
        int d=rr*8+cc;
        if(board[d].type==PieceType::None || board[d].color!=p.color)
            moves.append(d);
    };
    switch(p.type){
    case PieceType::Pawn: {
        int dir = p.color==Color::White?-1:1;
        int nr=r+dir;
        if(nr>=0&&nr<8 && board[nr*8+c].type==PieceType::None) moves.append(nr*8+c);
        if((p.color==Color::White && r==6)||(p.color==Color::Black && r==1)){
            if(board[(r+dir)*8+c].type==PieceType::None && board[(r+2*dir)*8+c].type==PieceType::None)
                moves.append((r+2*dir)*8+c);
        }
        for(int dc:{-1,1}){
            int cc=c+dc; nr=r+dir;
            if(cc>=0&&cc<8 && nr>=0&&nr<8 && board[nr*8+cc].type!=PieceType::None && board[nr*8+cc].color!=p.color)
                moves.append(nr*8+cc);
        }
        break;}
    case PieceType::Knight: {
        static const std::array<std::pair<int,int>,8> dirs{{{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}}};
        for(auto [dr,dc] : dirs)
            add(r+dr,c+dc);
        break;}
    case PieceType::Bishop:
        for(auto [dr,dc] : std::array<std::pair<int,int>,4>{{{1,1},{1,-1},{-1,1},{-1,-1}}})
            for(int k=1;k<8;k++){int rr=r+dr*k,cc=c+dc*k; if(rr<0||rr>=8||cc<0||cc>=8) break; if(board[rr*8+cc].type==PieceType::None){moves.append(rr*8+cc); continue;} if(board[rr*8+cc].color!=p.color) moves.append(rr*8+cc); break;}
        break;
    case PieceType::Rook:
        for(auto [dr,dc] : std::array<std::pair<int,int>,4>{{{1,0},{-1,0},{0,1},{0,-1}}})
            for(int k=1;k<8;k++){int rr=r+dr*k,cc=c+dc*k; if(rr<0||rr>=8||cc<0||cc>=8) break; if(board[rr*8+cc].type==PieceType::None){moves.append(rr*8+cc); continue;} if(board[rr*8+cc].color!=p.color) moves.append(rr*8+cc); break;}
        break;
    case PieceType::Queen:
        for(auto [dr,dc] : std::array<std::pair<int,int>,8>{{{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}}})
            for(int k=1;k<8;k++){int rr=r+dr*k,cc=c+dc*k; if(rr<0||rr>=8||cc<0||cc>=8) break; if(board[rr*8+cc].type==PieceType::None){moves.append(rr*8+cc); continue;} if(board[rr*8+cc].color!=p.color) moves.append(rr*8+cc); break;}
        break;
    case PieceType::King:
        for(auto [dr,dc] : std::array<std::pair<int,int>,8>{{{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}}})
            add(r+dr,c+dc);
        break;
    default: break;
    }
    return moves;
}

void ChessGame::movePiece(int from, int to){
    board[to]=board[from];
    board[from]={PieceType::None,Color::White};
    selected=-1; highlights.clear();
    turn = opposite(turn);
    if(std::none_of(board.begin(), board.end(), [&](const Piece&p){return p.type==PieceType::King && p.color==opposite(turn);} ))
        endGame(turn==Color::White?"White":"Black");
}

void ChessGame::checkTimers(){
    if(turn==Color::White) whiteTime=whiteTime.addSecs(-1); else blackTime=blackTime.addSecs(-1);
    if(whiteTime==QTime(0,0,0)) { endGame("Black"); return; }
    if(blackTime==QTime(0,0,0)) { endGame("White"); return; }
    update();
}

void ChessGame::endGame(const QString& winner){
    QMessageBox::information(this, "Game Over", winner + " wins");
    emit gameOver();
    close();
}

void ChessGame::paintEvent(QPaintEvent*) {
    QPainter p(this);
    int squareSize = width() / 8;
    for (int r=0; r<8; ++r) {
        for (int c=0; c<8; ++c) {
            bool dark = (r+c)%2;
            p.fillRect(c*squareSize, r*squareSize, squareSize, squareSize,
                       dark ? QColor(118,150,86) : QColor(238,238,210));
            int idx=r*8+c;
            const Piece& piece = board[idx];
            if (highlights.contains(idx))
                p.fillRect(c*squareSize, r*squareSize, squareSize, squareSize, QColor(246,246,105,120));
            if (idx==selected)
                p.fillRect(c*squareSize, r*squareSize, squareSize, squareSize, QColor(186,202,68,120));

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

    p.setPen(Qt::black);
    p.drawText(rect(), Qt::AlignBottom | Qt::AlignLeft, whiteTime.toString("mm:ss"));
    p.drawText(rect(), Qt::AlignTop | Qt::AlignLeft, blackTime.toString("mm:ss"));
}

void ChessGame::mousePressEvent(QMouseEvent* ev) {
    int sq = (ev->y()/ (height()/8))*8 + (ev->x()/(width()/8));
    if(selected==-1){
        if(board[sq].type!=PieceType::None && board[sq].color==turn){
            selected=sq;
            highlights=validMoves(sq);
        }
    } else {
        if(highlights.contains(sq))
            movePiece(selected,sq);
        else if(board[sq].type!=PieceType::None && board[sq].color==turn){
            selected=sq;
            highlights=validMoves(sq);
        } else {
            selected=-1; highlights.clear();
        }
    }
    update();
}

