;=================================================================================================================;

;; Armazena código executável (as instruções em assembly que o processador executa) ;;
SECTION .text
;; Para chamar a função que o corpo está no código em C ;;
extern lcd_drive_write
;; Para realizar a função chamada pelo código em C ;;
global pulse_enable

;=================================================================================================================;

pulse_enable:

;----------------------------------------------------------------------------------------------------------------;
    ;; en - 0
    ;; Começa a rotina: prepara novo stack frame, empilha ebp, move o valor de esp para ebp e aloca bytes ;;
    enter 0,0
    ;; Move data, empilhado na troca de rotina, para eax ;;
    mov eax, [ebp+12]
    ;; Move fd, também empilhado, para ebx ;;
    mov ebx, [ebp+8]
    ;; empilha os argumentos necessários para realizar a chamada da função ;;
    push eax
    push ebx
    ;; chama a função lcd_drive_write
    call lcd_drive_write
    ;; desaloca os espaços empilhados para a chamada ;;
    add esp, 8
;----------------------------------------------------------------------------------------------------------------;
    ;; en - 1
    ;; através de testes vimos que o valor em eax estava sendo mudado ao retornar da função,
    ;; devido a isso colocamos novamente no eax o valor de data
    mov eax, [ebp+12]
    ;; coloca o bit que está na posição 9 do eax para 1 (set en = 1) ;;
    bts eax, 9
    ;; refaz passos para um nova chamada ;;
    push eax
    push ebx
    call lcd_drive_write
    add esp, 8
;----------------------------------------------------------------------------------------------------------------;
    ;; en - 0
    mov eax, [ebp+12]
    ;; coloca o bit que está na posição 9 do eax para 0 (reset en = 0) ;;
    btr eax, 9
    ;; refaz passos para um nova chamada ;;
    push eax
    push ebx
    call lcd_drive_write
    add esp, 8
;----------------------------------------------------------------------------------------------------------------;
    ;; após esses passos foi escrito o caracter desejado no lcd ;;
    ;; inverter as ações do enter: move ebp para esp e restaura o valor antigo de ebp ;;
    leave
    ;; retorna para a função no código em C ;;
    ret
;----------------------------------------------------------------------------------------------------------------;

;=================================================================================================================;

; Define a seção .note.GNU-stack como não executável
SECTION .note.GNU-stack noalloc noexec nowrite progbits

;=================================================================================================================;