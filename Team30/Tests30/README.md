## Tests Directory Breakdown

This directory contains all the tests that are used in system-level testing

The following table summarises the directory structure

| Folder | Purpose |
| ------ | ------- |
| `query-source` | This folder contains the source files which are used to generate the TXT tests required by autotester. |
| `tests-broken` | This folder contains tests which are not working, as some features have not been implemented |
| `tests-working` | This folder contains tests which work, and are tested against autotester by a CI workflow |
| `tools` | this folder contains tooling that are used to help in the creation of tests |
| `auto-autotester.py` | This python script runs autotester against all the test cases in a batch |


## Working Tests folder

The folder is further sub-divided into folders based on the test category. At the moment, it contains the following subdirectories

| Subdirectory | Category |
| ------------ | -------- |
| tests-basic-spa | This folder contains tests for the relations in basic SPA |
| tests-other | This folder contains miscelleous tests cases that are arbitrarily generated |


## Tools - Auto-Autotester

The **auto-autotester** can be invoked with the following command:

```
python3 auto-autotester.py <path-to-autotester> <path-to-test-folder>
```

The script executes all the test cases found in the folder recursively.

## Tools - json-cases-to-txt

This converts test cases stored in a more readable JSON form into the txt file required by autotester.

```
python3 json-cases-to-txt.py <input.json> <output.txt>
```

## Tools - line-number-label

This script is a rudimetary line labelling script, based on certain heuristics. It may not be entirely accurate, but is good enough for our needs.

```
python3 line-number-label.py <source.txt>
```

## Tools - matrix-generator

This script generates a matrix of tests by permutating across variables.

```
python3 matrix-generator.py <matrix.json> <output.txt>
```
