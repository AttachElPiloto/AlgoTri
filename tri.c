// Compilation du programme sous
// - Linux:
// cc tri.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o Tri
// - MacOS:
// cc tri.c `pkg-config --libs --cflags raylib` -Wall -pthread -o Tri
//
// Exécution: ./Tri

#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>
#include <assert.h>

#define NB_POINTS 400
double lecture=0.001;
double ecriture=0.001;

int separer(int * t,int x,int p,int q){
  int buff = t[x];
  t[x]=t[p];
  t[p]=buff;
  int i=p+1;
  int h=q;
  while (i<=h)
    {
      if (t[i]<=t[p])
        {i++;}
      else 
        {int temp=t[h];t[h]=t[i];t[i]=temp;h--;WaitTime(5*lecture+5*ecriture);}

    }
  int buffer=t[h];
  t[h]=t[x];
  t[x]=buffer;
return h;

}

void tri_rapide_rec(int * t,int p, int q)
{
if (p<q)
{
  int pivot=p;
  int h=separer(t,pivot,p,q);
  tri_rapide_rec(t,p,h-1);
  tri_rapide_rec(t,h+1,q);
}
}

void *tri_rapide(void *arg) {
  int * t = (int *) arg;
  tri_rapide_rec(t,0,NB_POINTS);
  pthread_exit(NULL);
} 

void *tri_selection(void *arg) {
  int * t = (int *) arg;
  int min = INT_MAX;
  int indice=-1;
  for (int i = 0; i < NB_POINTS;i++){
    for (int j = i;j< NB_POINTS;j++){
    if (t[j]<min)
      {min=t[j];indice=j;WaitTime(5*lecture);}
    }
    t[indice]=t[i];
    t[i]=min;
    WaitTime(5*lecture+5*ecriture);
    min=INT_MAX;indice=-1;
  }
  pthread_exit(NULL);
}

void *tri_bulle(void *arg) {
  int * t = (int *) arg;
  for (int i = 0; i < NB_POINTS;i++){
    for (int j = 0; j < NB_POINTS-1-i;j++){
      if (t[j]>t[j+1])
        {int buffer=t[j];t[j]=t[j+1];t[j+1]=buffer;WaitTime(lecture/5+ecriture/5);}
    }
  }
  pthread_exit(NULL);
}

void *tri_insertion(void *arg) {
  int * t = (int *) arg;
  int j = 0;
  for (int i = 1; i < NB_POINTS;i++){
    int x=t[i];
    j=i;
    while (j>0 && t[j-1]>x)
      {t[j]=t[j-1];j--;WaitTime(lecture/2+ecriture/2);}
    t[j]=x;
  }
  pthread_exit(NULL);
}


int main(void)
{
  

    InitWindow(NB_POINTS * 4, 450, "Tris");

    int t1[NB_POINTS];
    int t2[NB_POINTS];
    int t3[NB_POINTS];
    int t4[NB_POINTS];

    // Initialisation des tableaux avec GetRandomValue(inf, sup) (inclus)
    for (int i = 0; i < NB_POINTS; i++)
      {
      int z=GetRandomValue(0,450);
      t1[i]=z;
      t2[i]=z;
      t3[i]=z;
      t4[i]=z;
      }
    // Initialisation des fils: un tableau par tri
    pthread_t tab[4];
    pthread_create(&tab[0],NULL,tri_bulle,t1);
    pthread_create(&tab[1],NULL,tri_insertion,t2);
    pthread_create(&tab[2],NULL,tri_selection,t3);
    pthread_create(&tab[3],NULL,tri_rapide,t4);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (unsigned int i = 0; i < NB_POINTS; i++) {
              DrawLine(i, 450, i, 450-t1[i], RED);
              DrawCircle(i, 450-t1[i], 3, BLACK);
              DrawLine(i + NB_POINTS, 450, i + NB_POINTS, 450-t2[i], BLUE);
              DrawCircle(i + NB_POINTS, 450-t2[i], 3, BLACK);
              DrawLine(i + 2*NB_POINTS, 450, i + 2*NB_POINTS, 450-t3[i], RED);
              DrawCircle(i + 2*NB_POINTS, 450-t3[i], 3, BLACK);
              DrawLine(i + 3*NB_POINTS, 450, i + 3*NB_POINTS, 450-t4[i], BLUE);
              DrawCircle(i + 3*NB_POINTS, 450-t4[i], 3, BLACK);
            }
            WaitTime(0.05);
        EndDrawing();
    }

    CloseWindow();
    ecriture=0;
    lecture=0;
    // Récupérer les fils
    pthread_join(tab[0],NULL);
    pthread_join(tab[1],NULL);
    pthread_join(tab[2],NULL);
    pthread_join(tab[3],NULL);

    return 0;
}