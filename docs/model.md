# Beckett Reference

NOTE: Because of the engine's origins as an attempt to clone *Animal Crossing New Horizons*, this was written with ripped ACNH assets in mind, where each mesh has a name like `Foo__mBar` where `mBar` is the mesh's material name, but the entire thing is the mesh's name. Something needs to be done to let the game define what the material name would be...

## Model class

### Methods

#### `Draw(glm::vec3 pos = {0,0,0}, float yaw = 0.0, int mesh = -1)`
Queues the model for drawing at the specified position and rotation. If the mesh argument is -1, the entire model is drawn.

#### `SetVisibility(bool visible = true)`
Sets the visibility of all parts of the model.

#### `SetVisibility(string name, bool visible = true`
Sets the visibility of a given mesh part, specified by mesh name.

#### `SetVisibility(int index, bool visible = true)`
Sets the visibility of a given mesh part, specified by index.

#### `SetLayer(int layer = 0)`
Sets the `TextureArray` layer index for all parts of the model.

#### `SetLayer(const std::string& name, int layer)`
Sets the `TextureArray` layer index of a given mesh part, specified by mesh name.

#### `SetLayerByMat(const std::string& name, int layer)`
Sets the `TextureArray` layer index of a given mesh part, specified by material name.

#### `SetLayer(int index, int layer)`
Sets the `TextureArray` layer index of a given mesh part, specified by index.

#### `GetMesh(string name)`
Returns a reference to a `Mesh` by name, which can be either the full mesh or the material part.

#### `GetMesh(int index)`
Returns a reference to a `Mesh` by index.

#### `FindBone(string name)`
Returns the index of a bone for this model by name.

#### `CalculateBoneTransforms()`
Calcalutes the final bone matrices for the entire armature.

#### `CopyBoneTransforms(Model target)`

### Members

#### `vector<Mesh> Meshes`

#### `vector<UfbxMisc::Light> Lights`

#### `vector<UfbxMisc::Camera> Cameras`

#### `vector<UfbxMisc::Empty> Empties`

#### `array<Bone> Bones`

#### `size_t BoneCt`

#### `hash Hash`

## Mesh class

## Bone class

## UfbxMisc struct

### Common members

#### `string Name`

#### `amp<string, jsonValue> Properties`

### Camera and Empty members

#### `glm::vec3 Position`

#### `glm::vec3 direction`

### Light members

#### `glm::vec4 Position`
It's a four-element vector so *W* can denote if it's directional or not.

#### `glm::vec4 Color`
Alpha indicates intensity.

