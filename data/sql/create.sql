CREATE TABLE IF NOT EXISTS editors (
    id TEXT NOT NULL,
    name TEXT,
    formats TEXT,
    encoding TEXT,
    intellisense INTEGER CHECK (intellisense IN (0, 1)),
    plugins INTEGER CHECK (plugins IN (0, 1)),
    compile INTEGER CHECK (compile IN (0, 1))
);