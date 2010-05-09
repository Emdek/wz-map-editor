#include "DockWidget.h"

namespace WZMapEditor
{

DockWidget::DockWidget(const QString &title, QWidget *parent) : QDockWidget(title, parent)
{
}

void DockWidget::showEvent(QShowEvent *event)
{
	raise();

	QDockWidget::showEvent(event);
}

}
