﻿SELECT *
FROM Light
WHERE
	DeviceId = @DeviceId
ORDER BY Id DESC
LIMIT 100