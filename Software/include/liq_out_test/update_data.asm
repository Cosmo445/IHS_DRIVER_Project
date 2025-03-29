;=================================================================================================================;

;; Armazena dados estáticos inicializados (variáveis globais e constantes pré-definidas) ;;
SECTION .data
;; database[7:0] lcd_data, database[9] en, database[11] rw, database[13] rs, database[15] on ;;
database_set EQU 0b00000000000000001010000000000000
database_reset EQU 0b00000000000000001000000000000000

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
    ;; Coloca em eax o valor 1 ;;
    mov eax, 1
    ;; Coloca em ebx o valor de rs escolhido ;;
    mov ecx, [ebp+12]
    ;; Compara o valor de ebx com o de eax, mas não altera o valor de nenhum dos operandos ;;
    ;; Flag ZF é alterada para 1 caso eles sejam iguais, senão fica em 0 ;;
    cmp eax, ecx
    ;; Se forem diferentes pula para reset, ou seja, rs = 0 ;;
    jz reset
;----------------------------------------------------------------------------------------------------------------;
    ;; Caso sejam iguais, ou seja, rs = 1 ;;
    ;; Move o valor em database_set para eax (é um EQU, então a notação está correta) ;;
    mov eax, database_set
    ;; pula para o próximo passo ;;
    jmp next
;----------------------------------------------------------------------------------------------------------------;
reset:

    ;; Move o valor em database_reset para eax ;;
    mov eax, database_reset
;----------------------------------------------------------------------------------------------------------------;
next:

    ;; Move o valor de symbol, empilhado na troca de rotina, para bl ;;
    mov ecx, [ebp+8]
    ;; Combina os bits inferiores de eax (al) com bl ;;
    or al, cl
    ;; inverter as ações do enter: move ebp para esp e restaura o valor antigo de ebp ;;
    leave
    ;; retorna para a função no código em C ;;
    ret
;----------------------------------------------------------------------------------------------------------------;

;=================================================================================================================;