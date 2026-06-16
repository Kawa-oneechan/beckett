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


