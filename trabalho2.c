#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#define TAM 10


/* Dado um mapa e o ponto de onde o Jack Sparrow começa sua busca, faça um programa que o ajude a
determinar a menor quantidade de tempo necessário para que ele encontre todos os tesouros.

A entrada é constituída de vários casos de testes que são lidos em um arquivo cujo nome será informado
por Jack Sparrow. A primeira linha do caso de teste contém dois números inteiros a e l que indicam a largura
e a altura do mapa respectivamente em quilômetros. Para simplificar, cada ponto do mapa representa um
quilômetro quadrado. As próximas a linhas contêm l caracteres, cada um descrevendo um quadrado no
mapa. Cada ponto do mapa é um dos seguintes pontos:
• @ O ponto onde Jack Sparrow começa a sua busca.
• ~ Água. Os tesouros estão dentro de uma ilha e Jack Sparrow não consegue atravessar água dentro da
ilha.
• # Grupos rochosos e de despenhadeiros que Jack Sparrow não consegue atravessar.
• . Terra firme, que pode ser facilmente atravessada.
• * Local onde habiltam canibais. O Jack Sparrow deve ficar a pelo menos um quadrado de distância dos
canibais para não correr o risco de ser capturado por eles. Note que ficar a um quadrado de distância
inclui as diagonais.
• ! Indica um tesouro. Jack Sparrow só sairá da ilha após pegar todos os tesouros escondidos.*/

struct coordenadas{
    int cima_baixo;
    int lados;
};
struct coordenadas ancoragem_busca (char mapa_tesouro[TAM][TAM], int cima_baixo, int lados){
    struct coordenadas c;

    c.cima_baixo = TAM + 1;
    c.lados = TAM + 1;

    if (mapa_tesouro[cima_baixo + 1][lados] == '@')
    {
        c.cima_baixo = cima_baixo + 1;
        c.lados = lados;
        mapa_tesouro[cima_baixo][lados] = '1';

    } else if (mapa_tesouro[cima_baixo - 1][lados] == '@') {
        c.cima_baixo = cima_baixo - 1;
        c.lados = lados;
        mapa_tesouro[cima_baixo][lados] = '1';

    }else if (mapa_tesouro[cima_baixo][lados + 1] == '@')
    {
        c.cima_baixo = cima_baixo;
        c.lados = lados + 1;
        mapa_tesouro[cima_baixo][lados] = '1';

    }else if (mapa_tesouro[cima_baixo][lados - 1] == '@')
    {
        c.cima_baixo = cima_baixo;
        c.lados = lados - 1;
        mapa_tesouro[cima_baixo][lados] = '1';
    }
    return c;
}
struct coordenadas localizacao_bau (char mapa_tesouro[TAM][TAM], int cima_baixo, int lados){
    struct coordenadas b;

    b.cima_baixo = TAM + 1;
    b.lados = TAM + 1;

    if (mapa_tesouro[cima_baixo + 1][lados] == '!')
    {
        b.cima_baixo = cima_baixo + 1;
        b.lados = lados;
        mapa_tesouro[cima_baixo][lados] = '.';

    }else if (mapa_tesouro[cima_baixo - 1][lados] == '!')
    {
        b.cima_baixo = cima_baixo - 1;
        b.lados = lados;
        mapa_tesouro[cima_baixo][lados] = '.';

    }else if (mapa_tesouro[cima_baixo][lados - 1] == '!')
    {
        b.cima_baixo = cima_baixo;
        b.lados = lados - 1;
        mapa_tesouro[cima_baixo][lados] = '.';

    }else if (mapa_tesouro[cima_baixo][lados + 1] == '!')
    {
        b.cima_baixo = cima_baixo;
        b.lados = lados + 1;
        mapa_tesouro[cima_baixo][lados] = '.';

    }
    return b;
}
struct coordenadas analisa_volta (char mapa_tesouro[TAM][TAM], int cima_baixo, int lados, struct coordenadas vetor_posicao[], int cont){
    struct coordenadas a = vetor_posicao[cont];

