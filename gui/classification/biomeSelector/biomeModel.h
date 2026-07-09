#pragma once

#include <QAbstractListModel>

// Опережающие объявления (Forward declarations) для уменьшения времени компиляции
namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

class BiomeModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum BiomeRoles {
        CodeRole = Qt::UserRole + 1,
        DescriptionRole
    };

    explicit BiomeModel(const Services::BiomeRepository& repository, QObject* parent = nullptr);
    ~BiomeModel() override = default;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void refresh();

private:
    const Services::BiomeRepository& m_repository;
};

} // namespace Cajander::Gui
