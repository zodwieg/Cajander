#include <algorithm>
#include <QMessageBox>
#include "biomeTableModel.h"
#include "services/biomes/biomeRepository.h"
#include "domain/biome.h"

namespace Cajander::Gui {

BiomeTableModel::BiomeTableModel(Services::BiomeRepository& repository, QObject* parent)
    : QAbstractTableModel(parent)
    , m_repository(repository)
{
    connect(&repository, &Services::BiomeRepository::biomesChanged, this, &BiomeTableModel::refresh);
}

int BiomeTableModel::rowCount(const QModelIndex& parent) const {
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

    const auto& biome = biomes[static_cast<std::size_t>(index.row())];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case Columns::CodeColumn:        return biome.code;
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
        case Columns::CodeColumn:        return tr("Code");
        case Columns::NameColumn:        return tr("Name");
        case Columns::DescriptionColumn: return tr("Description");
        default:                         return QVariant();
    }
}

Qt::ItemFlags BiomeTableModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool BiomeTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || role != Qt::EditRole) return false;

    const auto& biomes = m_repository.getBiomes();
    const std::size_t targetRow = static_cast<std::size_t>(index.row());
    
    if (index.row() < 0 || targetRow >= biomes.size()) {
        return false;
    }

    Domain::Biome updatedBiome = biomes[targetRow];

    switch (index.column()) {
        case Columns::CodeColumn: {
            bool ok = false;
            const int newCode = value.toInt(&ok);
            if (!ok) {
                QMessageBox::warning(
                    nullptr, 
                    tr("Validation error"), 
                    tr("Biome code must be integer")
                );
                return false;
            }

            if (newCode != updatedBiome.code) {
                const bool codeExists = std::any_of(biomes.begin(), biomes.end(), 
                    [newCode](const Domain::Biome& b) { 
                        return b.code == newCode; 
                    }
                );

                if (codeExists) {
                    QMessageBox::warning(
                        nullptr, 
                        tr("Validation error"), 
                        tr("Biome code %1 already exists. Codes have to be unique.").arg(newCode)
                    );
                    return false;
                }
            }

            updatedBiome.code = newCode;
            break;
        }
        case Columns::NameColumn: {
            const QString trimmedName = value.toString().trimmed();
            if (trimmedName.isEmpty()) {
                QMessageBox::warning(nullptr, tr("Validation error"), tr("Biome name is required."));
                return false;
            }
            updatedBiome.name = trimmedName;
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
    
    emit dataChanged(index, index, {role});
    return true;
}


void BiomeTableModel::refresh() {
    beginResetModel();
    endResetModel();
}

} // namespace Cajander::Gui
