#include "AlarmClock.h"
#include <QPainter>
#include "AlertTimeDlg.h"
#include <QTime>
#include <QMessageBox>
#include "AboutDlg.h"
#include "NowTimeDlg.h"

AlarmClock::AlarmClock(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//No title bar (no border)
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	setAttribute(Qt::WA_TranslucentBackground);
	m_titleImage.load(":/AlarmClock/Resources/background.jpg");

//System tray
	//Set prompt text
	m_systray.setToolTip("Iclock");
	// Set tray icon
	m_systray.setIcon(QIcon(":/AlarmClock/Resources/clock.png"));
	//Tray menu item
	QMenu * menu = new QMenu();
	menu->addAction(ui.actionExit);
	m_systray.setContextMenu(menu);
	//Show tray
	m_systray.show();

	m_systray.show();
//The main page close button, the most down button
	closeButton = new QPushButton(this);
	//closeButton->setIcon(QIcon(":/AlarmClock/Resources/close_normal.png"));
	closeButton->setGeometry(this->rect().right() - 45, 10, 40, 30);
	//closeButton->setFlat(true);
	closeButton->setStyleSheet("QPushButton{background-color:transparent; border-image: url(:/AlarmClock/Resources/close.png);}"
	"QPushButton:hover{background-color:rgb(6,168,240); border-image: url(:/AlarmClock/Resources/close.png);}"
	"QPushButton:pressed{background-color: rgb(6,168,220); border-image: url(:/AlarmClock/Resources/close.png);}");

	minButton = new QPushButton(this);
	minButton->setGeometry(this->rect().right() - 85, 10, 40, 30);
	minButton->setStyleSheet("QPushButton{background-color:transparent; border-image: url(:/AlarmClock/Resources/min.png);}"
		"QPushButton:hover{background-color:rgb(6,168,240); border-image: url(:/AlarmClock/Resources/min.png);}"
		"QPushButton:pressed{background-color: rgb(6,168,220); border-image: url(:/AlarmClock/Resources/min.png);}");

	aboutButton = new QPushButton(this);
	aboutButton->setGeometry(this->rect().right() - 125, 10, 40, 30);
	aboutButton->setStyleSheet("QPushButton{background-color:transparent; border-image: url(:/AlarmClock/Resources/about.png);}"
		"QPushButton:hover{background-color:rgb(6,168,240); border-image: url(:/AlarmClock/Resources/about.png);}"
		"QPushButton:pressed{background-color: rgb(6,168,220); border-image: url(:/AlarmClock/Resources/about.png);}");

	//labelTime display time
	QString strDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd  hh:mm:ss");
	ui.labelTime->setText(strDateTime);

	//Start timer
	timerId = startTimer(1000);

	//Tab1
	ui.btnStart1Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
		"QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
		"QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
	ui.btnStart2Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
		"QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
		"QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");


	//Initialize variables
	player = NULL;
	player2 = NULL;
	alertTimeDlg = NULL;
	alertTimeDlg2 = NULL;
	bRuningClock1 = false;
	bRuningClock2 = false;
	int clock1HH = 0;
	int clock1MM = 0;
	int clock2HH = 0;
	int clock2MM = 0;


	//Binding response event
	connect(minButton, SIGNAL(clicked()), this, SLOT(OnBtnMin()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(OnBtnClose()));
	connect(aboutButton, SIGNAL(clicked()), this, SLOT(OnBtnAbout()));
	connect(&m_systray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(OnSystemTrayClicked(QSystemTrayIcon::ActivationReason)));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(OnExit()));
	
	connect(ui.btnStart1Clock, &QPushButton::clicked, this, &AlarmClock::OnBtnStartClock1);
	connect(ui.btnStart2Clock, &QPushButton::clicked, this, &AlarmClock::OnBtnStartClock2);

	connect(ui.listenBtn1, &QPushButton::clicked, this, &AlarmClock::OnListen1);
	connect(ui.listenBtn2, &QPushButton::clicked, this, &AlarmClock::OnListen2);
}


void AlarmClock::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRect frameRect = rect();
	frameRect.adjust(1, 1, -1, -1);

	this->resize(452, 549);

	//Paint background and border
	painter.setPen(QColor(0x66, 0x66, 0x66));
	painter.setBrush(QColor(0xFF, 0xFF, 0xFF));
	painter.drawRoundRect(frameRect, 2, 2);


	//Draw title bar
	QRect titleRect = frameRect;
	titleRect.setBottom(frameRect.top() + 80);

	//Set the rounded corners after adding the title bar background image
	QPainterPath path;
	path.addRoundRect(frameRect.adjusted(1, 1, 0, -1), 2, 2);
	path.intersects(titleRect);
	painter.setClipPath(path);

	painter.drawPixmap(titleRect, m_titleImage);
}



