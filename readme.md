# ![Beckett Engine logo](docs/logo.png) Beckett Engine

## Features
Not a whole lot just yet lol

## Installation
(TODO: finish this bit.)

Provide implementations for `engine/Game.h` in any module you want.

Provide the following in `Game.h` (*not* `engine/Game.h`):
* `BECKETT_GAMENAME` and `BECKETT_VERSIONJOKE` defines.
* `BECKETT_SCREENWIDTH` and `BECKETT_SCREENHEIGHT` defines.
* `BECKETT_DEFAULTFILTER` define, set to either `GL_NEAREST` or `GL_LINEAR`.
* `BECKETT_EXTRASAVEDIRS` define, if you want to use the `Game::PrepareSaveDirs` callback function.
* `BECKETT_RESIZABLE` define, if you want the window to be resizable.
* `BECKETT_MOREVOLUME` define, if you want more volume controls than just music and generic sound effects.
* `BECKETT_3DAUDIO` define, if you want that.
* `BECKETT_NO3DMODELS` define, if you want to completely disable 3D model support.
* `BECKETT_NOBJTS` define, if you want to use only plain untagged text.
* `BECKETT_ONLYMURCAN` define, if you want to disable localization.
* `BECKETT_MSAA` define, if you want to use that.
* `BECKETT_ANALOGLEFT` and/or `RIGHT` defines, set to a "north" `Binds` value, if you want to use analog stick controls.
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
* [FMT](https://github.com/fmtlib/fmt)
* [GLAD](https://glad.dav1d.de/)
* [GLFW](https://www.glfw.org/)
* [GLM](https://github.com/icaven/glm)
* [json5pp](https://github.com/kimushu/json5pp)
* [Lua](https://www.lua.org/)
* [MiniZ](https://github.com/richgel999/miniz)
* [SoLoud](https://solhsa.com/soloud/)
* [Sol](https://sol2.readthedocs.io/en/latest/) (v2.20.6 specifically)
* [STB](https://github.com/nothings/stb)
* [UFBX](https://ufbx.github.io/)

