# Critérios de Correcção (usando `lizard`, `valgrind` e `fsanitize`)

Ver os [guidelines](guidelines.md) que demos aos alunos para escrita de código.

Outra fonte [aqui](https://www.cs.swarthmore.edu/~newhall/unixhelp/c_codestyle.html).

O objectivo é sistematização em grupos com cotação de -3 a +4 (sendo -3 para casos __muito__ particulares).

## Comentários: (descritivos: que digam algo de útil): 0 a 1

- Deve ter 1 comentário por função (.c ou .h, não necessariamente nos dois)
- detalhe em algum especial (???)
- Ter `0` qd houver excesso de comentários ou quando não há comentários.

## Indentação: 0 a 0.5

- espaçamento/identação consistente
- chaveta de fechar alinhada com o cabeçalho do bloco
- consistente (abrir chavetas sempre no fim ou sempre por baixo)
- linhas até 80 chars

## Estruturação: modularidade: abstracção: 0 a 1.5

- nomes explicativos (excepto contadores: `i, j,...`)
- 1 rotina por comando
- uso de constantes (`#define`)
- nomes de variáveis e rotinas em PT/EN mas consistentemente na mesma língua
- Estruturação de código em ADTs com interface bem definida e com encapsulamento
- Quem coloca código no .h deve ter 0 nesta parte da avaliação.
- Quem junta todos os .h num único ficheiro fica no máximo com 1.0

## Compilação Separada: 0 a 1

- deve ter pelo menos 3 ficheiros .c de entre os naturais (paragens, carreiras e main) para ter cotação máxima.
- Apenas dar 0 se está tudo no mesmo ficheiro. Caso contrário, dar cotação intermédia.

## Descontos 0 a -3 valores

- (-1 a 0) `lizard`
  - funções com mais de uma página
  - não descontar nada se tiverem até duas funções no intervalo [30, 50] linhas de código
- (-0.5) óbvia repetição de código (**não** vamos penalizar quem reimplementou listas várias vezes)
  - aplicar apenas em casos muito flagrantes
- (-1.5 a 0) `valgrind`
  - (0) quem tem o valgrind sem erros. reserva de memória sem fugas e usa mallocs
  - (descontar -0.5) para quem tem memória por libertar ou seg. fault devido a problemas de memória
  - (descontar -0.5) para quem tem invalid read/write
  - (-1.5) para quem não aloca memória dinamicamente ou não liberta nenhuma memória alocada de forma dinâmica. Devem ser casos muito raros nesta categoria.
