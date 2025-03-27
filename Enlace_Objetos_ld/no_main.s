.global _start
.text
_start:
    # Tu código aquí
    movl $1, %eax    # syscall exit
    movl $42, %ebx   # código de retorno
    int $0x80        # interrupción para llamada al sistema
