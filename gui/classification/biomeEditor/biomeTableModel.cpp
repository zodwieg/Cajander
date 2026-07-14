#include "biomeTableModel.h"
#include "services/biomes/biomeRepository.h"
#include "domain/biome.h"
#include <QColor>

namespace Cajander::Gui {

BiomeTableModel::BiomeTableModel(Services::BiomeRepository& repository, QObject* parent)
    : QAbstractTableModel(parent)
    , m_repository(repository)
{
    // Synchronize the model when external repository changes occur (e.g., during file import)
    connect(&repository, &Services::BiomeRepository::biomesChanged, this, &BiomeTableModel::refresh);
}

int BiomeTableModel::rowCount(const QModelIndex& parent) const {
    // For flat table models, row count is only valid for the root model index
    return parent.isValid() ? 0 : static_cast<int>(m_repository.getBiomes().size());
}

int BiomeTableModel::columnCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : Columns::ColumnCount;
}

QVariant BiomeTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return QVariant();

    const auto& biomes = m_repository.getBiomes();
    if (index.row() < 0 || index.row() >= static_cast<int>(biomes.size())) {
        return QVariant();
    }

    // Auxiliary Grip and Delete columns do not render standard text or data types
    if (index.column() == Columns::GripColumn || index.column() == Columns::DeleteColumn) {
        return QVariant(); 
    }

    if (role == Qt::TextAlignmentRole && index.column() == Columns::ColorColumn) {
        return Qt::AlignCenter;
    }

    const auto& biome = biomes[static_cast<std::size_t>(index.row())];

    // The color column uses a custom delegate; disable standard Qt decoration icons
    if (role == Qt::DecorationRole && index.column() == Columns::ColorColumn) {
        return QVariant(); 
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case Columns::CodeColumn:        return biome.code;
            case Columns::ColorColumn:       return QVariant(); // Do not render raw text within the color cell
            case Columns::NameColumn:        return biome.name;
            case Columns::DescriptionColumn: return biome.description;
            default:                         return QVariant();
        }
    }

    if (role == Qt::EditRole) {
        switch (index.column()) {
            case Columns::CodeColumn:        return biome.code;
            case Columns::ColorColumn:       return biome.color; // Return QColor for the delegate's QColorDialog
            case Columns::NameColumn:        return biome.name;
            case Columns::DescriptionColumn: return biome.description;
            default:                         return QVariant();
        }
    }
    return QVariant();
}

QVariant BiomeTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch (section) {
        case Columns::GripColumn:        return QString();
        case Columns::CodeColumn:        return tr("Code");
        case Columns::ColorColumn:       return tr("Color"); 
        case Columns::NameColumn:        return tr("Name");
        case Columns::DescriptionColumn: return tr("Description");
        case Columns::DeleteColumn:      return QString();
        default:                         return QVariant();
    }
}

Qt::ItemFlags BiomeTableModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;
    
    if (index.column() == Columns::GripColumn || index.column() == Columns::DeleteColumn) {
        return QAbstractTableModel::flags(index); 
    }

    // The color column is interactive but edited via clicks/delegates rather than text fields
    if (index.column() == Columns::ColorColumn) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled;
    }
    
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void BiomeTableModel::addBiome(const Cajander::Domain::Biome& newBiome) {
    int nextRow = rowCount();
    
    // Temporarily disconnect the repository signal to avoid redundant resetModel calls via refresh()
    disconnect(&m_repository, &Services::BiomeRepository::biomesChanged, this, &BiomeTableModel::refresh);
    
    beginInsertRows(QModelIndex(), nextRow, nextRow);
    m_repository.addBiome(newBiome);
    endInsertRows();
    
    connect(&m_repository, &Services::BiomeRepository::biomesChanged, this, &BiomeTableModel::refresh);
}

bool BiomeTableModel::removeBiome(int row) {
    if (row < 0 || row >= rowCount()) {
        return false;
    }
    
    // Disconnect the repository signal for atomic View row removal via begin/endRemoveRows
    disconnect(&m_repository, &Services::BiomeRepository::biomesChanged, this, &BiomeTableModel::refresh);
    
    beginRemoveRows(QModelIndex(), row, row);
    m_repository.deleteBiome(static_cast<std::size_t>(row));
    endRemoveRows();
    
    connect(&m_repository, &Services::BiomeRepository::biomesChanged, this, &BiomeTableModel::refresh);
    return true;
}

bool BiomeTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || role != Qt::EditRole) return false;

    const auto& biomes = m_repository.getBiomes();
    const std::size_t targetRow = static_cast<std::size_t>(index.row());
    
    if (index.row() < 0 || targetRow >= biomes.size()) {
        return false;
    }

    // Copy the structure for local mutation before writing back to the repository
    Domain::Biome updatedBiome = biomes[targetRow];

    switch (index.column()) {
        case Columns::CodeColumn: {
            updatedBiome.code = value.toInt();
            break;
        }
        case Columns::ColorColumn: {
            // Support both direct QColor instances and HEX string conversions
            QColor newColor = value.value<QColor>();
            if (!newColor.isValid()) {
                newColor = QColor(value.toString());
            }
            if (!newColor.isValid()) return false;
            
            updatedBiome.color = newColor;
            break;
        }
        case Columns::NameColumn: {
            updatedBiome.name = value.toString().trimmed();
            break;
        }
        case Columns::DescriptionColumn: {
            updatedBiome.description = value.toString().trimmed();
            break;
        }
        default:
            return false;
    }

    m_repository.updateBiome(targetRow, updatedBiome);
    
    // Notify the View about updates to both EditRole and DecorationRole for proper color repainting
    emit dataChanged(index, index, {Qt::EditRole, Qt::DecorationRole});
    return true;
}

void BiomeTableModel::refresh() {
    beginResetModel();
    endResetModel();
}

} // namespace Cajander::Gui
