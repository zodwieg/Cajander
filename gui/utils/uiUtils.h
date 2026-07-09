#pragma once

#include <QPushButton>
#include <QString>
#include <QIcon>
#include <QDebug>
#include <qgsapplication.h>

namespace Cajander::Gui::Utils {

    /**
     * @brief Safely loads a QGIS icon and setis it as a button icon
     * @param button Pointer to a Qt button
     * @param iconName Icon file mane (ex.g. "/mActionFileOpen.svg")
     * @param widgetContext Widget name for error logging (ex.g. "BiomeSelector")
     */
    inline void setupThemeIcon(QPushButton* button, const QString& iconName, const QString& widgetContext) 
    {
        if (!button) {
            qWarning() << "Cajander [" << widgetContext << "]: Null button pointer passed to setupThemeIcon!";
            return;
        }
        
        QIcon icon = QgsApplication::getThemeIcon(iconName);
        
        if (!icon.isNull()) {
            button->setIcon(icon);
        } else {
            qWarning() << "Cajander [" << widgetContext << "]: Failed to load theme icon:" << iconName;
        }
    }

} // namespace Cajander::Gui::Utils
