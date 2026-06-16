# Beckett Reference

## Utilities
TODO: Describe them here.

### Functions

#### `Screenshot()`
Takes a screenshot, saving as a timestamped PNG.

## ShapeUtils

### Functions

#### `PointInPoly(vec2 point, polygon poly)`
Returns true if the specified point is inside the polygon.

#### `PointInRect(vec2 point, vec4 rect)`
Returns true if the specified point is inside the rectangle.

## JsonUtils

### Functions

#### `GetJSONVec2(const jsonValue& val)`
Given a JSON array with two numbers in it, returns a `vec2` with those numbers.

#### `GetJSONVec3(const jsonValue& val)`
Given a JSON array with three numbers in it, returns a `vec3` with those numbers.

#### `GetJSONVec4(const jsonValue& val)`
Given a JSON array with four numbers in it, returns a `vec4` with those numbers.

#### `GetJSONColor(const jsonValue& val)`
Converts `[R,G,B]`, `[R,G,B,A]`, `"#RRGGBB"` or `"#AARRGGBB"` to a `vec4`. Returns an alpha of -1, which is impossible, on error.

#### `GetJSONDate(const jsonValue& val)`
Given a JSON array with two numbers in it, returns a `vec2` with those numbers (e.g. `[ 26, 6 ]`). But given a string that parses as `"mmm dd"` or `"mmm/dd"`, parses that as a three-letter month name and day pair (e.g. `"jun 26"`) and returns those numbers. Either way, the first returned value is a day from 1 to 31 and the second a month from 1 to 12 (e.g. `[ 26, 6 ]`).

#### `GetJSONVec(const glm::vec2& vec, bool asInt = false)`
Returns a JSON array initialized from a `vec2`, optionally casting to `int`.

#### `GetJSONVec(const glm::vec3& vec, bool asInt = false)`
Returns a JSON array initialized from a `vec3`, optionally casting to `int`.

#### `GetJSONVec(const glm::vec4& vec, bool asInt = false)`
Returns a JSON array initialized from a `vec4`, optionally casting to `int`.

## ColorUtils

### Functions

#### `LinearSRGBtoOkLab(vec3 c)`
Converts a linear sRGB color value to OkLab color space.

#### `LinearSRGBtoOkLab(float r, float g, float b)`
The same, but takes individual components. Still returns a `vec3` though.

#### `OkLabToLinearSRGB(vec3 c)`
Converts an OkLab color to linear sRGB.

#### `OkLabToLinearSRGB(float l, float a, float b)`
The same, but takes individual compoments.

#### `RGBtoHSV(vec3 c)`
Converts an RGB color value to HSV color space.

#### `RGBtoHSV(float r, float g, float b)`
The same, but takes individual components.

#### `HSVtoRGB(vec3 hsv)`
Converts an HSV color to RGB.

#### `HSVtoRGB(float h, float s, float v)`
The same, but takes individual components.

#### `RGBtoHSL(vec3 c)`
Converts an RGB color value to HSL color space.

#### `RGBtoHSL(float r, float g, float b)`
The same, but takes individual components.

#### `HSLtoRGB(vec3 hsl)`
Converts an HSL color to RGB.

#### `HSLtoRGB(float h, float s, float l)`
The same, but takes individual components.

As a reminder: in HSV space, bright red (255, 0, 0) would be 0°, 100%, 100%. In HSL, it'd be 0°, 100%, 50%.

## CrcUtils

### Functions

#### `GetCRC(string text)`
Returns the CRC32 hash for the given text.

#### `GetCRC(char* buffer, size_t len)`
Returns the CRC32 hash for the given data.


