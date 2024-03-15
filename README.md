# iaed24 - project 


Este repositório git pertence a ist1110306 e destina-se ao project de iaed24.


O enunciado do projecto está disponível em [enunciado.md](enunciado.md).


Os alunos devem submeter aqui a sua solução para o project que será avaliada automaticamente.


O resultado da avaliação do projecto ficará disponível no [README](https://gitlab.rnl.tecnico.ulisboa.pt/iaed24/feedback/project/ist1110306/-/tree/master/README.md) do repositório de feedback após cada submissão de código.


## Data de entrega: 01 de abril 2024, às 19h59


O desempenho global dos alunos no project pode ser consultado no [_dashboard_](https://gitlab.rnl.tecnico.ulisboa.pt/iaed24/iaed24/-/tree/master/dashboard/projects/project.md) do projecto, presente no repositório global de iaed24.


Informações detalhadas sobre depuração de programas estão disponíveis em [debugging.md](debugging.md).


Outras guidelines podem ser encontradas em [guidelines.md](guidelines.md) e mais informações detalhadas sobre os critérios de correcção podem ser encontradas em [criterios.md](criterios.md).



- **Notas importantes:**


  - [+Os alunos têm de esperar 10 minuto(s) entre submissões+]. Desta forma, têm de esperar 10 minuto(s) para resubmeter um novo programa. Caso contrário a submissão do aluno não será avaliada.


  - [-Os alunos não podem alterar o ficheiro .gitlab-ci.yml presente no repositorio. A alteração deste ficheiro fará com que o aluno fique sem acesso a este repositório, não existirão excepções. Desta forma o aluno será avaliado com 0 valores nesta componente de avaliação-]



- Processo de compilação usando o gcc (versão 12.3.0) :


```
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj1 *.c
```

- Após compilar o seu projeto, para correr os testes públicos, deve descompactar e entrar na directoria dos testes:


```
unzip public-tests.zip
```




```
cd public-tests
```


- Para correr todos os testes públicos através da Makefile disponibilizada, deve correr o seguinte comando dentro da directoria public-tests:


```
make
```


- Resultados de avaliação mais comuns para cada teste de avaliação:


  - _Accepted_ : O resultado do programa é igual ao esperado.


  - _Wrong Answer_ : O resultado do programa é diferente do esperado.


  - _Presentation Error_ : O resultado do programa difere do esperado em espaços em branco ou linhas em branco.


  - _Compile Time Error_ : Ocorreu um erro de compilação durante a compilação do programa.


  - _Time Limit Exceeded_ : O tempo de execução do programa programa excedeu o tempo permitido.


  - _Memory Limit Exceeded_ : A memória de execução do programa excedeu a memória permitida.


  - _Output Limit Exceeded_ : O output de execução do programa excedeu o espaço permitido.


  - Outros : Ocorreu um erro durante a execução do programa que levou à paragem inesperada do mesmo.


