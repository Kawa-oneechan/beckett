# Beckett Reference

## VFS namespace
The `VFS` namespace provides a Virtual File System, a single view of all asset sources available to the game. On top of that, it handles saved game data.

### Functions

#### `Initialize()`
Initializes the Virtual File System. This includes loading the initialization file, enumerating all the asset sources in the locations listed in that file, sorting the asset sources, checking for dependencies, and setting up the saved game directory.

#### `Enumerate(string path)`
Returns a list of all `Entry` matching the given `path`. If `path` does not contain a wildcard `*`, returns a list with only one item. If no files could be found, returns an empty list.

#### `Forget(vector<Entry> entries)`
Removes all knowledge of the given entries from the system.

### Reading file data

#### `ReadData(string path, size_t* size)`
Reads the raw contents of a file to a `char[]`, optionally returning the size of the data. If the file does not exist, returns `nullptr`.

#### `ReadString(string path)`
Reads the contents of a file as a string. If the file does not exist, returns `nullptr`.

#### `ReadJSON(string path)`
Reads the contents of a file as a JSON document, applying any patches it may find on the way. If the file does not exist or is malformed somehow, returns a `null` JSON value.

Each of the three `ReadXXX` functions above can also take an `Entry` instead of a `string`. If `ReadJSON` fails, a fatal error is raised.

### Reading and writing savegame data

#### `ReadSaveData(string path, size_t* size)`
Much the same as `ReadData`, but reads from the saved game folder instead.

#### `ReadSaveString(string path)`
Much the same as `ReadString`, but reads from the saved game folder instead.

#### `ReadSaveJSON(string path)`
Take a wild guess.

Each of the above `ReadSaveXXX` functions has a matching `WriteSaveXXX` function that takes an extra `data` parameter; `char[]`, `string`, or `jsonValue` respectively. For `WriteSaveData`, the `size` parameter is *not* optional.

### Path manipulation

#### `GetPathPart(string path)`
Given `foo/bar/baz.txt`, returns `foo/bar`.

#### `GetFilePart(string path)`
Given `foo/bar/baz.txt`, returns `baz.txt`.

#### `ChangeExtension(string path, string ext)`
Given `foo/bar/baz.txt` and `doc`, returns `foo/bar/baz.doc`.

#### `GoUpPath(string path)`
Given `a/b/c`, returns `a/b`. Given `a`, returns the empty string.

#### `ClimbDown(string path, string fallback)`
Given `foo/bar/baz.txt` and `fallback.txt`, finds the highest existing `baz.txt` or `fallback.txt`, or the empty string. That is, it will check, in order:
1. `foo/bar/baz.txt`
2. `foo/bar/fallback.txt`
3. `foo/baz.txt`
4. `foo/fallback.txt`
5. `baz.txt`
6. `fallback.txt`
And then return the name of whichever file was found to exist first. Ultimately, the `fallback` file is returned as a last resort.


