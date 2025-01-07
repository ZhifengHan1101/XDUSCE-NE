.data
	prompt:.asciiz "please input a value:"
	result:.asciiz "result:"
.text
main:
	li $v0,4
	la $a0,prompt	# 提示用户输入
	syscall

	li $v0,5		# 系统调用把控制台中的数据读入寄存器
	syscall

	move $t0,$v0	# 把斐波那契数列的第几项存入t0

	beq $t0,1,input_1	# 如果输入为1或者2，则直接跳转到输出部分
	beq $t0,2,input_2
	
	li $t1,0		# 寄存器t1，放置f[n-2]，初始置为0
	li $t2,1		# 寄存器t2，放置f[n-1]，初始置为1
	li $t4,1		# 寄存器t4，放置f[n]，即当前项，初始置为1
	li $t3,4 		# 寄存器t3，当前项的索引，初始为4，因为前3项已经被初始化
loop:
	bgt $t3,$t0,out	# 如果t3的值大于t0的值，即如果当前索引值大于用户输入索引值，则循环结束跳转到out
	move $t1,$t2	# 将寄存器t2的值赋值给t1，更新f[n-2]
	move $t2,$t4	# 将寄存器t4的值赋值给t2，更新f[n-1]
	add $t4,$t1,$t2	# 求和，更新f[n]
	addi $t3,$t3,1	# 当前索引项加一更新
	j loop		# loop循环
input_1:
	li $t4,0		# 第1项，直接将当前项置为0即可跳转到out
	j out
input_2:
	li $t4,1		# 第2项，直接将当前项置为1即可跳转到out
	j out
out:
	li $v0,4
	la $a0,result	# 给用户输出提示
	syscall

	li $v0,1
	move $a0,$t4	# 将当前项，即所求项赋值给a0，并输出
	syscall
	
	li $v0,10
	syscall
