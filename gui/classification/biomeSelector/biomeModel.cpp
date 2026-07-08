#include "gui/classification/biomeSelector/biomeModel.h"
#include "services/biomes/biomeRepository.h"
#include <qgsmessagelog.h>
#include <qgis.h>

namespace Cajander::Gui {

BiomeModel::BiomeModel(const Services::BiomeRepository& repository, QObject* parent)
    : QAbstractListModel(parent)
    , m_repository(repository)
{
}

int BiomeModel::rowCount(const QModelIndex& parent) const {
    // В плоском списке (ListModel) количество строк валидно только для корневого элемента
    if (parent.isValid()) {
        return 0;
    }
    QgsMessageLog::logMessage(QString("BiomeModel rowCount: %1").arg(m_repository.getBiomes().size()), "Cajander", Qgis::MessageLevel::Info);
    return static_cast<int>(m_repository.getBiomes().size());
}

QVariant BiomeModel::data(const QModelIndex& index, int role) const {
    // Базовые проверки на валидность индекса и выход за границы вектора
    if (!index.isValid()) {
        return QVariant();
    }

    const auto& biomes = m_repository.getBiomes();
    if (index.row() < 0 || index.row() >= static_cast<int>(biomes.size())) {
        return QVariant();
    }

    // Извлекаем нужный биом
    const auto& biome = biomes[static_cast<std::size_t>(index.row())];

    // Отдаем данные в зависимости от того, что запросил QComboBox
    switch (role) {
        case Qt::DisplayRole:
            // То, что будет непосредственно написано в выпадающем списке
            return biome.name;
            
        case Qt::ToolTipRole:
            // Всплывающая подсказка при наведении мыши на элемент списка
            return biome.description;
            
        case BiomeRoles::CodeRole:
            // Наш кастомный скрытый индентификатор биома (int)
            return biome.code;
            
        case BiomeRoles::DescriptionRole:
            // Кастомный запрос описания в явном виде
            return biome.description;
            
        default:
            return QVariant();
    }
}

void BiomeModel::refresh() {
    beginResetModel();
    endResetModel();
}

} // namespace Cajander::Gui
