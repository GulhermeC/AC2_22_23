    .equ    READ_CORE_TIMER, 11
    .equ    RESET_CORE_TIMER, 12
    .equ    PUT_CHAR, 3
    .equ    PRINT_INT, 6   
    .data
    .text
    .globl  main

main:
    li  $t0,0                   # counter = 0
while:                          # while (1) {
    li  $v0, RESET_CORE_TIMER   #
    syscall                     #   resetCoreTimer()
    
do: 
    li $v0, READ_CORE_TIMER
    syscall
    blt $v0, 200000, do         # while(readCoreTimer() < 200000)
   
    ori $a0, $t0, 0
    li $a1, 10
    li $v0, PRINT_INT
    syscall
    addi $t0, $t0, 1

    la $a0, '\r'
    li $v0, PUT_CHAR
    syscall
    
    j   while

    li  $v0, 0
    jr  $ra