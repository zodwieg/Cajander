#pragma once

#include <QWidget>

class QComboBox;
class QPushButton;
class QAbstractItemModel;
class QLabel;

namespace Cajander::Gui {

    class BiomeModel;
    class BiomeLoader;

    class BiomeSelector : public QWidget {
        Q_OBJECT

    public:
        explicit BiomeSelector(QWidget* parent = nullptr);
        ~BiomeSelector() override = default;

    signals:
        void biomeChanged(const QModelIndex& index);

        void loadBiomesRequested();
        void editBiomesRequested();
        

    protected: 
        void showEvent(QShowEvent* event) override;

    private slots:
        void onComboBoxIndexChanged(int index);
        void updateSchemeLabel();

    private:
        QComboBox* m_comboBox{nullptr};
        QPushButton* m_loadButton{nullptr};
        QPushButton* m_editButton{nullptr};
        QLabel* m_schemeNameLabel{nullptr};
        Cajander::Gui::BiomeModel* m_model{nullptr};

        std::unique_ptr<BiomeLoader> m_loader;

        void onLoadBiomesClicked();
        void onEditBiomesClicked();

        bool m_resourcesLoaded = false;
    };
}
