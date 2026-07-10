#pragma once

#include <QObject>
#include <QWidget>
#include <QString>

// Опережающие объявления
namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

class BiomeLoader : public QObject {
    Q_OBJECT

public:
    explicit BiomeLoader(Services::BiomeRepository& repository, QWidget* parent = nullptr);
    ~BiomeLoader() override = default;

public slots:
    void openFileDialog();

private:
    bool loadJsonFromFile(const QString& filePath, QString& errorMsg);
    void showErrorDialog(const QString& text);

    Services::BiomeRepository& m_repository;
    QWidget* m_parentWidget{nullptr};
};

} // namespace Cajander::Gui
