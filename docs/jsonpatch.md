# Beckett Reference

## JSON Patch namespace

Not meant for the game, but I'm documenting it here anyway. Might rename this if I ever get a Schema validator.

The name "JSON Patch" is actually outdated. The actual patch format being followed here is [RFC 7396](https://datatracker.ietf.org/doc/html/rfc7396) "JSON Merge Patch". The original plan was to use [RFC 6902](https://datatracker.ietf.org/doc/html/rfc6902) "JSON Patch", which is a fair bit different. The [modding](modding.md) page goes into a bit more detail on what a Merge Patch looks like.

### Functions

#### `ApplyPatch(jsonValue& source, jsonValue& patch)`
Applies `patch` to `source`, returning the result as a new `jsonValue`. If `DEEPERDOWN` is defined, prints the inputs and output to the console.

