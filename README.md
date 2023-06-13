# RESITECH

## Grupo: João Pedro Batista da Silva, Arthur wolmer, Diego Peter, Guilherme Silveira, Nicolas Miranda, Pedro Leite, Eduarda Sanguineto, Lucca Tardieux, François Arruda.

## Google Sites:
[Link para o Google Sites](https://sites.google.com/cesar.school/resitech/cliente)


### Linguagem e Ferramentas
![C](https://img.shields.io/badge/C-000000?style=for-the-badge&logo=code&logoColor=white)
![VSCode](https://img.shields.io/badge/VSCode-007ACC?style=for-the-badge&logo=visual-studio-code&logoColor=white)
![Figma](https://img.shields.io/badge/Figma-F24E1E?style=for-the-badge&logo=figma&logoColor=white)
![CLion](https://img.shields.io/badge/CLion-000000?style=for-the-badge&logo=clion&logoColor=white)
![Sqlite](https://img.shields.io/badge/SQLite-07405E?style=for-the-badge&logo=sqlite&logoColor=white)   

## Descrição
Um aplicativo de registro de frequencia, atribuição de notas e feedbacks para auxiliar o Hospital das Clínicas em seu processo de residência.


## Protótipo Físico com Arduino


```
Executar db_setup para inicializar o banco de dados de frequência, 
posteriormente executar db_entries para coletar frequências de usuários, em integração com o arduino com sensor MFRC522.
```

Para compilar os dois arquivos .c são necessárias as bibliotecas sqlite3 (para interação com o banco de dados) e libserialport (para leitura dos dados no serial).

No diretório em que estejam contidos (P2G4/attendance, por padrão), executar as seguintes linhas, para compilar corretamente:

```
gcc db_setup.c -o db_setup -lsqlite3
gcc db_entries.c read_port.c -o db_entries -lsqlite3 -lserialport
```

## Prova de conceito da leitura de escala

```
Executar o arquivo parsing para simular a leitura de escalas mensais de diferentes usuários, contidas em arquivos .csv devidamente formatados.
O programa é capaz de identificar de forma procedural as horas de serviço previstas e os turnos em comum entre dois usuários, em termos de matrizes em que
cada linha, após o cabeçalho, representa um intervalo de uma hora e cada coluna um dos dias do mês.

Houve uma tentativa de integrar as matrizes ao banco de dados, inclusive com a implementação de uma função serializadora, mas não houve tempo para concluir essa etapa.
```

No diretório em que esteja contido (P2G4/rotation_parsing, por padrão), executar a seguinte linha, para compilar corretamente:

```
gcc parsing.c -o parsing
```

## Fluxo principal

Executar o arquivo main, para navegar pelo fluxo plenamente integrado de autenticação, criação de usuários, elaboração e leitura de entradas de feedback.

### Instalação

Primeiro clone o repositório para sua máquina local:

```
git clone https://github.com/diegopluna/P2G4.git)
```

### Arquivos

## Arquivos .c:

- `main.c`: Este arquivo é responsável por iniciar o código e chamar as principais funções do aplicativo.
- `feedbacks.c`: Este arquivo é responsável por gerenciar a criação de feedbacks.
- `auth.c`: Este arquivo é responsável pela autenticação de contas.
- `sqlite3.c`: Este arquivo é responsável pelo banco de dados.
- `read_port.c`: Este arquivo é responsável pela integração com arduino.
- `db_entries.c`: Este arquivo é responsável pela interação com o banco de dados e manipulação das entradas do sistema. 
- `db_setup.c`: Este arquivo é responsável por configurar o banco de dados SQLite e criar as tabelas necessárias.
## Arquivos .h:
- `feedbacks.h`
- `auth.h`
- `read_port.h`
- `sqlite3.h`

