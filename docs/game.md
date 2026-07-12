# Beckett Reference

## Game settings
While `engine/Game.h` specifies an interface for the engine, `Game.h` one level higher decides on several game engine settings.

### `#define` flags
| Name | Effect |
| ---- | ------ |
| `BECKETT_GAMENAME` | The name of the game to show in the title bar and log. |
| `BECKETT_VERSIONJOKE` | What to put after the game name. |
| `BECKETT_SCREENWIDTH`<br>`BECKETT_SCREENHEIGHT` | The size of the *target* screen. |
| `BECKETT_DEFAULTFILTER` | Either `GL_NEAREST` or `GL_LINEAR`. Specifies the default texture interpolation method. |
| `BECKETT_EXTRASAVEDIRS` | If enabled, allows the use of `Game::PrepareSaveDirs` to do more setup as your game requires. |
| `BECKETT_RESIZABLE` | Allows the window to be resized. |
| `BECKETT_MOREVOLUME` | Adds more volume controls (Ambient and Voice) to what's available by default (Music and Sound). Will likely be removed when busses are implemented. |
| `BECKETT_3DAUDIO` | Unlocks the use of 3D positional audio, if you need it. |
| `BECKETT_NO3DMODELS` | Removees the ability to load and render models, leaving only 2D sprites. |
| `BECKETT_ONLYMURCAN` | Simplifies the text interface to only support one language, presumably US English. |
| `BECKETT_MSAA` | Enables multi-sampled anti-aliasing. Useless if you plan to have HDR and/or post-processing effects. |
| `BECKETT_ANALOGLEFT`<br>`BECKETT_ANALOGRIGHT` | Set to a "north" `Binds` value from `InputsMap.h`, if you want to use analog stick controls. |
| `BECKETT_PULLEDPORK` | Enables vertex pulling for 2D sprites. |
| `BECKETT_JSONCONSTANTS` | Enables named constants in certain JSON contexts (anything parsed by `GetJSONVal` basically). This requires a `constants.json` to list them. |

### `CommonUniforms`
The `CommonUniforms` struct *must* match `shaders/common.fs`. It must at least have the following fields:
```cpp
float TotalTime;
float DeltaTime;
glm::uvec2 ScreenRes;
glm::mat4 View;
glm::mat4 Projection;
glm::mat4 InvView;
```
You can add more if you need them, specific to your game. Just don't forget to also change `shaders/common.fs` accordingly. Note that `bool` values are a different size between GLSL and C++, so you may want to use `alignas(4) bool` instead.

### Externs
These should also be present in `Game.h`:
```cpp
extern float scale;
extern int width, height;
extern Texture* whiteRect;
extern CommonUniforms commonUniforms;

//BJTS functions that actually change the string content.
extern const std::map<std::string, BJTSFunc> bjtsPhase1;
//BJTS functions loaded from Lua scripts.
extern std::map<std::string, std::string> bjtsPhase1X;
```
