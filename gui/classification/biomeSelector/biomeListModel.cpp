#include "gui/classification/biomeSelector/biomeListModel.h"
#include "services/biomes/biomeRepository.h"

#include <QPixmap>
#include <QPainter>
#include <QIcon>

namespace Cajander::Gui {

namespace {

/**
 * Helper function with internal linkage to generate a standardized preview color square.
 */
QIcon createColorIcon(const QColor& color) {
    if (!color.isValid()) return QIcon();

    const int size = 12;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(color);
    // Standard subtle grey border around the biome color swatch.
    painter.setPen(QPen(QColor(180, 180, 180), 1));
    painter.drawRoundedRect(0, 0, size - 1, size - 1, 2, 2);
    painter.end();

    return QIcon(pixmap);
}

} // namespace

BiomeListModel::BiomeListModel(const Services::BiomeRepository& repository, QObject* parent)
    : QAbstractListModel(parent)
    , m_repository(repository)
{
    // React to domain-level database reloads by refreshing UI model states.
    connect(&repository, &Services::BiomeRepository::biomesChanged, this, &BiomeListModel::refresh);
}

int BiomeListModel::rowCount(const QModelIndex& parent) const {
    // For list models, children count is always zero for valid hierarchical indexes.
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_repository.getBiomes().size());
}

QVariant BiomeListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    const auto& biomes = m_repository.getBiomes();
    
    // Bounds check to avoid memory segmentation faults.
    if (index.row() < 0 || index.row() >= static_cast<int>(biomes.size())) {
        return QVariant();
    }

    const auto& biome = biomes[static_cast<std::size_t>(index.row())];

    // Decoration role formats visual indicators (like small color squares in QComboBox).
    if (role == Qt::DecorationRole) {
        return createColorIcon(biome.color);
    }

    switch (role) {
        case Qt::DisplayRole:
            // Combines code and name for user-friendly rendering: "1 — Cladina type"
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

void BiomeListModel::refresh() {
    // Completely invalidates the structural cache inside the attached views.
    beginResetModel();
    endResetModel();
}

} // namespace Cajander::Gui
