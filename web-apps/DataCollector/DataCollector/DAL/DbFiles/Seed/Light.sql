INSERT INTO 'Light'
	('DeviceId', 'LightReading', 'TimeLoggedUtc')
VALUES
	(1, 10, (SELECT time(CURRENT_TIMESTAMP, '-15 minutes'))),
	(1, 15, (SELECT time(CURRENT_TIMESTAMP, '-10 minutes'))),
	(1, 5, (SELECT time(CURRENT_TIMESTAMP, '-5 minutes'))),
	(1, 5, (SELECT time(CURRENT_TIMESTAMP, '-0 minutes')))