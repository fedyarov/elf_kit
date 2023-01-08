
        global _start

        section .text

_start: mov rax, 0x31000000
        push rax
        mov rax, 0x32333435
        push rax

        mov rdi, 1 ; stream 
        ;mov rsi, msg
        mov rsi, rsp ; data
        mov rdx, 1 ; data len
        mov rax, 1 ; "write"
        syscall

        xor rdi, rdi
        mov rax, 60
        syscall

        section .data
msg:    db "hi there", 10
