#include "gui/classification/ClassificationTab.h"
#include "gui/classification/biomeSelector/biomeSelector.h"
#include "gui/classification/biomeSelector/biomeModel.h"
#include "services/biomes/biomeRepository.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QDebug>
#include <QObject>

ClassificationTab::ClassificationTab(const Cajander::Services::BiomeRepository& repository, QWidget* parent) 
    : QWidget(parent) 
{
    // 1. Создаем основной вертикальный лейаут для всей вкладки
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    // 2. Инициализируем модель, передавая ей ссылку на репозиторий
    // std::make_unique — самый безопасный и современный (С++14+) способ создания unique_ptr
    m_biomeModel = std::make_unique<Cajander::Gui::BiomeModel>(repository, this);

    // 3. Создаем наш кастомный UI-селектор и передаем ему указатель на Qt-модель
    m_biomeSelector = new Cajander::Gui::BiomeSelector(m_biomeModel.get(), this);

    // 4. Используем QFormLayout для красивого выравнивания подписей элементов управления
    auto* formLayout = new QFormLayout();
    formLayout->addRow(tr("Select Biome:"), m_biomeSelector);
    
    // Добавляем форму в основной лейаут
    mainLayout->addLayout(formLayout);

    // Добавляем пружину (spacer) вниз, чтобы наш селектор не растягивался на весь экран, 
    // а аккуратно прижимался к верхней части вкладки
    mainLayout->addStretch(1);

    // 5. Коммуникация через сигналы и слоты
    connect(m_biomeSelector, &Cajander::Gui::BiomeSelector::biomeChanged, 
            this, &ClassificationTab::onBiomeChanged);
            
    connect(m_biomeSelector, &Cajander::Gui::BiomeSelector::editBiomesRequested, 
            this, &ClassificationTab::onEditBiomesRequested);
}

void ClassificationTab::onBiomeChanged(const QModelIndex& index) {
    if (!index.isValid()) return;

    // Демонстрируем строгость и мощь Qt Model: вытаскиваем скрытые данные из индекса!
    QString name = index.data(Qt::DisplayRole).toString();
    int code = index.data(Cajander::Gui::BiomeModel::CodeRole).toInt();

    // Пишем в отладочную консоль QGIS / IDE. 
    // В будущем здесь будет логика обновления параметров классификации для ГИС-движка.
    qDebug() << "Selected biome changed to:" << name << "(Code:" << code << ")";
}

void ClassificationTab::onEditBiomesRequested() {
    // Временная UX-заглушка. Когда пользователь нажмет на карандаш, он увидит этот месседж-бокс,
    // что докажет работоспособность нашей цепочки сигналов.
    QMessageBox::information(this, tr("Cajander Editor"), tr("Biome Editor will be here soon!"));
}
