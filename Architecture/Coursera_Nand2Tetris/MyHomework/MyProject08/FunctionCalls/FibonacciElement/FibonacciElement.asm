// 将SP 设置为RAM[256]起始处
@256
D=A
@SP
M=D

// 设置LCL=1
@1
D=A
@LCL
M=D

// 设置ARG=2
@2
D=A
@ARG
M=D

// 设置THIS=3
@3
D=A
@THIS
M=D

// 设置THAT=4
@4
D=A
@THAT
M=D

// 将启动程序的返回地址压入栈
@bootstrap
D=A
@SP
A=M
M=D
@SP
M=M+1

// 将LCL=1 压入栈内
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1

// 将ARG=2 压入栈内
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1

// 将THIS=3压入栈内
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
// 将THAT=4压入站内
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1

// 设置新函数ARG
@5
D=A
@SP
D=M-D
@ARG
M=D

// 设置新函数LCL为 SP
@SP
D=M
@LCL
M=D

// 调用
@Sys.init
0;JMP
(bootstrap)

//function Main.fibonacci 0
(Main.fibonacci)

//push argument 0
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1

//push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1

//lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=0
@lt_0
D;JGE
@SP
A=M-1
M=-1
(lt_0)

//if-goto IF_TRUE
@SP
AM=M-1
D=M
@Main.fibonacci$IF_TRUE
D;JNE

//goto IF_FALSE
@Main.fibonacci$IF_FALSE
0;JMP

//label IF_TRUE
(Main.fibonacci$IF_TRUE)

//push argument 0
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1

//return
@LCL
D=M
@R15
M=D
@5
D=A
@R15
A=M-D
D=M
@R14
M=D
@SP
AM=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@R15
A=M-1
D=M
@THAT
M=D
@2
D=A
@R15
A=M-D
D=M
@THIS
M=D
@3
D=A
@R15
A=M-D
D=M
@ARG
M=D
@4
D=A
@R15
A=M-D
D=M
@LCL
M=D
@R14
A=M
0;JMP

//label IF_FALSE
(Main.fibonacci$IF_FALSE)

//push argument 0
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1

//push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1

//sub
@SP
AM=M-1
D=M
A=A-1
M=M-D

//call Main.fibonacci 1
@End$Main.fibonacci$0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@6
D=A
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(End$Main.fibonacci$0)

//push argument 0
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1

//push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

//sub
@SP
AM=M-1
D=M
A=A-1
M=M-D

//call Main.fibonacci 1
@End$Main.fibonacci$1
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@6
D=A
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(End$Main.fibonacci$1)

//add
@SP
AM=M-1
D=M
A=A-1
M=M+D

//return
@LCL
D=M
@R15
M=D
@5
D=A
@R15
A=M-D
D=M
@R14
M=D
@SP
AM=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@R15
A=M-1
D=M
@THAT
M=D
@2
D=A
@R15
A=M-D
D=M
@THIS
M=D
@3
D=A
@R15
A=M-D
D=M
@ARG
M=D
@4
D=A
@R15
A=M-D
D=M
@LCL
M=D
@R14
A=M
0;JMP

//function Sys.init 0
(Sys.init)

//push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1

//call Main.fibonacci 1
@End$Main.fibonacci$2
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@6
D=A
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(End$Main.fibonacci$2)

//label WHILE
(Sys.init$WHILE)

//goto WHILE
@Sys.init$WHILE
0;JMP
