CREATE TABLE 'Light' (
    'Id'				INTEGER PRIMARY KEY AUTOINCREMENT,
    'DeviceId'			INTEGER NOT NULL,
    'LightReading'		REAL NOT NULL,
    'TimeLoggedUtc'		TIMESTAMP NOT NULL
);
