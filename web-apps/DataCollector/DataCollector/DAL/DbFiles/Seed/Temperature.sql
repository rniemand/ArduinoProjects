INSERT INTO 'Temperature'
	('DeviceId', 'Temperature', 'Humidity', 'HeatIndex', 'TimeLoggedUtc')
VALUES
	(1, 23.5, 50, 28, (SELECT time(CURRENT_TIMESTAMP, '-15 minutes'))),
	(1, 26, 66, 56, (SELECT time(CURRENT_TIMESTAMP, '-10 minutes'))),
	(1, 29, 23, 19, (SELECT time(CURRENT_TIMESTAMP, '-5 minutes')))