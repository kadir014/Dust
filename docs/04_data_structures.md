# Data structures in Dust
A data structure is a particular way of organizing data so that it can be used effectively.
Here is the list of built-in data structures included in Dust
- [Enumerations](#Enumerations)
- [Arrays](#Arrays)
- [Hashmaps](#Hashmaps)

## Enumerations
Enumerations (or enums) are data structures that consists of integral constants. They are used to declare related constants and group them. Note that also many languages refers them as _data types_.
Basic syntax is
```c
enum EnumName {
    field,
    field = expression,
    ...
}
```

## Arrays
Arrays are collection of values of the same type. Their size can't be altered once initialized.
Basic syntax is
```c
array<type> identifier = expression;
```
Arrays can be initialized with array expressions used with square brackets
```c
[expression, ...]

// examples:
array<int> arr = [1, -15, 3, 0xF3A32, -0b11011];

array<uint64> arr2 = [1354326537, 46587583];

array<float> arr3 = [-35.6, 2.57648, 3.1415926]
```