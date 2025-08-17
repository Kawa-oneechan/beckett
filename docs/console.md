# Beckett Reference

## Console class
The `Console` class offers a *Quake*-ish command console. It is `Tickable`, but not part of the tree. By default, the console key is the backtic <kbd>`</kbd>.

### Methods

####  `Console`
Constructor. Opens `console.log` for writing, prints `GAMENAME`, registers the `clear`, `version`, `cvarlist`, and `ccmdlist` commands, and calls the game-provided `ConsoleRegister` function.

#### `Execute(string str)`
Attempts to execute user input. If the input is just a console variable's name, its value will be displayed in response. If it's a variable followed by a value of the appropriate type, it'll be set. If it's a console command's name, that command will be executed. If it's none of these, it's assumed to be a Lua command, passed along to the Sol library.

#### `Print(int color, string str)`
Appends the given string to the console log. For the in-game console, the text is marked to be in the specific color (see below). Internal line breaks are allowed.

#### `Print(string str)`
Assumes color zero. Otherwise works the same.

#### `Flush()`
Flushes pending writes to the `console.log` file.

#### `Open()`
Opens the console with a little animation.

#### `Close()`
Closes the console with a little animation.

#### `RegisterCVar(string name, CVar::Type type, void* target, bool cheat = false, int min = -1, int max = -1)`
Registers a console variable, mapping it by name to an arbitrary variable in the game.

#### `RegisterCCmd(string name, function act)`
Registers a console command, mapping it by name to a `void(jsonArray&)` function.

### Niceties
For ease of use, two macros are provided by `Console.h`:

#### `conprint(C, F, ...)`
Wraps a `fmt::format` and passes the result to `Console::Print`. `C` is the color you want to use.

#### `debprint(C, F, ...)`
Works the same as `conprint`, but only emits actual code when `DEBUG` is defined.

## CVar struct
The `CVar` struct defines a console variable.

### Members

#### `string name`
The console variable's name, of course. Should not contain any spaces so as to not confuse the parser.

#### `CVar::Type type`
Console variable types can be `Bool`, `Int`, `Float`, `String`, `Vec2`, `Vec3`, `Vec4`, or `Color`.

#### A union of different types

#### `bool cheat`
If enabled, the player is not allowed to change the variable's value without first setting `cheatsEnabled` in some way.

#### `int min, max`
For `Int` and `Float` type variables, specifies the minimum and maximum values allowed.

#### `Set(string value)`
Attempts to set the variable to the given value, which is parsed as if it's JSON. So for a `Vec2` type console variable, the value should be in the form `[ x, y ]`. For a `Color`, it can be an array of three or four float values, a string hex-code (`""#rrggbb"` or `"#aarrggbb"`), ~~or a string naming a color from `ui.json`~~. Note that the *game* calling `Set` does *not* bother with the `cheat` flag.

## CCmd struct
There's not much to say about this one compared to `CVar`. It only holds a name and a function.

## Colors
Console text gets to use these colors in order:

| #   | Name    |
| --- | ------- |
| 0   | Gray    |
| 1   | Red     |
| 2   | Green   |
| 3   | Yellow  |
| 4   | Blue    |
| 5   | Magenta |
| 6   | Cyan    |
| 7   | Silver  |
| 8   | White   |

## Game expectations
The game is expected to provide `void ConsoleRegister(Console* console)`, which is meant to register console commands and variables. As an example:
```cpp
void ConsoleRegister(Console* console)
{
	#define RV console->RegisterCVar
	#define RC console->RegisterCCmd
	RV("sv_cheats", CVar::Type::Bool, &cheatsEnabled);
	RV("timescale", CVar::Type::Float, &timeScale, true);
	RV("noclip", CVar::Type::Bool, &, true);
	RV("s_musicvolume", CVar::Type::Float, &Audio::MusicVolume, false, 0, 100);
	RV("reshade", CCmdReshade);
	RV("hello", [&](jsonArray& args) { conprint(4, "Hello Beckett Engine!"); });
	#undef RC
	#undef RV
}
```

