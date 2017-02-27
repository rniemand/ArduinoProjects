CREATE TABLE 'Temperature' (
    'Id'				INTEGER PRIMARY KEY AUTOINCREMENT,
    'DeviceId'			INTEGER NOT NULL,
    'Temperature'		REAL NOT NULL,
    'Humidity'			REAL NOT NULL,
    'HeatIndex'			REAL NOT NULL,
    'TimeLoggedUtc'		TIMESTAMP NOT NULL
);
