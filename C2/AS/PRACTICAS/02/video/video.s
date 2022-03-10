###############################################################################
# video.s
###############################################################################

.code16                # código de 16 bits

.text                  # sección de código
	.globl _start      # punto de entrada al código

_start:	
	xorw %ax, %ax      # 0 --> ax   |
	movw %ax, %ds      # ax --> ds  | msg --> ds:si
	movw $msg, %si     # msg --> si |

	movw $0xb800, %ax  # 0xb800 --> ax |
	movw %ax, %es      # ax --> es     | video --> es:di
	xorw %di, %di      # 0 --> di      |

	movb $0x0c, %ah    # color rojo

char:
	lodsb              # ds:[si++] --> al
	orb %al, %al       # ¿es al == 0?
	jz fin             # si al != 0 imprimir el siguiente carácter 
	stosw              # ax --> es:[di++]
	jmp char

fin:
	cli                # deshabilitar las interrupciones
	hlt                # detener el procesador

msg: .string "cognito, ergo sum!"  # datos en la sección de código

	.org 510           # saltar 510 bytes
	.word 0xAA55       # marca del sector de arranque
