#include "biomeexporter.h"
#include "services/biomes/biomerepository.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

namespace Cajander::UI {

BiomeExporter::BiomeExporter(Cajander::Services::BiomeRepository* repository, QObject* parent)
    : QObject(parent)
    , m_repository(repository)
{
    Q_ASSERT_X(m_repository != nullptr, "BiomeExporter", "Repository pointer cannot be null");
}

void BiomeExporter::openSaveDialog() {
    // 1. Проверяем данные
    auto* parentWidget = qobject_cast<QWidget*>(parent());

    if (m_repository->getBiomes().empty()) {
        QMessageBox::warning(m_parentWidget, 
                             tr("Biomes export."), 
                             tr("The biome list is empty. There is nothing to export."));
        return;
    }

    // 2. Открываем нативный диалог сохранения
    QString selectedPath = QFileDialog::getSaveFileName(
        m_parentWidget,
        tr("Export to JSON"),
        QDir::homePath() + "/biomes_export.json",
        tr("JSON Files (*.json)")
    );

    if (selectedPath.isEmpty()) {
        return;
    }

    bool success = m_repository->exportBiomesTo(selectedPath);

    if (success) {
        QMessageBox::information(m_parentWidget, 
                                 tr("Success"), 
                                 tr("Biomes were successfully exported to:\n%1").arg(selectedPath));
    } else {
        QMessageBox::critical(m_parentWidget, 
                              tr("Error"), 
                              tr("Biomes were not exported. Please check your directory permissions."));
    }
}

} // namespace Cajander::UI
