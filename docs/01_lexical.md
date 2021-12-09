# Lexical syntax of Dust
- [Comments](#Comments)
- [Keywords](#Keywords)
- [Identifiers](#Identifiers)
- [Literals](#Literals)
  - [Integer literals](#Integer-literals)
  - [String literals](#String-literals)

## Comments
There are two types of comments in Dust
- Line comments that terminate at the end of the line
- Block comments that can span multiple lines

```c
// This is a line comment!

...

/*
  This is a 
  multiline
  block
  comment!
*/
```

## Keywords
A keyword is a reserved word that has special usage among a Dust program. Keywords cannot be used as identifiers.
List of all keywords in Dust
```
int8    uint8    float32  array   if    not
int16   uint16   float64  enum    elif  in
int32   uint32   float    class   else  when
int64   uint64   bool     func    and   for
int128  uint128  string   import  or    while
int     uint     buffer   from    xor   repeat
```

## Identifiers
Identifiers simply represents names in a Dust program. There are few rules while creating identifiers
- Identifier cannot be a keyword
- First character of identifier must be an alphabet or underscore, rest can be anything
- Commas, periods, whitespaces or any other special characters cannot be in identifier

## Literals
Literals are constant values
- ### Integer literals
An integer can be represented in decimal using decimal [0-9] digits
```c
1, -15, 9, 365376
```
An integer can be represented in binary using `0b` prefix
```c
0b0, 0b101, 0b1001110
```
An integer can be represented in hexedecimal using `0x` prefix \
either [a-f] or [A-F] can be used
```c
0xFF, 0x13A, 0xfcf3
```
- ### String literals
Strings can be represented in both double quotes (`"..."`) and single quotes (`'...'`)
```py
"Hello this is a string", 'Hi! this is also a string'
```
  - #### Escape sequences
  Escape sequences are special character sequences that are translated into other characters to perform various tasks
    - `\n` New line
    - `\"` Double quotation mark (used in double quoted string)
    - `\'` Single quotation mark (used in single quoted string)
    - `\\` Backslash
    - `\t` Horizontal tab
    - `\xnn..` `nn..` is interpreted as a hexedecimal number
    - `\unnnn` Unicode code point (4 hexedecimal digits)
    - `\Unnnnnnnn` Unicode code point (8 hexedecimal digits)