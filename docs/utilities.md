# Beckett Reference

## Utilities
TODO: Describe them here.

### Functions

#### `PointInPoly(vec2 point, polygon poly)`
Returns true if the specified point is inside the polygon.

#### `PointInRect(vec2 point, vec4 rect)`
Returns true if the specified point is inside the rectangle.

#### `RevAllTickables(vector<Tickable> tickables), float dt)`
Go through all specified tickables, calling their `Tick` methods in reverse order. Disabled tickables are skipped. Inputs are flushed if any of them return false.

#### `DrawAllTickables(vector<Tickable> tickables, float dt)`
Go through all specified tickables, calling their `Draw` methods in order. Invisible tickables are skipped.

#### `GetCRC(string text)`
Returns the CRC32 hash for the given text.

#### `GetCRC(char* buffer, size_t len)`
Returns the CRC32 hash for the given data.

