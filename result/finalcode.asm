.data
.space  392

_string1: .asciiz "foo_1 "
_string10: .asciiz "true_2 "
_string11: .asciiz "false_2 "
_string12: .asciiz "a1 "
_string13: .asciiz "a2 "
_string14: .asciiz "false_3 "
_string15: .asciiz "* "
_string16: .asciiz "aaaaa "
_string17: .asciiz "bbbbb "
_string18: .asciiz "case_int_1 "
_string19: .asciiz "case_int_2 "
_string2: .asciiz "foo_2 "
_string20: .asciiz "case_int_3 "
_string21: .asciiz "case_int_4 "
_string22: .asciiz "case_int_5 "
_string23: .asciiz "case_int_6 "
_string24: .asciiz "case_int_default "
_string25: .asciiz "case_char_1 "
_string26: .asciiz "case_char_2 "
_string27: .asciiz "case_char_3 "
_string28: .asciiz "case_char_default "
_string3: .asciiz "foo_3 "
_string4: .asciiz "foo_4 "
_string5: .asciiz "foo_5 "
_string6: .asciiz "foo_6 "
_string7: .asciiz " "
_string8: .asciiz "true_1 "
_string9: .asciiz "false_1 "
 
.text
move $fp,$sp
li $s1,392
sub $sp,$sp,$s1
li $s1,97
sw $s1,-76($fp)
li $s1,98
sw $s1,-80($fp)
li $s1,99
sw $s1,-84($fp)
li $s1,100
sw $s1,-64($fp)
li $s1,200
sw $s1,-68($fp)
li $s1,300
sw $s1,-72($fp)
move $fp,$sp
li $s1,536
sub $sp,$sp,$s1
j main
 
