# Beckett Reference

## Tilemap class
TODO: Describe them here.

**Warning**: Tiled implements flipping and rotation by using the top four bits of the tile number. Unfortunately, the JSON parser can't quite handle 32-bit integers. The workaround is to save the map data in Base64, which allows the map loader to bypass this limitation entirely.

### Methods

#### `Tilemap(string filename)`
Constructor. Builds a series of `MapLayer` objects and a `Tileset` according to the given Tiled map file. A layer named `Sprites` will yield a `MapSpriteLayer` to which you can attach `MapSprite` objects.

#### `GetLayer(size_t i)`
Returns the *i*th layer in the stack.

#### `SetTile(int row, int col, int tile)`
Sets the tile in the given location for the bottom `MapLayer`.

#### `SetTile(int row, int col, {...})`
Going from bottom to top, sets the tile in the given location for all `MapLayer` objects in the stack.

#### `GetTile(int layer, int row, int col)`
Returns the tile number at the given location, where *layer* refers to all `MapLayer` objects from bottom to top.

#### `GetTile(int layer, vec2 position)`
Same as the other `GetTile` but takes a pixel position. Adjusts for the camera.

#### `GetPixelSize()`
Returns the size of the map in pixels. If no `MapLayer` objects are available, returns 16×16.

#### `GetTileSize()`
Returns the size of the map in tiles. If no `MapLayer` objects are available, returns 16×16.

#### `GetCollision(int row, int col)`
Returns a rectangle denoting a collision shape at the given location, going from bottom to top. The first layer to have a collision shape at that location wins. If nothing is found, -1,-1,-1,-1 is returned. Out-of-bounds areas are always considered blocking.

