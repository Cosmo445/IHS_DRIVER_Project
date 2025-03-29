SECTION .text
extern lcd_drive_write
global pulse_enable

pulse_enable:
    push rbp
    mov rbp, rsp

    mov r12, rsi             ; Salva o valor original de data em r12
    mov rbx, rdi             ; Salva fd em rbx

    ;; Primeira chamada: en = 0 (data sem modificação)
   
    call lcd_drive_write         ; Chama lcd_drive_write(fd, data)

    ;; Segunda chamada: en = 1 (ativa o bit en)
    mov rax, r12
    bts rax, 9                   ; Seta o bit 9 (en = 1)
    mov rsi, rax                 ; Prepara o parâmetro data para lcd_drive_write
    mov rdi, rbx
    call lcd_drive_write         ; Chama lcd_drive_write

    ;; Terceira chamada: en = 0 (desativa o bit en)
    mov rax, r12
    btr rax, 9                   ; Limpa o bit 9 (en = 0)
    mov rsi, rax                 ; Prepara o parâmetro data para lcd_drive_write
    mov rdi, rbx
    call lcd_drive_write         ; Chama lcd_drive_write

    pop rbp
    ret