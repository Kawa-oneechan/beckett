# Beckett Reference

## File formats

### Autodesk FBX
Used for 3D models and animations. Models can be combined with material map files (see below).

### PNG
Used for sprites and textures. Sprite sheets can be combined with an atlas file.

### Ogg Vorbis
Used for all the audio. For any file in the `music` folder, you can include a `LOOP_START` tag measured in samples.

### JSON
Used for basically everything else.

#### Atlas
Atlas files are placed in the same folder as a PNG, with matching names (`foo/bar.png` pairs with `foo/bar.json`). If there is no atlas file, one is generated on the fly emcompassing the entire image as a single frame.

The contents of a *simple* atlas are like this:
```json
{
	"type": "simple",
	"size": [ w, h ], //width and height of a single frame in pixels
	"dims": [ c, r ]  //number of columns and rows
}
```
Whereas an *extended* atlas is like this:
```json
{
	"type": "atlas",
	"rects": [
		[ x, y, w, h ], //rectangle in pixels
		[ ... ]
	]
}
```

#### Material map
Material maps can be placed in the same folder as a model to specify shaders and their textures. The material map for a model `foo.fbx` should be called `foo.mat.json`. If no such file can be found, `default.mat.json` is used instead, and if that doesn't exist it'll look for a `default.mat.json` higher up in the path.

The contents of a material map are like this:
```json
{
	"<material name>": {
		"shader": ID, //taken from shaders/shaders.json
		"albedo": PNG filename, //may contain one *
		"mix": PNG filename, //same as before
		"normal": PNG filename, //same as before
		"opacity": PNG filename, //same as before
		"visible": boolean, //hides mesh with this material by default if false
		"translucent": boolean, //marks mesh with this material for a later render step
	},
	"<another material>": {
		...
	}
}
```
Certain places where a model is used may provide their own textures. In those cases, the `albedo` key and its ilk should be left out entirely.

#### Text
Files for the text database go in the `text` folder.

The contents of a text file are like this:
```json
{
	//Shorter "implied US English" style
	"ID": "Some text",
	"ID": ...
	
	//Longhand style
	"ID": {
		"en": "Some text in English",
		"ja": "同じテキストを日本語で",
		"de": "Der gleiche Text auf Deutsch",
		"nl": "Dezelfde tekst in het Nederlands"
	}
}
```
Both of these styles also apply to user-visible text in other JSON files.

If a file's name ends with a dash and a language code (`foo-usen.json` for example), the shorter style *must* be used, but instead of implied US English, it implies the language in the filename. If the current game language does not match, that file is skipped entirely. All this has the side effect of allowing different translations to offer different amounts of options.


