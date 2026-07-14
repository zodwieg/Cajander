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
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    QString filePath = QFileDialog::getOpenFileName(
        m_parentWidget,
        tr("Open Biomes JSON File"),
        defaultPath,
        tr("JSON Files (*.json);;All Files (*)")
    );

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
    Cajander::Infrastructure::JsonBiomeStorage temporaryStorage(filePath);
    
    std::optional<Domain::BiomeScheme> loadedScheme = temporaryStorage.loadScheme();
    
    if (!loadedScheme.has_value()) {
        errorMsg = tr("The file is corrupted, has an invalid format, or could not be read.");
        return false;
    }

    if (loadedScheme->biomes.empty()) {
        errorMsg = tr("The scheme was loaded successfully, but it contains no biomes.");
        return false;
    }

    m_repository.importScheme(std::move(loadedScheme.value()));

    return true; 
}

} // namespace Cajander::Gui
