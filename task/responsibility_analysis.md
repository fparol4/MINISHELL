# Análise de Mistura de Responsabilidades

> Cada seção abaixo corresponde a uma função listada na **Etapa 2** do `refactoring_tasks.md`.
> Use o identificador (a.1, a.2, b.1, …) para cruzar referências entre os dois arquivos.

---

## `source/modules/runner/expander.c`

### a.1 — `varvalue` (28 linhas)

**Problema:** Mistura casos especiais (`$?`, `$$`, `$0`) com busca normal no ambiente no mesmo bloco condicional. A lógica de `$?` exige conversão de inteiro para string (`ft_itoa`), o que aumenta as linhas.

**O que fazer:** Extrair `var_special` para tratar `$?`, `$$` e outros. `varvalue` vira dispatcher: chama `var_special` ou `env_get`.

---

### a.2 — `rn_argexpand` (38 linhas)

**Problema:** Itera caractere por caractere com máquina de estados (UNQUOTED, SINGLE, DOUBLE) e em cada estado decide expandir variável, adicionar literal ou alternar estado. Três responsabilidades no mesmo laço: controle de estado, expansão e montagem da string.

**O que fazer:** Extrair `argexpand_unquoted`, `argexpand_single`, `argexpand_double`. A `rn_argexpand` mantém só o laço com dispatch para a função do estado atual.

---

## `source/modules/runner/redir.c`

### b.1 — `rn_redir_value` (27 linhas)

**Problema:** Expande o nome do arquivo aplicando quote removal e expansão de variáveis. A decisão de expandir baseada no campo `quoted` está misturada com a chamada de expansão e a duplicação do resultado.

**O que fazer:** Extrair `redir_value_expand` (expande variáveis se necessário) e `redir_value_clean` (remove quotes). `rn_redir_value` chama as duas em sequência.

---

### b.2 — `rn_redir_readline` (29 linhas)

**Problema:** Lê linhas com `readline`, decide se expande variáveis e faz append no buffer. Mistura leitura, decisão de expansão e concatenação.

**O que fazer:** Extrair `heredoc_read_expand` (expande a linha se não quoted) e `heredoc_buffer_append` (concatena no buffer). `rn_redir_readline` coordena o laço.

---

### b.3 — `rn_redir_heredoc_fill` (36 linhas)

**Problema:** Abre arquivo temporário, itera chamando `rn_redir_readline` e escreve cada linha no fd. Mistura criação de tempfile, iteração e escrita. Nome do arquivo gerado inline.

**O que fazer:** Extrair `heredoc_create_tempfile` (gera nome + abre fd) e `heredoc_write_lines` (escreve linhas no fd). `rn_redir_heredoc_fill` coordena as duas.

---

### b.4 — `rn_redir_apply` (45 linhas)

**Problema:** Maior função do projeto. Itera sobre a lista de redirs e para cada um: faz backup do fd com `dup`, abre arquivo com `open`, aplica `dup2`, fecha fds, e no final restaura. Também trata erros de `dup2` e `open`. Backup, aplicação, restore e erro tudo misturado.

**O que fazer:** Extrair `redir_backup_fds` (dup de stdin/stdout/stderr), `redir_apply_single` (abre + dup2 para um redir), `redir_restore_fds` (restaura fds). `rn_redir_apply` itera chamando `redir_apply_single` com backup/restore ao redor.

---

## `source/modules/runner/pipe.c`

### c.1 — `rn_pipe` (37 linhas)

**Problema:** Cria pipes com `pipe()`, faz fork para cada comando, fecha descritores nos pais e filhos, espera todos com `waitpid`. Mistura 4 fases: setup de pipes, fork, fechamento de fds e sincronização.

**O que fazer:** Extrair `pipe_setup` (cria todos os pipes), `pipe_fork_all` (fork configurando fds), `pipe_cleanup` (fecha pipes no pai), `pipe_wait_all` (espera filhos). `rn_pipe` chama as 4 em sequência.

---

## `source/modules/runner/external.c`

*(Nenhuma função com mais de 25 linhas.)*

---

## `source/shared/string.c`

### e.1 — `sh_quote_remove` (26 linhas)

**Problema:** Implementa máquina de estados inline para rastrear aspas simples/duplas. Mistura atualização de estado com cópia seletiva de caracteres.

**O que fazer:** Extrair `quote_state_update` (recebe char + estado, retorna novo estado + flag se deve copiar). `sh_quote_remove` fica só com o laço.

---

## `source/core/reader.c`

### f.1 — `core_loop` (30 linhas)

**Problema:** Mistura leitura de input (`readline`), parsing (`core_parse`), execução (`rn_execute`), gerenciamento de histórico (`add_history`) e condição de saída. Tudo no mesmo laço.

**O que fazer:** Extrair `core_iteration` que recebe a linha e delega parsing + execução. `core_loop` fica só com `readline`, verificação de EOF e chamada a `core_iteration`.

---

## `source/shared/signal.c`

### g.1 — `sh_sig_mode` (26 linhas)

**Problema:** Configura sinais para 3 modos (interactive, heredoc, ignore) em um único bloco condicional. Cada modo configura SIGINT e SIGQUIT de forma diferente.

**O que fazer:** Extrair `sh_sig_mode_interactive`, `sh_sig_mode_heredoc`, `sh_sig_mode_ignore`. `sh_sig_mode` vira dispatcher.

---

## `source/modules/env/init.c`

### h.1 — `env_init` (29 linhas)

**Problema:** Inicializa o ambiente misturando iteração sobre `environ`, chamada a `entryparse` (que faz split `key=value`), criação de nós com `env_node_new` e incremento de SHLVL.

