#include "gui/classification/biomeSelector/biomeModel.h"
#include "services/biomes/biomeRepository.h"

#include <QPixmap>
#include <QPainter>
#include <QIcon>

namespace Cajander::Gui {

QIcon createColorIcon(const QColor& color) {
    if (!color.isValid()) return QIcon();

    const int size = 12;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(color);
    painter.setPen(QPen(QColor(180, 180, 180), 1));
    painter.drawRoundedRect(0, 0, size - 1, size - 1, 2, 2);
    painter.end();

    return QIcon(pixmap);
}

BiomeModel::BiomeModel(const Services::BiomeRepository& repository, QObject* parent)
    : QAbstractListModel(parent)
    , m_repository(repository)
{
    connect(&repository, &Services::BiomeRepository::biomesChanged, this, &BiomeModel::refresh);

}

int BiomeModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_repository.getBiomes().size());
}

QVariant BiomeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    const auto& biomes = m_repository.getBiomes();
    if (index.row() < 0 || index.row() >= static_cast<int>(biomes.size())) {
        return QVariant();
    }

    const auto& biome = biomes[static_cast<std::size_t>(index.row())];

    if (role == Qt::DecorationRole) {
        return createColorIcon(biome.color);
    }

    switch (role) {
        case Qt::DisplayRole:
            return QString::number(biome.code) + " — " + biome.name;
            
        case Qt::ToolTipRole:
            return biome.description;
            
        case BiomeRoles::CodeRole:
            return biome.code;
            
        case BiomeRoles::DescriptionRole:
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
