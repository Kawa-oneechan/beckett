# ![Beckett Engine logo](docs/logo.png) Beckett Engine

## Features
Not a whole lot just yet lol

## Installation
(TODO: finish this bit.)

Provide the following things in any module you want:
* `void GameInit()`
* `void GamePrepSaveDirs(const fs::path& savePath)` (if `BECKETT_EXTRASAVEDIRS` is defined)
* `void GameImGui()` (if `DEBUG` is defined)
* `void GameStart(std::vector<TickableP>& tickables)`
* `void GameMouse(double xPosIn, double yPosIn, float xoffset, float yoffset)`
* `void GameResize()`
* `void GameLoopStart()`
* `void GamePreDraw(float dt)`
* `void GamePostDraw(float dt)`
* `void GameQuit()`
* `void ConsoleRegister(Console* console)`
* `const std::map<std::string, BJTSFunc> bjtsPhase1`
* `std::map<std::string, std::string> bjtsPhase1X`

Provide the following in `Game.h`:
* `GAMENAME` and `VERSIONJOKE` defines
* `SCREENWIDTH` and `SCREENHEIGHT` defines
* `DEFAULTFILTER` define, set to either `GL_NEAREST` or `GL_LINEAR`
* `BECKETT_EXTRASAVES` define, if you want to use the `GamePrepSaveDirs` callback function
* The `CommonUniforms` struct, which must match `shaders/common.fs`
 * Must at least have `float TotalTime`, `float DeltaTime`, `glm::uvec2 ScreenRes`, `glm::mat4 View`, `glm::mat4 Projection`, and `glm::mat4 InvView` fields.
* `extern CommonUniforms commonUniforms` (the actual one is in `engine/main.cpp`)

## Building

## Usage

## Acknowledgements
Script support would not be possible without [Siapran](https://siapran.tumblr.com).
Also massive props to [Vaartis of the Ratular Bells](https://vaartis.tumblr.com) for their help.
The logo was created by [twocatsinatrenchcoat](https://twocatsinatrenchcoat.tumblr.com/).

The Beckett Engine uses several third-party libraries:
* [Dear ImGUI](https://github.com/ocornut/imgui)
* FMOD Ex (an older, free version that you can't get from them anymore)
* [FMT](https://github.com/fmtlib/fmt)
* [GLAD](https://glad.dav1d.de/)
* [GLFW](https://www.glfw.org/)
* [GLM](https://github.com/icaven/glm)
* [json5pp](https://github.com/kimushu/json5pp)
* [Lua](https://www.lua.org/)
* [MiniZ](https://github.com/richgel999/miniz)
* [STB](https://github.com/nothings/stb)
* [Sol](https://sol2.readthedocs.io/en/latest/) (v2.20.6 specifically)
* [UFBX](https://ufbx.github.io/)

