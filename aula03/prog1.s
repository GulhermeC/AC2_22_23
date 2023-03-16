    .equ    SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area
    .equ    TRISE, 0x6100       # TRISE address is 0xBF886100
    .equ    TRISB, 0x6040       # TRISB address is 0xBF886040
    .equ    LATE, 0x6120        # LATE address is 0xBF886120
    .equ    PORTB, 0x6050

    .data
    .text
    .globl  main

main:

    lui $t0, SFR_BASE_HI        # $t0= 0xBF880000
    lw  $t1, TRISE($t0)         # READ (Mem_addr = 0xBF880000 + 0x6100)
    andi $t1, $t1, 0xFFFE       # andi forçar bits a 1 == input
    sw  $t1, TRISE($t0)

    lw $t2, TRISB($t0)
    ori $t2, $t2, 0x0001        # ori forçar bits a 0 == output
    sw $t2, TRISB($t0)

    lw  $t2, LATE($t0)
    andi $t2, $t2, 0x0          # clear (everything = 0)
    sw $t2, LATE($t0)

while:
    lw $t3, PORTB($t0)          # portb = modify input rb0
    andi $t3, $t3, 0x0001       # set as input

    lw $t2, LATE($t0)               
    andi $t2, $t2, 0xFFFE       # set LATE to output in re0 
    or $t2, $t2, $t3            # move PORTB value to LATE
    sw $t2, LATE($t0)           # store

    j   while
    jr  $ra