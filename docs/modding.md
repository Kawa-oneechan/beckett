## With regards to mods

> ❗ **TODO**: Rewrite this for genericity.

Say what you want about how its developers handled *Starbound* after it came out, but they knew a thing or two about making games mod-able. And because I (Kawa) am a creature of habit who's copied the general methodology behind *Starbound* in his own projects several times over, why stop here?

To that effect, the above JSON files are to be stored in one or more `.zip` archive files, and in folders under `/mods`. The base game's data goes in one particular spot with a very specific file name, while the rest (in archives or folders) goes in `/mods`. Internally, they'll all mirror the same folder structure.

Exactly where the game gets the archives from is defined in `init.json`.

On startup, all asset sources are enumerated, sorted, and checked for dependencies. Then, a *master asset list* is compiled by going through this sorted list, finding each file's path relative to the asset source, listing the asset's file name, source, and if it's in a `.zip` archive, its location in said archive.

Here's where the magic happens.

If, during the creation of the master asset list, a file path is found that's *already listed*, the one that's already there is discarded. Therefore, if you have a `villagers/lolly.json` in the base game data and a mod provides its own `villagers/lolly.json`, any attempt to load Lolly will pick the mod's over the base game's.

If the requested file path has a `.json` extension, we can add an extra bit of magic from *Starbound*'s playbook: we run through the master asset list to find any entry that has that same path with `.patch` added to the end (`villagers/lolly.json.patch`), applying each according to [RFC 7396](https://datatracker.ietf.org/doc/html/rfc7396), like so:

```javascript
//This would be something like "mods/example/villagers/lolly.json.patch"
{
  "name": "Pop"
}
```

For the sake of speed and sanity, this patching should be done while the game itself as a whole is starting up. So when you start it up, the game loads all villager and item data in one go (excluding models, textures, and sounds which are done on demand and can't be patched only replaced), and *that's* when the patching happens. After all, we're supposed to see them on the title screen so why not?

Stuff like taking Kid Cat's helmet off would involve texture replacements, which are covered by simply having a mod with the proper file paths.

## Directory structure
```
📀 <asset source root>
├─ 📁 animations
│  └─ 🦾 Animations
├─ 📁 bjts
│  ├─ 📄 Custom tag list
│  └─ 📜 Tag scripts
├─ 📁 cameras
│  └─ 📄 Camera presets
├─ 📁 cinematics
│  └─ 📁 <one folder per item>
│     ├─ 📄 Panel layout
│     └─ 🖼 Textures
├─ 📁 hobbies
│  └─ 📄 JSON
├─ 📁 icons
│  ├─ 📁 items
│  │  └─ 🖼 Textures
│  └─ 📁 reactions
│     └─ 🖼 Textures
├─ 📁 items
│  ├─ 📁 outfits
│  │  └─ 📁 <one folder per type>
│  │     └─ 📁 <one folder per item>
│  │        ├─ 📄 Item definition
│  │        ├─ 🦾 Item model
│  │        └─ 🖼 Textures
│  ├─ 📁 furniture
│  │  └─ 📁 <one folder per item>
│  │     ├─ 📄 Furniture definition
│  │     ├─ 🦾 Furniture model
│  │     └─ 🖼 Textures
│  ├─ 📁 tools
│  │  └─ 📁 <one folder per item>
│  │     ├─ 📄 Tool definition
│  │     ├─ 🦾 Tool model
│  │     └─ 🖼 Textures
│  └─ ...
├─ 📁 music
│  ├─ 📁 bgm
│  │  ├─ 📁 interiors
│  │  │  └─ 🎵 Streams
│  │  ├─ 📁 clock
│  │  ├─ 📁 events
│  │  └─ 📜 Connective data
│  └─ 📁 kk
│     ├─ 📁 live
│     │  └─ 🎵 Streams
│     ├─ 📁 hifi, retro, cheap, phono
│     └─ 📜 Singalong data
├─ 📁 personalities
│  └─ 📄 Personality definitions
├─ 📁 player
│  ├─ 📁 eyee
│  │  └─ 📁 Numbered options
│  │     └─ 🖼 Textures
│  ├─ 📁 hair
│  │  └─ 📁 Numbered options
│  │     ├─ 🦾 Model
│  │     └─ 🖼 Textures
│  ├─ 📁 mouth
│  │  └─ 📁 Numbered options
│  │     └─ 🖼 Textures
│  ├─ 📁 outfits
│  │  └─ 🦾 Outfit models
│  ├─ 🦾 Model
│  └─ 🖼 Textures
├─ 📁 schemas
│  └─ 📄 Schema files
├─ 📁 shaders
│  ├─ 📄 Shader definitions
│  └─ 📄 GLSL vertex and buffer shaders
├─ 📁 sound
│  ├─ 📁 ui
│  ├─ 📁 voices
│  ├─ 📁 effects
│  └─ 📜 Connective data
├─ 📁 species
│  └─ 📁 <one folder per item>
│     ├─ 📄 Species definition
│     ├─ 🦾 Species model
│     └─ 🦾 Outfit models
├─ 📁 text
│  └─ 📄 Text library entries
├─ 📁 ui
│  ├─ 📁 dialogue
│  │  └─ 🖼 Textures
│  ├─ 📁 species
│  │  └─ 🖼 Species silhouettes
│  ├─ 📄 Panel layouts
│  └─ 🖼 Textures
├─ 📁 villagers
│  └─ 📁 <one folder per item>
│     ├─ 📄 Villager definition
│     ├─ 🦾 Specific model, if called for
│     └─ 🖼 Textures
└─ 📁 scripts
   └─ 📜 Lua scripts
(more to be defined)
```

## Some tricks to remember

* Images can have a JSON file with the same basename to work as an atlas. The trick is, you can put atlas data and, for example, panel layout data in the same file, like the title screen logo does.
* Though items of a given type *must* be stored in their own folder which is itself in the folder for that type (to allow standardized texture names etc), exactly *where* in that type's folder doesn't matter. `<root>/furniture/dev/greencube` is as valid as `<root>/furniture/greencube`, but just `<root>/furniture` is not. In the same vein `<root>/villagers/special/tomnook` is as valid as `<root>/villagers/cats/ugly/tabby`.
* So long as two different items with different paths also have different namespaces in their IDs, there's no problem.
* As such, mods need to take care to copy their target's path exactly lest they affect the wrong green cube or whatever.

