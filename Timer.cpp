// Timer
void AlarmClock::timerEvent(QTimerEvent * event)
{
	ui.labelTime->setText("");
	QString strDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd  hh:mm:ss");
	ui.labelTime->setText(strDateTime);

	if (bRuningClock1)
	{
		QTime sysTime = QTime::currentTime();
		if ((sysTime.hour() == clock1HH) && (sysTime.minute() == clock1MM))
		{
			if (alertTimeDlg == NULL)
			{
				alertTimeDlg = new AlertTimeDlg(this, "./Resources/sound/sound1.mp3", " Alarm 1£º" + ui.textEdit1->toPlainText());

				int width = GetSystemMetrics(SM_CXFULLSCREEN);
				int height = GetSystemMetrics(SM_CYFULLSCREEN);
				alertTimeDlg->move(width - alertTimeDlg->width(), height - alertTimeDlg->height());
				if (!bSystray)
				{
					this->setWindowFlags(Qt::WindowStaysOnTopHint);// It does not take effect in the tray situation, it can be displayed directly by show, and this item needs to be set in the minimized situation				}
					alertTimeDlg->show();

					connect(alertTimeDlg, &AlertTimeDlg::closeSignal, this, &AlarmClock::OnCloseAlertTimeDlg1);
					alertTimeDlg->setWindowTitle;
					ui.labelCountDown1->setText;

				}

			}
			else
			{
				// Alarm countdown
				QTime endTime;
				endTime.setHMS(clock1HH, clock1MM, 0);
				qDebug() << endTime.toString("hh:mm ss");

				QTime startTime = QTime::currentTime();
				int sec = startTime.secsTo(endTime);

				int hh = int(sec / 3600);
				int mm = int((sec - hh * 3600) / 60);
				int ss = sec - hh * 3600 - mm * 60;

				ui.labelCountDown1->setText(QString::asprintf("Countdown£º%02d:%02d %02d", hh, mm, ss));
			}
		}

		if (bRuningClock2)
		{
			QTime sysTime = QTime::currentTime();
			if ((sysTime.hour() == clock2HH) && (sysTime.minute() == clock2MM))
			{
				if (alertTimeDlg2 == NULL)
				{
					alertTimeDlg2 = new AlertTimeDlg(this, "./Resources/sound/sound1.mp3", " Alarm 2£º" + ui.textEdit2->toPlainText());

					int width = GetSystemMetrics(SM_CXFULLSCREEN);
					int height = GetSystemMetrics(SM_CYFULLSCREEN);
					alertTimeDlg2->move(width - alertTimeDlg2->width(), height - alertTimeDlg2->height());
					if (!bSystray)
					{
						this->setWindowFlags(Qt::WindowStaysOnTopHint);
					}
					alertTimeDlg2->show();
					connect(alertTimeDlg2, &AlertTimeDlg::closeSignal, this, &AlarmClock::OnCloseAlertTimeDlg2);

					alertTimeDlg2->setWindowTitle;
					ui.labelCountDown2->setText;
				}

			}
			else
			{
				// Alarm countdown
				QTime endTime;
				endTime.setHMS(clock2HH, clock2MM, 0);

				QTime startTime = QTime::currentTime();
				int sec = startTime.secsTo(endTime);

				int hh = int(sec / 3600);
				int mm = int((sec - hh * 3600) / 60);
				int ss = sec - hh * 3600 - mm * 60;

				ui.labelCountDown2->setText(QString::asprintf("Countdown£º%02d:%02d %02d", hh, mm, ss));
			}
		}
