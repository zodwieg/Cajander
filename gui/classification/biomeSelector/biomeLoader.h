#pragma once

#include <QObject>
#include <QWidget>
#include <QString>

namespace Cajander::Gui {

class BiomeModel;

class BiomeLoader : public QObject {
    Q_OBJECT

public:
    explicit BiomeLoader(BiomeModel* model, QWidget* parent = nullptr);
    ~BiomeLoader() override = default;

public slots:
    void openFileDialog();

private:
    bool loadJsonFromFile(const QString& filePath, QString& errorMsg);
    void showErrorDialog(const QString& text);

    BiomeModel* m_model{nullptr};
    QWidget* m_parentWidget{nullptr};
};

} // namespace Cajander::Gui
