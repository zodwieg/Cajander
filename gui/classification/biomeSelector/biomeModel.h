#pragma once

#include <QAbstractListModel>

// Опережающие объявления (Forward declarations) для уменьшения времени компиляции
namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

class BiomeModel : public QAbstractListModel {
    Q_OBJECT

public:
    // Роли данных. В Qt элемент списка может возвращать разные данные для разных задач.
    enum BiomeRoles {
        CodeRole = Qt::UserRole + 1, // Код биома (int), который мы спрячем внутри элемента
        DescriptionRole              // Полное описание биома для тултипов
    };

    // Принимаем репозиторий по ссылке. Объект должен существовать всё время жизни модели.
    explicit BiomeModel(const Services::BiomeRepository& repository, QObject* parent = nullptr);
    ~BiomeModel() override = default;

    // --- Обязательные методы интерфейса QAbstractListModel ---
    
    // Возвращает количество элементов в списке
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    
    // Возвращает данные для конкретного элемента по его индексу и роли
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    const Services::BiomeRepository& m_repository;
};

} // namespace Cajander::Gui
