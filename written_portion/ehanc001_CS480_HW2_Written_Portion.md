# Task 1

## Formulate the Sudoku puzzle as a constraint satisfaction problem (CSP).

## A detailed description of all variables, domains and constrains that are sufficient to model Sudoku as a CSP.

---

A Sudoku board consists of a grid of 81 *cells,* along 9 rows and 9 columns,
partitioned into 3-cell by 3-cell *sections,* with each cell containing a number in the closed range [1-9].
Each cell can be represented with a variable, $C_{11}$ through $C_{99}$, using a matrix-like two-index notation.
For the sake of brevity, these 3x3 sections will be referred to as $S_{11}$ through $S_{33}$, using the same matrix-like notation.


|          |          |          |     |          |          |          |     |          |          |          |
| :------: | :------: | :------: | :-: | :------: | :------: | :------: | :-: | :------: | :------: | :------: |
| $C_{11}$ | $C_{12}$ | $C_{13}$ | \|  | $C_{14}$ | $C_{15}$ | $C_{16}$ | \|  | $C_{17}$ | $C_{18}$ | $C_{19}$ |
| $C_{21}$ | $C_{22}$ | $C_{23}$ | \|  | $C_{24}$ | $C_{25}$ | $C_{26}$ | \|  | $C_{27}$ | $C_{28}$ | $C_{29}$ |
| $C_{31}$ | $C_{32}$ | $C_{33}$ | \|  | $C_{34}$ | $C_{35}$ | $C_{36}$ | \|  | $C_{37}$ | $C_{38}$ | $C_{39}$ |
| -------- | -------- | -------- | \|  | -------- | -------- | -------- | \|  | -------- | -------- | -------- |
| $C_{41}$ | $C_{42}$ | $C_{43}$ | \|  | $C_{44}$ | $C_{45}$ | $C_{46}$ | \|  | $C_{47}$ | $C_{48}$ | $C_{49}$ |
| $C_{51}$ | $C_{52}$ | $C_{53}$ | \|  | $C_{54}$ | $C_{55}$ | $C_{56}$ | \|  | $C_{57}$ | $C_{58}$ | $C_{59}$ |
| $C_{61}$ | $C_{62}$ | $C_{63}$ | \|  | $C_{64}$ | $C_{65}$ | $C_{66}$ | \|  | $C_{67}$ | $C_{68}$ | $C_{69}$ |
| -------- | -------- | -------- | \|  | -------- | -------- | -------- | \|  | -------- | -------- | -------- |
| $C_{71}$ | $C_{72}$ | $C_{73}$ | \|  | $C_{74}$ | $C_{75}$ | $C_{76}$ | \|  | $C_{77}$ | $C_{78}$ | $C_{79}$ |
| $C_{81}$ | $C_{82}$ | $C_{83}$ | \|  | $C_{84}$ | $C_{85}$ | $C_{86}$ | \|  | $C_{87}$ | $C_{88}$ | $C_{89}$ |
| $C_{91}$ | $C_{92}$ | $C_{93}$ | \|  | $C_{94}$ | $C_{95}$ | $C_{96}$ | \|  | $C_{97}$ | $C_{98}$ | $C_{99}$ |

The domains of each of these variables is identical. $D(C_n) = \{1, 2, 3, 4, 5, 6, 7, 8, 9\}$.

Informally, no number may appear multiple times in the same row, column, or section.
Formally, this defines 8 constraints per row, 8 constraints per column, and 8 constraints per section.
All 216 constraints could be enumerated explicitly, however, that would make this document far too long.
These constrains are defined generally 

\break

In the following definitions, let:

$$
    \mathbb{D} = {1, 2, 3, 4, 5, 6, 7, 8, 9} \\
$$

$$
    a, b, c, d, i, j, k \in \mathbb{D}
$$

such that:

$$
    C_{ij} \neq C_{kj}
$$

$$
    C_{ij} \neq C_{ik}
$$

$$
    \mathrm{if} \; C_{ab} \in S_{ij} \land C_{cd} \in S_{ij}, \; \mathrm{then} \; C_{ab} \neq C_{cd}
$$

---

\break