/************************************
@ Brief:		Mouse press response function
************************************/
void AlarmClock::mousePressEvent(QMouseEvent *event)
{
	// Only respond to the left button
	if (event->button() == Qt::LeftButton)
	{
		QRect titleRect = rect();
		titleRect.setBottom(titleRect.top() + 80);

		if (titleRect.contains(event->pos()))
		{
			m_dragging = true;
			m_startPosition = event->globalPos();
			m_framePosition = frameGeometry().topLeft();
		}
	}

	QWidget::mousePressEvent(event);
}

/************************************
@ Brief:		Mouse movement response function
************************************/
void AlarmClock::mouseMoveEvent(QMouseEvent *event)
{
	// Only respond to the left button
	if (event->buttons() & Qt::LeftButton)
	{
		if (m_dragging)
		{
			// delta Relative offset,
			QPoint delta = event->globalPos() - m_startPosition;

			// New position: Form original position + offset
			move(m_framePosition + delta);
		}
	}

	QWidget::mouseMoveEvent(event);
}

/************************************
@ Brief:		Mouse release response function
************************************/
void AlarmClock::mouseReleaseEvent(QMouseEvent * event)
{
	m_dragging = false;
	QWidget::mouseReleaseEvent(event);
}



/************************************
@ Brief:		Timer
************************************/
void AlarmClock::timerEvent(QTimerEvent *event)
{
	ui.labelTime->setText("");
	QString strDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd  hh:mm:ss");
	ui.labelTime->setText(strDateTime);

	if (bRuningClock1)
	{
		QTime sysTime = QTime::currentTime();
		if ((sysTime.hour()== clock1HH) && (sysTime.minute()== clock1MM))
		{
			if (alertTimeDlg==NULL)
			{
				alertTimeDlg = new AlertTimeDlg(this, "./Resources/sound/sound1.mp3", "clock1：" + ui.textEdit1->toPlainText());

				int width = GetSystemMetrics(SM_CXFULLSCREEN);
				int height = GetSystemMetrics(SM_CYFULLSCREEN);
				alertTimeDlg->move(width - alertTimeDlg->width(), height - alertTimeDlg->height());
				if (!bSystray)
				{
					this->setWindowFlags(Qt::WindowStaysOnTopHint);//It does not take effect in the tray case, it can be displayed directly by show, and this item needs to be set in the minimized case
				}
				alertTimeDlg->show();
				
				connect(alertTimeDlg, &AlertTimeDlg::closeSignal, this, &AlarmClock::OnCloseAlertTimeDlg1);
				alertTimeDlg->setWindowTitle("Alarm");
				ui.labelCountDown1->setText("Time is up!");

			}

		}
		else
		{
			//Alarm countdown
			QTime endTime;
			endTime.setHMS(clock1HH, clock1MM, 0);
			qDebug() << endTime.toString("hh:mm ss");

			QTime startTime = QTime::currentTime();
			int sec = startTime.secsTo(endTime);

			int hh = int(sec / 3600);
			int mm = int((sec - hh * 3600) / 60);
			int ss = sec - hh * 3600 - mm * 60;

			ui.labelCountDown1->setText(QString::asprintf("Countdown：%02d:%02d %02d", hh, mm, ss));
		}
	}

	if (bRuningClock2)
	{
		QTime sysTime = QTime::currentTime();
		if ((sysTime.hour() == clock2HH) && (sysTime.minute() == clock2MM))
		{
			if (alertTimeDlg2 == NULL)
			{
				alertTimeDlg2 = new AlertTimeDlg(this, "./Resources/sound/sound1.mp3", "clock2：" + ui.textEdit2->toPlainText());

				int width = GetSystemMetrics(SM_CXFULLSCREEN);
				int height = GetSystemMetrics(SM_CYFULLSCREEN);
				alertTimeDlg2->move(width - alertTimeDlg2->width(), height - alertTimeDlg2->height());
				if (!bSystray)
				{
					this->setWindowFlags(Qt::WindowStaysOnTopHint);
				}
				alertTimeDlg2->show();
				connect(alertTimeDlg2, &AlertTimeDlg::closeSignal, this, &AlarmClock::OnCloseAlertTimeDlg2);
				
				alertTimeDlg2->setWindowTitle("Alarm");
				ui.labelCountDown2->setText("Time is up!");
			}

		}
		else
		{
			//Alarm countdown
			QTime endTime;
			endTime.setHMS(clock2HH, clock2MM, 0);

			QTime startTime = QTime::currentTime();
			int sec = startTime.secsTo(endTime);

			int hh = int(sec / 3600);
			int mm = int((sec - hh * 3600) / 60);
			int ss = sec - hh * 3600 - mm * 60;

			ui.labelCountDown2->setText(QString::asprintf("Countdown：%02d:%02d %02d", hh, mm, ss));
		}
	}

	//report punctually
	if (QTime::currentTime().minute()==0 && QTime::currentTime().second()==0)
	{
		NowTimeDlg* nowTimeDlg = new NowTimeDlg;
		nowTimeDlg->show();
	}
}

