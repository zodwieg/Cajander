#include "gui/classification/ClassificationTab.h"
#include "gui/classification/biomeSelector/biomeSelector.h"
#include "infrastructure/pluginContext.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QDebug>
#include <QObject>

ClassificationTab::ClassificationTab(QWidget* parent) 
    : QWidget(parent) 
{
    // 1. Создаем основной вертикальный лейаут для всей вкладки
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    // 2. Получаем модель из синглтона и создаём селектор
    m_biomeSelector = new Cajander::Gui::BiomeSelector(this);

    // 3. Используем QFormLayout для красивого выравнивания подписей элементов управления
    auto* formLayout = new QFormLayout();
    formLayout->addRow(tr("Select Biome:"), m_biomeSelector);
    
    // Добавляем форму в основной лейаут
    mainLayout->addLayout(formLayout);

    // Добавляем пружину (spacer) вниз, чтобы наш селектор не растягивался на весь экран, 
    // а аккуратно прижимался к верхней части вкладки
    mainLayout->addStretch(1);

    // 4. Коммуникация через сигналы и слоты
    connect(m_biomeSelector, &Cajander::Gui::BiomeSelector::biomeChanged, 
            this, &ClassificationTab::onBiomeChanged);
            
    connect(m_biomeSelector, &Cajander::Gui::BiomeSelector::editBiomesRequested, 
            this, &ClassificationTab::onEditBiomesRequested);
}

void ClassificationTab::onBiomeChanged(const QModelIndex& index) {
}

void ClassificationTab::onEditBiomesRequested() {
    // Временная UX-заглушка. Когда пользователь нажмет на карандаш, он увидит этот месседж-бокс,
    // что докажет работоспособность нашей цепочки сигналов.
    QMessageBox::information(this, tr("Cajander Editor"), tr("Biome Editor will be here soon!"));
}
