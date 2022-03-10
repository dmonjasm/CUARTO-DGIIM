###############################################################################
# driver.s
###############################################################################

.code16               # código de 16 bits

###############################################################################

.text                 # sección de código
	.globl _start     # punto de entrada al código

###############################################################################

_start:
	xor %ax, %ax      # ax = 0                         |
	mov %ax, %ss      # ss = 0                         | pila en
	mov $0x9c00, %sp  # sp = 0x09c00 = 0x7c00 + 0x2000 |  ss:sp

	call clear

	xorw %ax, %ax     # 0 --> ax   |
	movw %ax, %ds     # ax --> ds  | msg --> ds:si
	movw $msg, %si    # msg --> si |

	call puts

	cli               # deshabilitar las interrupciones
	hlt               # detener el procesador

###############################################################################

clear:
	call init
	mov $2000, %cx    # video memory = 80 * 25 = 2000 words
	mov $' ', %al     # carácter blanco
.L4:
	call putc
	loop .L4
	ret

###############################################################################

init:
	xorw %di, %di     #
	movw %di, cursorl # video --> cursorh:cursorl --> es:di
	movw cursorh, %es #
	movb $0x0f, color # default color
	ret

###############################################################################

putc:                 # ascii --> al
	movw cursorl, %di
	movw cursorh, %es
	cmp $2000, %di    # estamos al final?
	jb .L1
	xorw %di, %di     # si di >= 2000 --> di = 0
.L1:
	movb color, %ah   # recuperar color
	stosw             # almacena carácter en memoria de video
	mov %di, cursorl
	mov %es, cursorh
	ret

###############################################################################

puts:              # string --> ds:si
	lodsb          # ds:si++ --> al
.L2:
	test %al, %al  # end of string?
	jz .L3
	call putc
	jmp puts
.L3:
	ret

###############################################################################

cursorl: .word 0
cursorh: .word 0xb800

color: .byte 0x0f

msg: .string "cognito, ergo sum"

###############################################################################

	.org 510      # saltar 510 bytes
	.word 0xAA55  # marca del sector de arranque

###############################################################################

