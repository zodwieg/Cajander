#pragma once

#include <QObject>

namespace Cajander::Services {
    class BiomeRepository;
}

namespace Cajander::UI {

class BiomeExporter : public QObject {
    Q_OBJECT
public:
    explicit BiomeExporter(Cajander::Services::BiomeRepository* repository, QObject* parent = nullptr);
    ~BiomeExporter() override = default;
    void openSaveDialog();

private:
    Cajander::Services::BiomeRepository* m_repository{nullptr};
    QWidget* m_parentWidget{nullptr};
};

} // namespace Cajander::UI
