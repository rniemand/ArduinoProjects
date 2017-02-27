SELECT *
FROM Devices
WHERE
	DeviceKey = @DeviceKey
	AND Deleted = 0
LIMIT 1