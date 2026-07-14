#pragma once

#include <QAbstractTableModel>

namespace Cajander::Services { 
class BiomeRepository; 
}

namespace Cajander::Domain {
struct Biome; 
}

namespace Cajander::Gui {

/**
 * @class BiomeTableModel
 * @brief Table model managing biome domain data for interactive UI editing.
 * 
 * Provides an interface between the BiomeRepository and QTableView, enabling
 * features like cell modification, custom rendering (colors), and action triggers
 * (row deletion).
 */
class BiomeTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /**
     * @brief Identifiers for table columns.
     */
    enum Columns {
        GripColumn = 0,     ///< Column for drag-and-drop handles or row selection indicators.
        CodeColumn,         ///< Unique code of the biome.
        NameColumn,         ///< Human-readable name of the biome.
        DescriptionColumn,  ///< Detailed description of the biome characteristics.
        ColorColumn,        ///< Visual color representation of the biome.
        DeleteColumn,       ///< Action column providing a button to delete the row.
        ColumnCount         ///< Helper constant representing the total number of columns.
    };

    /**
     * @brief Constructs the BiomeTableModel.
     * @param repository Reference to the service repository for persistent biome data.
     * @param parent Optional parent QObject for memory management.
     */
    explicit BiomeTableModel(Services::BiomeRepository& repository, QObject* parent = nullptr);

    /**
     * @brief Default virtual destructor.
     */
    ~BiomeTableModel() override = default;

    /**
     * @copydoc QAbstractTableModel::rowCount
     */
    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /**
     * @copydoc QAbstractTableModel::columnCount
     */
    [[nodiscard]] int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    
    /**
     * @copydoc QAbstractTableModel::data
     */
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    
    /**
     * @copydoc QAbstractTableModel::headerData
     */
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * @copydoc QAbstractTableModel::flags
     */
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;

    /**
     * @copydoc QAbstractTableModel::setData
     */
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    /**
     * @brief Appends a new biome to the model and updates the underlying repository.
     * @param newBiome The biome structure containing data to be added.
     */
    void addBiome(const Cajander::Domain::Biome& newBiome);

    /**
     * @brief Removes a biome row at the specified index from both the model and repository.
     * @param row Index of the row to be deleted.
     * @return True if the removal operation succeeded, false otherwise.
     */
    bool removeBiome(int row);

    /**
     * @brief Reloads data from the repository, resetting the model state.
     */
    void refresh();

private:
    Services::BiomeRepository& m_repository; ///< Reference to the storage backend.
};

} // namespace Cajander::Gui
