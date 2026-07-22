# Beckett Reference

## InputsMap class

### Methods

#### `Process(int scancode, int action)`

#### `MouseMove(float x, float y)`

#### `MouseMoved()`

#### `UpdateGamepad()`

#### `Clear(bool alsoGamepad = false);`

#### `KeyDown(Binds bind)`

#### `Clear(Binds bind)`

### Members

#### `InputKey Keys[NumKeyBinds]`

#### `bool Shift, Control, Alt`

#### `bool MouseLeft, MouseRight, MouseMiddle`

#### `bool MouseHoldLeft, MouseHoldRight, MouseHoldMiddle`

#### `glm::vec2 MousePosition`

#### `glm::vec2 MouseOffset`

#### `glm::vec2 ScaledMousePosition`

#### `float MouseScale`

#### `bool HaveGamePad`

#### `int StickAngles[2]`

#### `float StickDists[2]`

#### `float RunThreshold`

#### `float Deadzone`

## InputKey struct

### Members

#### `bool State`

#### `int ScanCode`

#### `int GamepadButton`

#### `string Name`

