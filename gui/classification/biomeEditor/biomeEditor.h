#pragma once

#include <QDialog>
#include <memory>

class QTableView;
class QPushButton;
class QLineEdit;

namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

class BiomeTableModel;

class BiomeEditor : public QDialog {
    Q_OBJECT

public:
    explicit BiomeEditor(Services::BiomeRepository& repository, QWidget* parent = nullptr);
    ~BiomeEditor() override = default;

protected:
    void showEvent(QShowEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onLoadBiomesClicked();
    void onExportBiomesClicked();
    void onSchemeNameEdited();
    void updateSchemeNameFromRepo();
    void onAddBiomeClicked();
    void updateTableWidgets();

private:
    void setupUi();

    Services::BiomeRepository& m_repository;
    BiomeTableModel* m_model{nullptr};
    
    QTableView* m_tableView{nullptr};
    QLineEdit* m_schemeNameEdit{nullptr};
    QPushButton* m_loadButton{nullptr};
    QPushButton* m_exportButton{nullptr};
    QPushButton* m_closeButton{nullptr};
    QPushButton* m_addButton{nullptr};

    std::once_flag m_resourcesLoadedFlag;
};

} // namespace Cajander::Gui
