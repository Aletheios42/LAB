.global _start

.section .data
    mensaje: .ascii "Hola mundo\n"
    longitud = . - mensaje

.section .text
_start:
    # Llamada al sistema write
    movl $4, %eax        # Número de syscall para write
    movl $1, %ebx        # File descriptor: stdout
    movl $mensaje, %ecx  # Puntero al mensaje
    movl $longitud, %edx # Longitud del mensaje
    int $0x80            # Llamar al kernel

    # Llamada al sistema exit
    movl $1, %eax        # Número de syscall para exit
    movl $0, %ebx        # Código de retorno: 0
    int $0x80            # Llamar al kernel
