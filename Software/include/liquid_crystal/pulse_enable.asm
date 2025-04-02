;=================================================================================================================;

;; Armazena dados estáticos inicializados (variáveis globais e constantes pré-definidas) ;;
SECTION .text
;; Para realizar a função chamada pelo código em Assembly (Assembly/C) ;;
extern lcd_drive_write
;; Para realizar a função chamada pelo código em C (C/Assembly) ;;
global pulse_enable

;=================================================================================================================;

pulse_enable:

;-----------------------------------------------------------------------------------------------------------------;

    ;; Equivalente ao enter 0, 0 ;;
    push rbp
    mov rbp, rsp

    ;; Salva o valor de data, segundo parâmetro, em r12 ;;
    mov r12, rsi
    ;; Salva fd em rbx ;;
    mov rbx, rdi 

    ;; Esses registradores forem escolhidos, pois, na arquitetura de 64 bits, após a chamada de função, ;;
    ;; integração Assembly/C, eles não perdem a informação armazenada, já que há outros que são resetados ;;

;-----------------------------------------------------------------------------------------------------------------;
    ;; Primeira chamada: en = 0 (data sem modificação) ;;

    ;; Na arquitetura de 64 bits, os primeiros argumentos de uma função são passados via registradores ;;
    ;; Os valores dos parâmetros, já estão nos registradores, no caso rdi e rsi ;;
    ;; Chama lcd_drive_write(fd, data) ;;
    call lcd_drive_write        

;-----------------------------------------------------------------------------------------------------------------;
    ;; Segunda chamada: en = 1 (ativa o bit en) ;;

    ;; Move o valor de data para rax ;;
    mov rax, r12
    ;; Seta o bit 9 (en = 1) ;;
    bts rax, 9    
    ;; Prepara os parâmetros fd e data para lcd_drive_write ;;
    mov rsi, rax                 
    mov rdi, rbx
    ;; Chama lcd_drive_write ;;
    call lcd_drive_write        

;-----------------------------------------------------------------------------------------------------------------;
    ;; Terceira chamada: en = 0 (desativa o bit en) ;;

    ;; Move o valor de data para rax ;;
    mov rax, r12
    ;; Reseta o bit 9 (en = 0) ;;
    btr rax, 9
    ;; Prepara os parâmetros fd e data para lcd_drive_write ;;
    mov rsi, rax                 
    mov rdi, rbx
    ;; Chama lcd_drive_write ;;
    call lcd_drive_write  

;-----------------------------------------------------------------------------------------------------------------;

    ;; equivalente ao leave ;;
    pop rbp
    ;; retorna para o código em C ;;
    ret

;-----------------------------------------------------------------------------------------------------------------;

;=================================================================================================================;
