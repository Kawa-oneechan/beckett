# Beckett Reference

## JSON Utilities

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

