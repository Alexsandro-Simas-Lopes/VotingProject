#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 100
#define MAX_PROJETOS 100
#define MAX_USUARIOS 100
#define TAM_MATRICULA 8  // 7 dígitos + \0
#define ARQUIVO_PROJETOS "projetos.txt"

// Estrutura para armazenar os dados do projeto
typedef struct {
    char nome[MAX_NOME];
    int votos;
} Projeto;

// Estrutura para armazenar dados do usuário
typedef struct {
    char matricula[TAM_MATRICULA];
    char nome[MAX_NOME];
    char projetoVotado[MAX_NOME];
    int jaVotou;
} Usuario;

// Função para limpar o caractere de nova linha no final de uma string
void limparNovaLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n')
        str[len-1] = '\0';
}

// Função para salvar projetos em arquivo
void salvarProjetos(Projeto projetos[], int numProjetos) {
    FILE *arquivo = fopen(ARQUIVO_PROJETOS, "w");
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os projetos.\n");
        return;
    }
    
    fprintf(arquivo, "%d\n", numProjetos);
    
    for (int i = 0; i < numProjetos; i++) {
        fprintf(arquivo, "%s\n%d\n", projetos[i].nome, projetos[i].votos);
    }
    
    fclose(arquivo);
    printf("Dados dos projetos salvos com sucesso em '%s'.\n", ARQUIVO_PROJETOS);
}

// Função para carregar projetos do arquivo
int carregarProjetos(Projeto projetos[]) {
    FILE *arquivo = fopen(ARQUIVO_PROJETOS, "r");
    
    if (arquivo == NULL) {
        printf("Arquivo de projetos não encontrado. Iniciando com lista vazia.\n");
        return 0;
    }
    
    int numProjetos;
    fscanf(arquivo, "%d\n", &numProjetos);
    
    if (numProjetos > MAX_PROJETOS) {
        printf("Aviso: O arquivo contém mais projetos que o limite suportado.\n");
        printf("Apenas os primeiros %d projetos serão carregados.\n", MAX_PROJETOS);
        numProjetos = MAX_PROJETOS;
    }
    
    for (int i = 0; i < numProjetos; i++) {
        fgets(projetos[i].nome, MAX_NOME, arquivo);
        limparNovaLinha(projetos[i].nome);
        fscanf(arquivo, "%d\n", &projetos[i].votos);
    }
    
    fclose(arquivo);
    printf("Dados de %d projetos carregados com sucesso.\n", numProjetos);
    return numProjetos;
}

// Funções de comparação para ordenação
int compararProjetosPorVotos(const void *a, const void *b) {
    return ((Projeto *)b)->votos - ((Projeto *)a)->votos;
}

int compararProjetosPorNome(const void *a, const void *b) {
    return strcmp(((Projeto *)a)->nome, ((Projeto *)b)->nome);
}

// Função para listar projetos
void listarProjetos(Projeto projetos[], int numProjetos, int ordenarPorVotos) {
    if (numProjetos == 0) {
        printf("Não há projetos cadastrados.\n");
        return;
    }
    
    // Ordena os projetos conforme o critério selecionado
    qsort(projetos, numProjetos, sizeof(Projeto), 
          ordenarPorVotos ? compararProjetosPorVotos : compararProjetosPorNome);
    
    printf("\n===== LISTAGEM DE PROJETOS POR %s =====\n", 
           ordenarPorVotos ? "NÚMERO DE VOTOS" : "NOME");
    
    for (int i = 0; i < numProjetos; i++) {
        printf("%02d - %s", i+1, projetos[i].nome);
        if (ordenarPorVotos) {
            printf("\nVotos: %d\n\n", projetos[i].votos);
        } else {
            printf("\n");
        }
    }
}

// Função para verificar se a matrícula é válida
int matriculaValida(const char *matricula) {
    if (strlen(matricula) != 7) {
        printf("Erro: A matrícula deve ter exatamente 7 dígitos.\n");
        return 0;
    }
    
    for (int i = 0; i < 7; i++) {
        if (matricula[i] < '0' || matricula[i] > '9') {
            printf("Erro: A matrícula deve conter apenas dígitos numéricos.\n");
            return 0;
        }
    }
    
    return 1;
}

