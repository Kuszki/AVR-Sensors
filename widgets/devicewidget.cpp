#include "devicewidget.hpp"
#include "ui_devicewidget.h"

DeviceWidget::DeviceWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DeviceWidget)
{
	ui->setupUi(this);
}

DeviceWidget::~DeviceWidget()
{
	delete ui;
}
