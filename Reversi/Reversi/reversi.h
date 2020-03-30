#ifndef REVERSI_H_INCLUDED
#define REVERSI_H_INCLUDED
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fifo.h"


typedef struct button {
    GtkWidget *btn;
    int rodzaj;            //0-bialy 1-czarny 2-nic
    char numer[4];          //numer pola
    GtkImage *obrazek;
    int isblocked;
} Buttons;

char   *myid,*yid;
char    id;
int     czyjruch; // 0 - bialy | 1 - czarny
int     numer_tury;
int     wielkosc_planszy;

GtkWidget   *window;
GtkWidget   *glowne;
GtkWidget   *menu;
GtkWidget   *all_box;
GtkWidget   *buffor;
GtkWidget   *newGame;
PipesPtr     potoki;
Buttons     *przyciski[18][18];
GtkWidget   *wait_dialog;
GtkWidget   *historia_label;
GtkWidget   *wczytaj_dialog;
GtkWidget   *pola;
GtkWidget   *wygrana_label;
GtkWidget   *finish_dialog;
GtkWidget   *wczytajGre;

int wczyta_czy_pusty();

void wait();

void nowy_dialog_start_po_koncu();

void rozp_wczytywanie(GtkWidget *widget,gpointer data);

void wczytaj();

void rozp_nowa_gre_menu(GtkWidget *widget,gpointer data);

void zapis(GtkWidget *btn_clicked,GdkEventButton *event,char *data);

int poza_plansza(int x, int y);

int odleglosc(int xp, int yp, int x, int y, int i, int j);

int poprawnosc(char *numer);

int aktualizuj_plansze(char *numer, int czyjruch);

int podswietl();

void clicked(GtkWidget *btn_clicked,GdkEventButton *event,char *data);

void wyslij_sygnal(char *data);

int odbierz_sygnal();

void przekaz_tekst( GtkWidget *widget,GtkWidget *text);

void pobierz_tekst(char *data);

void rozp_nowa_gre(GtkWidget *widget,gpointer data);

void nowa_gra();

void quit(gpointer data);

void dialog_koniec();

void nowy_dialog_start(gpointer data);

void dialog_finish();



#endif // REVERSI_H_INCLUDED

