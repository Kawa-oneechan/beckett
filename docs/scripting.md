# Beckett Reference

## Scripting namespace
`engine/Scripting.h` specifies an interface for the game to implement, much like `engine/Game.h`. The demo project uses Sol, but technically anything could be used.

### Functions

#### `Conditional(string expression)`
Should evaluate the given expression and return `true` or `false`.

#### `BJTS(string code, vector<string> args);`
Should evaluate the given script code and pass along its return value, which should be a string.

