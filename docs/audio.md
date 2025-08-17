# Beckett Reference

## Audio class
The `Audio` class wraps around an old copy of FMOD-EX that Kawa likes perhaps too much.

### Methods

#### `Initialize()`
Static method. Initializes FMOD.

#### `Update()`
Static method. Called every time down the game loop. Updates FMOD, then goes through any pending volume changes.

#### `Audio(string filename)`
Constructor. Loads a sound file for later use. Depending on the path, its type is set to be music, ambient noise, speech, or a general sound. No matter the type, if it's an Ogg Vorbis file it's allowed to loop using the `LOOP_START` tag, specified in samples.

#### `Play(bool force = false)`
Plays the sound. If it's already playing, it won't restart or anything *unless* `force` is true.

#### `Pause()`
Pauses the sound. Calling `Play` afterwards will resume playback.

#### `Stop()`
Stops playing the sound. Calling `Play` afterwards will restart playback.

#### `UpdateVolume()`
Mostly for internal use but you never know. Updates both the `Volume` member and the wrapped FMOD channel's volume.

#### `SetPitch(float ratio)`
Changes the pitch of the sound. Works best if it's not already playing. FMOD's pitch functions take a target frequency, but `Audio` takes a percentage where `1.0` is the original and `2.0` is double that. For convenience.

#### `SetPosition(glm::vec3 pos`
**UNTESTED**

#### `SetPan(float pos)`
Sets the sound's position in 2D stereo space where `-1.0` is fully to the left and `1.0` is fully to the right.

### Members

#### `bool Enabled`
Is audio enabled in general? Assumes true. If FMOD fails to initialize for any reason, this is set to false. Loading and playing sounds only works if it's true.

#### `float MusicVolume`
Background music volume, where `1.0` is full blast.

#### `float AmbientVolume`
Ambient noise volume.

#### `float SoundVolume`
General sounds volume.

#### `float SpeechVolume`
Dialogue volume.


