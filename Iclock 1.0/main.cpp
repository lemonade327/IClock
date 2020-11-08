#include "AlarmClock.h"
#include <QtWidgets/QApplication>
#include "AlertTimeDlg.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/AlarmClock/Resources/clock.png"));
	
	//Do not exit the program when the last visualization window is closed
	a.setQuitOnLastWindowClosed(false);

	AlarmClock w;
	w.show();

// 	int width = GetSystemMetrics(SM_CXFULLSCREEN);
// 	int height = GetSystemMetrics(SM_CYFULLSCREEN);
// 
// 	AlertTimeDlg alertTimeDlg;
// 	//alertTimeDlg.move(QApplication::desktop()->width() - alertTimeDlg.width(), QApplication::desktop()->height() - alertTimeDlg.height());
// 	alertTimeDlg.move(width - alertTimeDlg.width(), height - alertTimeDlg.height());
// 	alertTimeDlg.show();

// 	AlertTimeDlg alertTimeDlg;
// 	alertTimeDlg.show();

	return a.exec();
}
