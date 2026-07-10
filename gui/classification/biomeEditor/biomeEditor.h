#pragma once

#include <QDialog>
#include <memory>

class QTableView;
class QPushButton;

namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

class BiomeTableModel;

class BiomeEditor : public QDialog {
    Q_OBJECT

public:
    explicit BiomeEditor(Services::BiomeRepository& repository, QWidget* parent = nullptr);
    ~BiomeEditor() override = default;

private slots:
    void onLoadBiomesClicked();
    void onExportBiomesClicked();

private:
    void setupUi();

    Services::BiomeRepository& m_repository;
    BiomeTableModel* m_model{nullptr};
    
    QTableView* m_tableView{nullptr};
    QPushButton* m_loadButton{nullptr};
    QPushButton* m_exportButton{nullptr};
    QPushButton* m_closeButton{nullptr};
};

} // namespace Cajander::Gui
