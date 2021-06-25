# Mayura Manawadu - CO4352 Advanced Algorithms
## _Dancing Links Sudoku Solver_

###### An efficient Sudoku Solver using the Dancing Links Algorithm by Donald Knuth. Here Dancing Links structure in applied on Sudoku puzzle of any size in the form of Exact Cover problem. The approach is much faster than backtracking.

>  Copy the input.txt which contains the sudoku puzzle in to the root directory of sudoku.exe 

 Run the following command

```sh
sudoku input_hex3.txt
```

###### input format as follows

> 0 2 0 0 0 7 5 0 8 
0 0 7 0 0 0 3 0 0 
1 0 0 0 0 2 0 6 0 
0 0 5 0 0 0 4 0 0 
0 3 0 8 1 6 0 0 0 
0 0 0 4 0 0 0 8 0 
5 0 0 0 6 1 8 0 0 
0 0 3 0 4 0 6 0 0 
9 0 0 5 7 0 0 4 0

###### output will be written to input1_output.txt


> 3 2 4 6 9 7 5 1 8 
6 5 7 1 8 4 3 2 9 
1 9 8 3 5 2 7 6 4 
8 6 5 7 2 9 4 3 1 
4 3 9 8 1 6 2 5 7 
2 7 1 4 3 5 9 8 6 
5 4 2 9 6 1 8 7 3 
7 1 3 2 4 8 6 9 5 
9 8 6 5 7 3 1 4 2 

