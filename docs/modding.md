## With regards to mods

> ❗ **TODO**: Rewrite this for genericity.

Say what you want about how its developers handled *Starbound* after it came out, but they knew a thing or two about making games mod-able. And because I (Kawa) am a creature of habit who's copied the general methodology behind *Starbound* in his own projects several times over, why stop here?

To that effect, data files for Beckett engine games are to be stored in one or more `.zip` archive files *or* folders, both stored in the `/mods` folder, though it's neater to use `.zip` files. The base game's data goes in `/assets`, while the rest (in archives or folders) goes in `/mods`. Internally, they'll all mirror the same folder structure.

Exactly where the game gets the archives from is defined in `init.json`. `/assets` and `/mods` are just the default.

On startup, all asset sources are enumerated, sorted, and checked for dependencies. Then, a *master asset list* is compiled by going through this sorted list, finding each file's path relative to the asset source, listing the asset's file name, source, and if it's in a `.zip` archive, its location in said archive.

Sorting and dependency-checking are done via `manifest.json` files in each asset source. The base game data is always loaded first because its `priority` is set to an incredibly low number:
```json
{
	"id": "mygamedata",
	"friendlyName": "Base data for my game",
	"author": "You?",
	"priority": -9999
	//"namespaces": [ "..." ]
	//"dependencies": [ "..." ]
}
```
The default priority is 1. Dependencies list the `id` values of any other asset sources this one requires to be of any use. The namespaces list is currently unused.

Here's where the magic happens.

If, during the creation of the master asset list, a file path is found that's *already listed*, the one that's already there is discarded. Therefore, if you have a `foo/bar.json` in the base game data and a mod provides its own `foo/bar.json`, any attempt to load `/foo/bar.json` will pick the mod's over the base game's.

If the requested file path has a `.json` extension, we can add an extra bit of magic from *Starbound*'s playbook: we run through the master asset list to find any entry that has that same path with `.patch` added to the end (`foo/bar.json.patch`), applying each according to [RFC 7396](https://datatracker.ietf.org/doc/html/rfc7396), like so:

```json
//This would be something like "/foo/bar.json.patch" in an example mod.
{
	  "something": "blah"
}
```
Given the following starting point
```json
//This would be something like "/foo/bar.json" in the base game data.
{
	  "something": "stupid",
	  "trickery": false
}
```
this would produce the following result when trying to load `/foo/bar.json`
```json
//This would be something like "/foo/bar.json" in the base game data.
{
	  "something": "blah",
	  "trickery": false
}
```

