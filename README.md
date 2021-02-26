# CS 360 - Systems Programming

<h1> Assignment 4: <br>C code targeted at the Linux platform that performs like a shell pipeline</h1>

02/25/2021

<h2> Instruction: </h2>

<p>"Write a program (in C) called assignment4.c targeted at the Linux platform that performs like a shell pipeline. Example:
# ./assignment4 ls : sort = # ls | sort"</p>

<h3> Program Interface (Required) </h3>

<h4> ./assignment4 (arg1) : (arg2) </h4>
<p>"Where: (arg1) and (arg2) are optional parameters that specify the programs
to be run. If (arg1) is specified but (arg2) is not, then (arg1) should be
run as though there was not a colon. Same for if (arg2) is specified but
(arg1) is not."</p>
    
<h5> Helpful functions: fork, exec, dup (or dup2), open/ close, exit, wait, etc. <br> Do not use popen or system. </h5>
