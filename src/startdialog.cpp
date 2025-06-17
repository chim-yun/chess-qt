#include "startdialog.h"
#include <QVBoxLayout>
#include <QRandomGenerator>

StartDialog::StartDialog(QWidget* parent) : QDialog(parent), chosenMode(GameMode::TwoPlayer), chosenColor(Color::White) {
    setWindowTitle("New Game");
    auto* layout = new QVBoxLayout(this);
    twoPlayerRadio = new QRadioButton("Two Player");
    aiRadio = new QRadioButton("Vs AI");
    twoPlayerRadio->setChecked(true);
    layout->addWidget(twoPlayerRadio);
    layout->addWidget(aiRadio);
    colorBox = new QComboBox;
    colorBox->addItem("White", static_cast<int>(Color::White));
    colorBox->addItem("Black", static_cast<int>(Color::Black));
    colorBox->addItem("Random", -1);
    colorBox->setEnabled(false);
    layout->addWidget(colorBox);
    connect(aiRadio, &QRadioButton::toggled, colorBox, &QWidget::setEnabled);
    auto* start = new QPushButton("Start");
    layout->addWidget(start);
    connect(start, &QPushButton::clicked, this, &StartDialog::acceptGame);
}

void StartDialog::acceptGame() {
    chosenMode = aiRadio->isChecked() ? GameMode::VsAI : GameMode::TwoPlayer;
    int data = colorBox->currentData().toInt();
    if (data == -1) {
        chosenColor = (QRandomGenerator::global()->bounded(2)) ? Color::White : Color::Black;
    } else {
        chosenColor = static_cast<Color>(data);
    }
    accept();
}
