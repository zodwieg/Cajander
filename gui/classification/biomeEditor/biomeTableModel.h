#pragma once

#include <QAbstractTableModel>

namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Domain {
    struct Biome; 
}

namespace Cajander::Gui {

class BiomeTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum Columns {
        GripColumn = 0,
        CodeColumn,
        NameColumn,
        DescriptionColumn,
        ColorColumn,
        DeleteColumn,
        ColumnCount
    };

    explicit BiomeTableModel(Services::BiomeRepository& repository, QObject* parent = nullptr);
    ~BiomeTableModel() override = default;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    void addBiome(const Cajander::Domain::Biome& newBiome);
    bool removeBiome(int row);
    void refresh();

private:
    Services::BiomeRepository& m_repository;
};

} // namespace Cajander::Gui
