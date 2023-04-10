# Tests Directory Breakdown

This directory contains the tests that are used in system-level testing

The following table summarises the directory structure
|Folder | Purpose |
| --- | --- |
|`tests-advanced-spa` | This folder contains tests which falls under advanced SPA features (Milestone 2 & 3) |
| `tests-basic-spa` | This folder contains tests which falls under basic SPA features (Milestone 1 )|
| `tests-benchmark` | This folder contains tests to benchmark the performance or correctness of the system |
| `tests-bug-fixes` | This folder contains tests that were related to bug fixes |
| `tests-parse-failure` | This folder contains syntatically invalid source files which the system should capture |
| `tests-projection` | This folder contains tests which involves projection (BOOLEAN, Tuples) |
| `tests-query-errors` | This folder contains tests with queries that will output either a `SyntaxError` or `SemanticError` |
| `tests-source-stress` | This folder contains complex source files for SP Parsing |

For each test file, they have a ID which falls in one of the categories below
|ID Range | Description |
|---| --- |
|001 - 099 | General Tests
|100 - 199 | Clause Tests |
| 200 - 299 | Multiple clause tests|
|300 - 399 | Projection Tests |
| 400 - 499 | Stress Tests |
| 500 - 599 | Parsing Tests |
| 600 - 699 | Bug Fix Tests |
| 800 - 899 | BOOLEAN Tests |
