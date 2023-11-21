#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game_aux.h"

#define GAME_SIZE (DEFAULT_SIZE*DEFAULT_SIZE)

void safeMalloc(game g);
void checkIJ(uint i, uint j, char* text);
void gameNull(game g);
void gameConstNull(cgame g);

struct game_s{
    color *couleur;
    constraint *contrainte;
    status *statut;

};


void game_print(cgame g) {
    gameConstNull(g);
    //affichage de la barre du haut et des nombres du haut
    printf("   ");
    for(int i =0; i<DEFAULT_SIZE;i++){
        printf("%d",i);
        printf(" ");
    }
    printf("\n");
    printf("   ");
    for(int i =0; i<DEFAULT_SIZE;i++){
        printf("--");

    }
    printf("\n");
    
    //affichage des nombres
    for(int j=0; j<DEFAULT_SIZE; j++){
        printf("%d |", j);
        for(int i=0;i<DEFAULT_SIZE; i++){
            
            switch(g->couleur[i+5*j]){
                case EMPTY: //pour EMPTY
                    if(g->contrainte[i+5*j]==UNCONSTRAINED){
                        printf(" ");
                    }
                    else for(int k=0; k<10; k++){
                        if(g->contrainte[i+5*j]==k){
                            printf("%d", k);
                        }
                    }
                    printf(" ");
                    break;

                case WHITE: //pour WHITE
                    if(g->contrainte[i+5*j]==UNCONSTRAINED){
                            printf("□ ");
                    }

                    if(g->contrainte[i+5*j]==0){
                        printf("🄋 ");
                    }
                    if(g->contrainte[i+5*j]==1){
                        printf("➀ "); 
                    }
                    if(g->contrainte[i+5*j]==2){
                        printf("➁ ");
                    }
                    if(g->contrainte[i+5*j]==3){
                        printf("➂ "); 
                    }
                    if(g->contrainte[i+5*j]==4){
                        printf("➃ ");
                    }
                    if(g->contrainte[i+5*j]==5){
                        printf("➄ "); 
                    }
                    if(g->contrainte[i+5*j]==6){
                        printf("➅ ");
                    }
                    if(g->contrainte[i+5*j]==7){
                        printf("➆ "); 
                    }
                    if(g->contrainte[i+5*j]==8){
                        printf("➇ ");
                    }
                    if(g->contrainte[i+5*j]==9){
                        printf("➈ "); 
                    }
                    
                    break;
                case BLACK: //pour BLACK
                    if(g->contrainte[i+5*j]==UNCONSTRAINED){
                        printf("■ ");
                    }
                    if(g->contrainte[i+5*j]==0){
                        printf("🄌 ");
                    }
                    if(g->contrainte[i+5*j]==1){
                        printf("➊ "); 
                    }
                    if(g->contrainte[i+5*j]==2){
                        printf("➋ ");
                    }
                    if(g->contrainte[i+5*j]==3){
                        printf("➌ "); 
                    }
                    if(g->contrainte[i+5*j]==4){
                        printf("➍ ");
                    }
                    if(g->contrainte[i+5*j]==5){
                        printf("➎ "); 
                    }
                    if(g->contrainte[i+5*j]==6){
                        printf("➏ ");
                    }
                    if(g->contrainte[i+5*j]==7){
                        printf("➐ "); 
                    }
                    if(g->contrainte[i+5*j]==8){
                        printf("➑ ");
                    }
                    if(g->contrainte[i+5*j]==9){
                        printf("➒ "); 
                    }
                        
                    break;
            }
            
        }
        printf("|\n");
    }
    //affichage de la barre du bas
    printf("   ");
    for(int i =0; i<DEFAULT_SIZE;i++){
        printf("--");
    }
    printf("\n");
}


game game_default(void) {

    //création de game et vérification de sa création
    game g=malloc(sizeof(game));
    safeMalloc(g); 

    //mise en place des conditions de game_default
    for(int i=0; i<DEFAULT_SIZE*DEFAULT_SIZE; i++){   
        g->statut[i]=UNSATISFIED;
        g->couleur[i]=EMPTY;
        if(i==0){
            g->contrainte[i]=0;
        }
        else if(i==3){
            g->contrainte[i]=3;
        }
        else if(i==6){
            g->contrainte[i]=5;
        }
        else if(i==12){
            g->contrainte[i]=4;
        }
        else if(i==14){
            g->contrainte[i]=1;
        }
        else if(i==15){
            g->contrainte[i]=6;
        }
        else if(i==17){
            g->contrainte[i]=6;
        }
        else if(i==18){
            g->contrainte[i]=3;
        }
        else{
            g->contrainte[i]=-1;
            
        }
    }
    return g;
}


game game_default_solution(void) {
    game g=game_default();
    for(int j=0;j<DEFAULT_SIZE;j++){
        for(int i=0;i<DEFAULT_SIZE;i++){
            g->statut[i+5*j]=SATISFIED;
            if(j>1 && i>-1 && i<2){
                g->couleur[i+5*j]=BLACK;
            }
            else if(j!=3 && i==2){
                g->couleur[i+5*j]=BLACK;
            }
            else  if(j==4 && i==3){
                g->couleur[i+5*j]=BLACK;
            }
            else if(j==1 && i==4){
                g->couleur[i+5*j]=BLACK;
            }
            else{
                g->couleur[i+5*j]=WHITE;    
            }
        }
    }
    game_print(g);
    return g;

}

