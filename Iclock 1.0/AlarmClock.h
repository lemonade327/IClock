#pragma once
#pragma execution_character_set("utf-8")

#include <QtGui>
#include <QtWidgets>
#include "ui_AlarmClock.h"
#include <QPushButton>
#include "AlertTimeDlg.h"
#include <QtMultimedia/QMediaPlayer>
#include <QVideoWidget>

class AlarmClock : public QDialog
{
	Q_OBJECT

public:
	AlarmClock(QWidget *parent = Q_NULLPTR);
	virtual void paintEvent(QPaintEvent *event);

	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);

	//Timer
	virtual void timerEvent(QTimerEvent *event);
	int timerId;

	QPushButton* closeButton;
	QPushButton* minButton;
	QPushButton* aboutButton;

private:
	QPixmap m_titleImage;

	bool m_dragging; // Is dragging
	QPoint m_startPosition; // Mouse position before dragging starts
	QPoint m_framePosition; // Original position of the form

	QSystemTrayIcon m_systray;
	AlertTimeDlg* alertTimeDlg;
	AlertTimeDlg* alertTimeDlg2;

	QMediaPlayer *player;
	QMediaPlayer *player2;
	bool bSystray = false;

	//Clock1
	bool bRuningClock1;
	int clock1HH;
	int clock1MM;


	//Clock2
	bool bRuningClock2;
	int clock2HH;
	int clock2MM;

private slots:
	void OnBtnClose();
	void OnBtnMin();
	void OnBtnAbout();
	void OnExit();
	int  OnSystemTrayClicked(QSystemTrayIcon::ActivationReason reason);
	void OnBtnStartClock1();
	void OnBtnStartClock2();
	void OnListen1();
	void OnListen2();
	void OnCloseAlertTimeDlg1();
	void OnCloseAlertTimeDlg2();
	

private:
	Ui::AlarmClockClass ui;
};
