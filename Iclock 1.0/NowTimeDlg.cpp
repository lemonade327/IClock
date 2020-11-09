#include "NowTimeDlg.h"
#include <windows.h>
#include <QTime>

NowTimeDlg::NowTimeDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//Remove?
	Qt::WindowFlags flags = Qt::Dialog; flags |= Qt::WindowCloseButtonHint;
	this->setWindowFlags(flags);
	this->setAttribute(Qt::WA_DeleteOnClose);
	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	this->move(width - this->width(), height - this->height());

	this->setWindowTitle("report punctually!");
	
	ui.label->setStyleSheet(QString("border-image:url(:/AlarmClock/Resources/bk.png);background-image:url(:/AlarmClock/Resources/bk.png);color: rgb(170, 0, 0);font: 16pt \"华文中宋\";"));
	
	QString strTime = QTime::currentTime().toString("HH:mm");
	if (QTime::currentTime().hour() < 12)
	{
		ui.label->setText("It is: morning" + strTime);
	}
	else if (QTime::currentTime().hour() == 12)
	{
		ui.label->setText("it's noon now" + strTime);
	}
	else if (QTime::currentTime().hour() > 12)
	{
		ui.label->setText("It is: afternoon" + strTime);
	}

	playList = NULL;
	player = NULL;
	speakTime(QTime::currentTime().hour());

	delayDlg = 10;	//Close the window after 10s
	num = 1;		
	startTimer(1000);
}

NowTimeDlg::~NowTimeDlg()
{
}

/************************************
@ Brief:		Close window event
************************************/
void NowTimeDlg::closeEvent(QCloseEvent *event)
{

}

/************************************
@ Brief:		Voice timekeeping   
************************************/
void NowTimeDlg::speakTime(const int hour)
{
	if (player)
	{
		delete player;
		player = NULL;
	}
	if (playList)
	{
		delete playList;
		playList = NULL;
	}
	
	QString strTimePath;

	playList = new QMediaPlaylist;
	playList->addMedia(QUrl::fromLocalFile("./Resources/sound/nowtime.wav"));
	if (hour<=7)
	{
		playList->addMedia(QUrl::fromLocalFile("./Resources/sound/am0.wav"));
		strTimePath = QString::asprintf("./Resources/sound/%d.wav", hour);
		playList->addMedia(QUrl::fromLocalFile(strTimePath));
	}
	else if ((hour>7)&&(hour<=12))
	{
		playList->addMedia(QUrl::fromLocalFile("./Resources/sound/am.wav"));
		strTimePath = QString::asprintf("./Resources/sound/%d.wav", hour);
		playList->addMedia(QUrl::fromLocalFile(strTimePath));
	}
	else if ((hour > 12) && (hour <= 20))
	{
		playList->addMedia(QUrl::fromLocalFile("./Resources/sound/pm.wav"));
		strTimePath = QString::asprintf("./Resources/sound/%d.wav", hour-12);
		playList->addMedia(QUrl::fromLocalFile(strTimePath));
	}
	else if (hour > 20)
	{
		playList->addMedia(QUrl::fromLocalFile("./Resources/sound/em.wav"));
		strTimePath = QString::asprintf("./Resources/sound/%d.wav", hour-12);
		playList->addMedia(QUrl::fromLocalFile(strTimePath));
	}
	playList->addMedia(QUrl::fromLocalFile("./Resources/sound/point.wav"));

	player = new QMediaPlayer;
	player->setPlaylist(playList);
	player->play();
}

/************************************
@ Brief:		Exit the window after 10S timing detection
************************************/
void NowTimeDlg::timerEvent(QTimerEvent *event)
{
	if (num++ >= delayDlg)
	{
		this->close();
	}
}