/************************************
@ Brief:		Close the main window
************************************/
void AlarmClock::OnBtnClose()
{
	this->hide();
	bSystray = true;
}

/************************************
@ Brief:		Minimize the main window 
************************************/
void AlarmClock::OnBtnMin()
{
	this->showMinimized();
}

/************************************
@ Brief:		regarding           
************************************/
void AlarmClock::OnBtnAbout()
{
	AboutDlg aboutDlg(this);
	aboutDlg.setModal(true);
	aboutDlg.setWindowTitle("regarding ");
	aboutDlg.ui.label1->setText("Author:Iclock production team\r\nversion:1.0.0\r\nRelease date:2020/11/1\r\n");
	aboutDlg.exec();
}

/************************************
@ Brief:		Tray eject button
************************************/
void AlarmClock::OnExit()
{
	QApplication::exit(0);
}

/************************************
@ Brief:		Double-click the tray
************************************/
int  AlarmClock::OnSystemTrayClicked(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger
		|| reason == QSystemTrayIcon::DoubleClick)
	{
		//No title bar (no border)
		setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

		// Show main window
		this->showNormal();
		bSystray = false;
	}
	return 0;
}

/************************************
@ Brief:		Alarm 1 audition	    
************************************/
void AlarmClock::OnListen1()
{	//player->setMedia(QUrl::fromLocalFile(":/AlarmClock/Resources/sound/Ringtone 1.mp3"));
	//player->setMedia(QUrl(":/AlarmClock/Resources/sound/sound1.mp3"));
	//player->setMedia(QMediaContent(QUrl::fromLocalFile("./Resources/sound/sound1.mp3")));
	if (player)
	{
		delete player;
		player = NULL;
	}
	player = new QMediaPlayer;
	player->setMedia(QUrl::fromLocalFile("./Resources/sound/sound1.mp3"));
	player->play();
}

/************************************
@ Brief:		Alarm 2 audition
************************************/
void AlarmClock::OnListen2()
{
	if (player2)
	{
		delete player2;
		player2 = NULL;
	}
	player2 = new QMediaPlayer;
	player2->setMedia(QUrl::fromLocalFile("./Resources/sound/sound1.mp3"));
	player2->play();
}

/************************************
@ Brief:		Turn off alarm 1           
************************************/
void AlarmClock::OnCloseAlertTimeDlg1()
{
	//Do not display the title bar (no border)
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	this->showNormal();
	bSystray = false;
	bRuningClock1 = false;
	ui.btnStart1Clock->setText("Turn on alarm");
	ui.btnStart1Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
		"QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
		"QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
	ui.comboBoxHH1->setEnabled(true);
	ui.comboBoxMM1->setEnabled(true);
	ui.comboBoxRing1->setEnabled(true);
	ui.textEdit1->setEnabled(true);
	ui.labelCountDown1->setText("Countdown!");
	alertTimeDlg = NULL;
}

/************************************
@ Brief:		Turn off alarm 2
************************************/
void AlarmClock::OnCloseAlertTimeDlg2()
{
	//Do not display the title bar (no border)
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	this->showNormal();
	bRuningClock2 = false;
	ui.btnStart2Clock->setText("Turn on alarm");
	ui.btnStart2Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
		"QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
		"QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
	ui.comboBoxHH2->setEnabled(true);
	ui.comboBoxMM2->setEnabled(true);
	ui.comboBoxRing2->setEnabled(true);
	ui.textEdit2->setEnabled(true);
	ui.labelCountDown2->setText("Countdown!");
	alertTimeDlg2 = NULL;
}


