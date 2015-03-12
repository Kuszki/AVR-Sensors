#include "eventdialog.hpp"
#include "ui_eventdialog.h"

EventDialog::EventDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EventDialog)
{
	ui->setupUi(this);
}

EventDialog::~EventDialog()
{
	delete ui;
}
