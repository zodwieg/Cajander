#include "biomeEditor.h"
#include "biomeTableModel.h"
#include "biomeItemDelegate.h"
#include "gui/utils/uiUtils.h" 

#include "services/biomes/biomeRepository.h"
#include "gui/classification/biomeLoader/biomeLoader.h"
#include "gui/classification/biomeExporter/biomeExporter.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHeaderView>
#include <QKeyEvent>
#include <QColorDialog>

namespace Cajander::Gui {

BiomeEditor::BiomeEditor(Services::BiomeRepository& repository, QWidget* parent)
    : QDialog(parent)
    , m_repository(repository)
{
    setupUi();
    
    m_model = new BiomeTableModel(m_repository, this);
    m_tableView->setModel(m_model);

    m_tableView->setItemDelegate(new BiomeItemDelegate(m_repository, this));
    
    // Configure column geometry and resizing behavior
    m_tableView->horizontalHeader()->setSectionResizeMode(BiomeTableModel::Columns::GripColumn, QHeaderView::Fixed);
    m_tableView->setColumnWidth(BiomeTableModel::Columns::GripColumn, 30);
    
    m_tableView->horizontalHeader()->setSectionResizeMode(BiomeTableModel::Columns::CodeColumn, QHeaderView::ResizeToContents);
    
    m_tableView->horizontalHeader()->setSectionResizeMode(BiomeTableModel::Columns::ColorColumn, QHeaderView::Fixed);
    m_tableView->setColumnWidth(BiomeTableModel::Columns::ColorColumn, 60);
    
    m_tableView->horizontalHeader()->setSectionResizeMode(BiomeTableModel::Columns::NameColumn, QHeaderView::Stretch);
    m_tableView->horizontalHeader()->setSectionResizeMode(BiomeTableModel::Columns::DescriptionColumn, QHeaderView::Stretch);
    
    m_tableView->horizontalHeader()->setSectionResizeMode(BiomeTableModel::Columns::DeleteColumn, QHeaderView::Fixed);
    m_tableView->setColumnWidth(BiomeTableModel::Columns::DeleteColumn, 30);

    updateSchemeNameFromRepo();

    // Track model structure changes to re-inject custom inline widgets
    connect(m_model, &BiomeTableModel::rowsInserted, this, &BiomeEditor::updateTableWidgets);
    connect(m_model, &BiomeTableModel::rowsRemoved, this, &BiomeEditor::updateTableWidgets);
    connect(m_model, &BiomeTableModel::modelReset, this, &BiomeEditor::updateTableWidgets);

    updateTableWidgets();

    // Re-sync dialog headers if database state changes globally
    connect(&m_repository, &Services::BiomeRepository::biomesChanged, this, &BiomeEditor::updateSchemeNameFromRepo);
    m_tableView->installEventFilter(this);
}

void BiomeEditor::setupUi() {
    resize(700, 450);
    setWindowTitle(tr("Biomes List Editor"));

    auto* mainLayout = new QVBoxLayout(this);

    auto* topLayout = new QHBoxLayout();
    auto* schemeLabel = new QLabel(tr("Classification Scheme:"), this);
    m_schemeNameEdit = new QLineEdit(this);
    m_schemeNameEdit->setPlaceholderText(tr("Enter scheme name..."));
    
    m_loadButton = new QPushButton(this);
    m_exportButton = new QPushButton(this);
    m_loadButton->setToolTip(tr("Load biomes from file..."));
    m_exportButton->setToolTip(tr("Export biomes to file..."));
    m_loadButton->setFixedSize(30, 24);
    m_exportButton->setFixedSize(30, 24);
    
    topLayout->addWidget(schemeLabel);
    topLayout->addWidget(m_schemeNameEdit, 1);
    topLayout->addWidget(m_loadButton);
    topLayout->addWidget(m_exportButton);
    mainLayout->addLayout(topLayout);

    m_tableView = new QTableView(this);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    mainLayout->addWidget(m_tableView);

    auto* bottomLayout = new QHBoxLayout();
    
    m_addButton = new QPushButton(this);
    m_addButton->setFixedSize(30, 24);
    m_addButton->setToolTip(tr("Add new biome"));

    m_closeButton = new QPushButton(tr("Close"), this);
    
    bottomLayout->addWidget(m_addButton);
    bottomLayout->addStretch(); 
    bottomLayout->addWidget(m_closeButton);
    mainLayout->addLayout(bottomLayout);

    connect(m_schemeNameEdit, &QLineEdit::editingFinished, this, &BiomeEditor::onSchemeNameEdited);
    connect(m_loadButton, &QPushButton::clicked, this, &BiomeEditor::onLoadBiomesClicked);
    connect(m_exportButton, &QPushButton::clicked, this, &BiomeEditor::onExportBiomesClicked);
    connect(m_addButton, &QPushButton::clicked, this, &BiomeEditor::onAddBiomeClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void BiomeEditor::onSchemeNameEdited() {
    const QString newName = m_schemeNameEdit->text().trimmed();
    m_repository.updateSchemeName(newName);
}

void BiomeEditor::updateSchemeNameFromRepo() {
    const QString currentName = m_repository.getSchemeName(); 
    if (m_schemeNameEdit->text() != currentName) {
        m_schemeNameEdit->setText(currentName);
    }
}

void BiomeEditor::onLoadBiomesClicked() {
    auto* loader = new BiomeLoader(m_repository, this);
    loader->openFileDialog();
    loader->deleteLater();
}

void BiomeEditor::onExportBiomesClicked() {
    auto* exporter = new Cajander::Gui::BiomeExporter(&m_repository, this);
    exporter->openSaveDialog();
    exporter->deleteLater();
}

void BiomeEditor::onAddBiomeClicked() {
    Cajander::Domain::Biome newBiome;
    newBiome.code = 0;
    newBiome.name = tr("New Biome");
    newBiome.description = "";
    newBiome.color = QColor(Qt::gray);

    m_model->addBiome(newBiome);
}

void BiomeEditor::updateTableWidgets() {
    const QSize iconSize(16, 16);

    // Iterating over rows to inject widgets into specific cells dynamically
    for (int row = 0; row < m_model->rowCount(); ++row) {
        
        // Setup visual grip handle indicator for row drag/selection context
        QModelIndex gripIndex = m_model->index(row, BiomeTableModel::Columns::GripColumn);
        if (!m_tableView->indexWidget(gripIndex)) {
            auto* gripLabel = new QLabel(m_tableView);
            gripLabel->setText("⁞⁞"); 
            gripLabel->setAlignment(Qt::AlignCenter);
            gripLabel->setCursor(Qt::OpenHandCursor);

            QFont font = gripLabel->font();
            font.setPixelSize(14);
            font.setBold(false);
            gripLabel->setFont(font);

            gripLabel->setStyleSheet(
                "QLabel { "
                "  color: #aaaaaa; "
                "  background: transparent; "
                "  padding-top: 0px; "
                "  padding-bottom: 2px; " 
                "  margin-top: 0px; "
                "  margin-bottom: 1px; "
                "}"
            );
            m_tableView->setIndexWidget(gripIndex, gripLabel);
        }

        // Setup individual transactional 'Delete' button per table row
        QModelIndex deleteIndex = m_model->index(row, BiomeTableModel::Columns::DeleteColumn);
        if (!m_tableView->indexWidget(deleteIndex)) {
            auto* deleteButton = new QPushButton(m_tableView);
            deleteButton->setFlat(true);
            deleteButton->setToolTip(tr("Delete biome"));
            
            Cajander::Gui::Utils::setupThemeIcon(deleteButton, QString("/mActionDeleteSelected.svg"), QString("BiomeEditor"));
            deleteButton->setIconSize(iconSize);
            deleteButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
            
            // Capture the specific row index for the destruction signal callback
            connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                m_model->removeBiome(row);
            });

            m_tableView->setIndexWidget(deleteIndex, deleteButton);
        }
    }
}

void BiomeEditor::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);

    // Guarantee thread-safe icon resolving and SVG theme extraction triggers once UI is mapped
    std::call_once(m_resourcesLoadedFlag, [this]() {
        Cajander::Gui::Utils::setupThemeIcon(m_loadButton, QString("/mActionFileOpen.svg"), QString("BiomeEditor"));
        Cajander::Gui::Utils::setupThemeIcon(m_exportButton, QString("/mActionFileSaveAs.svg"), QString("BiomeEditor"));
        Cajander::Gui::Utils::setupThemeIcon(m_addButton, QString("/mActionAdd.svg"), QString("BiomeEditor"));
    });
}

bool BiomeEditor::eventFilter(QObject* watched, QEvent* event) {
    // Intercept keyboard events within the grid to improve accessibility handles
    if (watched == m_tableView && event->type() == QEvent::KeyPress) {
        auto* keyEvent = static_cast<QKeyEvent*>(event);
        
        if (keyEvent->key() == Qt::Key_Space) {
            QModelIndex currentIndex = m_tableView->currentIndex();
            
            if (currentIndex.isValid() && currentIndex.column() == BiomeTableModel::Columns::ColorColumn) {
                QColor currentColor = currentIndex.data(Qt::EditRole).value<QColor>();
                QColor chosenColor = QColorDialog::getColor(currentColor, m_tableView, tr("Select Biome Color"));
                
                if (chosenColor.isValid() && chosenColor != currentColor) {
                    m_tableView->model()->setData(currentIndex, chosenColor, Qt::EditRole);
                }
                return true; // Consume event to prevent view from scrolling down or passing focus
            }
        }
    }
    return QDialog::eventFilter(watched, event);
}

} // namespace Cajander::Gui
