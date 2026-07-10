#include "biomeEditor.h"
#include "biomeTableModel.h"

#include "services/biomes/biomeRepository.h"
#include "gui/classification/biomeLoader/biomeLoader.h"
#include "gui/classification/biomeExporter/biomeExporter.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QDialog>
#include <QWidget>
#include <QObject>
#include <QHeaderView>

namespace Cajander::Gui {

BiomeEditor::BiomeEditor(Services::BiomeRepository& repository, QWidget* parent)
    : QDialog(parent)
    , m_repository(repository)
{
    setupUi();
    
    m_model = new BiomeTableModel(m_repository, this);
    m_tableView->setModel(m_model);
    
    m_tableView->horizontalHeader()->setSectionResizeMode(BiomeTableModel::Columns::CodeColumn, QHeaderView::ResizeToContents);
    m_tableView->horizontalHeader()->setSectionResizeMode(BiomeTableModel::Columns::NameColumn, QHeaderView::Stretch);
    m_tableView->horizontalHeader()->setSectionResizeMode(BiomeTableModel::Columns::DescriptionColumn, QHeaderView::Stretch);
}

void BiomeEditor::setupUi() {
    resize(700, 450);
    setWindowTitle(tr("Biomes List Editor"));

    auto* mainLayout = new QVBoxLayout(this);

    m_tableView = new QTableView(this);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    mainLayout->addWidget(m_tableView);

    auto* buttonLayout = new QHBoxLayout();
    
    m_loadButton = new QPushButton(tr("Load biomes..."), this);
    m_exportButton = new QPushButton(tr("Export biomes..."), this);
    m_closeButton = new QPushButton(tr("Close"), this);
    
    buttonLayout->addWidget(m_loadButton);
    buttonLayout->addStretch(); 
    buttonLayout->addWidget(m_exportButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_closeButton);
    
    mainLayout->addLayout(buttonLayout);

    connect(m_loadButton, &QPushButton::clicked, this, &BiomeEditor::onLoadBiomesClicked);
    connect(m_exportButton, &QPushButton::clicked, this, &BiomeEditor::onExportBiomesClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void BiomeEditor::onLoadBiomesClicked() {
    auto* loader = new BiomeLoader(m_repository, this);
    loader->openFileDialog();
    loader->deleteLater();
}

void BiomeEditor::onExportBiomesClicked() {
    auto* exporter = new Cajander::UI::BiomeExporter(&m_repository, this);
    exporter->openSaveDialog();
    exporter->deleteLater();
}

} // namespace Cajander::Gui
