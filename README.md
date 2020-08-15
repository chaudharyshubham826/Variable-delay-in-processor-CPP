# Variable-delay-in-processor-CPP

Assignment_10 README
Done by:              Shubham               2018CS10641

⦁	Please make a "out.txt" file for the output of the program and "instructions.txt" for the input instructions in the same folder as this file. 
⦁	Instructions should be of the form "add $t1, $t2, $t3" with no extra spaces.
⦁	INPUT IS TAKEN AS "./exe x N", where" "x" is a float and "N" is an integer.
⦁	The function "random_value_generator" generates random numbers using "mt19937" in C++

		In this assignment, we have implemented variable delay feature in our pipeline. It takes x, the probability and N, the cycles as input. By generating a random number between 0 and 1(both inclusive), the program concludes if the memory access was a hit or miss(only for data memory access). If it is a hit, program executes normally but if, it is a miss, the program takes (N-1) more cycles. In this way, the behaviour of the processor can be seen for different values of "x" and "N".
Instruction memory: Instructions are read from a input file "instructions.txt" and stored in the instruction memory. Instruction memory is of the type <vector<vector<string>>. For ex, "add $t1, $t2, $t3" would be stored as {"add", "$t1", "$t2", "t3"} on the instruction memory.
Data memory: Data memory is an array of integers.
Comparison in performance in terms of clock cycles as well as IPC with different values of "x" and "N":
				The performance of the pipelined processor is better when "x" is high and "N" is as low as possible. It can be seen both from no. of clock cycles as well as the IPC, as shown below:(We haven't added the phots because the file size was becoming too large, and moodle was not allowing to submit)
⦁	Test file used for this demonstration is included in the same folder as this file.
1) x = 0.2, N = 5
	Total clock cycles = 43
	IPC = 0.418605
2) x = 0.4, N = 5
	Total clock cycles = 35
	IPC = 0.514286
3) x = 0.6, N = 5
	Total clock cycles = 39
	IPC = 0.461538
4) x = 0.8, N = 5
	Total clock cycles = 27
	IPC = 0.666667
5) x = 1, N = 5
	Total clock cycles = 23
	IPC = 0.782609
6) x = 0, N = 5
	Total clock cycles = 51
	IPC = 0.352941
7) x = 0.6, N = 10
	Total clock cycles = 50
	IPC = 0.36
8) x = 0.6, N = 50
	Total clock cycles = 268
	IPC = 0.0671642
9) x = 0.6, N = 100
	Total clock cycles = 320
	IPC = 0.05625
10) x = 0.8, N = 100
	Total clock cycles = 221
	IPC = 0.081448
11) x = 0.9, N = 100
	Total clock cycles = 122
	IPC = 0.147541
12) x = 0.99, 0.95, N = 100
	Total clock cycles = 23
	IPC = 0.782609

	Clearly, there is a significant improvement in both clock cycles consumed and IPC. It was visible taht when x was >= 0.9, the penalty was significantly lower. And also, when N was lower, it also had much lower penalty. 
		Thus, faster memories and intelligent memory heirarchy are keys for fast computers. Bad memeory access time can make a day and night difference when normal day to day computations are done, i.e, when the test cases are very large!!!
