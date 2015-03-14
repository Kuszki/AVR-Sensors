#include "sensordialog.hpp"
#include "ui_sensordialog.h"

#include "windows/mainwindow.hpp"

SensorDialog::SensorDialog(QWidget* parent, unsigned char uID)
: QDialog(parent), ID(uID), Interface(new Ui::SensorDialog)
{
	Interface->setupUi(this);

	if (!ID) setWindowTitle("Dodaj sensor");

	GetData(DefaultData);
}

SensorDialog::~SensorDialog(void)
{
	delete Interface;
}

bool SensorDialog::LoadSettings(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	query.prepare("SELECT name, expr, type, active, style, min,  max \
			    FROM sensors \
			    WHERE ID=:ID");

	query.bindValue(":ID", ID);

	if (query.exec() && query.next())
	{
		LastData.Name = query.value(0).toString();
		LastData.Equation = query.value(1).toString();
		LastData.Label = query.value(2).toString();
		LastData.Active = query.value(3).toBool();
		LastData.Style = query.value(4).toBool();
		LastData.Minimum = query.value(5).toDouble();
		LastData.Maximum = query.value(6).toDouble();
	}
	else	return false;

	SetData(LastData, true);

	return true;
}

bool SensorDialog::SaveSettings(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	if (ID) query.prepare(
				"UPDATE sensors SET \
				name=:name, \
				expr=:expr, \
				type=:type, \
				active=:active, \
				style=:style, \
				min=:min, \
				max=:max \
				WHERE ID=:ID");
	else	query.prepare(
				"INSERT INTO sensors \
				(name, expr, type, active, style, min, max) \
				VALUES \
				(:name, :expr, :type, :active, :style, :min, :max)");

	GetData(LastData);

	query.bindValue(":name", LastData.Name);
	query.bindValue(":expr", LastData.Equation);
	query.bindValue(":type", LastData.Label);
	query.bindValue(":active", LastData.Active);
	query.bindValue(":style", LastData.Style);
	query.bindValue(":min", LastData.Minimum);
	query.bindValue(":max", LastData.Maximum);
	query.bindValue(":ID", ID);

	if (!query.exec()) return false;

	if (ID)
		emit onSettingsAccept(LastData);
	else
		emit onWidgetAdd(query.lastInsertId().toUInt());

	return true;
}

bool SensorDialog::DeleteSettings(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	query.prepare("SELECT \
			    count(ID) \
			    FROM events WHERE \
			    SENSOR_ID=:ID");

	query.bindValue(":ID", ID);

	if (!query.exec() || (query.next() && query.value(0).toBool())) return false;

	query.prepare("DELETE FROM sensors WHERE \
			    ID=:ID");

	query.bindValue(":ID", ID);

	return query.exec();
}

void SensorDialog::GetData(SensorData& tData)
{
	tData.Name = Interface->Name->text();
	tData.Equation = Interface->Equation->document()->toPlainText();
	tData.Label = Interface->Type->text();
	tData.Minimum = Interface->Min->value();
	tData.Maximum = Interface->Max->value();
	tData.Active = Interface->Enabled->isChecked();
	tData.Style = Interface->StyleBar->isChecked();
}

void SensorDialog::SetData(SensorData& tData, bool bRefresh)
{
	Interface->Name->setText(tData.Name);
	Interface->Equation->document()->setPlainText(tData.Equation);
	Interface->Type->setText(tData.Label);
	Interface->Min->setValue(tData.Minimum);
	Interface->Max->setValue(tData.Maximum);
	Interface->Enabled->setChecked(tData.Active);
	Interface->StyleBar->setChecked(tData.Style);
	Interface->StyleLCD->setChecked(!tData.Style);

	if (bRefresh) emit onSettingsAccept(tData);
}

void SensorDialog::open(void)
{
	if (!ID) SetData(DefaultData, false);

	QDialog::open();
}

void SensorDialog::accept(void)
{
	const QString& Equation = Interface->Equation->document()->toPlainText();

	QScriptEngine Script;

	QScriptValue Result = Script.evaluate("x1=x2=x3=x4=x5=x6=1;" + Equation);

	if (Script.hasUncaughtException()) QMessageBox::warning(
				this,
				"Błąd",
				QString("Podane równanie zawiera błąd w linii %1\n\n\"%2\"")
				.arg(Script.uncaughtExceptionLineNumber())
				.arg(Result.toString()));
	else if (!SaveSettings()) QMessageBox::warning(
				this,
				"Błąd",
				"Nie udało się zapisać rekordu w bazie danych");
	else QDialog::accept();
}

void SensorDialog::reject(void)
{
	QDialog::reject();

	SetData(LastData, false);
}

void SensorDialog::onParamsChange(void)
{
	Interface->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
				Interface->Min->value() < Interface->Max->value() &&
				!Interface->Name->text().isEmpty() &&
				!Interface->Equation->document()->toPlainText().isEmpty()
				);
}
