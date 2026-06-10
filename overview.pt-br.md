# Minishell — Uma Visão Geral Completa e Humana

> Um pequeno shell Unix escrito em C para o currículo da 42.
> Este documento foi feito para ser **lido de cima a baixo** e deixar você
> entendendo o projeto inteiro: cada arquivo, cada responsabilidade, os cantos
> obscuros (sinais, redirecionamentos, heredocs, pipelines) e, por fim, o fluxo
> completo desde uma tecla pressionada no prompt até um processo terminando.
> Autores do shell: *fcardozo* & *g-alves-*.

---

## Índice

1. [O panorama geral](#1-o-panorama-geral)
2. [Estrutura de diretórios](#2-estrutura-de-diretórios)
3. [Os dados que fluem pelo shell](#3-os-dados-que-fluem-pelo-shell)
4. [`shared/` — a caixa de ferramentas comum](#4-shared--a-caixa-de-ferramentas-comum)
5. [`core/` — o coração que pulsa (REPL)](#5-core--o-coração-que-pulsa-repl)
6. [`env/` — o ambiente como lista encadeada](#6-env--o-ambiente-como-lista-encadeada)
7. [`lexer/` — transformando texto em tokens](#7-lexer--transformando-texto-em-tokens)
8. [`parser/` — transformando tokens em uma árvore de comandos](#8-parser--transformando-tokens-em-uma-árvore-de-comandos)
9. [`runner/` — fazendo as coisas realmente acontecerem](#9-runner--fazendo-as-coisas-realmente-acontecerem)
   - 9.1 [O expansor](#91-o-expansor--divisão-de-palavras-e-variáveis)
   - 9.2 [Redirecionamentos](#92-redirecionamentos--a-arte-cuidadosa-de-mover-fds)
   - 9.3 [Heredocs](#93-heredocs--o-redirecionamento-mais-sutil)
   - 9.4 [Comandos externos & PATH](#94-comandos-externos--resolução-de-path)
   - 9.5 [Pipelines](#95-pipelines--muitos-processos-um-fluxo-de-dados)
10. [`bin/` — os builtins](#10-bin--os-builtins)
11. [Sinais — o aprofundamento](#11-sinais--o-aprofundamento)
12. [O fluxo completo, de ponta a ponta](#12-o-fluxo-completo-de-ponta-a-ponta)

---

## 1. O panorama geral

Um shell é um **Read–Eval–Print Loop** (REPL). Ele faz quatro coisas para
sempre:

```
   lê uma linha  ──▶  entende ela  ──▶  executa ela  ──▶  lembra do resultado
        ▲                                                        │
        └────────────────────────────────────────────────────────┘
```

O Minishell quebra o "entender" e o "executar" em um pipeline clássico no estilo
de um compilador. Uma linha de texto passa por quatro estágios, cada um
entregando ao próximo um objeto mais rico e mais estruturado:

```
  "cat file | grep foo > out"
        │
        ▼
  ┌──────────┐  lista de tokens   ┌──────────┐  árvore de comandos  ┌──────────┐
  │  LEXER   │ ─────────────────▶ │  PARSER  │ ───────────────────▶ │  RUNNER  │
  └──────────┘                    └──────────┘                      └──────────┘
   divide o texto                  agrupa tokens em                  expande vars,
   em palavras e                   comandos, redirs                  abre fds,
   operadores                      e pipes                           fork, exec
```

- **Lexer** responde: *"onde cada palavra/operador começa e termina?"*
- **Parser** responde: *"quais palavras pertencem a qual comando, e como eles
  estão conectados?"*
- **Runner** responde: *"o que eu realmente faço para isso acontecer no SO?"*

O módulo `core/` é o laço que conduz tudo isso, e `shared/`, `env/` e `bin/` são
o elenco de apoio usado por todos.

---

## 2. Estrutura de diretórios

```
minishell/
├── Makefile               Regras de build. Compila a libft, depois o shell.
├── README.md              Descrição do projeto & lista de recursos.
├── headers/               Headers PÚBLICOS — o contrato entre os módulos.
│   ├── minishell.h          Header guarda-chuva: inclui tudo + libs do sistema.
│   ├── core.h               t_shell, modos de sinal, códigos de status do shell.
│   ├── env.h                Lista encadeada t_env + sua API.
│   ├── lexer.h              t_list_token, tipos de token, entrada lexer().
│   ├── parser.h             Árvore t_command, t_ast, erros de sintaxe, tipos redir.
│   ├── runner.h             Entrada rn_execute() + constantes de código de saída.
│   ├── bin.h                Os 7 protótipos de builtins.
│   └── shared.h             Helpers transversais (erros, aspas, sinais).
│
├── libraries/libft/       A libc própria do estudante (strings, listas, arrays...).
│
└── source/
    ├── main.c             Ponto de entrada. Chama core_run(envp).
    │
    ├── core/              ── O REPL ──
    │   ├── core.c           init / run / destroy do shell.
    │   ├── reader.c         o laço: readline → parse → execute.
    │   └── _core.h          header privado do core.
    │
    ├── shared/            ── CAIXA DE FERRAMENTAS COMUM (prefixo sh_) ──
    │   ├── signal.c         os handlers globais de sinal + troca de modo.
    │   ├── error.c          impressão uniforme "minishell: scope: msg".
    │   ├── quote.c          detecção de aspas + remoção de aspas.
    │   ├── string.c         pequenos predicados de char (isspace, varchar...).
    │   └── free.c           libera um array de ponteiros terminado em NULL.
    │
    ├── env/               ── AMBIENTE (prefixo env_) ──
    │   ├── init.c           constrói a lista t_env a partir do envp (+ SHLVL).
    │   ├── list.c           new/free de nó.
    │   ├── get.c set.c unset.c   busca / upsert / remoção por chave.
    │   ├── size.c           conta os nós.
    │   ├── toarr.c          achata a lista de volta para char** para o execve.
    │   ├── free.c           libera a lista inteira.
    │   └── _env.h           header privado.
    │
    ├── lexer/             ── TOKENIZADOR ──
    │   ├── lexer.c          orquestra a varredura da string de entrada.
    │   ├── state.c          o laço principal: palavra? operador? pular?
    │   ├── extract.c        extrai uma palavra / um operador.
    │   ├── rules.c          quais classes de char iniciam palavras/operadores.
    │   ├── token.c          cria / libera um token, anexa à lista.
    │   ├── _lexer.h         header privado (t_rules).
    │   └── scanner/         um cursor sobre a string crua:
    │       ├── load.c         inicializa o cursor.
    │       ├── move.c         avança / marca / fim.
    │       ├── get.c          peek / char atual / extrai substring.
    │       ├── table.c        constrói as tabelas de classe de char (256 entradas).
    │       └── _scanner.h     bits P_* (classes de char) e L_* (classes de token).
    │
    ├── parser/            ── GRAMÁTICA ──
    │   ├── parser.c         topo: constrói o pipeline (árvore assoc. à esquerda).
    │   ├── command.c        analisa um comando simples (palavras + redirs).
    │   ├── state.c          inicia/finaliza um comando, anexa arg/redir.
    │   ├── utils.c          cursor de token + classificação token→classe.
    │   ├── errors.c         registra um erro de sintaxe no parser/ast.
    │   ├── cleanup.c        libera a árvore de comandos / ast.
    │   └── _parser.h        header privado (t_parser, t_token_class).
    │
    ├── runner/            ── EXECUÇÃO (prefixo rn_) ──
    │   ├── control.c        o despachante: cmd vs pipe; a dança redir+run.
    │   ├── builtin.c        args[0] é um builtin? se sim, chama-o.
    │   ├── status.c         armazena/lê $? no ambiente.
    │   ├── _runner.h        header privado + constantes de código de saída.
    │   ├── expander/        expansão de $VAR, remoção de aspas, divisão de palavras.
    │   ├── redir/           <, >, >>, << — abrindo fds e trocando stdio.
    │   ├── external/        resolve + fork + execve de um programa externo.
    │   ├── path/            busca no $PATH por um executável.
    │   └── pipe/            constrói e roda um pipeline de N processos.
    │
    └── bin/               ── BUILTINS ──
        ├── echo.c cd.c pwd.c exit.c env.c unset.c
        ├── export/          export.c keys.c print.c (saída ordenada, estilo bash).
        └── _bin.h
```

Uma convenção de nomes percorre o projeto inteiro, e ela ajuda muito:

| Prefixo    | Módulo    | Significado                                  |
|------------|-----------|----------------------------------------------|
| `sh_`      | shared    | helper compartilhado, usável em qualquer lugar |
| `env_`     | env       | operação sobre a lista de ambiente           |
| `rn_`      | runner    | algo acontecendo em tempo de execução        |
| `exp_`     | expander  | uma parte do expansor de variáveis/palavras  |
| `bin_`     | bin       | um comando builtin                           |
| `core_`    | core      | a maquinaria do REPL                         |
| `scanner_` | lexer     | o cursor sobre a string crua                 |
| `parser_`  | parser    | máquina de estados do parser                 |

---

## 3. Os dados que fluem pelo shell

Antes de ler os módulos, guarde estas quatro formas na cabeça. Tudo é uma
transformação entre elas.

**1. Entrada crua** — apenas um `char *` vindo do readline.

**2. Lista de tokens** (`t_list_token` numa lista duplamente encadeada). Cada
token:
```c
typedef struct s_list_token {
    char         *value;   // o texto literal, ex.: "cat" ou ">"
    t_token_type  type;    // TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN, ...
    t_bool        expand;  // contém um '$'? (uma dica para depois)
    t_bool        quoted;  // contém um caractere de aspas?
}   t_list_token;
```

**3. Árvore de comandos** (`t_command`). É uma pequena **árvore binária**. Um nó
é ou um *comando simples* (uma folha) ou um *pipe* (um nó interno):
```c
typedef struct s_command {
    t_pnode_type type;              // PNODE_CMD ou PNODE_PIPE
    union {
        t_simple simple;            // folha: args[] + redirs[]
        t_pipe   pipe;              // nó: filhos esquerdo + direito
    } t_define;
}   t_command;
```
`a | b | c` vira (associativo à esquerda):
```
            PIPE
           /    \
         PIPE    c
        /    \
       a      b
```
Um `t_simple` guarda dois arrays dinâmicos: `args` (as palavras) e `redirs` (os
redirecionamentos ligados àquele comando).

**4. O resultado completo da análise** (`t_ast`) envolve a `root` (raiz) da
árvore, mais uma flag `error` e um `error_type`, para que quem chamou saiba *por
que* a análise falhou.

---

## 4. `shared/` — a caixa de ferramentas comum

Helpers pequenos e com poucas dependências, usados por toda parte. Vale conhecer
porque seus nomes aparecem o tempo todo.

**`string.c`** — predicados de caractere:
- `sh_isspace(c)` — espaço, tab ou newline.
- `sh_varstart(c)` — uma variável pode *começar* com este char? (`[A-Za-z_]`)
- `sh_varchar(c)` — uma variável pode *conter* este char? (`[A-Za-z0-9_]`)
- `sh_isidentifier(s)` — a string inteira é um identificador de shell válido?
  (usado por `export`/`unset` para rejeitar coisas como `1foo` ou `a-b`).

**`quote.c`** — lógica de aspas:
- `sh_has_quotes(s)` — a string contém `'` ou `"`?
- `sh_quote_remove(s)` — percorre a string com uma pequena máquina de estados
  (`quote_should_copy`) que alterna uma flag "dentro de qual aspa" e copia apenas
  os caracteres que *não* são os delimitadores de aspas ativos. Transforma
  `"a'b'"` em `ab`. Usado para delimitadores de heredoc (onde `$` não é expandido
  mas as aspas ainda somem).

**`error.c`** — relato de erros uniforme no stderr:
- `sh_err(scope, msg)`   → `minishell: scope: msg`
- `sh_err2(scope, arg, msg)` → `minishell: scope: arg: msg`
- `sh_stxerr(type)`      → imprime a linha "syntax error: ..." correta.

**`free.c`** — `sh_freesec` / `sh_freeargs` liberam um `char **` terminado em
`NULL` (cada elemento, depois o array). Os arrays `args` e `envp` são liberados
com isso em toda parte.

**`signal.c`** — coberto em profundidade em [§11](#11-sinais--o-aprofundamento).
Ele é dono da única global, `g_signal`, e da função `sh_sig_mode()` que
reconfigura os handlers para as três situações em que o shell pode estar.

---

## 5. `core/` — o coração que pulsa (REPL)

`core/` é o laço. Ele é intencionalmente fino: lê uma linha, entrega ao pipeline,
repete.

### `t_shell` (em `core.h`)
```c
typedef struct s_shell {
    t_env  *env;       // o ambiente, possuído aqui durante toda a sessão
    t_bool  running;   // condição do laço
}   t_shell;
```

### `core.c` — ciclo de vida
- **`core_init`** — zera `g_signal`, constrói o ambiente a partir do `envp`
  (`env_init`), semeia `$?` com `"0"` (armazenado sob a chave `"$?"`), define
  `running = TRUE` e coloca os sinais em modo **interativo**. Se algo falhar, ele
  desmonta tudo de forma limpa.
- **`core_destroy`** — libera o ambiente e limpa o histórico do readline.
- **`core_run`** — a entrada pública vinda do `main`: init → loop → destroy,
  retornando o status de saída final.

### `reader.c` — o laço em si
Este é o arquivo mais importante do `core/`. Leia como um funil:

- **`core_loop`** — salva as configurações do terminal com `tcgetattr`, depois
  repete `core_iteration` enquanto `running`. Após cada comando, restaura o
  terminal com `tcsetattr` (defensivo: um filho mal-comportado poderia deixar o
  terminal num estado estranho). Retorna o último `$?` quando o laço termina.

- **`core_iteration`** — um turno do REPL:
  1. `readline(PROMPT)` imprime o prompt e bloqueia esperando uma linha.
  2. Se retorna `NULL`, isso é **EOF** (Ctrl-D) → `core_eof`.
  3. Se um `SIGINT` chegou enquanto esperava, define `$?` como `130`.
  4. Reseta `g_signal` para 0.
  5. Se a linha não está vazia: `add_history` e depois `core_process_line`.
  6. `free(line)`.
  7. Se um builtin pediu saída (`g_signal == SH_EXIT_REQUESTED`), retorna `-1`
     para parar o laço.

- **`core_eof`** — tratamento do Ctrl-D. Se chegamos aqui *por causa* de um
  SIGINT, apenas reseta e continua; senão imprime `exit\n` e sinaliza o laço para
  parar. Isso espelha o bash imprimindo `exit` quando você aperta Ctrl-D num
  prompt vazio.

- **`core_process_line`** → **`core_parse`** — a ponte para dentro do pipeline:
  ```c
  manager = lexer(line);                 // texto  → lista de tokens
  ast     = parser_controller(manager);  // tokens → árvore de comandos
  lexer_free(manager);                   // tokens não são mais necessários
  ```
  Depois:
  - falha do lexer (ex.: aspa não fechada) → define `$?` = 2, pula.
  - erro de sintaxe do parser → imprime o erro específico, define `$?` = 2,
    libera, pula.
  - senão → `rn_execute(ast->root, &env, STDIN_FILENO, ast)` e libera a ast.

  Note que `STDIN_FILENO` é passado como o `heredoc_fd`: no nível mais alto, um
  heredoc lê do terminal real. (Dentro de um pipeline este argumento vira uma
  *cópia salva* do terminal — veja
  [§9.5](#95-pipelines--muitos-processos-um-fluxo-de-dados).)

---

## 6. `env/` — o ambiente como lista encadeada

O ambiente é uma lista simplesmente encadeada de pares chave/valor (`t_env`). Uma
lista (e não um array) torna inserção/remoção triviais, do que `export`/`unset`
precisam.

- **`init.c`**
  - `env_init(envp)` — transforma o `char **envp` na lista. `entryparse` divide
    cada `"KEY=VALUE"` no primeiro `=`. Uma entrada sem `=` vira uma chave com
    valor `NULL` (uma variável *exportada mas não definida*, exatamente como no
    bash).
  - `shlvl()` — encontra `SHLVL` e o incrementa (ou define como 1), para que
    shells aninhados contem para cima como o de verdade.
  - `minenv()` — se o processo foi iniciado *sem* ambiente nenhum, constrói um
    mínimo com `PWD` e `SHLVL=1` para que o shell ainda funcione.
- **`list.c`** — `env_node_new` (dup da chave, dup do valor se presente) e
  `env_node_free`.
- **`get.c`** — `env_get` retorna um *ponteiro para dentro* do valor do nó (não
  uma cópia); quem chama não deve liberá-lo.
- **`set.c`** — `env_set` é um **upsert**: se a chave existe, substitui seu
  valor; senão, anexa um novo nó. Um valor `NULL` significa "manter a chave, sem
  valor".
- **`unset.c`** — `env_unset` desliga e libera o nó correspondente.
- **`size.c`** — conta os nós.
- **`toarr.c`** — `env_toarr` reconstrói um `char **` (`"KEY=VALUE"`) para o
  `execve`. Crucialmente, ele **pula** a pseudo-variável interna `$?` e qualquer
  chave com valor `NULL` (variáveis exportadas-mas-não-definidas não são passadas
  aos filhos — o bash concorda).

**Um truque elegante:** o shell armazena o último status de saída `$?` *dentro do
ambiente* sob a chave `"$?"` (a constante `ENV_ERRCODE`). É por isso que
`rn_status_set`/`rn_status_get` (no runner) são apenas finos invólucros sobre
`env_set`/`env_get`. Como `"$?"` não é um identificador válido, o usuário nunca
pode colidir com ele, e `env`/`export`/`execve` todos o filtram para fora.

---

## 7. `lexer/` — transformando texto em tokens

**Objetivo:** ler a linha crua e produzir uma lista plana de tokens (palavras e
operadores), respeitando aspas e rejeitando aspas não fechadas.

### O scanner — um cursor com uma tabela de classes de caractere

O lexer nunca inspeciona caracteres com `if (c == ...)` espalhados por aí. Em vez
disso, ele pré-computa, uma única vez, uma **tabela de 256 entradas** que
classifica cada byte. Essa é a ideia mais limpa do lexer, então vamos
destrinchá-la.

`scanner/_scanner.h` define dois conjuntos de flags de bit empacotados num único
`unsigned int` por caractere:

```
 P_* (natureza do char, bits baixos)    L_* (papel de token, bits altos, OFFSET=8)
 ──────────────────────────             ──────────────────────────────────
 P_ALPHA   letra                        L_WORD     inicia/continua uma palavra
 P_DIGIT   dígito                        L_PIPE     '|'
 P_SPACE   espaço em branco              L_REDIR_IN '<'
 P_SQUOTE  '                            L_REDIR_OUT'>'
 P_DQUOTE  "                            L_DOLLAR   '$'
 P_ESCAPE  contrabarra                   L_NONE     qualquer outra coisa
 P_SYMBOL  qualquer outro símbolo
```

`scanner_table_char` preenche os bits `P_*`; `scanner_table_token` preenche os
bits `L_*`. Como as duas famílias vivem em faixas de bits diferentes, um único
valor `props[c]` carrega tanto "que tipo de caractere é este" quanto "qual papel
de token ele desempenha". A classificação é então uma única consulta de array —
rápida e sem ramificações.

O cursor (`t_scanner`) é só `{ input, cursor, len, start }`:
- `scanner_current` — o byte no cursor (como `unsigned char`, 0 no fim).
- `scanner_advance` — avança um.
- `scanner_peek(offset)` — olha adiante sem se mover.
- `scanner_mark_start` — lembra onde o token atual começou.
- `scanner_extract` — `ft_substr` de `start` até `cursor` (o texto do token).
- `scanner_is_end` — cursor passou do fim?

### `rules.c` — o que inicia uma palavra vs um operador

`t_rules` agrupa a tabela mais duas máscaras:
- `start_word` = `L_WORD | P_DQUOTE | P_SQUOTE | P_SYMBOL`
  → letras, dígitos, aspas e símbolos avulsos todos iniciam uma *palavra*.
- `start_operator` = `L_PIPE | L_REDIR_IN | L_REDIR_OUT`
  → `|`, `<`, `>` iniciam um *operador*.

### `state.c` — o laço de despacho

`state_machine` percorre a entrada. Para cada posição ele lê `props` e decide:

```
 props & start_operator ? ──▶ extract_operator
 props & start_word     ? ──▶ extract_word   (pode falhar → aspa não fechada)
 caso contrário           ──▶ pula (espaço em branco, etc.)
```

Ele retorna 0 apenas em caso de aspa não fechada, que quem chama transforma num
erro de sintaxe.

### `extract.c` — extraindo um token

- **`extract_word`** consome caracteres até bater, *enquanto não estiver dentro
  de uma aspa*, em espaço em branco ou num char de operador. A sutileza é a
  máquina de estados de aspas `quote_update`: entrar em `'` ou `"` define o
  estado; o fechamento correspondente o limpa. Enquanto dentro de aspas, espaços
  e `|`/`<`/`>` são caracteres comuns e **não** terminam a palavra. Então
  `"a | b"` é **uma** palavra, mas `a | b` são três tokens. Se a palavra termina
  ainda dentro de uma aspa (`state != P_NONE`), ela retorna `NULL` → erro de
  **aspa não fechada**. Note que a palavra mantém suas aspas por enquanto; elas
  são removidas depois pelo expansor.

- **`extract_operator`** consome até `OPERATOR_MAX_LEN` (2) chars de operador, de
  forma que `>>`, `<<` sejam lidos como um único token, enquanto `>`, `<`, `|`
  são tokens de um caractere. `operator_type` então mapeia o texto literal para o
  `TOKEN_*` preciso: `>>`→`TOKEN_APPEND`, `<<`→`TOKEN_HEREDOC`, `|`→`TOKEN_PIPE`,
  etc.

### `token.c` — construindo a lista

`token_add` aloca um `t_list_token`, duplica o valor, registra o `type`, define
as duas dicas (`expand` = "contém `$`", `quoted` = "contém uma aspa"), o envolve
num nó de lista duplamente encadeada e o anexa ao manager
(`ft_dlist_add_tail`).

### `lexer.c` — orquestração & posse

`lexer()` amarra tudo: inicializa o scanner, aloca a lista (`t_manager`) e as
regras, constrói as tabelas, roda a máquina de estados. Em caso de aspa não
fechada, imprime o erro e libera tudo (`lexer_cleanup`). Em caso de sucesso,
libera apenas as regras e retorna a lista de tokens. `lexer_free` percorre a
lista liberando cada token e nó.

> **Nota sobre `t_manager` / `t_node`:** estes são typedefs (em `lexer.h`) para a
> lista duplamente encadeada da libft `t_dlist` / `t_dlist_node`. A lista de
> tokens *é* uma lista encadeada genérica cujo `content` é um `t_list_token *`.

---

## 8. `parser/` — transformando tokens em uma árvore de comandos

**Objetivo:** consumir a lista plana de tokens e produzir uma `t_ast` cuja `root`
é a árvore de comandos — ou um erro de sintaxe explicando o que está errado.

### O cursor do parser (`utils.c`)

`t_parser` guarda a posição na lista de tokens mais o comando sendo construído:
```c
typedef struct s_parser {
    t_node       *current;     // nó atual da lista
    t_list_token *token;       // seu token (ou NULL no fim)
    t_token_type  last_type;   // tipo do token anterior
    unsigned int  state;       // não-zero == "batemos num erro, abortar"
    t_simple     *current_cmd; // o comando sendo montado no momento
    t_ast        *ast;         // onde registrar error_type
}   t_parser;
```
- `parser_init` aponta o cursor para a cabeça da lista.
- `parser_next` avança, lembrando `last_type`.
- `parser_is_end` — acabaram os tokens?
- `parser_get_class` colapsa os sete tipos de token em três **classes**:
  `C_WORD`, `C_REDIR` (qualquer um de `<`, `>`, `>>`, `<<`) ou `C_PIPE`. O parser
  raciocina em classes, o que mantém o código da gramática curto.

### Gramática, de cima para baixo (`parser.c`)

A gramática é minúscula:
```
pipeline       := comando_simples ( '|' comando_simples )*
comando_simples := ( WORD | redirecionamento )+
redirecionamento := ( '<' | '>' | '>>' | '<<' ) WORD
```

- **`parser_controller`** — aloca a `t_ast`. Se o primeiríssimo token é um `|`,
  isso é `SNTX_PIPE_START`. Senão, chama `build_pipeline` e marca `ast->error`
  se ele voltar `NULL`.
- **`build_pipeline`** — analisa um comando simples como `left`. Enquanto o
  próximo token for um `|`: pula sobre ele, garante que algo se segue (senão
  `SNTX_PIPE_END`), analisa o comando da direita e os funde num nó `PNODE_PIPE`
  via `build_pipe_node`. Isso constrói a árvore **associativa à esquerda**
  mostrada no §3.

### Analisando um comando (`command.c`)

- **`parse_simple_command`** — `parser_start_command` aloca um `t_simple` novo
  (dois arrays dinâmicos vazios). Depois faz o laço até um `|` ou o fim, tratando
  cada token com `parse_simple_token`. Por fim, `parser_finish_command` e envolve
  o `t_simple` num `t_command` (`simple_to_command`).
- **`parse_simple_token`** despacha por classe:
  - `C_WORD` → `consume_word`: duplica a palavra para o array `args` do comando.
  - `C_REDIR` → `consume_redir`.
  - qualquer outra coisa aqui → `SNTX_EMPTY_CMD`.
- **`consume_redir`** é onde a *sintaxe* do redirecionamento é checada. Ele
  computa o tipo de redir por aritmética sobre o enum:
  `type = token->type - TOKEN_REDIR_IN` — porque `parser.h` ordena
  `REDIR_IN, REDIR_OUT, REDIR_APPEND, REDIR_HEREDOC` para alinhar com o enum de
  token `TOKEN_REDIR_IN, TOKEN_REDIR_OUT, TOKEN_APPEND, TOKEN_HEREDOC`. Ele então
  avança e exige que o **próximo** token seja um `C_WORD` (o alvo); senão,
  `SNTX_REDIR_NO_TARGET`. A palavra-alvo é armazenada com o redir.

### Montando o estado (`state.c`)

- **`parser_start_command`** aloca preguiçosamente o `t_simple` e inicializa os
  arrays dinâmicos `args` e `redirs`.
- **`parser_finish_command`** fecha um comando. Se um erro foi sinalizado, libera
  o comando meio-construído. Se o comando acabou **completamente vazio** (sem
  args e sem redirs — ex.: a entrada era só espaços), isso é `SNTX_EMPTY_CMD`.
- **`parser_add_arg`** anexa um `char *` a `args` e mantém o array terminado em
  `NULL` (para que possa ser entregue direto ao `execve` depois).
- **`parser_add_redir`** constrói um
  `t_parser_redir { type, file, expand, quoted }` e o anexa a `redirs`. Note que
  `redir.expand = !token->quoted` — um alvo de redirecionamento é expandido *a
  menos que* tenha sido colocado entre aspas. (Delimitadores de heredoc são
  especiais; veja §9.3.)

### Erros & limpeza

- **`errors.c`** — `parser_set_syntax_error` define `state = TRUE` e registra o
  `error_type` na ast, para que a mensagem e o abort aconteçam juntos.
- **`cleanup.c`** — `parser_free_cmd` libera a árvore recursivamente (ambos os
  filhos de um pipe, ou os arrays de uma folha). `parser_free_ast` libera a raiz
  e o invólucro ast. `parser_free_simple` libera um `t_simple` avulso.

Os cinco erros de sintaxe possíveis (`t_syntax_error`): aspa não fechada, pipe no
início, pipe no fim, comando vazio, redirecionamento sem alvo.

---

## 9. `runner/` — fazendo as coisas realmente acontecerem

Este é o maior e mais interessante subsistema: ele pega uma árvore de comandos e
realiza operações reais sobre o sistema operacional — expandindo variáveis,
abrindo arquivos, duplicando descritores de arquivo, criando processos com fork e
chamando `execve`.

### O despachante (`control.c`)

`rn_execute(cmd, env, heredoc_fd, ast)` é o único ponto de entrada e ele é
**recursivo** através de pipelines:

```
 rn_execute(cmd)
   ├─ cmd é PNODE_CMD  ──▶ rn_exec_cmd   (um único comando)
   └─ cmd é PNODE_PIPE ──▶ rn_pipe       (um pipeline; vai chamar rn_execute de
                                          novo dentro de cada filho)
 ...então armazena o resultado como $?.
```

**`rn_exec_cmd`** — o ciclo de vida de *um* comando simples, e ele se lê como uma
dança cuidadosa de quatro passos:

1. **Expande** os args: `rn_expand(simple->args.items, env)`. Isso realiza
   expansão de `$VAR`, remoção de aspas e divisão de palavras (veja §9.1). O
   resultado é um `char **` novo.
2. **Aplica redirecionamentos** (`rn_exec_redirs` → `rn_redir_push`): abre cada
   alvo, salva o stdin/stdout atual e troca os fds. O stdin/stdout original é
   lembrado em `saved[2]` para que possa ser restaurado.
3. **Roda** o comando (`rn_cmd_run`): se `args[0]` é um builtin, chama-o; senão,
   roda um programa externo.
4. **Restaura** o stdin/stdout original (`rn_redir_restore`) e libera os args.

> Por que salvar & restaurar em vez de só deixar os fds mudarem? Porque um
> **builtin roda no *mesmo* processo do shell** (sem fork). Se
> `export FOO=bar > file` redirecionasse o stdout e nunca o restaurássemos, todo
> prompt seguinte escreveria em `file`. Então, para o processo pai, sempre
> desfazemos o redirecionamento depois. (Comandos externos rodam num *filho*,
> então seus redirecionamentos morrem com o filho — mas o salvar/restaurar é
> uniforme e inofensivo lá também.)

**`rn_cmd_run`** — `if rn_exec_bin(...) retorna seu status; senão rn_exec_ext(...)`.

### Status (`status.c`)

`rn_status_set` escreve `$?` (armazena `ft_itoa(status)` sob `ENV_ERRCODE`);
`rn_status_get` lê e faz `ft_atoi`. O resultado de todo comando vai parar aqui, e
o expansor o lê de volta quando você digita `$?`.

---

### 9.1 O expansor — divisão de palavras e variáveis

Arquivos: `runner/expander/`. Entrada: `rn_expand(char **args, t_env **env)`.

Este é o estágio que faz `echo $HOME` imprimir seu diretório home e transforma
`"$X"` e `$X` em coisas sutilmente diferentes. Ele percorre **cada argumento
caractere por caractere**, mantendo um **buffer de palavra** crescente
(`t_word`), e emite palavras prontas para uma lista. A saída é um `char **`
novinho em folha.

Estruturas-chave:
```c
typedef struct s_word {           // uma única palavra sendo construída
    char *buf; int len; int cap;
    int   active;                 // esta palavra já recebeu algum char?
}   t_word;

typedef struct s_exp_ctx {        // tudo de que os tratadores de char precisam
    t_array *list;   // palavras finalizadas
    t_word  *word;   // palavra atual
    char    *arg;    // o argumento de origem
    int     *i;      // cursor dentro de arg
    t_env  **env;
}   t_exp_ctx;
```

**`exp_process_char`** (em `dispatch.c`) é o coração. Para cada caractere ele
escolhe um tratador:

| Char        | Tratador          | Comportamento                                       |
|-------------|-------------------|-----------------------------------------------------|
| `'`         | `exp_single`      | copia literalmente até o `'` de fechamento. **Sem expansão.** |
| `"`         | `exp_double`      | copia até o `"` de fechamento, mas **expande** `$` dentro. |
| `$`         | `exp_var`         | expande uma variável (contexto sem aspas).          |
| espaço      | `exp_process_space` | finaliza a palavra atual, pula os espaços (divisão). |
| caso contrário | `exp_wordchar` | anexa o caractere literal.                          |

A flag `active` é o que faz palavras vazias-mas-reais funcionarem. Abrir uma aspa
define `active = 1` mesmo antes de qualquer caractere ser copiado, então `""`
produz um **argumento string vazia**, enquanto um `$UNSET` que expande para nada
não produz **palavra nenhuma**. Isso bate exatamente com o bash.

**Expansão de variáveis** (`variable.c`):
- `exp_varvalue` avança após o `$`, depois:
  - `$?` → o status de saída atual (via `exp_var_status`, lê `ENV_ERRCODE`).
  - um nome válido (`sh_varstart` depois `sh_varchar*`) → `env_get` daquela
    chave.
  - qualquer outra coisa (ex.: `$` seguido de espaço ou dígito) → retorna `NULL`,
    significando "isto não é uma variável", e quem chama emite um `$` literal.
- A diferença **com aspas vs sem aspas** é o ponto inteiro de ter dois
  chamadores:
  - `exp_var` (sem aspas) → `exp_add_unquoted`: o valor da variável é, ele
    próprio, **dividido em palavras** por espaços. Então se `X="a b"`, então
    `echo $X` imprime dois argumentos.
  - `exp_var_quoted` (dentro de `"`) → `exp_wordstr`: o valor é anexado como
    está, **sem divisão**. Então `echo "$X"` imprime um único argumento `a b`.

**O buffer de palavra** (`word.c`): `exp_wordchar`/`exp_wordstr` anexam a `buf`,
crescendo-o geometricamente (`wordgrow` dobra a capacidade). O `exp_flush_word`
de `list.c` finaliza o buffer atual numa string no heap e a empurra para a lista
(só se `active`), depois reseta o buffer para a próxima palavra.

`expand.c` conduz tudo isso por argumento (`rn_argexpand`), coleta os resultados
num `t_array` e por fim anexa um terminador `NULL` (`rn_expand_done`) para que o
resultado seja um `char **` pronto para o exec.

> **Numa frase:** o expansor substitui variáveis, remove aspas e divide os
> espaços fora de aspas — produzindo o vetor de argumentos final que o programa
> realmente vai receber.

---

### 9.2 Redirecionamentos — a arte cuidadosa de mover fds

Arquivos: `runner/redir/`. Esta é uma das duas áreas "obscuras", então vamos
construir o modelo mental primeiro.

#### O modelo mental

Todo processo tem uma tabela de **descritores de arquivo** (pequenos inteiros).
Por convenção `0` = stdin, `1` = stdout, `2` = stderr. Um programa como o `cat`
não sabe nem se importa com *o que* o fd 0 está conectado — um terminal, um
arquivo, um pipe. Ele só lê o fd 0.

Um redirecionamento como `cat < file` significa: *antes de rodar o cat, faça o
fd 0 referir-se a `file` em vez do terminal.* A ferramenta para "fazer este fd
referir-se ao que aquele fd se refere" é **`dup2(oldfd, newfd)`**: ela fecha
`newfd` e o torna uma cópia de `oldfd`. Então `dup2(file_fd, 0)` é exatamente "o
stdin agora vem do arquivo".

Os quatro redirecionamentos:

| Sintaxe | Significado            | Flags do `open`                       | fd alvo   |
|---------|------------------------|---------------------------------------|-----------|
| `<`     | lê do arquivo          | `O_RDONLY`                            | stdin (0) |
| `>`     | escreve, trunca        | `O_WRONLY \| O_CREAT \| O_TRUNC`      | stdout(1) |
| `>>`    | escreve, anexa         | `O_WRONLY \| O_CREAT \| O_APPEND`     | stdout(1) |
| `<<`    | heredoc (entrada inline) | (um pipe, veja §9.3)                | stdin (0) |

#### O fluxo, arquivo por arquivo

**`push.c` — salvar, aplicar, (depois) restaurar.**
- `redir_save_stdio(saved)` — faz `dup` do stdin e do stdout atuais para
  `saved[0]`/`saved[1]`. Estes são *backups* para podermos voltar ao terminal
  depois.
- `rn_redir_push` — salva, depois `rn_redir_apply`. Se a aplicação falhar,
  restaura imediatamente e relata a falha.
- `rn_redir_restore(saved)` — faz `dup2` dos backups de volta sobre 0 e 1, depois
  fecha os backups. Este é o "desfazer" usado após um builtin (veja o despachante
  no §9).

**`target.c` — descobrir *o que* abrir.**
- `rn_redir_target` — para um redirecionamento normal, a palavra-alvo passa pelo
  **expansor** (`rn_expand`). Se ela expandir para zero ou mais-de-uma palavra,
  isso é o erro *"ambiguous redirect"* do bash (você não pode redirecionar para
  dois arquivos). Para um **heredoc**, o delimitador *não* é expandido; ele só é
  desprovido de aspas com `sh_quote_remove`.
- `rn_redir_open` — a chamada `open(2)` com as flags corretas da tabela acima.
- `rn_redir_fd` — a cola: resolve o alvo, depois ou abre o arquivo ou roda o
  heredoc; numa falha de open normal, imprime
  `minishell: target: <mensagem do errno>`.

**`apply.c` — abrir todos, depois trocar todos.** Este arquivo é cuidadoso com um
problema real: se um comando tem vários redirecionamentos, alguns podem abrir com
sucesso antes de um falhar, e esses fds já abertos não podem vazar.
- `redir_open_all` — abre cada redirecionamento num array temporário
  `t_redir_fd[]`, cada um lembrando tanto o `fd` aberto quanto qual `stdio` (0 ou
  1) ele tem como alvo. Se algum open falhar, fecha tudo que foi aberto até ali e
  retorna (propagando `EXIT_SIGINT` se um heredoc foi interrompido com Ctrl-C).
- `redir_dup_all` — apenas depois que *todos* os opens tiverem sucesso, faz
  `dup2` de cada um sobre seu stdio alvo e fecha o original agora duplicado.
  Fazer os opens e os dups em duas fases é o que torna a limpeza-em-caso-de-falha
  correta.
- Quando um comando tem **múltiplos redirecionamentos para o mesmo fluxo**
  (`> a > b`), eles são aplicados em ordem, então o *último* vence para os dados
  reais — enquanto ambos os arquivos ainda são criados. Isso bate com o bash.

---

### 9.3 Heredocs — o redirecionamento mais sutil

Arquivos: `runner/redir/heredoc.c`, `heredoc_read.c`, `expand.c`,
`strbuilder.c`.

Um heredoc (`<< EOF`) alimenta texto inline para o stdin de um comando até uma
linha delimitadora ser vista:
```
cat << EOF
hello $USER
EOF
```
Aqui o stdin do `cat` deve ser as duas-e-tanto linhas que você digitou, e *não*
o terminal.

#### Como funciona

O truque: **um pipe é usado como buffer temporário.** `rn_redir_heredoc`:
1. `pipe(pfd)` — cria um pipe: `pfd[0]` é a ponta de leitura, `pfd[1]` a de
   escrita.
2. Lê as linhas do heredoc e **escreve-as em `pfd[1]`**.
3. Fecha a ponta de escrita e retorna `pfd[0]` — a ponta de leitura — como o fd
   que virará o stdin do comando. Assim o comando lê exatamente o que escrevemos.

#### Lendo as linhas (`heredoc_read.c`)

- `rn_redir_readline(input_fd)` imprime o prompt de continuação `> ` (no stderr)
  e lê **uma linha, um byte por vez** de `input_fd` (um pequeno buffer
  crescível via `rn_redir_linegrow`). Ler byte a byte — em vez de usar o
  readline — é deliberado: permite que o heredoc leia de um fd arbitrário, o que
  importa dentro de pipelines (onde o stdin foi substituído por um pipe, então o
  fd do terminal é passado separadamente como `input_fd`).
- `rn_redir_heredoc_loop` repete:
  - linha `NULL` → **EOF** (Ctrl-D antes do delimitador) → `HEREDOC_EOF`.
  - um `SIGINT` foi capturado → `HEREDOC_INTR`.
  - linha **igual ao delimitador** → `HEREDOC_DONE` (para, não a escreve).
  - caso contrário → expande-a se necessário e a escreve no pipe.

#### Expandindo corpos de heredoc (`expand.c`)

Se o corpo expande `$VAR` depende de se o **delimitador foi colocado entre
aspas**. Lembre que o parser definiu `redir.expand = !token->quoted`. Se o
delimitador foi escrito como `<< "EOF"` ou `<< 'EOF'`, a expansão fica desligada
e as linhas são copiadas literalmente; senão, `$VAR` e `$?` são expandidos linha
a linha por `rn_redir_expand_line` (usando o mesmo `exp_varvalue` do expansor
principal, mais o pequeno construtor de string em `strbuilder.c`). Toda linha
recebe um `\n` ao final.

#### Sinais durante um heredoc (`heredoc.c`)

É aqui que sinais e redirs se cruzam. `rn_redir_heredoc_fill`:
- troca os sinais para o modo **heredoc** (`SIG_HEREDOC`) para que o Ctrl-C
  interrompa o *laço de leitura* em vez do shell,
- roda o laço,
- troca os sinais de volta para **interativo**,
- e traduz os vários estados finais no retorno correto:
  - `HEREDOC_INTR` → `REDIR_FD_INTR` (um `-2` especial): o comando é abortado e
    `$?` vira 130. O fd é fechado.
  - `HEREDOC_FAIL` → `REDIR_FD_ERR` (`-1`): um erro de alocação/escrita.
  - `HEREDOC_EOF` → imprime o aviso do bash *"here-document delimited by
    end-of-file"* mas ainda usa o que foi coletado.
  - caso contrário → retorna o fd da ponta de leitura.

Esse valor `REDIR_FD_INTR` é propagado todo o caminho para cima através do
`apply.c` para que o comando inteiro seja cancelado de forma limpa com status de
saída 130 — exatamente o que o bash faz quando você aperta Ctrl-C ao digitar um
heredoc.

---

### 9.4 Comandos externos & resolução de PATH

Arquivos: `runner/external/`, `runner/path/`.

Quando `args[0]` **não** é um builtin, o shell precisa encontrar e rodar um
programa.

**Resolvendo o caminho (`external/resolve.c`):**
- Se `args[0]` contém uma `/`, ele é tratado como um caminho literal (relativo ou
  absoluto) — sem busca no `$PATH`.
- Senão, `rn_path` busca no `$PATH`.
- `rn_ext_classify` então faz `stat` no caminho resolvido e decide:
  `EXT_NOT_FOUND` / `EXT_DIR` (é um diretório) / `EXT_DENIED` (sem permissão de
  execução) / `EXT_READY`. `rn_ext_report` imprime a mensagem correspondente e
  retorna o código de saída correspondente (`127` não encontrado, `126` não
  executável).

**Buscando no `$PATH` (`path/path.c`, `path/parts.c`):**
- `rn_path_search` divide o `$PATH` em `:` e, para cada diretório, faz
  `rn_path_join` de `dir + "/" + cmd`.
- `rn_path_candidate` aceita a primeira entrada que existe, não é um diretório e
  é executável (`access(X_OK)`). Se ele só encontrar uma correspondência *não
  executável*, ele a lembra como um `fallback` para que o erro eventual seja
  "permission denied" em vez de "command not found" — um pequeno toque de
  fidelidade ao bash.

**Fork e execução (`external/process.c`):**
```
 rn_exec_ext
   ├─ rn_ext_resolve            encontra o programa (ou falha com 126/127)
   ├─ env_toarr                 constrói o char** envp para o execve
   └─ ext_fork
        ├─ sh_sig_mode(SIG_EXEC)   pai ignora Ctrl-C/Ctrl-\ enquanto o filho roda
        ├─ fork()
        │    └─ filho: rn_ext_child
        │         ├─ signal(SIGINT/SIGQUIT, SIG_DFL)   filho usa sinais padrão
        │         ├─ execve(path, args, envp)
        │         └─ _exit(rn_ext_execfail(path))      só se o execve falhar
        └─ pai: rn_ext_wait
             ├─ waitpid pelo filho
             ├─ sh_sig_mode(SIG_INTERACTIVE)           restaura sinais do prompt
             └─ traduz o status do wait → código de saída
```

A tradução do status é padrão e importante:
- `WIFEXITED` → o próprio código de saída do programa (`WEXITSTATUS`).
- `WIFSIGNALED` → `128 + número_do_sinal`. Então um programa morto por `SIGINT`
  (2) resulta em 130, por `SIGQUIT` (3) resulta em 131, etc. Essa é a convenção
  universal de shells e é por isso que `$?` é 130 após um Ctrl-C.

`rn_ext_execfail` (rodado apenas no filho, após um `execve` falho) mapeia o
`errno` para a mensagem/código de saída corretos (`ENOENT`→127,
`EISDIR`/`EACCES`/`ENOEXEC`→126).

---

### 9.5 Pipelines — muitos processos, um fluxo de dados

Arquivos: `runner/pipe/`. Esta é a segunda área "obscura".

Um pipeline `a | b | c` roda três programas **concorrentemente**, com o stdout de
`a` conectado ao stdin de `b`, e o stdout de `b` ao stdin de `c`. O conector é,
de novo, o `pipe(2)`: ele retorna dois fds, e bytes escritos na ponta de escrita
saem na ponta de leitura.

Para N comandos você precisa de **N−1 pipes**.

#### Passo 1 — achatar a árvore (`tree.c`)

O parser construiu uma *árvore* de pipes, mas a execução quer um **array** plano
de comandos. `rn_pipe_count` conta as folhas; `rn_pipe_flatten` faz uma travessia
em ordem preenchendo `cmds[]` da esquerda para a direita. Então a árvore para
`a | b | c` achata para `[a, b, c]`.

#### Passo 2 — montar o contexto (`pipe.c`)

`rn_pipe_ctx` (um `t_pipe_ctx`) reúne tudo num lugar só:
```c
typedef struct s_pipe_ctx {
    t_command **cmds;        // comandos achatados, terminado em NULL
    t_command  *root;        // a raiz da árvore de pipes (p/ limpeza do filho)
    t_ast      *ast;
    t_env     **env;
    int        *fds;         // todos os fds de pipe: 2 ints por pipe, em sequência
    pid_t      *pids;        // um por comando
    size_t      pipe_count;  // número de pipes == comandos - 1
}   t_pipe_ctx;
```
`pipe_alloc` aloca `cmds` e o array `fds` (tamanho `(N-1) * 2`).
`rn_pipe_create` (em `fd.c`) inicializa todo fd como `-1`, depois chama `pipe()`
para cada pipe, armazenando o par em `fds[i*2]` (leitura) e `fds[i*2+1]`
(escrita).

#### Passo 3 — fork de cada comando (`process.c`)

`rn_pipe_fork_wait`:
1. aloca `pids[]`,
2. coloca os sinais em modo **exec** (o shell pai ignora Ctrl-C enquanto o
   pipeline roda),
3. `rn_pipe_fork_all` — faz `fork` uma vez por comando; cada filho roda
   `rn_pipe_child`,
4. **o pai fecha todos os fds de pipe** (`rn_pipe_close_all`) — isso é essencial:
   se o pai mantivesse uma ponta de escrita aberta, o leitor no fim do pipe nunca
   veria EOF e travaria para sempre,
5. `rn_pipe_wait` — faz `waitpid` em cada filho; o status de saída do pipeline é
   o status do **último** comando (semântica do bash),
6. restaura os sinais **interativos**, libera `pids`.

#### Passo 4 — conectar cada filho (`fd.c`, `rn_pipe_child`)

Este é o cerne. Para o comando na posição `pos`:
```
 se pos > 0:    dup2(fds[(pos-1)*2],     STDIN)   // lê do pipe anterior
 se tem próximo:dup2(fds[pos*2 + 1],     STDOUT)  // escreve no nosso próprio pipe
 fecha TODOS os fds de pipe neste filho  (só precisamos dos dois que acabamos de dup)
 reseta SIGINT/SIGQUIT para o padrão     (filhos são processos de primeiro plano normais)
 rn_execute(cmds[pos], env, heredoc_fd, NULL)     // recursão! roda o comando
 _exit(status)
```
O primeiro comando mantém o stdin real; o último mantém o stdout real. Todo filho
fecha *todos* os fds de pipe após o dup, porque deixar cópias extras abertas
iria, de novo, impedir o EOF de se propagar.

**A sutileza do heredoc-num-pipeline** (`rn_heredoc_fd`): um comando dentro de um
pipeline pode conter um heredoc (`a | cat << EOF`). Mas o stdin daquele filho
está prestes a ser substituído por um pipe via `dup2`. Então, *antes* do `dup2`,
o filho salva uma cópia privada do fd atual do terminal com `dup(STDIN_FILENO)` e
a passa para `rn_execute` como o `heredoc_fd`. Esse é exatamente o argumento que
desce todo o caminho até `rn_redir_readline`, para que o heredoc ainda possa ler
sua digitação do terminal mesmo que o stdin real do comando seja um pipe. (O
primeiro comando, `pos == 0`, ainda tem o terminal real no stdin, então só passa
`STDIN_FILENO`.)

**Limpeza do filho** (`cleanup.c`): cada filho forkado é uma cópia completa do
shell, então antes do `_exit` ele libera o ambiente, o histórico, a árvore de
comandos e a contabilidade do pipe — mantendo os filhos livres de vazamentos sob
ferramentas como o valgrind.

> **Por que um builtin como `export` não muda o pai num pipeline?**
> Porque num pipeline todo comando — builtin ou não — roda num filho forkado
> (`rn_pipe_child` chama `rn_execute`, que para uma folha chama o builtin *no
> filho*). A mudança de ambiente do filho morre com ele. Esse é exatamente o
> comportamento do bash: `export X=1 | cat` não define de fato `X` no seu shell.

---

## 10. `bin/` — os builtins

Builtins são comandos que o shell roda **por si mesmo**, no seu próprio processo
(quando não está num pipeline), porque eles precisam afetar o estado do próprio
shell (diretório atual, ambiente, saída). O `rn_exec_bin` de `runner/builtin.c` é
apenas uma tabela de despacho que casa `args[0]` com uma das sete funções; cada
uma retorna um código de saída e o escreve através de `*status`.

- **`echo.c`** — imprime seus argumentos separados por espaços. Suporta `-n`
  (suprime o newline ao final) e até sequências dele como `-nnn`;
  `echo_has_flag` valida que um argumento `-...` é *só* de `n`s antes de tratá-lo
  como flag.
- **`pwd.c`** — imprime `getcwd`; recorre à variável `PWD` se `getcwd` falhar.
- **`cd.c`** — muda de diretório. `getpath` resolve o destino: sem arg →
  `$HOME`; `-` → `$OLDPWD` (e o ecoa, como o bash); senão, o arg literal. Rejeita
  mais de um argumento. Após um `chdir` bem-sucedido, atualiza `OLDPWD` (para o
  `PWD` anterior) e `PWD` (para o novo `getcwd`). *(Nota: o código usa a chave
  `OPWD` via a constante `ENV_OPWD` para o "old pwd".)*
- **`env.c`** — imprime `KEY=VALUE` para toda variável que tem valor, pulando o
  `$?` interno. Com qualquer argumento, dá erro (o `env` deste minishell não
  recebe args/opções).
- **`unset.c`** — remove cada variável nomeada, validando os identificadores
  primeiro (nomes inválidos produzem um erro e código de saída 1, mas o
  processamento continua).
- **`export/`** — três arquivos:
  - `export.c` — sem args, imprime o ambiente ordenado (`export_print`). Com
    args, para cada `KEY=VALUE` define a variável; para um `KEY` solto, marca-o
    como exportado com um valor `NULL`. Valida os identificadores
    (`export_invalid`).
  - `keys.c` — coleta as chaves (pulando `$?`) e faz um **bubble-sort** delas para
    que a listagem seja alfabética, como o bash.
  - `print.c` — imprime cada uma como `declare -x KEY="VALUE"` (o formato do
    `export` do bash), omitindo o `="..."` para chaves exportadas sem valor.
- **`exit.c`** — o builtin mais complicado. Ele analisa o argumento numérico
  opcional com verificação completa de overflow contra `LLONG_MAX`/`LLONG_MIN`
  (`exit_parse_prefix` / `exit_parse_digits` / `exit_apply_sign`). Um argumento
  não numérico → `"numeric argument required"` e código de saída 2. Argumentos
  demais → erro, código 1, e ele **não** sai. O código de saída final é
  `(unsigned char)code` (então ele dá a volta módulo 256, como todo shell). Ele
  sinaliza o REPL para parar definindo `g_signal = SH_EXIT_REQUESTED` — uma forma
  limpa de pedir ao laço em `reader.c` para sair. Também imprime `exit` no stderr
  quando interativo.

---

## 11. Sinais — o aprofundamento

Sinais são o outro tópico genuinamente obscuro, então aqui está a história
inteira num lugar só. Tudo vive em `shared/signal.c`, mais os resets por filho no
runner.

### A única global

O subject permite exatamente uma variável global, e é esta:
```c
volatile sig_atomic_t g_signal = 0;
```
- `volatile` — diz ao compilador que o valor pode mudar a qualquer momento
  (dentro de um handler de sinal), então ele deve sempre relê-lo da memória.
- `sig_atomic_t` — um tipo cuja leitura/escrita é garantidamente feita em um
  único passo ininterrupto, então um handler nunca pode observar um valor
  escrito pela metade.

Ele é usado como uma pequena caixa de correio: um handler escreve nela, e o laço
principal a lê para saber "houve um SIGINT?". Ele também é sobrecarregado para
carregar o sentinela `SH_EXIT_REQUESTED` que o `exit` usa para pedir ao laço que
pare.

### Os três modos (`sh_sig_mode`)

Um shell precisa de comportamento de sinal *diferente* dependendo do que está
fazendo. O enum `t_sig_mode` nomeia as três situações, e `sh_sig_mode` instala os
handlers corretos via `sigaction`:

| Modo             | Quando                       | SIGINT (Ctrl-C)                 | SIGQUIT (Ctrl-\\) |
|------------------|------------------------------|---------------------------------|-------------------|
| `SIG_INTERACTIVE`| sentado no prompt            | `sh_sigint_interactive`         | ignorado          |
| `SIG_EXEC`       | um filho/pipeline rodando    | ignorado no **pai**             | ignorado          |
| `SIG_HEREDOC`    | lendo o corpo de um heredoc  | `sh_sigint_heredoc`             | ignorado          |

`SIGQUIT` é **sempre ignorado** no próprio shell — o bash nunca sai do prompt com
Ctrl-\\. Os filhos, porém, o resetam para o padrão para que um programa
descontrolado ainda possa gerar core-dump com Ctrl-\\.

### Os handlers

- **`sh_sigint_interactive`** (no prompt): imprime um newline, depois usa os
  helpers do readline `rl_on_new_line` + `rl_replace_line("", 0)` +
  `rl_redisplay` para apagar a entrada atual e desenhar um prompt novo e vazio.
  Esse é o clássico comportamento "Ctrl-C limpa a linha e te dá um novo prompt".
  (Ele define e depois limpa `g_signal`; o laço separadamente sobe `$?` para
  130.)
- **`sh_sigint_heredoc`** (lendo um heredoc): define `g_signal = SIGINT` e imprime
  um newline. O laço de leitura do heredoc nota `g_signal == SIGINT` e aborta o
  heredoc, que vira status de saída 130.

### `ECHOCTL` — uma pequena gentileza de terminal

`sh_echoctl` alterna a flag `ECHOCTL` do terminal com `termios`. Quando
habilitada, teclas de controle ecoam como `^C`; o shell a liga **on** no modo
interativo e **off** durante um heredoc para que um `^C` perdido não suje a
transcrição do heredoc.

### Quem troca de modo, e quando

- Na inicialização e após cada comando → `SIG_INTERACTIVE` (o prompt está
  seguro).
- Prestes a forkar um comando externo ou um pipeline → `SIG_EXEC` (para que o
  Ctrl-C vá para o *filho*, não para o shell — o shell só espera).
- Dentro dos filhos `execve`ados e dos filhos de pipeline →
  `signal(SIGINT, SIG_DFL)` e `signal(SIGQUIT, SIG_DFL)`, para que o programa se
  comporte como qualquer processo normal de primeiro plano.
- Entrando num heredoc → `SIG_HEREDOC`; saindo dele → `SIG_INTERACTIVE`.

Essa separação é o que faz o shell parecer certo: Ctrl-C no prompt limpa a linha,
Ctrl-C durante `sleep 100` mata o sleep (e define `$? = 130`) mas deixa seu shell
vivo, e Ctrl-C durante um heredoc cancela apenas aquele heredoc.

---

## 12. O fluxo completo, de ponta a ponta

Vamos rastrear uma linha realista todo o caminho:

```
GA&Fabricio:minishell$ grep "$NEEDLE" file.txt | wc -l > count.txt
```

**0. Inicialização** (`main` → `core_run` → `core_init`)
O ambiente é analisado numa lista `t_env`, `SHLVL` é incrementado, `$?` é semeado
com `0`, e os sinais entram em `SIG_INTERACTIVE`. As configurações do terminal
são salvas.

**1. Ler** (`core_iteration`)
`readline` imprime o prompt e retorna a linha. Ela não está vazia, então é
adicionada ao histórico e passada para `core_process_line`.

**2. Lexar** (`core_parse` → `lexer`)
O scanner percorre a string usando a tabela de classes. Ele produz tokens:
```
WORD("grep")  WORD("\"$NEEDLE\"")  WORD("file.txt")  PIPE("|")
WORD("wc")    WORD("-l")           REDIR_OUT(">")     WORD("count.txt")
```
Note que a segunda palavra mantém suas aspas por enquanto, e é marcada com
`expand=true` (contém `$`) e `quoted=true`.

**3. Analisar** (`parser_controller` → `build_pipeline`)
O primeiro token não é um `|`, ótimo. `parse_simple_command` constrói o comando
da esquerda:
- `args = ["grep", "\"$NEEDLE\"", "file.txt"]`, `redirs = []`.
Depois um `|` é visto, então o comando da direita é analisado:
- `args = ["wc", "-l"]`, e `consume_redir` vê `>` e depois a palavra
  `count.txt`, registrando `redir { REDIR_OUT, "count.txt", expand=true }`.
As duas folhas são fundidas num `PNODE_PIPE`. A `t_ast` o envolve, `error = 0`.

**4. Executar** (`rn_execute(root, ...)`)
A raiz é um `PNODE_PIPE` → `rn_pipe`.
- Achata a árvore para `cmds = [grep-cmd, wc-cmd]`. Um pipe necessário; cria-o.
- Sinais → `SIG_EXEC`. Fork duas vezes.

**Filho 0 (`grep`)** em `rn_pipe_child(pos=0)`:
- É o primeiro comando, então o stdin continua o terminal; ele tem um próximo
  comando, então `dup2(pipe_write, STDOUT)`. Fecha todos os fds de pipe. Reseta
  os sinais para o padrão.
- `rn_execute(grep-cmd, ..., PNODE_CMD)` → `rn_exec_cmd`:
  - **Expande** os args: `"$NEEDLE"` está dentro de aspas duplas, então `$NEEDLE`
    é buscado e substituído **sem** divisão de palavras; as aspas são removidas.
    Resultado: `["grep", "<valor de NEEDLE>", "file.txt"]`.
  - Sem redirs neste comando.
  - `grep` não é builtin → `rn_exec_ext`: resolve `grep` via `$PATH`, faz
    `execve`. Seu stdout é o pipe.
- `_exit` com o status do grep.

**Filho 1 (`wc -l`)** em `rn_pipe_child(pos=1)`:
- `pos > 0`, então `dup2(pipe_read, STDIN)` — sua entrada é a saída do grep. É o
  último comando, então o stdout continua... mas espere, ele tem um
  redirecionamento.
- `rn_execute(wc-cmd, ...)` → `rn_exec_cmd`:
  - Expande os args → `["wc", "-l"]`.
  - **Aplica redirs**: salva o stdio, abre `count.txt` com
    `O_WRONLY|O_CREAT|O_TRUNC`, faz `dup2` dele sobre o STDOUT. Então a saída do
    `wc` vai para o arquivo, não para o pipe-ou-terminal.
  - `execve` do `wc`. Ele lê as linhas do grep do stdin (o pipe), conta-as,
    escreve o número em `count.txt`.

**Pai** (`rn_pipe_fork_wait`):
- Fecha todos os fds de pipe (para que o leitor possa ver EOF).
- Faz `waitpid` dos dois filhos; o status do pipeline é o do **`wc`**.
- Restaura `SIG_INTERACTIVE`.

**5. Lembrar** (`rn_status_set`)
O status do pipeline é escrito em `$?`. De volta em `core_iteration`, `g_signal`
é checado (nenhuma saída pedida), a linha é liberada, e o laço dá a volta para o
próximo prompt.

E essa é a jornada inteira: **texto → tokens → árvore → processos → um número em
`$?`**, com sinais mantendo o terminal civilizado e redirecionamentos
silenciosamente recabeando descritores de arquivo todo o caminho abaixo.

---

*Leia os módulos nesta ordem se você estiver explorando o código pela primeira
vez:* `core/reader.c` → `lexer/` → `parser/` → `runner/control.c` →
`runner/expander/` → `runner/redir/` → `runner/pipe/`. Todo o resto é um helper
chamado a partir desses.
