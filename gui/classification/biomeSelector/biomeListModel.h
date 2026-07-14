/**
 * @file BiomeListModel.h
 * @brief List model providing data for biome selection components.
 */

#pragma once

#include <QAbstractListModel>

namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

/**
 * @class BiomeListModel
 * @brief Adaptor class that exposes BiomeRepository domain data to Qt list views.
 *
 * Implements a read-only flat list model suitable for combo boxes and simple lists.
 * Automatically synchronizes updates whenever the underlying repository dataset changes.
 */
class BiomeListModel : public QAbstractListModel {
    Q_OBJECT

public:
    /**
     * @brief Custom item data roles for extended model queries.
     */
    enum BiomeRoles {
        CodeRole = Qt::UserRole + 1, ///< Returns the integer unique identifier of the forest biome.
        DescriptionRole              ///< Returns the detailed text description string.
    };

    /**
     * @brief Constructs the biome list model.
     * @param repository Thread-safe domain service providing access to forest biomes.
     * @param parent Pointer to the parent QObject lifecycle owner.
     */
    explicit BiomeListModel(const Services::BiomeRepository& repository, QObject* parent = nullptr);

    /**
     * @copydoc QAbstractListModel::~QAbstractListModel
     */
    ~BiomeListModel() override = default;

    /**
     * @copydoc QAbstractListModel::rowCount
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /**
     * @copydoc QAbstractListModel::data
     */
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Forces the view component to completely reload the underlying data structure.
     */
    void refresh();

private:
    const Services::BiomeRepository& m_repository; ///< Reference to the back-end data repository.
};

} // namespace Cajander::Gui
