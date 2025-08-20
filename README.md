# CLL
CLL is a high-level interpreted language inspired by Python and C++ made to be comfortable and easy to use.
### ToC
- [Compiling](#compiling)
- - [CMake](#build-with-cmake)
- - [Clang](#build-with-clang)
- - [GCC](#build-with-gcc)
- [Usage](#usage)
- [Features](#features)
- - [Comments](#comments)
- - [Numbers](#numbers)
- - [Strings](#strings)
- - [Characters](#characters)
- - [Booleans](#booleans)
- - [Null](#null)
- - [Operators](#supported-operators)
- - [Operator Precedence](#operator-precedence)
- - [Standard Functions](#built-in-functions)
- - [Calling Functions](#calling-functions)
- - [Variables](#variable-declaration)
- - [Constants](#constant-declaration)
- - [Shadowing](#variable-shadowing)
- - [Assignment](#assignment)
- - [Scope](#scopes)
- - [Delete](#delete-statements)
- - [Escape Codes](#escape-codes)
## Compiling
CLL uses no dependencies and is easy to build.
#### Build with CMake
```bash
mkdir build  # Create the build directory
cmake -B build  # Create build files
cmake --build build  # Build CLL interpreter. Executable 'cll' can be found in the build folder
```
#### Build with Clang
```bash
clang++ -Iinclude src/*.cpp -o cll
```
#### Build with GCC
```bash
g++ -Iinclude src/*.cpp -o cll
```
## Usage
CLL interpreter expects a single argument. It currently has no help support.
```bash
cll [CODE]
```
To run code directly:
```bash
cll 'println("Hello, World!")'
```
To read a file and run the contents:
```bash
cll file.cll
```
## Features
#### Comments
CLL uses C-style comments:
```cxx
// This is a single-line comment
/*
   This is a
   multi-line
   comment
*/
```
#### Numbers
Number type includes any number, both integral and floating:
```cxx
let x, y, z = 29, 6.12, -2.29
```
CLL supports scientific notation and binary, hexadecimal, and octal prefixes:
```cxx
let s1, s2 = 1.125e3, 188e-4
let b1, b2 = 0b1011, 0b11111111
let h1, h2 = 0XFFFF, 0x2f
let o1, o2 = 0o17, 0O224
```
#### Strings
Strings can be defined with `"` quotes:
```cxx
let name = "John"
```
Some [escape codes](#escape-codes) are supported:
```cxx
print("This is \"CLL\".\n")
// Outputs:
// This is "CLL".
```
#### Characters
Characters can be defined with `'` quotes. A character must be one character or one escape code large:
```cxx
let c1, c2 = 'a', '\n'
```
Just like strings, characters support [escape codes](#escape-codes).
#### Booleans
Booleans are either true or false. Use `true` constant to represent a true boolean and `false` to represent a false boolean:
```cxx
let x, y = true, false
```
#### Null
Null values do not represent a value. Statements like variable declaration, delete statements, etc. always return a null value. Values operating with null will turn to null. Null values can be defined using `null` constant:
```cxx
let x = null
```
#### Supported operators
Unary:
- `-a` - negate `a`.
- `+a` - do nothing.
- `a--` - decrement `a` (only right side).
- `a++` - increment `a` (only right side).
- `!a` - if `a` is true, return false, else true

Binary:
- `a + b` - add `a` to `b`. If one or both of the values is a string, concatenate it and return string.
- `a - b` - subtract `a` by `b`.
- `a * b` - multiply `a` by `b`. If one value is a number (this includes characters and booleans) and other a string, then return a string that is repeated n amount of times.
- `a / b` - divide `a` by `b`.
- `a % b` - divide `a` by `b` and return remainder (works with real numbers: `2.50 % 2 -> 0.50`).
- `a ** b` - exponentiate `a` to the power of `b`.
- `a && b` - if `a` and `b` are true, return true, else false
- `a || b` - if either `a` or `b` is true, return true, else false
- `a %% b` - if `a` is divisible by `b`, return true, else false
- `a ?? b` - if `a` is true, return `b`, else null
- `a == b` - if `a` is equal to `b`, return true, else false
- `a === b` - if `a` is equal to `b` and is the same type, return true, else false
- `a != b` - if `a` is not equal to `b`, return true, else false
- `a !== b` - if `a` is not equal to `b` or is not the same type, return true, else false
- `a > b` - if `a` is greater than `b`, return true, else false
- `a >= b` - if `a` is greater than or equal to `b`, return true, else false
- `a < b` - if `a` is smaller than `b`, return true, else false
- `a <= b` - if `a` is smaller than or equal to `b`, return true, else false

Ternary:
- `a ? b : c` - if a is true, return b, else c

Assignment:
- `a = b` - assign `b` to `a`.
- `a += b` - add `a` to `b` and assign the result.
- `a -= b` - subtract `a` by `b` and assign the result.
- `a *= b` - multiply `a` by `b` and assign the result.
- `a /= b` - divide `a` by `b` and assign the result.
- `a %= b` - divide `a` by `b` and assign the remainder.
- `a **= b` - exponentiate `a` to the power of `b` and assign the result.

Parentheses (`()`) can change operator precedence as follows:
```cxx
(2 + 2) * 3  // -> 12
2 + 2 * 3  // -> 8 
```
#### Operator precedence
The following table lists precedence and associativity of operators and expressions.
|Precedence|Expression|Description|Associativity|
|-|-|-|-|
|1|`identifier`, `number`, `character`, `string`, `()`, `{}`, `variable declaration`, `delete statement`|Primary expressions, parentheses, scopes, statements.|Single|
|2|`a, b, c`|Argument list expression.|Single|
|3|`a(b, c)`|Call expression.|Single|
|4|`a--`, `a++`|Increment, decrement.|Single|
|5|`-a`, `+a`, `!a`|Unary minus and plus, logical not.|Right-to-left|
|6|`a ** b`|Exponentiation.|Right-to-left|
|7|`a * b`, `a / b`, `a % b`|Multiplication, division, remainder.|Left-to-right|
|8|`a + b`, `a - b`|Addition, subtraction.|Left-to-right|
|9|`a > b`, `a >= b`, `a < b`, `a <= b`|Relation operators.|Left-to-right|
|10|`a == b`, `a === b`, `a != b`, `a !== b`, `a %% b`|Equality operators and divisible operator.|Left-to-right|
|11|`a && b`|Logical and.|Left-to-right|
|12|`a \|\| b`|Logical or.|Left-to-right|
|13|`a = b`, `a += b`, `a -= b`, `a *= b`, `a /= b`, `a %= b`, `a **= b`|Assignment and compound assignment.|Left-to-right|
|14|`a ?? b`|Binary condition.|Right-to-left|
|15|`a ? b : c`|Ternary condition.|Right-to-left|

Left-to-right and right-to-left associativity shows in which direction expressions get parsed. Single means that expressions cannot be chained.
#### Built-in functions
The following table lists all built-in functions and their expected arguments and return values.
|Function|Arguments|Return|Description|
|-|-|-|-|
|`print`|(...)|`Null`|Print all arguments separated by a space.|
|`println`|(...)|`Null`|Print all arguments separated by a space and print a newline at the end.|
|`printf`|(`String`, ...)|`Null`|Format the string using other arguments and print it (e.g. `print("Hello, {}!", "World")` -> `Hello, World!`).|
|`printfln`|(`String`, ...)|`Null`|Format the string using other arguments and print it, and print a newline.|
|`format`|(`String`, ...)|`String`|Format the string using other arguments and return the result.|
|`raise`|(`String`, ...)|`No Return`|Format the string using other arguments and raise an error at the line of the call.|
|`assert`|(`Boolean`, `String`)|`Null`|If boolean is true, throw an error at the line of the call with the given error.|
|`throw`|(`String`, `Number`)|`No Return`|Both arguments are optional. Throw an error at no line with the given exit code.|
|`exit`|(`Number`)|`No Return`|Argument is optional. Exit the program with the given exit code.|
|`input`|(`Any`)|`String`|Print argument (if any) and return user's input as string.|
|`inputnum`|(`Any`)|`Number`|Print argument (if any) and return user's input as number.|
|`inputch`|(`Any`)|`Character`|Print argument (if any) and return user's input as a single character.|
|`string`|(`Any`)|`String`|Convert argument to string, or return empty string if no arguments.|
|`number`|(`Any`)|`Number`|Convert argument to number, or return 0 if no arguments.|
|`char`|(`Any`)|`Character`|Convert argument to character, or return 0 if no arguments.|
|`bool`|(`Any`)|`Boolean`|Convert argument to boolean, or return false if no arguments.|
#### Calling functions
Functions can be called using `()` operators like so:
```cxx
println()  // Print a newline
println(35)  // Print '35'
printfln("Hello, {} {}!", "John", "Doe")  // Print 'Hello, John Doe!'
```
#### Variable declaration
As could be seen before, variables can be declared in two ways. For a single variable:
```cxx
let x = 1
let y = false
let x = "Hello!"
```
For multiple variables at once:
```cxx
let x, y, z = 1, false, "Hello!"
```
For multiple variable declaration, it is important that identifier count is smaller than value count. Here's what happens with different counts:
```cxx
let x = 1  // x is 1
let x, y = 1  // Both x and y are 1
let x, y = 1, 2  // x is 1 and y is 2
let x, y, z = 1, 2  // x is 1, y is 2 and z is null
let x, y, z  // All 3 values are null

let x, y = 1, 2, 3 // ERROR! Identifier count is smaller than value count
```
#### Constant declaration
Constants can be declared the same way as variables, except constants cannot implicitly be defined null, here's the same example as before, using constants:
```cxx
// Do note that constants cannot be shadowed unlike variables, let's ignore that in this example
con x = 1  // x is 1
con x, y = 1  // Both x and y are 1
con x, y = 1, 2  // x is 1 and y is 2
con x, y, z = 1, 2  // ERROR: z would be null
con x, y, z  // ERROR: All 3 values would be null
con x, y, z = 1, 2, null  // x is 1, y is 2 and z is null. Explicit null constants are okay

con x, y = 1, 2, 3 // ERROR! Identifier count is smaller than value count
```
#### Variable shadowing
Variables can be shadowed, whereas constants cannot:
```cxx
let x = "String"
println(x)  // -> String
let x = 24
println(x)  // -> 24

con x = true  // We shadow x once again
println(x)  // -> true
con x = null  // ERROR: Cannot shadow constant
```
#### Assignment
Variables can be assigned new values without the need for shadowing. Constants, again, cannot be assigned a new value:
```cxx
let x = 23
println(x)  // -> 23
println(x = 9)  // -> 9
println(x += 2)  // -> 11
println(x++)  // -> 12

con y = 2
println(y)  // -> 2
y = 3  // ERROR: Cannot assign to a constant
```
Variables can be incremented using `++` and decremented using `--`:
```cxx
let x = 0
println(x++)  // -> 1
println(x--)  // -> 0
```
Unlike other binary and unary operators, `++` and `--` operators cannot be chained (e.g. `x-- --`), for that, there are the `+=` and `-=` operators.
#### Scopes
Scopes can be defined using `{}` like so:
```cxx
let x = 1
{
   let x = 2
   println(x)  // -> 2
   // Global scope: x = 1, local scope: x = 2, local takes precedence
}
println(x)  // -> 1
// Global scope: x = 1, no local scopes, using global
```
Scopes return last value or null, if it's a statement:
```cxx
let z = {
   let x, y = 1, 2
   x + y
}
// x and y are no longer defined here
println(z)  // -> 3
```
#### Delete statements
Variables can be deleted early using `delete` keyword. Constants cannot be deleted.
```cxx
let x, y, z = 1, 2, x + y
delete x, y
// x and y do not exist anymore, while z is fine to use
println(z)
```
#### Escape codes
Supported escape codes:
- `\a` - Terminal bell.
- `\b` - Backspace.
- `\t` - Horizontal tab.
- `\n` - Newline.
- `\v` - Vertical tab.
- `\f` - Formfeed.
- `\r` - Carriage return.
- `\e` - Escape.
- `\\` - Escape backslash.
- `\'` - Escape single quote.
- `\"` - Escape double quotes.

Colors can be added by using the `Escape` escape code as follows (note that Windows may not support this):
- `\e[30m` - Black color. 
- `\e[31m` - Red color.
- `\e[32m` - Green color.
- `\e[33m` - Yellow color.
- `\e[34m` - Blue color.
- `\e[35m` - Magenta color.
- `\e[36m` - Cyan color.
- `\e[37m` - White color.
- `\e[39m` - Default color (reset only the color).
- `\e[0m` - Reset (reset all color and text effects).

Example:
```cxx
println("\e[31m This is red text! \e[0m")
println("\e[32m This is green text! \e[0m")
println("\e[34m This is blue text! \e[0m")
```
There's a lot more that can be done using the `Escape` escape code. [This](https://gist.github.com/ConnerWill/d4b6c776b509add763e17f9f113fd25b) is a good reference on escape codes.