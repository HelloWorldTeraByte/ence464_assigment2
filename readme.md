# Jacobi Relaxation Algorithms

Contents
 - `doc/` - assignment instructions, lab notes, report template.
 - `graphs/` - Generated graphs and figures
 - `reference/` - correct output for test comparison.
 - `src/` - different Poisson solver implementations
 - `ci.py` - Tester, timer, benchmarker and visualizer
 - `Makefile` - Makefile to build everything
 - `test.sh` - automatic testing script.

Building
--------
Every algorithm implementation can be built running make in the root directory

Testing
-------
All of the algorithms can be tested by running ci.py file. Make sure that the testing flag is enabled.

Benchmarking
-------
Different benchmarks can be run from ci.py. Enable the benchmarks to be run by enabling the global flags.