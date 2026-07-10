#pragma once

#include <QAbstractTableModel>

namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

class BiomeTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    // Перечисление колонок для удобства и читаемости кода
    enum Columns {
        CodeColumn = 0,
        NameColumn,
        DescriptionColumn,
        ColumnCount
    };

    explicit BiomeTableModel(Services::BiomeRepository& repository, QObject* parent = nullptr);
    ~BiomeTableModel() override = default;

    // Системные методы Qt для отображения структуры таблицы
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    
    // Метод чтения: View запрашивает данные для конкретной ячейки (строка/колонка)
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    
    // Метод чтения: Заголовки колонок ("Код", "Название"...)
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Методы для обеспечения РЕДАКТИРОВАНИЯ
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    void refresh();

private:
    // Ссылка НЕ const, так как через эту модель мы будем изменять данные в репозитории
    Services::BiomeRepository& m_repository;
};

} // namespace Cajander::Gui
