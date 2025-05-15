# Sistema de Votação de Projetos

Este é um sistema de votação de projetos desenvolvido para a disciplina de Estrutura de Dados. O código foi organizado em uma arquitetura modular para facilitar a apresentação e explicação por uma equipe de 3 pessoas.

## Divisão para Apresentação

### Parte 1: Gerenciamento de Projetos
  - Estrutura de dados dos projetos
  - Funções de manipulação de arquivos (salvar/carregar)
  - Algoritmos de ordenação e busca
  - Gerenciamento da persistência de dados

### Parte 2: Gerenciamento de Usuários
  - Estrutura de dados dos usuários
  - Sistema de validação de matrícula
  - Processo de votação e alteração de voto
  - Criação de novos usuários e projetos

## Fluxograma de Gerenciamento de Usuários

  ```
  ┌───────────────────┐
  │ Verificar Votação │
  └─────────┬─────────┘
            ▼
  ┌────────────────────┐     Não     ┌────────────────────┐
  │ Matrícula é válida?│────────────▶│ Exibe mensagem de  │
  └─────────┬──────────┘             │      erro          │
            │ Sim                    └────────────────────┘
            ▼
  ┌────────────────────┐     Não     ┌────────────────────┐
  │ Usuário já existe? │────────────▶│   Criar novo       │
  └─────────┬──────────┘             │    usuário         │
            │ Sim                    └─────────┬──────────┘
            ▼                                  │
  ┌────────────────────┐                       │
  │   Acessa menu do   │◀──────────────────────┘
  │      usuário       │
  └─────────┬──────────┘
            ▼
  ┌────────────────────┐
  │ 1. Votar           │
  │ 2. Alterar Voto    │
  │ 3. Listar Projetos │
  │ 0. Logout          │
  └────────────────────┘
  ```

### Parte 3: Interface e Main
  - Estrutura principal do programa
  - Sistema de menus e interação com usuário
  - Fluxo de execução e inicialização
  - Integração entre os diferentes módulos
    

## Funcionalidades do Sistema

1. Listar projetos por número de votos
2. Listar projetos por nome
3. Sistema de autenticação por matrícula
4. Cadastro de novos usuários
5. Sistema de votação com possibilidade de alteração
6. Persistência de dados em arquivo
