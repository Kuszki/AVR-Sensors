#include "eventwidget.hpp"
#include "ui_eventwidget.h"

EventWidget::EventWidget(QWidget *parent)
: QWidget(parent), Interface(new Ui::EventWidget)
{
	Interface->setupUi(this);
}

EventWidget::~EventWidget()
{
	delete Interface;
}
