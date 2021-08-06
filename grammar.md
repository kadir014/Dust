# Grammar Specification of Dust

```java
/* Operator */
+    -   *    /    ^   !
+=   -=  *=   /=   ^=
==   !=  <    <=   >   >=
and  or  xor  not  has
++   --  ..

/* Expression */
expression
operator expression            // Unary
expression operator            // Reverse Unary
expression operator expression // Binary

/* Decleration */
type identifier = expression;

/* Assignment */
identifier = expression;

/* Indexing (Subscripting) */
expression[integer_expression]

/* Importing */
import module;
       ^ module.submodule...

import member from module;
                   ^ module.submodule.subsubmodule...

/* If - Elif - Else */
if expression {statement; ...}

elif expression {statement; ...}

else {statement; ...}

/* Loops */
repeat integer_expression {statement; ...}

for identifier in iterable statement; ...}

while expression {statement; ...}

/* Enumeration */
enum {identifier|assignment, ...}
```