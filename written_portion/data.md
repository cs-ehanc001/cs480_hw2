| Command                                         |  Mean [ms] | Min [ms] | Max [ms] |     Relative |
|:------------------------------------------------|-----------:|---------:|---------:|-------------:|
| `./sudoku_solver --simple ../inputs/easy.dat`   |  3.7 ± 1.1 |      1.2 |      8.2 |  1.20 ± 0.44 |
| `./sudoku_solver --smart ../inputs/easy.dat`    |  3.0 ± 0.6 |      1.0 |      4.0 |         1.00 |
| `./sudoku_solver --simple ../inputs/medium.dat` |  4.4 ± 1.6 |      2.6 |      8.1 |  1.45 ± 0.61 |
| `./sudoku_solver --smart ../inputs/medium.dat`  |  3.3 ± 0.7 |      1.0 |      4.4 |  1.10 ± 0.33 |
| `./sudoku_solver --simple ../inputs/hard.dat`   |  4.5 ± 1.6 |      2.2 |      8.6 |  1.48 ± 0.61 |
| `./sudoku_solver --smart ../inputs/hard.dat`    |  3.5 ± 0.8 |      1.0 |      4.9 |  1.14 ± 0.35 |
| `./sudoku_solver --simple ../inputs/evil.dat`   | 41.6 ± 0.4 |     33.3 |     54.1 | 13.66 ± 2.89 |
| `./sudoku_solver --smart ../inputs/evil.dat`    |  7.1 ± 1.9 |      4.8 |     16.9 |  2.34 ± 0.79 |