**O que fazer:** Extrair `env_increment_shlvl` (incrementa o valor), `env_parse_entry` (faz split key=value). `env_init` coordena o laço.

---

## `source/modules/env/set.c`

### i.1 — `env_set` (33 linhas)

**Problema:** Mistura validação do nome da variável, busca na lista encadeada, atualização de nó existente e criação de novo nó.

**O que fazer:** Extrair `env_key_validate` (valida nome), `env_find_node` (busca na lista), `env_node_update` (substitui valor). `env_set` coordena as chamadas.

---

## `source/modules/env/toarr.c`

### j.1 — `env_toarr` (27 linhas)

**Problema:** Converte lista ligada em `char **`, mas a formatação `"key=value"` está inline no laço.

**O que fazer:** Extrair `env_format_entry` (recebe nó, retorna `"key=value"`). `env_toarr` fica só com o laço de alocação + `env_format_entry`.

---

## `source/modules/bin/echo.c`

### k.1 — `bin_echo` (28 linhas)

**Problema:** Interpreta flags (`-n`, `-nnnnn`) e imprime argumentos com espaçamento no mesmo laço. Detecção de flag é repetitiva.

**O que fazer:** Extrair `echo_has_flag` (verifica se string é `-n` válida) e `echo_print_args` (imprime argumentos sem flag). `bin_echo` coordena.

---

## `source/modules/bin/exit.c`

### l.1 — `exit_value` (37 linhas)

**Problema:** Valida argumento numérico misturando verificação de dígitos, overflow (`ft_atoll` vs `ft_atoi`), detecção de argumento inválido e cálculo do valor (`% 256`).

**O que fazer:** Extrair `exit_validate_num` (verifica dígitos) e `exit_overflow_check` (compara com LLONG_MAX). `exit_value` chama ambas e aplica o módulo.

---

## `source/modules/bin/export.c`

### m.1 — `getkeys` (29 linhas)

**Problema:** Percorre a lista de env, copia chaves para array e ordena com bubble sort — tudo no mesmo bloco.

**O que fazer:** Extrair `env_keys_to_array` (copia as chaves) e manter o sort como `sort_keys` separada.

---

### m.2 — `print_export` (29 linhas)

**Problema:** Itera sobre keys ordenadas e imprime `declare -x key="value"` com escape de caracteres especiais inline.

**O que fazer:** Extrair `export_quote_value` (escapa/entre aspas o valor). `print_export` fica só com iteração.

---

### m.3 — `bin_export` (36 linhas)

**Problema:** Mistura parsing de argumentos (`key=value` vs `key`), validação, inserção no env e chamada a `print_export`.

**O que fazer:** Extrair `export_parse_arg` (separa key/value) e `export_validate_key`. `bin_export` coordena: sem args → `print_export`; com args → valida + `env_set`.

---

## `source/modules/lexer/scanner/table.c`

### n.1 — `scanner_table_char` (26 linhas)

**Problema:** Classifica caracteres em categorias (CHAR, OPERATOR, QUOTE, etc.) com um switch grande que centraliza toda a lógica de classificação.

**O que fazer:** Extrair `scanner_is_operator_char`, `scanner_is_quote_char`, `scanner_is_blank_char`. `scanner_table_char` vira dispatcher.

---

## `source/modules/parser/parser.c`

### o.1 — `build_pipeline` (26 linhas)

**Problema:** Constrói a árvore de pipeline misturando alocação de nós, consumo de tokens e delegação para `parse_simple_command`.

**O que fazer:** Extrair `pipeline_add_node` (cria nó pipe e liga left/right). `build_pipeline` mantém o loop.

---

## `source/modules/parser/parser_command.c`

### p.1 — `parse_simple_command` (26 linhas)

**Problema:** Itera sobre tokens decidindo se consome word ou redir no mesmo laço. Mistura decisão de tipo com consumo.

**O que fazer:** Extrair `consume_token` que examina `token_class` e chama `consume_word` ou `consume_redir`. O laço principal fica mais enxuto.

---

## `source/modules/parser/parser_state.c`

### q.1 — `parser_add_arg` (29 linhas)

**Problema:** Realoca array dinâmico, copia strings e trata erros manualmente. A lógica de `dynarray_append` e checagem de `expand` estão misturadas.

**O que fazer:** Extrair `arg_grow_array` (encapsula `dynarray_append` + tratamento de erro). `parser_add_arg` fica só com atribuição e flag expand.

---

### q.2 — `parser_add_redir` (33 linhas)

**Problema:** Mesmo padrão de `parser_add_arg`: realoca array, copia token->value, preenche struct. Mistura alocação com preenchimento.

**O que fazer:** Extrair `redir_init_element` (preenche `t_parser_redir` a partir do token) e `redir_array_append` (encapsula `dynarray_append` + erro). `parser_add_redir` coordena.

---

## `source/modules/runner/control.c`

### r.1 — `rn_exec_cmd` (29 linhas)

**Problema:** Decide se comando é builtin ou externo, executa e trata erros. Mistura despacho com execução e pós-execução.

**O que fazer:** Extrair `rn_try_builtin` (tenta builtin, retorna -1 se não for) e `rn_exec_fallback` (executa externo). `rn_exec_cmd` vira if/else simples.

---

## `source/modules/runner/path.c`

### s.1 — `rn_path_search` (32 linhas)

**Problema:** Itera sobre PATH, concatena com comando, faz `stat()` e testa `access()`. Mistura iteração, concatenação e verificação de arquivo.

**O que fazer:** Extrair `path_try_dir` (concatena `dir/cmd` e testa com stat/access). `rn_path_search` coordena a iteração sobre PATH.
