#include "gui/MainDialog.h"
#include <QVBoxLayout>
#include <QLabel>

MainDialog::MainDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("CAJANDER Biome Classifier");
    resize(400, 300);

    // Создаем Layout
    auto* layout = new QVBoxLayout(this);

    // Создаем Label с текстом "Hello World"
    auto* label = new QLabel("Hello World! CAJANDER Core Interface Stub", this);
    
    // Выравниваем текст по центру
    label->setAlignment(Qt::AlignCenter); 
    
    // Добавляем виджет в компоновщик
    layout->addWidget(label);
}
