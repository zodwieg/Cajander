#include "biomeLoader.h"
#include "infrastructure/biomes/jsonBiomeStorage.h"
#include "services/biomes/biomeRepository.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

namespace Cajander::Gui {

// Инициализируем ссылку на репозиторий
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
    
    std::vector<Domain::Biome> loadedBiomes = temporaryStorage.loadBiomes();
    
    if (loadedBiomes.empty()) {
        errorMsg = tr("The file is empty, corrupted, or has an invalid format.");
        return false;
    }
    m_repository.importBiomes(std::move(loadedBiomes));

    return true; 
}

} // namespace Cajander::Gui
