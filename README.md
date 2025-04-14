# FFF class wrapper
A header allowing limited use of C++ methods with Fake Function Framework.

This wrapper is intended for mixed C and C++ codebases that do not use virtual
functions, such as firmware and kernel drivers. As such it only partly supports
C++ methods - see limitations below.

It is of course possible to switch `virtual` on and off with a preprocessor
macro, but this leads to not insignificant pollution of function declarations,
especially if there is already a `TESTABLE` macro used to switch `static` on and
off. Nonetheless, a `virtual`function based framework such as Google Mock may
better suite your requirements and you should explore those first.

These instructions assume you are using Unity as the unit testing framework.

Please note this is unmaintained. I am unlikely to modify this at all except to
fix bugs that I find.

## Usage
To fake a C++ class's methods, just define these macros:

```
    FAKE_CLASS_NAME
    FAKE_VALUE_FUNCS(x_fn)
    FAKE_VOID_FUNCS(x_fn)
    FAKE_STATIC_VALUE_FUNCS(x_fn)
    FAKE_STATIC_VOID_FUNCS(x_fn)
```

The first macro should contain only the name of the class to mock. It must meet
the requirements of a C/C++ symbol name and contain no `::` operators. The other
macros comprise a list of the function signatures as would be expected by the
equivalent FFF macro. For example, the `FAKE_STATIC_VOID_FUNCS` list entries
would have the form of `FAKE_VOID_FUNC`. The below will create two functions
with signature `void(int, int, int) and void(bool)`:

```
    #define FAKE_STATIC_VOID_FUNCS(x_fn) \
        x_fn(func_1_name, int, int, int) \
        x_fn(func_2_name, bool)
```

If no method exists to go in one of the macros above, just use `NONE`:

```
    #define FAKE_VALUE_FUNCS(x_fn) \
        NONE
```

Then create the class using:

```
    MAKE_FAKE_CLASS
```

Then undef the macros (very important!):

```
    #undef FAKE_CLASS_NAME
    #undef FAKE_VALUE_FUNCS
    #undef FAKE_VOID_FUNCS
    #undef FAKE_STATIC_VALUE_FUNCS
    #undef FAKE_STATIC_VOID_FUNCS
```

When inspecting the history of the mock function, note that you need to
prepend the namespace, which is not necessary when mocking free functions.
This namespace exists to avoid infinite recursion of the member function. For
example:

```
    TEST_ASSERT_EQUAL(123, fff_mocks::[class_name]::[fn_name]_fake.arg0_val);
```

To reset the history of all mock functions in the class, call the helper
function provided:

```
    mock_class_name::reset_fakes();
```

## Limitations
Note that any limitations can be avoided by writing stub classes manually.

### Return types and arguments that have autofree mechanisms (e.g. vector)
These may produce double free errors in the test. This can be worked around
by memsetting the problematic fields in the mock's struct to zero so that at
worst `free()` is called on nullptr, which is safe. This should be done in the
`TEST_TEAR_DOWN` function for a test group.

### Const-by-value arguments
Note that at time of writing FFF doesn't permit arguments to be const-by-val
in C++, because the preprocessor macro method of generating structs to
record the arguments would declare a const field, which would not be
writable. This limitation exists in both C and C++, but in C++ it will
produce a mile-long error starting with: `error: use of deleted function
‘fff_[class_name]::...::[fn_name]_Fake()’`, which is really just saying that
there isn't a constructor. The only way to fix this is to not use const-by-val
args in the definitions above.

### References
These are just const pointers, so they have the limitation above.

### Operators
These probably cannot be mocked at all.