/************************************
@ Brief:		Turn on/off alarm 1
************************************/
void AlarmClock::OnBtnStartClock1()
{
// 	NowTimeDlg* nowTimeDlg = new NowTimeDlg;
// 	nowTimeDlg->show();
// 	return;
	if (bRuningClock1)
	{
		bRuningClock1 = false;
		ui.btnStart1Clock->setText("Turn on alarm");
		ui.btnStart1Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
			"QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
			"QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
		ui.comboBoxHH1->setEnabled(true);
		ui.comboBoxMM1->setEnabled(true);
		ui.comboBoxRing1->setEnabled(true);
		ui.textEdit1->setEnabled(true);
		ui.labelCountDown1->setText("Countdown!");
	}
	else
	{
		// 		alertTimeDlg = new AlertTimeDlg(this, "./Resources/sound/sound1.mp3", "Alarm 1: Time is up!");
		// 
		// 		int width = GetSystemMetrics(SM_CXFULLSCREEN);
		// 		int height = GetSystemMetrics(SM_CYFULLSCREEN);
		// 		alertTimeDlg->move(width - alertTimeDlg->width(), height - alertTimeDlg->height());
		// 		alertTimeDlg->show();
		// 		connect(alertTimeDlg, &AlertTimeDlg::closeSignal, this, &AlarmClock::OnCloseAlertTimeDlg1);
		// 		alertTimeDlg->setWindowTitle("Alarm");

		clock1HH = ui.comboBoxHH1->currentText().toInt();
		clock1MM = ui.comboBoxMM1->currentText().toInt();

		QTime curTime = QTime::currentTime();
		//if ((clock1HH*3600 +clock1MM*60) >= (curTime.hour()*3600+ curTime.minute()*60+curTime.second()))
		if ((clock1HH * 3600 + clock1MM * 60) >= (curTime.hour() * 3600 + curTime.minute() * 60))
		{
			bRuningClock1 = true;
			ui.btnStart1Clock->setText("Alarm");
			ui.btnStart1Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:red; font-size:30px; border-radius:10px;padding:2px 4px;}"
				"QPushButton:hover{background-color: rgb(6,168,240); color:red; border-radius:10px;padding:2px 4px;}"
				"QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:red; border-radius:10px;padding:2px 4px;}");
			ui.comboBoxHH1->setEnabled(false);
			ui.comboBoxMM1->setEnabled(false);
			ui.comboBoxRing1->setEnabled(false);
			ui.textEdit1->setEnabled(false);
		}
		else
		{
			QMessageBox::about(NULL, "prompt", "<font color='red'>Wrong alarm time!</font>" );
		}
	}
}

/************************************
@ Brief:		Turn on/off alarm 2            
************************************/
void AlarmClock::OnBtnStartClock2()
{
	if (bRuningClock2)
	{
		bRuningClock2 = false;
		ui.btnStart2Clock->setText("Turn on alarm");
		ui.btnStart2Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:white; font-size:30px; border-radius:10px;padding:2px 4px;}"
			"QPushButton:hover{background-color: rgb(6,168,240); color:white; border-radius:10px;padding:2px 4px;}"
			"QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:white; border-radius:10px;padding:2px 4px;}");
		ui.comboBoxHH2->setEnabled(true);
		ui.comboBoxMM2->setEnabled(true);
		ui.comboBoxRing2->setEnabled(true);
		ui.textEdit2->setEnabled(true);
		ui.labelCountDown2->setText("Countdown!");
	}
	else
	{
		clock2HH = ui.comboBoxHH2->currentText().toInt();
		clock2MM = ui.comboBoxMM2->currentText().toInt();

		QTime curTime = QTime::currentTime();
		if ((clock2HH * 3600 + clock2MM * 60) >= (curTime.hour() * 3600 + curTime.minute() * 60 + curTime.second()))
		{
			bRuningClock2 = true;
			ui.btnStart2Clock->setText("Alarm");
			ui.btnStart2Clock->setStyleSheet("QPushButton{background-color:rgb(6,168,255); color:red; font-size:30px; border-radius:10px;padding:2px 4px;}"
				"QPushButton:hover{background-color: rgb(6,168,240); color:red; border-radius:10px;padding:2px 4px;}"
				"QPushButton:pressed{background-color: rgb(6,168,220);border:none;color:red; border-radius:10px;padding:2px 4px;}");
			ui.comboBoxHH2->setEnabled(false);
			ui.comboBoxMM2->setEnabled(false);
			ui.comboBoxRing2->setEnabled(false);
			ui.textEdit2->setEnabled(false);
		}
		else
		{
			QMessageBox::about(NULL, "prompt", "<font color='red'>Wrong alarm time!</font>");
		}
	}
}
