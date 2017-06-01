#include "wireframe_widget.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QSharedPointer>

#include "settings.h"
#include "generatrix_view.h"
#include "canvas3d.h"
#include "configuration.h"

WireframeWidget::WireframeWidget(QSharedPointer<Configuration> configuration, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QTabWidget *tab_widget = new QTabWidget(this);

    GeneratrixView *generatrix_view_widget = new GeneratrixView(configuration, this);
    Canvas3D *canvas_3d = new Canvas3D(configuration, this);
    tab_widget->addTab(canvas_3d, "3D &canvas");
    tab_widget->addTab(generatrix_view_widget, "&Generatrix");

    Settings *settings = new Settings(configuration, this);
    layout->addWidget(tab_widget);
    layout->addWidget(settings);
}
