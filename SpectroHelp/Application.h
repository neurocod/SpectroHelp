//Application.h by Kostya Kozachuck as neurocod
#pragma once
//#include <qtsingleapplication.h>

class Application:
	//public QtSingleApplication {
	public QApplication
{
	public:
		Application(int &argc, char **argv);
		virtual ~Application() {}
};