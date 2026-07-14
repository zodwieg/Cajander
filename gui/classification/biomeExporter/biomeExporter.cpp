#include "biomeexporter.h"
#include "services/biomes/biomerepository.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QRegularExpression>

namespace Cajander::Gui {

BiomeExporter::BiomeExporter(Services::BiomeRepository* repository, QObject* parent)
    : QObject(parent)
    , m_repository(repository)
    // Safely cast parent QObject to QWidget for downstream UI positioning contexts.
    , m_parentWidget(qobject_cast<QWidget*>(parent))
{
    // Fail-fast assertion boundary check validating input storage parameter integrity.
    Q_ASSERT_X(m_repository != nullptr, "BiomeExporter", "Repository pointer cannot be null");
}

void BiomeExporter::openSaveDialog() {
    // Structural Invariant Validation: Do not trigger export workflows on empty datasets.
    if (m_repository->getBiomes().empty()) {
        QMessageBox::warning(m_parentWidget, 
                             tr("Biomes Export"), 
                             tr("The biome list is empty. There is nothing to export."));
        return;
    }

    // Sanitize string metrics to filter out platform-illegal filesystem symbol structures.
    QString safeSchemeName = m_repository->getSchemeName().trimmed();
    safeSchemeName.replace(QRegularExpression("[\\\\/:*?\"<>|]"), "_");
    if (safeSchemeName.isEmpty()) {
        safeSchemeName = "biomes_export";
    }

    // Launch native operational save request framework overlay.
    QString selectedPath = QFileDialog::getSaveFileName(
        m_parentWidget,
        tr("Export to JSON"),
        QDir::homePath() + "/" + safeSchemeName + ".json",
        tr("JSON Files (*.json)")
    );

    // Immediate exit path if transaction execution is cancelled.
    if (selectedPath.isEmpty()) {
        return;
    }

    // Delegate deep serialization write routines down to core logic services.
    bool success = m_repository->exportSchemeTo(selectedPath);

    // Provide immediate reactive modal toast feedback summarizing structural status.
    if (success) {
        QMessageBox::information(m_parentWidget, 
                                 tr("Success"), 
                                 tr("Biome scheme was successfully exported to:\n%1").arg(selectedPath));
    } else {
        QMessageBox::critical(m_parentWidget, 
                              tr("Error"), 
                              tr("Scheme was not exported. Please check your directory permissions."));
    }
}

} // namespace Cajander::Gui