// Função para buscar um usuário pelo número de matrícula
int buscarUsuario(Usuario usuarios[], int numUsuarios, const char *matricula) {
    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(usuarios[i].matricula, matricula) == 0) {
            return i;
        }
    }
    return -1;
}

// Função para buscar um projeto pelo nome
int buscarProjeto(Projeto projetos[], int numProjetos, const char *nome) {
    for (int i = 0; i < numProjetos; i++) {
        if (strcmp(projetos[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Função para processar o voto de um usuário
void processarVoto(Projeto projetos[], int numProjetos, Usuario *usuario, int alterarVoto) {
    if (alterarVoto && !usuario->jaVotou) {
        printf("Você ainda não votou em nenhum projeto.\n");
        printf("Use a opção 'Votar' para registrar seu voto.\n");
        return;
    }
    
    if (!alterarVoto && usuario->jaVotou) {
        printf("Você já votou no projeto '%s'.\n", usuario->projetoVotado);
        printf("Use a opção 'Alterar Voto' para mudar seu voto.\n");
        return;
    }
    
    // Se estiver alterando o voto, remove o voto anterior
    if (alterarVoto) {
        int projetoIdx = buscarProjeto(projetos, numProjetos, usuario->projetoVotado);
        if (projetoIdx != -1) {
            projetos[projetoIdx].votos--;
        }
    }
    
    printf("\nProjetos disponíveis:\n");
    for (int i = 0; i < numProjetos; i++) {
        printf("%d - %s\n", i+1, projetos[i].nome);
    }
    
    int indice;
    printf("\nDigite o número do projeto para votar: ");
    scanf("%d", &indice);
    getchar(); // Consome o caractere de nova linha
    
    if (indice < 1 || indice > numProjetos) {
        printf("Número de projeto inválido!\n");
        return;
    }
    
    projetos[indice-1].votos++;
    strcpy(usuario->projetoVotado, projetos[indice-1].nome);
    usuario->jaVotou = 1;
    
    printf("Voto %s com sucesso para o projeto '%s'!\n", 
           alterarVoto ? "alterado" : "registrado", projetos[indice-1].nome);
    
    // Salva os projetos após registrar um voto
    salvarProjetos(projetos, numProjetos);
}

// Função para criar um novo usuário
int criarNovoUsuario(Usuario usuarios[], int numUsuarios, Projeto projetos[], 
                    int *numProjetos, const char *matricula) {
    if (numUsuarios >= MAX_USUARIOS) {
        printf("Erro: Limite máximo de usuários atingido.\n");
        return -1;
    }
    
    strcpy(usuarios[numUsuarios].matricula, matricula);
    
    printf("Nome do usuário: ");
    fgets(usuarios[numUsuarios].nome, MAX_NOME, stdin);
    limparNovaLinha(usuarios[numUsuarios].nome);
    
    printf("Nome do Projeto: ");
    fgets(usuarios[numUsuarios].projetoVotado, MAX_NOME, stdin);
    limparNovaLinha(usuarios[numUsuarios].projetoVotado);
    
    // Verifica se o projeto já existe
    int projetoIndice = buscarProjeto(projetos, *numProjetos, usuarios[numUsuarios].projetoVotado);
    
    // Se o projeto não existe, adiciona à listagem
    if (projetoIndice == -1) {
        if (*numProjetos >= MAX_PROJETOS) {
            printf("Erro: Limite máximo de projetos atingido.\n");
            printf("Não foi possível adicionar o novo projeto.\n");
        } else {
            strcpy(projetos[*numProjetos].nome, usuarios[numUsuarios].projetoVotado);
            projetos[*numProjetos].votos = 0;
            (*numProjetos)++;
            
            // Salva os projetos após adicionar um novo
            salvarProjetos(projetos, *numProjetos);
            printf("Novo projeto '%s' adicionado com sucesso!\n", 
                   usuarios[numUsuarios].projetoVotado);
        }
    }
    
    printf("Digite (1-Salvar / 0-Cancelar): ");
    int escolha;
    scanf("%d", &escolha);
    getchar(); // Consome o caractere de nova linha
    
    if (escolha != 1) {
        printf("Cadastro cancelado.\n");
        return -1;
    }
    
    usuarios[numUsuarios].jaVotou = 0;
    printf("Usuário cadastrado com sucesso!\n");
    return numUsuarios++;
}

int main() {
    Projeto projetos[MAX_PROJETOS];
    Usuario usuarios[MAX_USUARIOS];
    int numProjetos = 0;
    int numUsuarios = 0;
    int opcao;
    
    // Carrega os projetos do arquivo
    numProjetos = carregarProjetos(projetos);
    
    // Se não há projetos carregados, popula com alguns exemplos
    if (numProjetos == 0) {
        const char *projetosExemplo[] = {
            "Sistema de Gestão", "Aplicativo Mobile", 
            "Website Institucional", "Banco de Dados"
        };
        
        for (int i = 0; i < 4; i++) {
            strcpy(projetos[i].nome, projetosExemplo[i]);
            projetos[i].votos = 0;
        }
        numProjetos = 4;
        printf("Lista de projetos de exemplo criada.\n");
    }
    
    do {
        printf("\n===== SISTEMA DE VOTAÇÃO DE PROJETOS =====\n");
        printf("1. Listar projetos por número de votos\n");
        printf("2. Listar projetos por nome\n");
        printf("3. Verificar Votação\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Consome o caractere de nova linha
        
        switch(opcao) {
            case 1:
                listarProjetos(projetos, numProjetos, 1); // 1 = ordenar por votos
                break;
                
            case 2:
                listarProjetos(projetos, numProjetos, 0); // 0 = ordenar por nome
                break;
                
            case 3: {
                char matricula[TAM_MATRICULA];
                
                printf("\n===== VERIFICAÇÃO DE VOTAÇÃO =====\n");
                printf("Número de matrícula (7 dígitos): ");
                scanf("%s", matricula);
                getchar(); // Consome o caractere de nova linha
                
                if (!matriculaValida(matricula)) {
                    break;
                }
                
                // Busca usuário pelo número de matrícula
                int usuarioIndice = buscarUsuario(usuarios, numUsuarios, matricula);
                
                // Se o usuário não existe, cadastra novo
                if (usuarioIndice == -1) {
                    usuarioIndice = criarNovoUsuario(usuarios, numUsuarios, projetos, 
                                                    &numProjetos, matricula);
                    if (usuarioIndice == -1) {
                        break;
                    }
                    numUsuarios++;
                }
                
                // Menu do usuário logado
                int opcaoUsuario;
                do {
                    printf("\n===== MENU DO USUÁRIO: %s =====\n", usuarios[usuarioIndice].nome);
                    printf("1. Votar\n");
                    printf("2. Alterar Voto\n");
                    printf("3. Listar Projetos por nome\n");
                    printf("0. Logout\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcaoUsuario);
                    getchar(); // Consome o caractere de nova linha
                    
                    switch(opcaoUsuario) {
                        case 1:
                            processarVoto(projetos, numProjetos, &usuarios[usuarioIndice], 0);
                            break;
                            
                        case 2:
                            processarVoto(projetos, numProjetos, &usuarios[usuarioIndice], 1);
                            break;
                            
                        case 3:
                            listarProjetos(projetos, numProjetos, 0);
                            break;
                            
                        case 0:
                            printf("Logout realizado com sucesso.\n");
                            break;
                            
                        default:
                            printf("Opção inválida!\n");
                    }
                    
                } while (opcaoUsuario != 0);
                break;
            }
            
            case 0:
                salvarProjetos(projetos, numProjetos);
                printf("Encerrando o programa...\n");
                break;
                
            default:
                printf("Opção inválida!\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}