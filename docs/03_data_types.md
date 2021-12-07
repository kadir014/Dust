# Basic data types in Dust
A data type is a classification that specifies which type of value a variable holds. \
Here is the list of built-in data types included in Dust
- [Integers](#Integers)
- [Floating-point numbers](#Floating-point-numbers)
- [Booleans](#Booleans)
- [Strings](#Strings)
- [Buffers](#Buffers)

## Integers
An integer is a number without a fractional component. Integers are either signed or unsigned, keyword for unsigned ones starts with `uint` and signed ones starts with `int`. \

| Signed   | Unsigned | Memory              |
|----------|----------|---------------------|
| `int8`   | `uint8`  | 8 bits (1 byte)     |
| `int16`  | `uint16` | 16 bits (2 bytes)   |
| `int32`  | `uint32` | 32 bits (4 bytes)   |
| `int64`  | `uint64` | 64 bits (8 bytes)   |
| `int128` | `uint128`| 128 bits (16 bytes) |

Note that `int32` and `int` are the same, so are `uint32` and `uint`.

## Floating-point numbers
A floating-point number, _or float_, is a number with decimal points.

| Type      | Memory            |
|-----------|-------------------|
| `float32` | 32 bits (4 bytes) |
| `float64` | 64 bits (8 bytes) |

Note that `float32` and `float` are the same.

## Booleans
A boolean, _or bool_, is a data type with one of two possible values to represent a logical value; `true` and `false`. Bools takes up only 8 bit (1 byte) of space.

## Strings
A string is an array of characters. Strings supports UTF-8 encoded characters by default because every character in the string takes up 32 bits (4 bytes).

## Buffer
A buffer is an array of characters. Unlike strings, every character takes up only 8 bits (1 byte). Buffers are usually used when encoding strings, sending/receiving raw socket data, etc.. 