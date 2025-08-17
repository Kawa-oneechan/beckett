# Beckett Reference

## Cursor class

### Methods

#### `Select(int style)`
Sets up the cursor to a specific style.

#### `SetScale(float newScale)`
Sets the scale of the cursor as a percentage float from 0.2 to 10.0.

### Members

#### `int WaitIndex`
Index of the cursor style that depicts an hourglass. This cursor will be animated to slowly spin. Defaults to 1.

#### `int PenMinIndex` and `int PenMaxIndex`
Lowest and highest indices, inclusive, of the cursor styles that have colored parts. Defaults to cursors 11 to 15.

#### `int PenOffset`
How many frames to add to get to the colored parts for colored cursors. Defaults to 8.

#### `glm::vec4 Pen`
The color to use for colored cursors. Defaults to red.

