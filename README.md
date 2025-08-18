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
- - [Standard Functions](#built-in-functions)
- - [Calling Functions](#calling-functions)
- - [Variables](#variable-declaration)
- - [Constants](#constant-declaration)
- - [Shadowing](#variable-shadowing)
- - [Assignment](#assignment)
- - [Scope](#scopes)
- - [Delete](#delete-statements)
- - [Escape Codes](#escape-codes)
- [Quirks](#quirks)
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
- `-` - negate value.
- `+` - do nothing.
- `--` - decrement value (only right side).
- `++` - increment value (only right side).

Binary:
- `+` - add both values together. If one or both of the values is a string, concatenate it and return string.
- `-` - subtract values apart.
- `*` - multiply both values. If one value is a number (this includes characters and booleans) and other a string, then return a string that is repeated n amount of times.
- `/` - perform division.
- `%` - perform integer division and return the remainder.
- `**` - exponentiate left value to the power of right value ((2 ** 2 ** 3) will evaluate to 2 ** 8 -> 256. Operation precedence is reversed for exponentiation).

Assignment:
- `=` - assign value to variable.
- `+=` - add value to variable and assign the result.
- `-=` - subtract the values apart and assign the result.
- `*=` - multiply values and assign the result.
- `/=` - divide the values apart and assign the result.
- `%=` - divide the values apart and assign the remainder.
- `**=` - exponentiate variable to the power of the right side and assign the result.

Parentheses (`()`) can change operator precedence as follows:
```cxx
(2 + 2) * 3  // -> 12
2 + 2 * 3  // -> 8 
```
#### Built-in functions
Built-in functions:
- `print` - print all arguments to the terminal.
- `println` - print all arguments to the terminal and newline.
- `prinf` - format first argument (must be a string) with the passed in arguments. Arguments will replace '{}'.
- `printfln` - same as `printf`, but print a newline.
- `format` - format first argument (must be a string) with the passed in arguments and return the result.

- `input` - get a string input from user. If an argument is passed in, print it.
- `inputnum` - get a number input from user. If an argument is passed in, print it.
- `inputch` - get a single character input from user. Does not ignore whitespace and does not wait for the enter key. If an argument is passed in, print it.

- `string` - convert the first argument to a string or return a null string if no arguments.
- `number` - attempt to convert the first argument to a string and return if successful or return 0 if no arguments.
- `char` - attempt to convert the first argument to a character and return if successful or return 0 if no arguments.
- `bool` - convert first argument to a boolean or return false. Empty strings, 0's and null values are false, everything else is true.
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
println(x----)  // -> -1
```
Here's where it gets quirky. `--` and `++` expressions get converted to a `+=` expression. So these expressions:
```cxx
let x = 0
println(x++++)
println(x----)
println(x--++--)
println(1++)  // Looks okay
println("String"++)  // Probably an "Invalid unary operation: ++ "String".
```
Are converted to these:
```cxx
let x = 0
println(x += 2)
println(x += -2)
println(x += -1)
println(1 += 1)  // Instead, a cryptic error! ERROR: Expected an Identifier at the left side
println("String"++)  // Instead, a cryptic error! ERROR: Expected an Identifier at the left side
```
Why is it done this way? Because in the interpreter, identifiers are all evaluated as early as possible. Which means expressions like `x----` (let's say x is 2) would get evaluated to `x--` and then `1--`, because `--` returns the value, not an identifier. In the end x would be 1, not 0. Either I have to rewrite my whole interpreter (not happening), live with the cryptic error messages in rare cases, or, also in rare cases, live without chained `--` and `++` operators. I will think about it.
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
## Quirks
Here's a list of some quirks of this language that might cause confusion:
- `--` and `++` expressions get converted to `+=` expressions. More on that [here](#assignment).
- Variables can have the same names as functions, meaning follow code is okay:
```cxx
let println = 2
println(println)
```
- For escape codes, such as colors, `\e` must be used. Other codes, such as `\033` are not supported for this.
- All operations that use null, return null, even operations such as
```cxx
println(1 + null)  // -> null
```