foo1 : 
sw $ra,0($fp)
lw $s1,-12($fp)
sw $s1,-64($fp)
lw $s1,-16($fp)
sw $s1,-68($fp)
li $s1,1
sw $s1,-72($fp)
lw $s1,-72($fp)
sw $s1,-64($fp)
la $a0,_string1
li $v0,4
syscall
lw $v0,-64($fp)
lw $ra,0($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
jr $ra
 
foo2 : 
sw $ra,0($fp)
la $a0,_string2
li $v0,4
syscall
li $s1,2
sw $s1,-64($fp)
lw $v0,-64($fp)
lw $ra,0($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
jr $ra
 
foo3 : 
sw $ra,0($fp)
lw $s1,-12($fp)
sw $s1,-64($fp)
la $a0,_string3
li $v0,4
syscall
li $s1,51
sw $s1,-68($fp)
lw $v0,-68($fp)
lw $ra,0($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
jr $ra
 
foo4 : 
sw $ra,0($fp)
li $s1,52
sw $s1,-68($fp)
lw $s1,-68($fp)
sw $s1,-64($fp)
la $a0,_string4
li $v0,4
syscall
lw $v0,-64($fp)
lw $ra,0($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
jr $ra
 
foo5 : 
sw $ra,0($fp)
la $a0,_string5
li $v0,4
syscall
lw $ra,0($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
jr $ra
 
foo6 : 
sw $ra,0($fp)
lw $s1,-12($fp)
sw $s1,-64($fp)
la $a0,_string6
li $v0,4
syscall
li $s1,1
sw $s1,-68($fp)
lw $s1,-64($fp)
lw $s2,-68($fp)
beq $s1,$s2,.label1
j .label2
 
.label1 : 
lw $v0,-64($fp)
lw $ra,0($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
jr $ra
j .label3
 
.label2 : 
li $s1,1
sw $s1,-72($fp)
lw $s1,-64($fp)
lw $s2,-72($fp)
sub $s2,$s1,$s2
sw $s2,-76($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $s1,-76($fp)
sw $s1,-12($sp)
move $fp,$sp
li $s1,104
sub $sp,$sp,$s1
jal foo6
sw $v0,-80($fp)
lw $s1,-64($fp)
lw $s2,-80($fp)
mult $s1,$s2
mflo $s2
sw $s2,-84($fp)
lw $v0,-84($fp)
lw $ra,0($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
jr $ra
 
.label3 : 
 
main : 
li $s1,100
sw $s1,-72($fp)
li $s1,101
sw $s1,-76($fp)
li $s1,400
sw $s1,-64($fp)
li $s1,500
sw $s1,-68($fp)
li $s1,2
sw $s1,-224($fp)
lw $s1,-224($fp)
sw $s1,-80($fp)
li $s1,1
sw $s1,-228($fp)
li $s1,10
sw $s1,-232($fp)
lw $s3,-232($fp)
lw $s2,-228($fp)
sll $s2,$s2,2
sub $s2,$fp,$s2
sw $s3,-84($s2)
li $s1,2
sw $s1,-236($fp)
li $s1,20
sw $s1,-240($fp)
lw $s3,-240($fp)
lw $s2,-236($fp)
sll $s2,$s2,2
sub $s2,$fp,$s2
sw $s3,-84($s2)
li $s1,1
sw $s1,-248($fp)
lw $s2,-248($fp)
sll $s2,$s2,2
li $t1,84
add $t1,$s2,$t1
sub $t1,$fp,$t1
lw $s3,0($t1)
sw $s3,-244($fp)
li $s1,10
sw $s1,-252($fp)
lw $s1,-244($fp)
lw $s2,-252($fp)
add $s2,$s1,$s2
sw $s2,-256($fp)
lw $s1,-256($fp)
lw $s2,-80($fp)
mult $s1,$s2
mflo $s2
sw $s2,-260($fp)
lw $s1,-260($fp)
sw $s1,-80($fp)
lw $a0,-80($fp)
li $v0,1
syscall
la $a0,_string7
li $v0,4
syscall
li $s1,97
sw $s1,-264($fp)
li $s1,10
sw $s1,-268($fp)
lw $s1,-264($fp)
lw $s2,-268($fp)
add $s2,$s1,$s2
sw $s2,-272($fp)
lw $s1,-272($fp)
sw $s1,-168($fp)
lw $a0,-168($fp)
li $v0,11
syscall
la $a0,_string7
li $v0,4
syscall
li $s1,1
sw $s1,-276($fp)
lw $s1,-276($fp)
sw $s1,-80($fp)
li $s1,1
sw $s1,-280($fp)
li $s1,2
sw $s1,-284($fp)
lw $s3,-284($fp)
lw $s2,-280($fp)
sll $s2,$s2,2
sub $s2,$fp,$s2
sw $s3,-84($s2)
li $s1,97
sw $s1,-288($fp)
li $s1,65
sw $s1,-292($fp)
lw $s1,-288($fp)
lw $s2,-292($fp)
sub $s2,$s1,$s2
sw $s2,-296($fp)
lw $s1,-168($fp)
lw $s2,-296($fp)
sub $s2,$s1,$s2
sw $s2,-300($fp)
li $s1,2
sw $s1,-304($fp)
li $s1,3
sw $s1,-308($fp)
lw $s1,-304($fp)
lw $s2,-308($fp)
mult $s1,$s2
mflo $s2
sw $s2,-312($fp)
lw $s1,-312($fp)
lw $s2,-80($fp)
sub $s2,$s1,$s2
sw $s2,-316($fp)
li $s1,1
sw $s1,-324($fp)
lw $s2,-324($fp)
sll $s2,$s2,2
li $t1,84
add $t1,$s2,$t1
sub $t1,$fp,$t1
lw $s3,0($t1)
sw $s3,-320($fp)
lw $s1,-316($fp)
lw $s2,-320($fp)
add $s2,$s1,$s2
sw $s2,-328($fp)
lw $s1,-300($fp)
lw $s2,-328($fp)
add $s2,$s1,$s2
sw $s2,-332($fp)
lw $s1,-332($fp)
sw $s1,-212($fp)
lw $a0,-212($fp)
li $v0,11
syscall
la $a0,_string7
li $v0,4
syscall
li $s1,1
sw $s1,-336($fp)
li $s1,1
sw $s1,-340($fp)
lw $s3,-340($fp)
lw $s2,-336($fp)
sll $s2,$s2,2
sub $s2,$fp,$s2
sw $s3,-84($s2)
li $s1,98
sw $s1,-344($fp)
li $t1,0x7fffeffc
lw $s1,-76($t1)
lw $s2,-344($fp)
bne $s1,$s2,.label4
j .label5
 
.label4 : 
la $a0,_string8
li $v0,4
syscall
j .label6
 
.label5 : 
la $a0,_string9
li $v0,4
syscall
 
.label6 : 
li $t1,0x7fffeffc
lw $s1,-80($t1)
li $t1,0x7fffeffc
lw $s2,-84($t1)
sub $s1,$s1,$s2
bgez $s1,.label7
j .label8
 
.label7 : 
la $a0,_string10
li $v0,4
syscall
j .label9
 
.label8 : 
la $a0,_string11
li $v0,4
syscall
 
.label9 : 
li $s1,100
sw $s1,-348($fp)
li $t1,0x7fffeffc
lw $s1,-64($t1)
lw $s2,-348($fp)
beq $s1,$s2,.label10
j .label11
 
.label10 : 
li $s1,300
sw $s1,-352($fp)
li $t1,0x7fffeffc
lw $s1,-68($t1)
lw $s2,-352($fp)
beq $s1,$s2,.label13
j .label14
 
.label13 : 
la $a0,_string12
li $v0,4
syscall
j .label15
 
.label14 : 
la $a0,_string13
li $v0,4
syscall
 
.label15 : 
j .label12
 
.label11 : 
la $a0,_string14
li $v0,4
syscall
 
.label12 : 
 
.label16 : 
li $s1,1
sw $s1,-360($fp)
lw $s2,-360($fp)
sll $s2,$s2,2
li $t1,84
add $t1,$s2,$t1
sub $t1,$fp,$t1
lw $s3,0($t1)
sw $s3,-356($fp)
li $s1,10
sw $s1,-364($fp)
lw $s1,-356($fp)
lw $s2,-364($fp)
sub $s1,$s1,$s2
bltz $s1,.label17
j .label18
 
.label17 : 
li $s1,1
sw $s1,-368($fp)
li $s1,1
sw $s1,-376($fp)
lw $s2,-376($fp)
sll $s2,$s2,2
li $t1,84
add $t1,$s2,$t1
sub $t1,$fp,$t1
lw $s3,0($t1)
sw $s3,-372($fp)
li $s1,1
sw $s1,-380($fp)
lw $s1,-372($fp)
lw $s2,-380($fp)
add $s2,$s1,$s2
sw $s2,-384($fp)
lw $s3,-384($fp)
lw $s2,-368($fp)
sll $s2,$s2,2
sub $s2,$fp,$s2
sw $s3,-84($s2)
la $a0,_string15
li $v0,4
syscall
j .label16
 
.label18 : 
li $s1,2
sw $s1,-388($fp)
li $s1,1
sw $s1,-392($fp)
li $s1,2
sw $s1,-396($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $s1,-392($fp)
sw $s1,-12($sp)
lw $s1,-396($fp)
sw $s1,-16($sp)
move $fp,$sp
li $s1,92
sub $sp,$sp,$s1
jal foo1
sw $v0,-400($fp)
lw $s3,-400($fp)
lw $s2,-388($fp)
sll $s2,$s2,2
sub $s2,$fp,$s2
sw $s3,-84($s2)
li $s1,2
sw $s1,-408($fp)
lw $s2,-408($fp)
sll $s2,$s2,2
li $t1,84
add $t1,$s2,$t1
sub $t1,$fp,$t1
lw $s3,0($t1)
sw $s3,-404($fp)
lw $a0,-404($fp)
li $v0,1
syscall
la $a0,_string7
li $v0,4
syscall
li $s1,3
sw $s1,-412($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
move $fp,$sp
li $s1,84
sub $sp,$sp,$s1
jal foo2
sw $v0,-416($fp)
lw $s3,-416($fp)
lw $s2,-412($fp)
sll $s2,$s2,2
sub $s2,$fp,$s2
sw $s3,-84($s2)
li $s1,3
sw $s1,-424($fp)
lw $s2,-424($fp)
sll $s2,$s2,2
li $t1,84
add $t1,$s2,$t1
sub $t1,$fp,$t1
lw $s3,0($t1)
sw $s3,-420($fp)
lw $a0,-420($fp)
li $v0,1
syscall
la $a0,_string7
li $v0,4
syscall
li $s1,2
sw $s1,-428($fp)
li $s1,97
sw $s1,-432($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $s1,-432($fp)
sw $s1,-12($sp)
move $fp,$sp
li $s1,88
sub $sp,$sp,$s1
jal foo3
sw $v0,-436($fp)
lw $s3,-436($fp)
lw $s2,-428($fp)
sll $s2,$s2,2
sub $s2,$fp,$s2
sw $s3,-172($s2)
li $s1,2
sw $s1,-444($fp)
lw $s2,-444($fp)
sll $s2,$s2,2
li $t1,172
add $t1,$s2,$t1
sub $t1,$fp,$t1
lw $s3,0($t1)
sw $s3,-440($fp)
lw $a0,-440($fp)
li $v0,11
syscall
la $a0,_string7
li $v0,4
syscall
li $s1,3
sw $s1,-448($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
move $fp,$sp
li $s1,88
sub $sp,$sp,$s1
jal foo4
sw $v0,-452($fp)
lw $s3,-452($fp)
lw $s2,-448($fp)
sll $s2,$s2,2
sub $s2,$fp,$s2
sw $s3,-172($s2)
li $s1,3
sw $s1,-460($fp)
lw $s2,-460($fp)
sll $s2,$s2,2
li $t1,172
add $t1,$s2,$t1
sub $t1,$fp,$t1
lw $s3,0($t1)
sw $s3,-456($fp)
lw $a0,-456($fp)
li $v0,11
syscall
la $a0,_string7
li $v0,4
syscall
sw $sp,-4($sp)
sw $fp,-8($sp)
move $fp,$sp
li $s1,80
sub $sp,$sp,$s1
jal foo5
li $s1,4
sw $s1,-464($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $s1,-464($fp)
sw $s1,-12($sp)
move $fp,$sp
li $s1,104
sub $sp,$sp,$s1
jal foo6
sw $v0,-468($fp)
lw $s1,-468($fp)
sw $s1,-164($fp)
lw $a0,-164($fp)
li $v0,1
syscall
la $a0,_string7
li $v0,4
syscall
li $v0,5
syscall
sw $v0,-80($fp)
lw $a0,-80($fp)
li $v0,1
syscall
la $a0,_string7
li $v0,4
syscall
li $v0,12
syscall
sw $v0,-168($fp)
lw $a0,-168($fp)
li $v0,11
syscall
la $a0,_string7
li $v0,4
syscall
la $a0,_string16
li $v0,4
syscall
la $a0,_string17
li $v0,4
syscall
lw $a0,-80($fp)
li $v0,1
syscall
la $a0,_string7
li $v0,4
syscall
li $s1,10
sw $s1,-472($fp)
lw $s1,-472($fp)
sw $s1,-216($fp)
li $s1,2
sw $s1,-476($fp)
lw $s1,-476($fp)
lw $s2,-216($fp)
beq $s1,$s2,.label20
li $s1,5
sw $s1,-480($fp)
lw $s1,-480($fp)
lw $s2,-216($fp)
beq $s1,$s2,.label21
li $s1,10
sw $s1,-484($fp)
lw $s1,-484($fp)
lw $s2,-216($fp)
beq $s1,$s2,.label22
j .label19
 
.label20 : 
la $a0,_string18
li $v0,4
syscall
j .label19
 
.label21 : 
la $a0,_string19
li $v0,4
syscall
j .label19
 
.label22 : 
la $a0,_string20
li $v0,4
syscall
j .label19
 
.label19 : 
li $s1,20
sw $s1,-488($fp)
lw $s1,-488($fp)
sw $s1,-216($fp)
li $s1,2
sw $s1,-492($fp)
lw $s1,-492($fp)
lw $s2,-216($fp)
beq $s1,$s2,.label24
li $s1,5
sw $s1,-496($fp)
lw $s1,-496($fp)
lw $s2,-216($fp)
beq $s1,$s2,.label25
li $s1,10
sw $s1,-500($fp)
lw $s1,-500($fp)
lw $s2,-216($fp)
beq $s1,$s2,.label26
j .label27
 
.label24 : 
la $a0,_string21
li $v0,4
syscall
j .label23
 
.label25 : 
la $a0,_string22
li $v0,4
syscall
j .label23
 
.label26 : 
la $a0,_string23
li $v0,4
syscall
j .label23
 
.label27 : 
la $a0,_string24
li $v0,4
syscall
j .label23
 
.label23 : 
li $s1,97
sw $s1,-504($fp)
lw $s1,-504($fp)
sw $s1,-220($fp)
li $s1,97
sw $s1,-508($fp)
lw $s1,-508($fp)
lw $s2,-220($fp)
beq $s1,$s2,.label29
li $s1,98
sw $s1,-512($fp)
lw $s1,-512($fp)
lw $s2,-220($fp)
beq $s1,$s2,.label30
li $s1,99
sw $s1,-516($fp)
lw $s1,-516($fp)
lw $s2,-220($fp)
beq $s1,$s2,.label31
j .label32
 
.label29 : 
la $a0,_string25
li $v0,4
syscall
j .label28
 
.label30 : 
la $a0,_string26
li $v0,4
syscall
j .label28
 
.label31 : 
la $a0,_string27
li $v0,4
syscall
j .label28
 
.label32 : 
la $a0,_string28
li $v0,4
syscall
j .label28
 
.label28 : 
li $v0,10
syscall
