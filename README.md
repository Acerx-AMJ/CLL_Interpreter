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
- - [Delete Statements](#delete-statements)
- - [Exists Statements](#exists-statements)
- - [Do Statements](#do-statements)
- - [If Statements](#if-statements)
- - [While Loop](#while-loop)
- - [For Loop](#for-loop)
- - [Break, Continue & Return](#break-continue--return)
- - [Unless Statement](#unless)
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
- `!a` - if `a` is true, return false, else true. Same as `not` operator.

Binary:
- `a + b` - add `a` to `b`. If one or both of the values is a string, concatenate it and return string.
- `a - b` - subtract `a` by `b`.
- `a * b` - multiply `a` by `b`. If one value is a number (this includes characters and booleans) and other a string, then return a string that is repeated n amount of times.
- `a / b` - divide `a` by `b`.
- `a % b` - divide `a` by `b` and return remainder (works with real numbers: `2.50 % 2 -> 0.50`).
- `a ** b` - exponentiate `a` to the power of `b`.
- `a && b` - if `a` and `b` are true, return true, else false. Skips evaluating `b` if `a` is false. Same as `and` operator.
- `a || b` - if either `a` or `b` is true, return true, else false. Skips evaluating `b` is `a` is true. Same as `or` operator.
- `a %% b` - if `a` is divisible by `b`, return true, else false.
- `a ?? b` - if `a` is true, return `b`, else null.
- `a == b` - if `a` is equal to `b`, return true, else false.
- `a === b` - if `a` is equal to `b` and is the same type, return true, else false.
- `a != b` - if `a` is not equal to `b`, return true, else false.
- `a !== b` - if `a` is not equal to `b` or is not the same type, return true, else false.
- `a > b` - if `a` is greater than `b`, return true, else false.
- `a >= b` - if `a` is greater than or equal to `b`, return true, else false.
- `a < b` - if `a` is smaller than `b`, return true, else false.
- `a <= b` - if `a` is smaller than or equal to `b`, return true, else false.

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
#### Exists statements
It is possible to check if a variable exists using `exists` keyword. It returns true if the given variable exists, false otherwise.
```cxx
let x
println(exists x, exists y)  // -> true false
delete x

let y
{
   let z
}
println(exists x, exists y, exists z)  // -> false true false
```
Note that the following code is valid and may be more readable to some:
```cxx
println(exists(x), exists(y))
```
#### Do statements
Do statements simply execute a single statement after them. They might seem useless, but are really useful in `unless`, `if`, `while`, etc. statements that are covered later on.
```cxx
let x = 10
do x = 20
println(x)  // -> 20

do {
   let y = 20
   x += y
}
println(x)  // -> 40
```
Do note that even if a new scope is not defined, `do` statements create a new scope:
```cxx
do let x = 20
println(exists x)  // -> false
```
Also note that `do` statements that do not execute a scope can only execute a single statement:
```cxx
do let y = 0 println(y)  // ERROR: Y is not defined in the give scope 
```
#### If statements
If statements use the following syntax:
```cxx
if condition {
   statements
} elif condition {
   statements
} elif condition {
   statements
} else {
   statements
}
```
An if statement can have a single if clause, infinite optional elif clauses and a single optional else clause. If statements cannot start with elif clauses or else clauses. If condition is false, program will check the next clause until a condition is true, or until it checks everything. In the case that every condition is false and an else clause is present, it will be executed.

This is where `do` statements come in handy:
```cxx
let x, y = true, false
if x && y do
   println("Both X and Y are true!")
elif x do
   println("Only X is true!")
elif y do
   println("Only Y is true!")
else do
   println("None are true!")
```
Do not that the following code is valid:
```cxx
if true do {
   println("True!")
}
```
And that the following is not valid:
```cxx
if true do
   println("True!")
   println("True!")
```
#### While loop
While loop statement follows this syntax:
```cxx
while condition {

}
```
While loop will keep executing statements until condition is false. Just like in if statements, `do` keyword can be used here:
```cxx
let i = 10
while i-- > 0 do
   println(i)
```
To create an infinite loop, the condition can be left out:
```cxx
while {

}
```
#### For loop
For loops use the following syntax:
```cxx
for stmt; condition; stmt {

}
```
First statement will be executed on loop creation, condition is checked at the beginning of the loop and the second statement is executed at the end of the loop. The following code prints numbers 1-10 including:
```cxx
for let i = 1; i <= 10; i++ do
   println(i)
```
The three statements inside the for loop declaration and the scope after the for loop use the same scope.

Statements and the condition can be left out, but the semicolons must stay, so the program knows which statement is which.
```cxx
let i = 1
for ; i <= 10; do
   println(i++)
```
Infinite loop can be created like so:
```cxx
for ;; {

}
// Semicolons are fine to ommit in this case
for {

}
```
#### Break, continue & return
It is possible to break out of a loop using `break` keyword:
```cxx
while {
   // Get user input
   let ch = inputch()

   if ch == 'y' do
      break
}
println("Broke!")
```
Break statements, of course, only break out of the current loop they are in:
```cxx
for let i = 0;; i++ {
   for let j = 0;; j++ {
      print(i, j, ' ')
      if j == 5 do
         break
   }

   println()
   if i == 5 do
      break
}
```
A single iteration of a loop can be skipped using the `continue` keyword:
```cxx
for let i = 0; i < 100; i++ {
   if i %% 2 do
      continue
   println(i)
}
```
Continue statements also only target the most recent loop. Both statements ignore any other scopes than loops.

Values can be returned early from scopes using the `return` keyword:
```cxx
let x = {
   return 10
   println("This will not print!")
}
printfln("X is {}.", x)
```
Currently, return statements only return to the latest scope, so they don't work under if statements. For that, there is the `unless` statement.

Return statements must return something, if no value should be returned, null must be used:
```cxx
return null
```
#### Unless
Do, break, continue, return, delete, for, while and if statements can be ignored based on a condition with the `unless` keyword:
```cxx
do statement unless condition
break unless condition
continue unless condition
return value unless condition
delete identifier unless condition

for stmt1; condition; stmt2 {

} unless condition

while condition {

} unless condition

if condition {

} elif condition {

} else condition {

} unless condition
```
In the case that condition is false, the whole statement is skipped. Try running this and then changing x to false:
```cxx
let y = inputnum()
let x = true

if y == 0 do
   println("y is zero")
elif y > 0 do
   println("y is positive")
else do
   println("y is negative")
unless x
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