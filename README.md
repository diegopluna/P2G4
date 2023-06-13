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
Rodar de forma invidudal o db_setup para inicializa or banco de dados de frequência, 
e depois isso é só rodar o db_entries para coletar frequências de usuários.
```

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

