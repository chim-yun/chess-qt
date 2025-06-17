#pragma once
#include <QDialog>
#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>
#include "chessgame.h"

enum class GameMode { TwoPlayer, VsAI };

class StartDialog : public QDialog {
    Q_OBJECT
public:
    explicit StartDialog(QWidget* parent = nullptr);
    GameMode mode() const { return chosenMode; }
    Color aiColor() const { return chosenColor; }
private slots:
    void acceptGame();
private:
    GameMode chosenMode;
    Color chosenColor;
    QRadioButton* twoPlayerRadio;
    QRadioButton* aiRadio;
    QComboBox* colorBox;
};
