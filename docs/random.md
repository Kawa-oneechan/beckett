# Beckett Reference

## Random namespace
The `Random` namespace provides easy access to randomized values. Internally it uses an `std::mt19937` Mersenne Twister.

### Functions

#### `Seed(unsigned int newSeed)`
Resets the randomizer seed to the given value.

#### `Seed()`
Resets the randomizer seed to the current tick count.

#### `GetInt(int min, int max)`
Returns a value from min to max, exclusively.

#### `GetInt(int max)`
Returns a value from zero to max, exclusively.

#### `GetInt()`
Returns a value from zero to RAND_MAX, exclusively.

#### `GetFloat(float min, float max)`
Returns a value from min to max, inclusively.

#### `GetFloat(float max)`
Returns a value from zero to max, inclusively.

#### `GetFloat()`
Returns a value from 0.0 to 1.0 inclusively.

#### Flip()
Returns a 50% chance of true or false.

Note the pattern: *integer* values are exclusive so giving a range of 5 to 10 will only return values from 5 to 9, while *float* values are inclusive, so giving a range of 5.0 to 10.0 will return values up to and including 10.0. **I'm not sure why I did this and I *may* change this at some point in the future.**

