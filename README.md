libqth -- Convert QTH into coordinates and back
===============================================

## DESCRIPTION

libqth is a tiny library built to convert QTH Locators into coordinates, and back.

## SYSTEM REQUIREMENTS

 - An ISO C99, an ISO C11 or a C++ building environment.
 - *nix-like operating system (I don't have a windows dev environment to check the library on MS).

## PYTHON MODULE

Works with Python 2.x and 3.x. Build inplace with:

```python setup.py build_ext --inplace```

Or install using:

```python setup.py install```

Use ```python``` or ```python3``` as you need.

### Difference in Python module from lib

Where the lib returns EDOM, the python module returns False, or True for the check QTH.
If no error, a dict with infos is returned.

## USAGE

You can use the library by compiling it using make, or you can run the test suite by executing the following command:

```bash
make testlib
```

The test suite return value is 0 if all tests passed, and 1 if at least 1 test failed.

## DETAILS ON QTH PRECISION

The Maidenhead QTH Locator system stores a coordinates vector in a short string of characters, arranged by alternating pairs of letters and figures.

The pair of chars are named as follow:
 * Field (precision 0): A 18x18 grid covering the whole earth. 
 * Square (precision 1): A 10x10 grid covering the content of a field.
 * Subsquare (precision 2, most common): A 24x24 grid covering the content of a square.
 * Extended square (precision 3): A 10x10 grid covering the content of a subsquare.
 * Extended pair 1 (precision 4): A 24x24 grid covering the content of an extended subsquare.
 * Extended pair 2 (precision 5): A 10x10 grid covering the content of an extended pair 1.

The precision refers to the aforementioned listing. The values are completely arbitrary, and intended for the function inner behaviour.

The most popular precision used by amateur radio operators is 2 (Field, square and subsquare).
