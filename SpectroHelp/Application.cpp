//Application.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "Application.h"

Application::Application(int &argc, char **argv): QApplication(argc, argv) {
	QCoreApplication::setOrganizationName("SpectroHelp.com");
	QCoreApplication::setOrganizationDomain("SpectroHelp.com");
	QCoreApplication::setApplicationName("SpectroHelp");
	QSettings::setDefaultFormat(QSettings::IniFormat);//as Windows registry has limiations by value length
	//connect(qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));
}