# Beckett Reference

## Framebuffer class
TODO: Describe it here.

### Methods

#### `Framebuffer(string fragmentShader, int width, int height)`
Constructor. Creates a framebuffer of the specified size and loads a new privately-owned `Shader` instance, using the specified fragment shader and the `sprite.vs` vertex shader.

#### `Framebuffer(Shader* fragmentShader, int width, int height)`
Constructor. Creates a framebuffer of the specified size and assigns it a publically-owned `Shader`.

The difference is that if the `Framebuffer` is destroyed or it has a new `Shader` assigned to it, a privately-owned `Shader` is destroyed along with it.

#### `~Framebuffer()`
Destructor. Unbinds whatever framebuffer is currently active just to be sure, and destroys the `Shader` if it's privately-owned.

#### `Use()`
Binds the framebuffer and sets up the viewport accordingly.

#### `Drop()`
Unbinds the framebuffer and resets the viewport accordingly.

#### `Draw(vec2 pos = {0, 0})`
Renders the framebuffer's image to whatever is the current target (another framebuffer or the actual screen), at its full size, using the attached `Shader`.

#### `Draw(vec2 pos, vec2 size)`
Renders the framebuffer's image to the current target, but at different size.

#### `ChangeShader(string newShader)`
Replaces the current `Shader` with a new privately-owned one.

#### `ChangeShader(Shader* newShader, own = false)`
Replaces the current `Shader` with a new one. You decide if it's to be considered privately-owned.

#### `ReloadShader()`
Reloads the current `Shader`.

#### `GetTexture()`
Returns the framebuffer's backing `Texture`

#### `SetLut(Texture* newLut)`
Replaces the framebuffer's lookup `Texture` (which may be used by the `Shader`) with another.

#### `GetLut()`
Returns the current lookup `Texture`.


