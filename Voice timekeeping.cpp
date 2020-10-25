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
	playList->addMedia(QUrl("qrc:/AlarmClock/Resources/sound/nowtime.wav"));
	if (hour <= 7)
	{
		playList->addMedia(QUrl("qrc:/AlarmClock/Resources/sound/am0.wav"));
		strTimePath = QString::asprintf("qrc:/AlarmClock/Resources/sound/%d.wav", hour);
		playList->addMedia(QUrl(strTimePath));
	}
	else if ((hour > 7) && (hour <= 12))
	{
		playList->addMedia(QUrl("qrc:/AlarmClock/Resources/sound/am.wav"));
		strTimePath = QString::asprintf("qrc:/AlarmClock/Resources/sound/%d.wav", hour);
		playList->addMedia(QUrl(strTimePath));
	}
	else if ((hour > 12) && (hour <= 20))
	{
		playList->addMedia(QUrl("qrc:/AlarmClock/Resources/sound/pm.wav"));
		strTimePath = QString::asprintf("qrc:/AlarmClock/Resources/sound/%d.wav", hour - 12);
		playList->addMedia(QUrl(strTimePath));
	}
	else if (hour > 20)
	{
		playList->addMedia(QUrl("qrc:/AlarmClock/Resources/sound/em.wav"));
		strTimePath = QString::asprintf("qrc:/AlarmClock/Resources/sound/%d.wav", hour - 12);
		playList->addMedia(QUrl(strTimePath));
	}
	playList->addMedia(QUrl("qrc:/AlarmClock/Resources/sound/point.wav"));

	playList->setPlaybackMode(QMediaPlaylist::Sequential);
	player = new QMediaPlayer;
	player->setPlaylist(playList);
	player->play();
}
