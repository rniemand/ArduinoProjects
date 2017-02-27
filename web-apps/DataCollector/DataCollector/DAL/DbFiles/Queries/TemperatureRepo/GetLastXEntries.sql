SELECT *
FROM Temperature
WHERE
	DeviceId = @DeviceId
ORDER BY Id DESC
LIMIT 100