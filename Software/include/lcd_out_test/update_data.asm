;=================================================================================================================;

;; Armazena dados estáticos inicializados (variáveis globais e constantes pré-definidas) ;;
SECTION .data
;; database[7:0] lcd_data, database[9] en, database[11] rw, database[13] rs, database[15] on ;;
database_set EQU 0b0000000000000000000000000000000000000000000000001010000000000000
database_reset EQU 0b0000000000000000000000000000000000000000000000001000000000000000

;=================================================================================================================;

;; Armazena código executável (as instruções em assembly que o processador executa) ;;
SECTION .text
;; Para realizar a função chamada pelo código em C ;;
global update_data

;=================================================================================================================;

update_data:

;----------------------------------------------------------------------------------------------------------------;
    ;; Começa a rotina: prepara novo stack frame, empilha ebp, move o valor de esp para ebp e aloca bytes ;;
    enter 0,0
    ;; Coloca em rax o valor 1 ;;
    mov rax, 1
    ;; Coloca em rbx o valor de rs escolhido ;;
    mov rcx, rsi
    ;; Compara o valor de ebx com o de rax, mas não altera o valor de nenhum dos operandos ;;
    ;; Flag ZF é alterada para 1 caso eles sejam iguais, senão fica em 0 ;;
    cmp rax, rcx
    ;; Se forem diferentes pula para reset, ou seja, rs = 0 ;;
    jz set
;----------------------------------------------------------------------------------------------------------------;
    ;; ;; Caso sejam diferentes, ou seja, rs = 0 ;;
    ;; Move o valor em database_reset para rax ;;
    mov rax, database_reset
    ;; pula para o próximo passo ;;
    jmp next
;----------------------------------------------------------------------------------------------------------------;
set:
    
    ;; Caso sejam iguais, ou seja, rs = 1 ;;
    ;; Move o valor em database_set para rax (é um EQU, então a notação está correta) ;;
    mov rax, database_set  
;----------------------------------------------------------------------------------------------------------------;
next:

    ;; Move o valor de symbol, empilhado na troca de rotina, para bl ;;
    mov rcx, rdi
    ;; Combina os bits inferiores de rax (al) com cl ;;
    or al, cl
    ;; inverter as ações do enter: move ebp para esp e restaura o valor antigo de ebp ;;
    leave
    ;; retorna para a função no código em C ;;
    ret
;----------------------------------------------------------------------------------------------------------------;

;=================================================================================================================;

; Define a seção .note.GNU-stack como não executável
SECTION .note.GNU-stack noalloc noexec nowrite progbits

;=================================================================================================================;