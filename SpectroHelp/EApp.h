//EApp.h by Kostya Kozachuck as neurocod
#pragma once
//#include <qtsingleapplication.h>

class EApp:
	//public QtSingleApplication {
	public QApplication
{
	public:
		EApp(int &argc, char **argv);
		virtual ~EApp() {}
};