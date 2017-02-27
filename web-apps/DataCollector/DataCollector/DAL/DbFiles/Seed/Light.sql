INSERT INTO 'Light'
	('DeviceId', 'LightReading', 'TimeLoggedUtc')
VALUES
	(1, 10, (SELECT CURRENT_TIMESTAMP as utc)),
	(1, 15, (SELECT CURRENT_TIMESTAMP as utc)),
	(1, 5, (SELECT CURRENT_TIMESTAMP as utc))