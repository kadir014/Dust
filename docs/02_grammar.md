# Grammar Specification of Dust

```java
/* Operator */
+    -   *    /    ^   !
+=   -=  *=   /=   ^=
==   !=  <    <=   >   >=
and  or  xor  not  in
++   --  ..

/* Expression */
expression
operator expression            // Unary
expression operator            // Reverse Unary
expression operator expression // Binary

/* Decleration */
type identifier;
type identifier = expression;
type<type, ...> = expression;
type<type, ...> identifier = expression;

/* Assignment */
identifier = expression;

/* Indexing (Subscripting) */
expression[expression];

/* Function call */
expression();
expression(expression, ...);

/* Function decleration */
func identifier(type arg, ...) {statement; ...}
func<type, ...> identifier(type arg, ...) {statement; ...}

/* Importing */
import module;
       ^ module.submodule...

import member from module;
                   ^ module.submodule...

/* If - Elif - Else */
if expression {statement; ...}

elif expression {statement; ...}

else {statement; ...}

/* Loops */
repeat expression {statement; ...}

for identifier in iterable {statement; ...}

while expression {statement; ...}

/* Enumeration */
enum {identifier|assignment, ...}
```