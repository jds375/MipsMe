#A Basic Introduction to Assembly with MIPS#

###Introduction###

Computer programming has come a long way in the last 40 years. We have progressed from flimsy punchcards and faulty timesharing machines to high-level programming languages that are almost completely divorced from their underlying computer hardware. This is fantastic and has enabled developers to write powerful programs in easy-to-maintain and easy-to-read languages. As a result, fewer and fewer programmers are learning how to directly interface with hardware and understand the basics of machine code. This is unfortunate since there are many valuable lessons that can be learned from knowing the inner-workings of a computer and how machine code (assembly) functions. Moreover, one can't really appreciate the functionality and usability of modern languages without knowing where it all started. So, I decided to compile a brief (and interactive!) tutorial on assembly code. This tutorial bypasses the many complications in learning assembly by abstracting away complicated instruction sets, addressing issues, and other such problems. 

###The MIPS Assembly Language###

The fancy chip inside your computer is made up of millions of transistors, wires, and other electrical components. This hardware is used to manipulate data in your computer and perform powerful calculations. Most data in your computer is stored as bits, 0's and 1's. These 0's and 1's correspond to transistors that hold a little charge and a lot of charge, respectively. Complicated circuits in your chip then perform simple operations such as addition by manipulating the charge held in certain transistors that corresponds to a given number. These complicated circuits give a programmer a small arsenal of functions to manipulate the data in transistors with. For example, a programmer can use the addition circuit, followed by the subtraction circuit, to perform a simple calculation. The set of functions that correspond to these circuits is what assembly language is. As a result, each chip has their own assembly language. Most chips are made by Intel and use an assembly language called x86. The x86 language is very complicated and contains tons of different functions. This makes it difficult to learn. A simpler and smaller assembly language is called MIPS. Here, I will use a subset of the MIPS language to teach you the basics of assembly. There are many academic resources on MIPS if you want to learn more. (Don't be dissapointed if you wanted to learn about x86. It really doesn't matter what language you learn the basics in. All of the same principles apply and x86 contains almost every command that MIPS does. So you'll at the very least be able to use a subset of x86 commands, which is more than enough.)

###Memory: Registers and the Stack###

In order to perform meaningful calculations we need to be able to use memory to store our results and perform intermediate steps. Memory is stored in two spots in hardware, the registers and the stack. Most computers have 32 registers that are each capable of holding a 32-bit number. These registers can be accessed quickly and allow for fast computation. However, they are expensive in terms of hardware. This is problematic since any powerful program is going to need to work with way more than 32 instances of memory. In order to accomodate the billions of numbers involved in computation, we need another form of memory that is more scalable than registers. This form of memory is the stack. The stack is capable of holding
huge amounts of memory. However, accessing this memory is more costly and not as efficient as using registers. A programmer needs to use both of these types of memory when writing a program in assembly. Below we will discuss when and where to use them. But for now, this explanation is sufficient. Just remember, registers are fast and few, whereas stack is slow and large. 

###Math and Logic: Performing Basic Arithmetic###

The first command we will discuss is <i>add $x, $y, $z</i>. This command takes the contents of register y and adds it to the contents of register z. The result is then stored in register x. Thus, we might have <i>add $3, $10, $31</i>. This command adds the numbers that registers 10 and 31 hold, then stores the result in register 3 (overriding whatever is already there). When referring to the contents of a register we use the $ symbol. These types of commands that perform an arithmetic operation using 3 registers are known as R-type arithmetic commands. They perform the operation using the second and third argument and then store the result in the first. A list of such is below:
```
add $x, $y, $z - addition
sub $x, $y, $z - subtraction
or $x, $y, $z - bitwise OR
and $x, $y, $z - bitwise AND
xor $x, $y, $z - bitwise XOR
nor $x, $y, $z - bitwise NOR
sllv $x, $y, $z - shift-left logical ($z specifies the spaces to shift $y by)
srlv $x, $y, $z - shift-right logical ($z specifies the spaces to shift $y by) 
slt $x, $y, $z - set less than (sets $x to 1 if $y<$z, otherwise $x is set to 0)
```
Don't worry if you aren't sure how bitwise operations and shifts work. It's just important to know that they are available for now.

Another very similar type of command is an I-type arithmetic command. These are the same as R-type arithmeic commands, except the third argument is no longer a register. Instead, it is a plain number. This number will be used instead of the contents of $z. We differentiate these types of commands by adding the letter 'i' to the end of the function name and omitting the register delimiter from the third argument. For example, the command <i>addi $4, $6, 5</i> takes the contents of register 6 and adds 5 to it then stores the result in register 4. A list of these commands is below:
```
addi $x, $y, z - addition (there is no subtraction command here since we can simply let z be a negative number)
ori $x, $y, z - bitwise OR
andi $x, $y, z - bitwise AND
xori $x, $y, z - bitwise XOR
nori $x, $y, z - bitwise NOR
sll $x, $y, z - shift-left logical
srl $x, $y, z - shift-right logical
```

We can now write a simple program using these commands. This program will check if at the value in $2 or the value in $3 is greater than the sum of the values in $2 and $3.
```
add $4, $2, $3 # Stores $2+$3 in $4
slt $5, $4, $2 # Stores a 1 in $5 if $4 is less than $2, 0 otherwise
slt $6, $4, $3 # Stores a 1 in $6 if $4 is less than $3, 0 otherwise
or $1, $5, $6 # Stores a 1 in $1 if either or both $2 and $3 are larger than the sum in $4, 0 otherwise
```

Our program works very simply. We compute the sum of $2 and $3 and store the result in $4. We then store a 1 in $5 if that sum is less than $2. Likewise, we store a 1 in $6 if that sum is less than $3. (0's are stored otherwise). Finally, we store a 1 in $1 if either $5 or $6 contained a 1. Thus, $1 will contain a 1 if either $2 or $3 was larger than their sum. 

One complaint might be that we haven't set any of the values in the registers. If we can't set the contents of $2 and $3, then our computation is meaningless since we don't know what we're computing (registers default to hold the number 0). We can solve this by loading values automatically into $2 and $3 as such:
```
addi $2, $0, 5 # Loads the sum of 5 and $0 (defaulted to 0) into $2. Thus, 5 is stored in $2.
addi $3, $0, -2 # Likewise
add $4, $2, $3 # Stores $2+$3 in $4
slt $5, $4, $2 # Stores a 1 in $5 if $4 is less than $2, 0 otherwise
slt $6, $4, $3 # Stores a 1 in $6 if $4 is less than $3, 0 otherwise
or $1, $5, $6 # Stores a 1 in $1 if either or both $2 and $3 are larger than the sum in $4, 0 otherwise
```
We can thus modify our program by changing the third arugment in the first two addi commands. Congrats. We can now perform simple logic and math using assembly.

###Code Blocks: Sections###
In nearly all programming languages blocks of code are separated into methods, which are further separated into smaller blocks via comments and other tools. Features like this do not explicitly exist in assembly. Instead, we have sections. A section is essentially an in-line delimiter marking where a certain section of the code begins. For example, we can modify the example above and insert sections. A section is a one-word name followed by a colon. We might have:
```
setup:
addi $2, $0, 5
addi $3, $0, -2
work:
add $4, $2, $3
slt $5, $4, $2
slt $6, $4, $3
result:
or $1, $5, $6
```
We have thus broken our code into three sections. The code will function in the exact same way. These section delimiters are useful since they can further clarify code. However, they serve a much more important purpose. There are assembly commands that take a section delimiter name as an argument. We take a look at these commands below.

###Control Flow: If-Statements and Branches###
One of the cornerstones of programming is the if-statement. Almost every program has them and they are very natural to use. In assembly, this doesn't directly exist. Instead, there are branches. A branch command takes three arguments. The first is the name of a section. The second two arguments are registers. The command then does some comparison using the second two arguments. If the result is true, then the computer will 'jump' to the command following the section specified as argument one. If the result is false, then the code continues to execute sequentially. Let's first take a look at the command 'beq exit, $2, $3'. This command checks if the contents of $2 and $3 are the same. If they are, then the program will jump to the section labeled 'exit'. Otherwise, the code will simply continue to execute in order. We have two main branch commands:
```
beq x, $y, $z - Jumps to section x if the contents of $y and $z are the same (otherwise continue in order)
bne x, $y, $z - Jumps to section x if the contents of $y and $z are different (otherwise continue in order)
```
We see that branches allow us to control the flow of our code. They are very similar to if-statements. (In this tutorial we ignore delay slots). Consider the following code:
```
addi $2, $0, 5 # Set $2 to contain the number 5
increment:
addi $3, $3, 1 # Increment the contents of $3 by 1
bne increment, $2, $3 # Jump to the increment: section if the contents of $2 and $3 are not equal, otherwise continue
addi $1, $0, 1 # Set $1 to contain the number 1
```

This code will increment the contents of $3 by 1 until it is equal to the contents of $2. When they are equal the contents of $1 gets set to 1. It is easy to see how it executes. We jump to the section 'increment' if $2 and $3 are not equal in the bne command. When we go back to this section we call addi again, thus incrementing $3. The process loops until we are done. 

Now, we have done some actual programming. We have learned how to implement control-flow into our assembly code. This will allow us to write code that
can emulate structures such as if-statemens and loops.

###More Control Flow: Jumps and Loops###
In addition to branches there are also commands that let us jump without doing a comparison. The first such command is simply 'j x'. This will have the code jump to section x. An example might be 'j increment'. This will jump to the section labeled increment immediately. Another such command is jal. It stands for jump-and-link. It functions exactly like 'j'. It will jump to the section of code specified as its first argument. However, in addition jal will store the line number of the next sequential command in register 31 (for the experts: this is a simplification. It doesn't actually store the line to the next command, it stores the hex address. This complicates the explanation, but isn't necessary to get the point across). Why might we want to store the line of a command in a register? This is what the command 'jr' does. An example is 'jr $31' or 'jr $5'. This command will jump to the line of code specified inside of the given register. Our complete list is:
```
j x # Jumps to the section x
jal x # Jumps to the section x and stores the next line number of the next command in $31
jr $x # Jumps to the line of code indexed by the number contained in $x
```
Jumps are useful in the same way branches are. Later, we will write some more complicated programs that will use jumps.

###More Memory: How to Use the Stack###
So far all of the examples we have gone over and all of our tools can only utilize 32 different number values. This is because we only have 32 registers to store numbers. We can solve this problem with the stack. MIPS has commands that let us take the value in a register and store it in a large pool of memory that can hold thousands of numbers. Likewise, MIPS has commands that let us pull values from the stack and load them into our registers. Thus, we can use as much memory as we need.

The first command is the store command. It has the form 'sb $x, $y'. It takes the value held in $x and puts it in the stack at the index in $y (this is yet another simplification that ignores addressing and byte/word distinction, but this is fine for our purposes). The counterpart to sb is lb. It thas the form 'lb $x, $y'. It takes the value stored in the stack at the index in $y and loads it into register $x. We thus have the commands:
```
sb $x, $y - Stores the value at $x in the stack at the index in $y
lb $x, $y - Loads the value in the stack at the index in $y into register $x
```
We can create a simple program to put a bunch of incremental values in the stack as follows:
```
addi $1, $0, 0 # Initializes $1 to hold 0
loop:
sb $1, $1 # Stores the value in $1 at the index in $1
addi $1, $1, 1 # $1++
j loop # Restart the loop
```
We have now learned all of the basic commands. The rest of assembly programming simply amounts to combining these techniques to write more sophisticated
programs.

###Putting it all Together:###
Here we will write a program to compute the nth digit of the Fibonacci sequence. We will do two versions. We will do the loop version and the recursive
version. 

We will first implement the loop version. Recall that the Fibonacci sequence is calculated by adding the previous two values in the sequence together, where the first number in the sequence is 0 and the second is 1. 
In puesdocode we have:
```
fib(n) {
int previous2 = 0;
int previous1 = 1;
 if (n == 0) {
   return previous2;
 } else if (n == 1) {
   return previous1;
 } else {
   for (int i = 2; i <= n; i++) {
     int next = previous2 + previous1;
     previous2 = previous1;
     previous1 = next;
   }
   return previous1; 
 }
}
```
In assembly (note this is not the most efficient, but the simplest to read):
```
initialization:
addi $2, $0, n # We load and store the value of n in $2
addi $3, $0, 0 # We load and store the vaue of i in $3
addi $4, $0, 0 # We load and store the value of previous2 in $4
addi $5, $0, 1 # We load and store the value of previous1 in $5
addi $31, $0, 0 # We use this as the result register, which is defaulted to 0
addi $0, $0, 0 # We permanently store the value 0 in $0
addi $1, $0, 1 # We permanently store the value 1 in $20
zerocheck:
beq exit, $0, $2 # If the value in the register holding n is 0 jump to exit since there the return is defaulted to 0 anyway; else continue in order
onecheck:
beq returnone, $1, $2 # If the vaue in the register holding n is 1 jump to segment that will return 1; else continue in order
forloop:
beq returnresult, $3, $2 # If the value of i is equal to that of n, then the loop condition is no longer valid and we should return; otherwise do loop
add $6, $4, $5 # We compute the sum of previous2 and previous1 and store it in $6, which holds the value 'next' from the psuedocode
add $4, $0, $5 # We set previous2 to the value of previous1 by overriding it with 0 + the value in previous1, which is just previous1
add $5, $0, $6 # We set previous1 to the value of previous2 by overriding it with 0 + the value in next, which is just next
addi $3, $3, 4 # i++
j forloop # Restart loop
returnone:
add $31, $0, $1 # We add 1 to the return register which is initially 0 to have a return value of 1
j exit # Go to exit the file by going to the final line
returnresult:
add $31, $0, $5 # Set the result to previous1
j exit # Go to exit
exit:
# end of file. code ends here.
```

We will now implement the recursive version. It is less efficient, but teaches about recursion in assembly. The recursive formula for the Fibonacci sequence is f(n) = f(n-1) + f(n-2). That is, the nth value in the sequence is given by the sum of the n-1th value and the n-2th value. We have the base case that f(0) = 0 and f(1) = 1. A normal psuedocode for implementing this recursive equation would be:
```
fib(n) {
 if (n == 0) {
   return 0;
 } else if (n == 1) {
   return 1;
 } else {
   return (fib(n-1) + fib(n-2));
 }
}
```
We now implement the assembly code. This will be more complicated than the last function. This is because we will need to use the stack to store the data from our succesive calls. For example, when we call fib(3) we will need to loop back to the top of our code due to the recursive calls to fib(2),  fib(1), and fib(0). As we go down this chain of calls we will need to store their results and keep track of which fib(n) we are evaluating, as well as which comes next and in what order. There isn't enough room to do this with the registers. What if n were large? Thus we have to use the stack.

However, we are doing a lot more than just simply storing some values here and there in the stack. We are using it to keep track of our entire  algorithm. This means we will need to come up with a system of storing data in the stack that will work for any value of n.  We do this as follows. We will start at index 0 in our stack and work our way up for each succesive call. We will only use one line of stack for each call. This will store the value of n. For more complicated functions, we might use multiple stack lines per call. This would require us to properly index and space our use of the stack.

Our plan will thus work as follows. We will begin each call by loading the value of n off of the stack. We will keep track of our index in the stack using a register, $3. When we are done with computation we will add the result to register $31 (this works sonce addition is associative and the Fibonacci recurrence is reslly just a large sum). When we need to make another call, we will put corresponding values of n onto the stack and increment $3 to know where we are to go in the stack.
```
setinput:
addi $0, $0, 0 # We permanently store the value 0 in $0
addi $1, $0, 1 # We permanently store the value 1 in $1
addi $2, $0, n # Set input here. This is the register we will store the current value of n in
sb $2, $0 # Place n in the stack. It will be used as the current n since $3, our call index, defaults to 0
function:
lb $2, $3 # We load and store the value of n in $2 based off the index in the stack we are on, $3
zerocheck:
beq returnzero, $0, $2 # If the value in the register holding n is 0 jump to segment that will return 0; else continue in order
onecheck:
beq returnone, $1, $2 # If the vaue in the register holding n is 1 jump to segment that will return 1; else continue in order
recurse:
addi $4, $2, -2 # Store n-2 in $4
addi $5, $2, -1 # Store n-1 in $5
addi $6, $3, 1 # Store the stack index plus one
sw $4, $3 # Override the n value in the current stack position to be n-2 since we are done with the current n and will reuse its stack space
sw $5, $6 # Put n-1 at the next stack position up
addi $3, $3, 1 # Increment our position in the stack by 1 to accomdate the new call (we only increment by 1 since we overrode the current spot to save room)
j function # Recurse
returnzero:
add $31, $31, $0 # We add 0 to the sum of the recursive calls
j nextcall # We jump to the code that sets up the stack for the next call
returnone:
add $31, $31, $1 # We add 1 to the sum of the recursive calls
j nextcall # We jump to the code that sets up the stack for the next call
nextcall:
beq exit, $3, $0 # If the stack index we are on is 0 there are no more calls to evaluate so we exit; otherwise we set up the next call
addi $3, $3, -1 # We finished the most recent call and now move back down the stack
j function # We go back to the start
exit:
# end of file. code ends here.
```
###Wrap-Up###
Congrats. You now know the basics of assembly. You can find more information in the sources below. If you're up for a challenge try the memoization version of fib.
