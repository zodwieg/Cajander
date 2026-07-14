#include "biomeLoader.h"
#include "infrastructure/biomes/jsonBiomeStorage.h"
#include "services/biomes/biomeRepository.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

namespace Cajander::Gui {

BiomeLoader::BiomeLoader(Services::BiomeRepository& repository, QWidget* parent)
    : QObject(parent)
    , m_repository(repository)
    , m_parentWidget(parent)
{
}

void BiomeLoader::openFileDialog() {
    // Resolve platform-specific safe fallback points (e.g., ~/Documents on UNIX).
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    QString filePath = QFileDialog::getOpenFileName(
        m_parentWidget,
        tr("Open Biomes JSON File"),
        defaultPath,
        tr("JSON Files (*.json);;All Files (*)")
    );

    // Immediate exit path if the user aborts operation or closes file manager.
    if (filePath.isEmpty()) {
        return; 
    }

    QString errorMsg;
    if (!loadJsonFromFile(filePath, errorMsg)) {
        showErrorDialog(errorMsg);
    }
}

void BiomeLoader::showErrorDialog(const QString& text) {
    QMessageBox::critical(
        m_parentWidget, 
        tr("Loading Error"), 
        tr("Failed to load biomes:\n%1").arg(text)
    );
}

bool BiomeLoader::loadJsonFromFile(const QString& filePath, QString& errorMsg) {
    // RAII temporary storage allocation bound strictly to file-read transaction cycle.
    Cajander::Infrastructure::JsonBiomeStorage temporaryStorage(filePath);
    
    std::optional<Domain::BiomeScheme> loadedScheme = temporaryStorage.loadScheme();
    
    // Validation Stage 1: File integrity, structural health, and base serialization errors.
    if (!loadedScheme.has_value()) {
        errorMsg = tr("The file is corrupted, has an invalid format, or could not be read.");
        return false;
    }

    // Validation Stage 2: Ensure semantic rules are satisfied (non-empty configuration sets).
    if (loadedScheme->biomes.empty()) {
        errorMsg = tr("The scheme was loaded successfully, but it contains no biomes.");
        return false;
    }

    // Efficiently dispatch dataset updates downwards to the core thread-safe model using move semantics.
    m_repository.importScheme(std::move(*loadedScheme));

    return true; 
}

} // namespace Cajander::Gui