    if (mapa_tesouro[cima_baixo][lados] == '1' && mapa_tesouro[cima_baixo - 1][lados] == '0' || mapa_tesouro[cima_baixo - 1][lados] == '1'){
        if (mapa_tesouro[cima_baixo + 1][lados] == '0' || mapa_tesouro[cima_baixo + 1][lados] == '1'){
            if(mapa_tesouro[cima_baixo][lados - 1] == '0' || mapa_tesouro[cima_baixo][lados - 1] == '1'){
                if(mapa_tesouro[cima_baixo][lados + 1] == '0' || mapa_tesouro[cima_baixo][lados + 1] == '1'){
                    mapa_tesouro[cima_baixo][lados] = '0';
                    a = vetor_posicao[cont--];
                }
            }
        }
    }

    return a;
}


int main(int argc, char *argv[]) {
    
    SetConsoleOutputCP(65001);

    FILE *arq;
    struct coordenadas analise;
    struct coordenadas local_bau;
    struct coordenadas retorno_origem;
    struct coordenadas ancoragem;
    struct coordenadas caminho[1000];
    struct coordenadas bau;
    char nome[20];
    int i, j, qtdbau=0, horas=0, pos=0, largura=0, altura=0;
    char mapa_tesouro[TAM][TAM];

    printf("\nInforme o nome do arquivo que contém o mapa do tesouro: ");
    fgets(nome, 30, stdin);
    nome[strlen(nome)-1] = '\0';
    

    arq = fopen(nome, "r");

    if (arq == NULL){
        printf("\nArquivo inválido.\n");
        return 0;
    }

    do {

        fscanf (arq,"%d%d\n", &largura, &altura);

        if (largura == 0 || altura == 0 ){
            printf("Não há mais mapas.");
            exit(-1);
        }

        for (i = 0; i < largura; i++){
            fgets (mapa_tesouro[i], largura + 2, arq);
            pos = strlen(mapa_tesouro[i]) - 1;
            mapa_tesouro[i][pos] = '\0';
        }

        for (i = 0; i < TAM; i++){
            for (j = 0; j < TAM; j++){
                if (mapa_tesouro[i][j] == '@'){
                    ancoragem.cima_baixo = i;
                    ancoragem.lados = j;
                }
            }
        }

        for (i = 0; i < TAM; i++){
            for (j = 0; j < TAM; j++){
                if (mapa_tesouro[i][j] == '*'){
                    mapa_tesouro[i][j] = '0';

                    if (mapa_tesouro[i+1][j] != '*' && mapa_tesouro[i+1][j] == '.'){
                        mapa_tesouro[i+1][j] = '0';
                    }
                    if (mapa_tesouro[i-1][j] != '*' && mapa_tesouro[i-1][j] == '.'){
                        mapa_tesouro[i-1][j] = '0';
                    }

                    if (mapa_tesouro[i][j+1] != '*' && mapa_tesouro[i][j+1] == '.'){
                        mapa_tesouro[i][j+1] = '0';
                    }
                    if (mapa_tesouro[i][j-1] != '*' && mapa_tesouro[i][j-1] == '.'){
                        mapa_tesouro[i][j-1] = '0';
                    }

                    if (mapa_tesouro[i+1][j+1] != '*' && mapa_tesouro[i+1][j+1] == '.'){
                        mapa_tesouro[i+1][j+1] = '0';
                    }

                    if (mapa_tesouro[i+1][j-1] != '*' && mapa_tesouro[i+1][j-1] == '.'){
                        mapa_tesouro[i+1][j-1] = '0';
                    }
                    
                    if (mapa_tesouro[i-1][j+1] != '*' && mapa_tesouro[i-1][j+1] == '.'){
                        mapa_tesouro[i-1][j+1] = '0';
                    }

                    if (mapa_tesouro[i-1][j-1] != '*' && mapa_tesouro[i-1][j-1] == '.'){
                        mapa_tesouro[i-1][j-1] = '0';
                    }
                }
                if (mapa_tesouro[i][j] == '#'){
                    mapa_tesouro[i][j] = '0';
                } 
                if (mapa_tesouro[i][j] == '~'){
                    mapa_tesouro[i][j] = '0';
                }
            }
        }
        
        for (i = 0; i < TAM; i++){
            for (j = 0; j < TAM; j++){
                if (mapa_tesouro[i][j] == '!'){
                    bau.cima_baixo = i;
                    bau.lados = j;
                    if(mapa_tesouro [bau.cima_baixo + 1][bau.lados] == '0' && (mapa_tesouro [bau.cima_baixo - 1][bau.lados]) == '0'
                    && (mapa_tesouro [bau.cima_baixo][bau.lados + 1]) == '0' && (mapa_tesouro [bau.cima_baixo][bau.lados - 1]) == '0' || (mapa_tesouro[bau.cima_baixo][bau.lados] == '0')){
                        printf("-1");
                        exit(1);
                    }else{
                        qtdbau++;
                    }
                }
            }  
        }
        printf("============================ INFORMAÇÕES DA BUSCA DE JACK SPARROW =========================\n\n");
        printf("a) Jack Sparrow ancorou na posição X: %d || Y: %d\n", ancoragem.cima_baixo, ancoragem.lados);
        printf("b) Há %d báus encontraveis no mapa!\n", qtdbau);
        
        do {
            caminho[horas] = ancoragem;
            
            if (mapa_tesouro[ancoragem.cima_baixo + 1][ancoragem.lados] == '.'){
                ancoragem.cima_baixo += 1;  
                mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados] = '1';
                caminho[horas] = ancoragem;
                horas++;
            }
            else if (mapa_tesouro[ancoragem.cima_baixo - 1][ancoragem.lados] == '.'){
                ancoragem.cima_baixo -= 1;
                mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados] = '1';
                caminho[horas] = ancoragem;
                horas++;
            }
            else if (mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados + 1] == '.'){
                ancoragem.lados += 1;
                mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados] = '1';
                caminho[horas] = ancoragem;
                horas++;
            }
            else if (mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados - 1] == '.'){ 
                ancoragem.lados -= 1;
                mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados] = '1';
                caminho[horas] = ancoragem;
                horas++;
            } 

            analise = analisa_volta(mapa_tesouro, ancoragem.cima_baixo, ancoragem.lados, caminho, horas);

            if(analise.cima_baixo == ancoragem.cima_baixo && analise.lados == ancoragem.lados){
                horas--;
                mapa_tesouro[caminho[horas-1].cima_baixo][caminho[horas-1].lados] = '.';
            }

            local_bau = localizacao_bau(mapa_tesouro, ancoragem.cima_baixo, ancoragem.lados);

            if (mapa_tesouro[local_bau.cima_baixo][local_bau.lados]=='!'){
                ancoragem.cima_baixo = local_bau.cima_baixo;
                ancoragem.lados = local_bau.lados;
                mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados]='.';
                horas++;
                qtdbau--;
            }

        } while(qtdbau > 0);

        while(mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados] != '@') {
            

            if (mapa_tesouro[ancoragem.cima_baixo + 1][ancoragem.lados] == '1' || mapa_tesouro[ancoragem.cima_baixo + 1][ancoragem.lados] == '.'){
                ancoragem.cima_baixo+=1;
                mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados] = '0';
                horas++;
            }
            else if (mapa_tesouro[ancoragem.cima_baixo - 1][ancoragem.lados] == '1' || mapa_tesouro[ancoragem.cima_baixo - 1][ancoragem.lados] == '.'){
                ancoragem.cima_baixo-=1;
                mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados] = '0';
                horas++;
            }
            else if (mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados + 1] == '1' || mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados + 1] == '.'){
                ancoragem.lados+=1;
                mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados] = '0';
                horas++;
            }
            else if (mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados - 1] == '1' || mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados - 1] == '.'){
                ancoragem.lados-=1;
                mapa_tesouro[ancoragem.cima_baixo][ancoragem.lados] = '0';
                horas++;
            }  

            retorno_origem=ancoragem_busca(mapa_tesouro, ancoragem.cima_baixo, ancoragem.lados);
        
            if (mapa_tesouro[retorno_origem.cima_baixo][retorno_origem.lados] == '@'){
                horas++;
                ancoragem.cima_baixo = retorno_origem.cima_baixo;
                ancoragem.lados = retorno_origem.lados;
                printf("c) Jack Sparrow precisou de %d horas para encontrar todos os tesouros da ilha!", horas);
                printf("\n\n===========================================================================================\n\n");
                horas=0;
            }
        }
    } while(TAM > 0); 
        fclose(arq);


    return 0;
}
