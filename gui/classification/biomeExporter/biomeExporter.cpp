#include "biomeexporter.h"
#include "services/biomes/biomerepository.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QRegularExpression>

namespace Cajander::UI {

BiomeExporter::BiomeExporter(Cajander::Services::BiomeRepository* repository, QObject* parent)
    : QObject(parent)
    , m_repository(repository)
{
    Q_ASSERT_X(m_repository != nullptr, "BiomeExporter", "Repository pointer cannot be null");
}

void BiomeExporter::openSaveDialog() {
    auto* parentWidget = qobject_cast<QWidget*>(parent());

    if (m_repository->getBiomes().empty()) {
        QMessageBox::warning(m_parentWidget, 
                             tr("Biomes export."), 
                             tr("The biome list is empty. There is nothing to export."));
        return;
    }

    QString safeSchemeName = m_repository->getSchemeName().trimmed();
    safeSchemeName.replace(QRegularExpression("[\\\\/:*?\"<>|]"), "_");
    if (safeSchemeName.isEmpty()) {
        safeSchemeName = "biomes_export";
    }

    QString selectedPath = QFileDialog::getSaveFileName(
        m_parentWidget,
        tr("Export to JSON"),
        QDir::homePath() + "/" + safeSchemeName + ".json",
        tr("JSON Files (*.json)")
    );

    if (selectedPath.isEmpty()) {
        return;
    }

    bool success = m_repository->exportSchemeTo(selectedPath);

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


} // namespace Cajander::UI
