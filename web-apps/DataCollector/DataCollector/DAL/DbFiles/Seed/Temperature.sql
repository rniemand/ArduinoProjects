INSERT INTO 'Temperature'
	('DeviceId', 'Temperature', 'Humidity', 'HeatIndex', 'TimeLoggedUtc')
VALUES
	(1, 23.5, 50, 28, (SELECT CURRENT_TIMESTAMP as utc)),
	(1, 26, 66, 56, (SELECT CURRENT_TIMESTAMP as utc)),
	(1, 29, 23, 19, (SELECT CURRENT_TIMESTAMP as utc))