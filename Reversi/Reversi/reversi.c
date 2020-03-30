#include "reversi.h"
#include "fifo.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAKS_DL_TEKSTU 10000


void nowy_dialog_wczytaj()
{
    GtkWidget *dialog_wczytaj=gtk_dialog_new_with_buttons("Wczytaj Gre",
                              GTK_WINDOW(window),
                              GTK_DIALOG_MODAL,"TAK",
                              GTK_RESPONSE_YES,
                              "NIE",
                              GTK_RESPONSE_NO,NULL,NULL);

    gtk_window_set_deletable(GTK_WINDOW(dialog_wczytaj),FALSE);
    gtk_window_set_resizable(GTK_WINDOW(dialog_wczytaj),FALSE);

    GtkWidget *label=gtk_label_new("Czy chcesz wczytac gre?");
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog_wczytaj))),label,0,0,0);
    gtk_widget_show_all(dialog_wczytaj);
    gint response=gtk_dialog_run(GTK_DIALOG(dialog_wczytaj));
    if(response==GTK_RESPONSE_NO)
    {
        char tmp[4]="^NGN";
        wyslij_sygnal(tmp);
        gtk_widget_destroy(dialog_wczytaj);
    }
    if(response==GTK_RESPONSE_YES)
    {
        char tmp[5]=")@NGT";
        wyslij_sygnal(tmp);
        gtk_widget_destroy(dialog_wczytaj);
        wczytaj();
    }
}


void rozp_wczytywanie(GtkWidget *widget,gpointer data)
{
        g_timeout_add(100,odbierz_sygnal,NULL);

        GtkWidget *spin=gtk_spinner_new();
        wczytaj_dialog=gtk_dialog_new_with_buttons("Musisz poczekać.",
                       GTK_WINDOW(window),
                       GTK_DIALOG_MODAL,NULL,NULL);

        GtkWidget *label=gtk_label_new("Czekaj na odpowiedź przeciwnika.");

        gtk_window_set_deletable(GTK_WINDOW(wczytaj_dialog),FALSE);
        gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(wczytaj_dialog))),label,0,0,0);
        gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(wczytaj_dialog))),spin,0,0,0);
        gtk_spinner_start(GTK_SPINNER(spin));
        gtk_widget_show_all(wczytaj_dialog);

        char tmp[4]=")NG";
        wyslij_sygnal(tmp);

}


void wczytaj()
{

    GtkWidget *nowe = gtk_container_get_focus_child (GTK_CONTAINER (window));
    gtk_container_remove(GTK_CONTAINER(window),nowe);
    gtk_container_add(GTK_CONTAINER(window),glowne);
    gtk_widget_show_all(window);

    FILE *fp,*fp2;
    if ((fp=fopen("plansza.txt", "r"))==NULL || (fp2=fopen("historia.txt", "r"))==NULL)
    {
        printf ("Nie udało się otworzyć pliku do odczytania!\n");
        exit(1);
    }
    fscanf(fp2,"%d",&czyjruch);
    fscanf(fp2,"%d",&numer_tury);
    fscanf(fp2,"%d",&wielkosc_planszy);

    for(int i=wielkosc_planszy; i>=1; i--)
    {
        for(int j=1; j<=wielkosc_planszy; j++)
        {
            przyciski[j][i]=malloc(sizeof(Buttons));
            przyciski[j][i]->rodzaj=2;
            sprintf(przyciski[j][i]->numer,"%c%c",'a'+j-1,'a'+i-1);

            przyciski[j][i]->obrazek=(GtkImage *)gtk_image_new_from_file("pusty.png");
            przyciski[j][i]->btn=gtk_event_box_new();

            gtk_container_add(GTK_CONTAINER(przyciski[j][i]->btn),GTK_WIDGET(przyciski[j][i]->obrazek));
            gtk_grid_attach(GTK_GRID(pola),przyciski[j][i]->btn, j-1,wielkosc_planszy-i,1,1);
        }
    }
    for(int i=0; i<wielkosc_planszy+2; i++)
        przyciski[i][0]=przyciski[0][i]=przyciski[wielkosc_planszy+1][i]=przyciski[i][wielkosc_planszy+1]=NULL;

    for(int i=0; i<wielkosc_planszy; i++)
    {
        char a[3];
        sprintf(a,"%d",wielkosc_planszy-i);
        char c[3];
        sprintf(c,"%c",'a'+i);
        gtk_grid_attach(GTK_GRID(pola),gtk_label_new(a),wielkosc_planszy,i,1,1);
        gtk_grid_attach(GTK_GRID(pola),gtk_label_new(c),i,wielkosc_planszy,1,1);
    }

    for(int i=1; i<=wielkosc_planszy; i++)
        for(int j=1; j<=wielkosc_planszy; j++)
        {
            przyciski[i][j]->isblocked=1;
            g_signal_connect(G_OBJECT(przyciski[i][j]->btn),"button_press_event",G_CALLBACK(clicked),przyciski[i][j]->numer);
        }
    gtk_widget_show_all(window);

    for(int i=1; i<=wielkosc_planszy; i++)
        for(int j=1; j<=wielkosc_planszy; j++)
        {
            fscanf(fp,"%c",&przyciski[i][j]->numer[0]);
            fscanf(fp,"%c",&przyciski[i][j]->numer[1]);
            fscanf(fp,"%d",&przyciski[i][j]->rodzaj);
            if(przyciski[i][j]->rodzaj==2)  gtk_image_set_from_file(przyciski[i][j]->obrazek,"pusty.png");
            else if(przyciski[i][j]->rodzaj==1) gtk_image_set_from_file(przyciski[i][j]->obrazek,"czarny.png");
            else gtk_image_set_from_file(przyciski[i][j]->obrazek,"bialy.png");
        }

    char c;
    int i=0;
    char historia[100000];
    fscanf(fp2,"%c",&c);
    while(c!='&')
    {
        historia[i]=c;
        fscanf(fp2,"%c",&c);
        i++;
    }

    gtk_label_set_text(GTK_LABEL(historia_label),historia);
    podswietl();

    fclose (fp);
    fclose (fp2);
}


void zapis(GtkWidget *btn_clicked,GdkEventButton *event,char *data)
{
    gtk_widget_set_sensitive(GTK_WIDGET(wczytajGre),TRUE);
    FILE *fp,*fp2;
    if ((fp=fopen("plansza.txt", "w"))==NULL || (fp2=fopen("historia.txt", "w"))==NULL)
    {
        printf ("Nie udało się otworzyć pliku do zapisu!\n");
        exit(1);
    }
    for(int i=1; i<=wielkosc_planszy; i++)
        for(int j=1; j<=wielkosc_planszy; j++)
        {
            fprintf(fp,"%s%d",przyciski[i][j]->numer,przyciski[i][j]->rodzaj);
        }
    const gchar *histora = gtk_label_get_text (GTK_LABEL(historia_label));
    fprintf(fp2,"%d ",czyjruch);
    fprintf(fp2,"%d ",numer_tury);
    fprintf(fp2,"%d",wielkosc_planszy);
    fprintf(fp2,"\n%s&",histora);
    fclose (fp); /* zamknij plik */
    fclose (fp2);



}


int poza_plansza(int x, int y)
{
    if(x>=1 && y<=wielkosc_planszy && x<=wielkosc_planszy && y>=1) return 1;
    return 0;
}


int odleglosc(int xp, int yp, int x, int y, int i, int j)
{
    //sprawdzenie czy odleglosc w kazda strone jest wieksza niz 1
    if(i > 0 && abs(xp-x)>1)     return 1;
    if(i < 0 && abs(xp-x)>1)     return 1;
    if(i == 0)
    {
        if(j > 0 && abs(yp-y)>1) return 1;
        if(j < 0 && abs(yp-y)>1) return 1;
    }
    return 0;
}


int poprawnosc(char *numer)
{
    //wydobycie wspolrzednych sprawdzanego pola
    int x=(int)numer[0] - 96;
    int y=(int)numer[1] - 96;

    //pomocnicza zmienna na okreslenie ruchu przeciwnika
    int tura_przeciwnika;
    if(id=='A' && czyjruch%2==0)
        tura_przeciwnika=1;
    else if(id=='B' && czyjruch%2==1)
        tura_przeciwnika=0;

    int xpom,ypom;

    //sprawdzenie sasiednich pol w poszukiwaniu lancucha warcabow przeciwnika
    for(int i=-1; i<=1; i++)
    {
        for(int j=-1; j<=1; j++)
        {
            if(i != 0 || j != 0)
            {
                xpom = x + i;
                ypom = y + j;

                //dopoki lancuch nie prowadzi poza plansze i jest zlozony z warcabow przeciwnika -> podazaj tym lancuchem
                while(poza_plansza(xpom,ypom)==1 && przyciski[xpom][ypom]->rodzaj==tura_przeciwnika)
                {
                    xpom = xpom + i;
                    ypom = ypom + j;

                }
                //jesli znaleziony warcab znajduje sie na planszy, jest twoim warcabem i jest w odpowiedniej linii, jest to poprawne pole do ruchu
                if(odleglosc(xpom,ypom,x,y,i,j) && poza_plansza(xpom,ypom)==1 && (przyciski[xpom][ypom]->rodzaj==czyjruch%2))
                    return 1;
            }
        }
    }
    return 0;

}


int aktualizuj_plansze(char *numer, int czyjruch)
{
    int x,y;

    //wydobycie wspolrzednych sprawdzanego pola
    if(numer[0]!='&')
    {
        x=(int)numer[0] - 96;
        y=(int)numer[1] - 96;
    }
    else
    {
        x=(int)numer[1] - 96;
        y=(int)numer[2] - 96;
    }

    int tura_przeciwnika;
    if(czyjruch%2==0)
        tura_przeciwnika=1;
    else
        tura_przeciwnika=0;

    if(czyjruch % 2 == 1)
        numer_tury++;

    przyciski[x][y]->rodzaj=czyjruch%2;
    if(czyjruch%2==0) gtk_image_set_from_file(przyciski[x][y]->obrazek,"bialy.png");
    else gtk_image_set_from_file(przyciski[x][y]->obrazek,"czarny.png");

    int xpom,ypom,xpom2,ypom2;

    for(int i=-1; i<=1; i++)
    {
        for(int j=-1; j<=1; j++)
        {
            if(i != 0 || j != 0)
            {
                xpom = x + i;
                ypom = y + j;

                //dopoki lancuch nie prowadzi poza plansze i jest zlozony z warcabow przeciwnika -> podazaj tym lancuchem
                while(poza_plansza(xpom,ypom)==1 && przyciski[xpom][ypom]->rodzaj==tura_przeciwnika)
                {
                    xpom = xpom + i;
                    ypom = ypom + j;
                }
                //jesli znaleziony warcab znajduje sie na planszy, jest twoim warcabem i jest w odpowiedniej linii, jest to poprawne pole do ruchu
                if(poza_plansza(xpom,ypom)==1 && (przyciski[xpom][ypom]->rodzaj==czyjruch%2) && odleglosc(xpom,ypom,x,y,i,j))
                {
                    xpom2 = x + i;
                    ypom2 = y + j;

                    while(poza_plansza(xpom2,ypom2)==1 && przyciski[xpom2][ypom2]->rodzaj==tura_przeciwnika)
                    {
                        przyciski[xpom2][ypom2]->rodzaj=czyjruch%2;
                        if(czyjruch%2==0) gtk_image_set_from_file(przyciski[xpom2][ypom2]->obrazek,"bialy.png");
                        else gtk_image_set_from_file(przyciski[xpom2][ypom2]->obrazek,"czarny.png");
                        xpom2 = xpom2 + i;
                        ypom2 = ypom2 + j;
                    }
                }
            }
        }
    }


}


int podswietl()                                 //funkcja sprawdza dla pustych pol na planszy, czy mozliwe jest wykonanie na nie ruchu
{
    //ilosc odblokowanych pol
    int czyodblokowano = 0;

    //odswiezenie pol, zaktualizowanie podswietlonych
    for(int i=1; i<=wielkosc_planszy; i++)
        for(int j=1; j<=wielkosc_planszy; j++)
        {
            przyciski[i][j]->isblocked=1;
            if(przyciski[i][j]->rodzaj==2)
            {
                gtk_image_set_from_file(przyciski[i][j]->obrazek,"pusty.png");
            }
        }
    for(int i=1; i<=wielkosc_planszy; i++)
        for(int j=1; j<=wielkosc_planszy; j++)
        {
            if((czyjruch%2==0 && id=='A') || (czyjruch%2==1 && id=='B'))
            {
                if(przyciski[i][j]->rodzaj==2 && poprawnosc(przyciski[i][j]->numer)==1)
                {
                    czyodblokowano = 1;
                    przyciski[i][j]->isblocked=0;
                    gtk_image_set_from_file(przyciski[i][j]->obrazek,"podswietlony.png");
                }
            }
        }
    return czyodblokowano;

}


void aktualizuj_historie(char *data)
{
    //rozdzielenie wspolrzednych, zachowujac jednak konwencje w stylu szachownicy
    char x=data[0];
    int y=(int)data[1] - 96;
    char historia[1000000];
    if(czyjruch%2==0)
    {
        sprintf(historia,"%sTURA: %d.\nBialy: %c-%d\t",gtk_label_get_text(GTK_LABEL(historia_label)),numer_tury,x,y);
    }
    else if(czyjruch%2==1)
        sprintf(historia,"%sCzarny: %c-%d\n",gtk_label_get_text(GTK_LABEL(historia_label)),x,y);

    gtk_label_set_text(GTK_LABEL(historia_label),historia);
}


void clicked(GtkWidget *btn_clicked,GdkEventButton *event,char *data)
{
    //rozdzielenie wspolrzednych, ale do lokalizacji wcisnietego przycisku, dlatego jako int
    int x=(int)data[0] - 96;
    int y=(int)data[1] - 96;


    if(przyciski[x][y]->isblocked==0)
    {
        g_print("Przycisk zostal nacisniety\n");

        char tmp[4];
        sprintf(tmp,"&%s",data);
        wyslij_sygnal(tmp);

        aktualizuj_plansze(data,czyjruch);

        aktualizuj_historie(data);

        czyjruch++;

        podswietl();

    }
}


void wyslij_sygnal(char *data)
{
    printf("wyslano sygnal: %s\n",data);
    sendStringToPipe(potoki,data);

}


int odbierz_sygnal()
{
    char bufor[MAKS_DL_TEKSTU];
    if(getStringFromPipe(potoki,bufor,MAKS_DL_TEKSTU)==1)
    {
        printf("odebrano sygnal: %s\n",bufor);
        if(bufor[0]=='!')                           //odebranie wiadomosci
        {
            pobierz_tekst(bufor+1);
            return TRUE;
        }
        if(bufor[0]=='@')                           //odebranie wiadomosci o koncu rozgrywki (koniec ruchow dla obu graczy)
        {
            dialog_finish();
            //wait();
            return TRUE;
        }
        if(bufor[0]=='#')                          //jeden z graczy wychodzi z gry
        {
            if(bufor[1]=='@')                      //szczegolny przypadek gdy jeden z graczy wychodzi z rozgrywki po rozegraniu pelnej partii
                gtk_widget_hide(finish_dialog);
            dialog_koniec();
            return TRUE;
        }
        if(bufor[0]=='$')                          //wyslanie do przeciwnika zapytania o nowa gre
        {
            if(bufor[1]=='@')                      //szczegolny przypadek, gdy jest to zapytanie z poziomu menu, wtedy wlicza sie takze wielkosc planszy
            {
                if(bufor[2]=='8')
                    wielkosc_planszy=8;
                else if(bufor[2]=='1')
                {
                    if(bufor[3]=='2')
                        wielkosc_planszy=12;
                    else wielkosc_planszy=16;
                }
                nowy_dialog_start("MENU");
            }
            else nowy_dialog_start(NULL);
            return TRUE;
        }
        if(bufor[0]=='%')                           //odpowiedz pozytywna na zapytanie o nowa gre
        {
            if(bufor[1]=='@')
            {
            GtkWidget *nowe = gtk_container_get_focus_child (GTK_CONTAINER (window));
            gtk_container_remove(GTK_CONTAINER(window),nowe);
            gtk_container_add(GTK_CONTAINER(window),glowne);
            gtk_widget_show_all(window);
            gtk_widget_destroy(wait_dialog);
            nowa_gra();
            }
            else
            {
            gtk_widget_destroy(wait_dialog);
            nowa_gra();
            }
            return TRUE;
        }
        if(bufor[0]=='^')                            //odpowiedz negatywna na zapytanie o nowa gre
        {
            gtk_widget_destroy(wait_dialog);
            return TRUE;
        }
        if(bufor[0]=='&')                           //sygnal informujacy drugi terminal o wykonaniu ruchu
        {
            aktualizuj_plansze(bufor,czyjruch);
            aktualizuj_historie(bufor+1);
            czyjruch++;

            if(podswietl()==0)                      //jesli nie ma mozliwosci wykonania ruchu, wysyla sygnal BR = Brak Ruchow
            {
                char tmp[4]=".BR";
                czyjruch++;
                wyslij_sygnal(tmp);
            }
            return TRUE;
        }
        if(bufor[0]=='.')                           //sygnal o braku mozliwosci wykonania ruchu przez przeciwnika
        {
            czyjruch++;
            if(podswietl()==0)                      //jesli i gracz ktorego jest teraz kolej nie moze wykonac ruchu, jest to GO - Game Over
            {
                char tmp[4]="@GO";
                wyslij_sygnal(tmp);
                dialog_finish();
            }
            return TRUE;
        }
        if(bufor[0]=='(')                           //pomocniczo rozdzielilem rozpoczecie dialogu o nowa rozgrywke pomiedzy terminalami, jesli jest to po zakonczeniu pelnej partii
        {
            nowy_dialog_start_po_koncu();
            return TRUE;
        }
        if(bufor[0]==')')                           //sygnal dotyczacy wczytania gry
        {
            if(bufor[1]=='@')
            {
                gtk_widget_destroy(wczytaj_dialog);
                wczytaj();
            }
            else
                nowy_dialog_wczytaj();
            return TRUE;
        }
    }
    return TRUE;
}


void quit(gpointer data)
{
    if(data == "KONIEC")                        //jesli sygnal jest wysylany z poziomu konca partii, rozdzielilem to, poniewaz wystepowala kolizja pomiedzy odbieranymi sygnalami i w wyniku nie zamykaly sie niektore widgety
    {
        char tmp[5]="#@END";
        printf("wyslano sygnal: %s\n",tmp);
        sendStringToPipe(potoki,tmp);
    }
    else
    {
        char tmp[5]="#END";                     //sygnal po zamknieciu programu
        printf("wyslano sygnal: %s\n",tmp);
        sendStringToPipe(potoki,tmp);
    }
    closePipes(potoki);
    gtk_main_quit();
}


void dialog_finish()
{
    g_timeout_add(100,odbierz_sygnal,NULL);

    int il_b=0;
    int il_cz=0;
    for(int i=1; i<=wielkosc_planszy; i++)
        for(int j=1; j<=wielkosc_planszy; j++)          //zliczenie ilosci pionow obu graczy, wygrywa ten ktorego pionow jest wiecej
        {
            if(przyciski[i][j]->rodzaj==0) il_b++;
            if(przyciski[i][j]->rodzaj==1) il_cz++;
        }
    if(il_b>il_cz)
    {
        if(id=='A')
            wygrana_label=gtk_label_new("ZWYCIĘSTWO!");
        else
            wygrana_label=gtk_label_new("PORAŻKA..");
    }
    else if(il_b<il_cz)
    {
        if(id=='B')
            wygrana_label=gtk_label_new("ZWYCIĘSTWO!");
        else
            wygrana_label=gtk_label_new("PORAŻKA..");
    }
    else
        wygrana_label=gtk_label_new("REMIS");

    const char *title = "KONIEC GRY";

    finish_dialog=gtk_dialog_new_with_buttons(title,
                  GTK_WINDOW(window),
                  GTK_DIALOG_MODAL,"NOWA GRA?",GTK_RESPONSE_ACCEPT,"WYJDŹ",GTK_RESPONSE_REJECT,NULL,NULL);

    gtk_window_set_deletable(GTK_WINDOW(finish_dialog),FALSE);
    gtk_window_set_resizable(GTK_WINDOW(finish_dialog),FALSE);
    gtk_box_pack_end(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(finish_dialog))),
                     wygrana_label,TRUE,TRUE,0);

    gtk_widget_show_all(finish_dialog);

    gint response=gtk_dialog_run(GTK_DIALOG(finish_dialog));

    if(response==GTK_RESPONSE_ACCEPT)
    {
        char tmp[4]="(NGT";                                     //pozytywna odpowiedz na zapytanie przez program po zakonczeniu partii o nowa gre NGT - Nowa Gra Tak
        wyslij_sygnal(tmp);
        gtk_widget_destroy(finish_dialog);
        wait();
    }
    else if(response==GTK_RESPONSE_REJECT)
    {
        gtk_widget_destroy(finish_dialog);
        quit("KONIEC");
    }


}


void wait()
{
    g_timeout_add(100,odbierz_sygnal,NULL);

    GtkWidget *label, *spin;
    spin=gtk_spinner_new();
    wait_dialog=gtk_dialog_new_with_buttons("Musisz poczekać.",
                                            GTK_WINDOW(window),
                                            GTK_DIALOG_MODAL,NULL,NULL);

    label=gtk_label_new("Czekaj na odpowiedź przeciwnika.");
    gtk_window_set_deletable(GTK_WINDOW(wait_dialog),FALSE);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(wait_dialog))),label,0,0,0);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(wait_dialog))),spin,0,0,0);
    gtk_spinner_start(GTK_SPINNER(spin));

    gtk_widget_show_all(wait_dialog);

}


void dialog_koniec()                                               //dialog, jesli jeden z graczy opuscil rozgrywke
{
    const char *title = "KONIEC";
    GtkWidget *dialog=gtk_dialog_new_with_buttons(title,
                      GTK_WINDOW(window),
                      GTK_DIALOG_MODAL,"ZAMKNIJ",NULL,NULL);

    GtkWidget *label = gtk_label_new("Przeciwnik wyszedł z gry.");
    gtk_window_set_deletable(GTK_WINDOW(dialog),FALSE);
    gtk_window_set_resizable(GTK_WINDOW(dialog),FALSE);
    gtk_box_pack_end(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                     label,TRUE,TRUE,0);

    gtk_widget_show_all(dialog);
    gint response=gtk_dialog_run(GTK_DIALOG(dialog));

    if(response==GTK_RESPONSE_OK) gtk_main_quit();
    else gtk_main_quit();
}


void rozp_nowa_gre_menu(GtkWidget *widget,gpointer data)                                                    //rozpoczecie nowej gry z poziomu menu, w mojej konwencji mape wybiera sie przy starcie rozgrywki z tego poziomu
{                                                                                                           //a po nacisnieciu "Nowa Gra" z poziomu gry zaczyna sie partie na planszy tej samej wielkosci
    GtkWidget *dialog_start, *label;

    dialog_start=gtk_dialog_new_with_buttons("Rozmiar",
                 GTK_WINDOW(window),
                 GTK_DIALOG_MODAL,"8x8",
                 GTK_RESPONSE_OK,"12x12",GTK_RESPONSE_YES,"16x16",GTK_RESPONSE_NO,NULL,NULL);

    gtk_window_set_deletable(GTK_WINDOW(dialog_start),FALSE);
    gtk_window_set_resizable(GTK_WINDOW(dialog_start),FALSE);

    label=gtk_label_new("Wybierz rozmiar planszy");
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog_start))),label,0,0,0);
    gtk_widget_show_all(dialog_start);
    gint response=gtk_dialog_run(GTK_DIALOG(dialog_start));
    if(response==GTK_RESPONSE_OK)
    {
        wielkosc_planszy=8;
    }
    if(response==GTK_RESPONSE_YES)
    {
        wielkosc_planszy=12;
    }
    if(response==GTK_RESPONSE_NO)
    {
        wielkosc_planszy=16;
    }
    gtk_widget_destroy(dialog_start);


    rozp_nowa_gre(NULL,"menu");
}


void rozp_nowa_gre(GtkWidget *widget,gpointer data)
{
        g_timeout_add(100,odbierz_sygnal,NULL);

        GtkWidget *label, *spin;
        spin=gtk_spinner_new();
        wait_dialog=gtk_dialog_new_with_buttons("Musisz poczekać.",
                                                GTK_WINDOW(window),
                                                GTK_DIALOG_MODAL,NULL,NULL);
        label=gtk_label_new("Czekaj na odpowiedź przeciwnika.");
        gtk_window_set_deletable(GTK_WINDOW(wait_dialog),FALSE);
        gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(wait_dialog))),label,0,0,0);
        gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(wait_dialog))),spin,0,0,0);
        gtk_spinner_start(GTK_SPINNER(spin));
        gtk_widget_show_all(wait_dialog);


        if(data=="menu")                            //jesli wczesniej wywolana byla funkcja rozp_nowa_gra_menu, przekazywana jest tez wybrana wielkosc planszy
        {
            if(wielkosc_planszy==8)
            {
                char tmp[6]="$@8";
                wyslij_sygnal(tmp);
            }
            else if(wielkosc_planszy==12)
            {
                char tmp[6]="$@12";
                wyslij_sygnal(tmp);
            }
            else
            {
                char tmp[6]="$@16";
                wyslij_sygnal(tmp);
            }
        }
        else
        {
            char tmp[4]="$NG";
            wyslij_sygnal(tmp);
        }

}


void nowy_dialog_start(gpointer data)                                    //zapytanie o rozpoczecie nowej rozgrywki
{
    GtkWidget *dialog_start, *label;

    dialog_start=gtk_dialog_new_with_buttons("Nowa Gra",
                 GTK_WINDOW(window),
                 GTK_DIALOG_MODAL,"TAK",
                 GTK_RESPONSE_YES,
                 "NIE",
                 GTK_RESPONSE_NO,NULL,NULL);

    gtk_window_set_deletable(GTK_WINDOW(dialog_start),FALSE);
    gtk_window_set_resizable(GTK_WINDOW(dialog_start),FALSE);

    label=gtk_label_new("Czy chcesz rozpoczac nowa gre?");
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog_start))),label,0,0,0);
    gtk_widget_show_all(dialog_start);

    gint response=gtk_dialog_run(GTK_DIALOG(dialog_start));
    if(response==GTK_RESPONSE_NO)
    {
        char tmp[4]="^NGN";
        wyslij_sygnal(tmp);
        gtk_widget_destroy(dialog_start);
    }
    if(response==GTK_RESPONSE_YES)
    {
        if(data=="MENU")
        {
        char tmp[5]="%@NGT";
        wyslij_sygnal(tmp);
        GtkWidget *nowe = gtk_container_get_focus_child (GTK_CONTAINER (window));
                gtk_container_remove(GTK_CONTAINER(window),nowe);
                gtk_container_add(GTK_CONTAINER(window),glowne);
                gtk_widget_show_all(window);
        }
        else
        {
            char tmp[5]="%NGT";
        wyslij_sygnal(tmp);
        }
        nowa_gra();
        gtk_widget_destroy(dialog_start);
    }
}


void nowy_dialog_start_po_koncu()                               //zapytanie o nowa rozgrywke po zakonczeniu rozgrywki
{
    gtk_widget_hide(finish_dialog);
    GtkWidget *dialog_start, *label;

    dialog_start=gtk_dialog_new_with_buttons("Nowa Gra",
                 GTK_WINDOW(window),
                 GTK_DIALOG_MODAL,"TAK",
                 GTK_RESPONSE_YES,
                 "NIE",
                 GTK_RESPONSE_NO,NULL,NULL);

    gtk_window_set_deletable(GTK_WINDOW(dialog_start),FALSE);
    gtk_window_set_resizable(GTK_WINDOW(dialog_start),FALSE);

    label=gtk_label_new("Czy chcesz rozpoczac nowa gre?");
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog_start))),label,0,0,0);
    gtk_widget_show_all(dialog_start);
    gint response=gtk_dialog_run(GTK_DIALOG(dialog_start));
    if(response==GTK_RESPONSE_NO)
    {
        char tmp[4]="^NGN";
        wyslij_sygnal(tmp);
        gtk_widget_destroy(dialog_start);
    }
    if(response==GTK_RESPONSE_YES)
    {
        char tmp[5]="%NGT";
        wyslij_sygnal(tmp);
        gtk_widget_destroy(dialog_start);
        nowa_gra();
    }
}


void nowa_gra()                                                     //rozstawienie pionow na planszy, po pozytywnym rozpatrzeniu zapytan o nowa rozgrywke, przypisanie sygnalow, stworzenie planszy, ustawienie poczatkowych pionow
{
    for(int i=wielkosc_planszy; i>=1; i--)
    {
        for(int j=1; j<=wielkosc_planszy; j++)
        {
            przyciski[j][i]=malloc(sizeof(Buttons));
            przyciski[j][i]->rodzaj=2;
            sprintf(przyciski[j][i]->numer,"%c%c",'a'+j-1,'a'+i-1);

            przyciski[j][i]->obrazek=(GtkImage *)gtk_image_new_from_file("pusty.png");
            przyciski[j][i]->btn=gtk_event_box_new();

            gtk_container_add(GTK_CONTAINER(przyciski[j][i]->btn),GTK_WIDGET(przyciski[j][i]->obrazek));
            gtk_grid_attach(GTK_GRID(pola),przyciski[j][i]->btn, j-1,wielkosc_planszy-i,1,1);
        }
    }
    for(int i=0; i<wielkosc_planszy+2; i++)
        przyciski[i][0]=przyciski[0][i]=przyciski[wielkosc_planszy+1][i]=przyciski[i][wielkosc_planszy+1]=NULL;

    for(int i=0; i<wielkosc_planszy; i++)
    {
        char a[3];
        sprintf(a,"%d",wielkosc_planszy-i);
        char c[3];
        sprintf(c,"%c",'a'+i);
        gtk_grid_attach(GTK_GRID(pola),gtk_label_new(a),wielkosc_planszy,i,1,1);
        gtk_grid_attach(GTK_GRID(pola),gtk_label_new(c),i,wielkosc_planszy,1,1);
    }

    for(int i=1; i<=wielkosc_planszy; i++)
        for(int j=1; j<=wielkosc_planszy; j++)
        {
            przyciski[i][j]->isblocked=1;
            g_signal_connect(G_OBJECT(przyciski[i][j]->btn),"button_press_event",G_CALLBACK(clicked),przyciski[i][j]->numer);
        }
    gtk_widget_show_all(window);

    czyjruch=0;
    numer_tury=1;

    for(int i=1; i<=wielkosc_planszy; i++)
        for(int j=1; j<=wielkosc_planszy; j++)
        {
            przyciski[i][j]->rodzaj=2;
        }
    przyciski[wielkosc_planszy/2][wielkosc_planszy/2]->rodzaj=1;
    przyciski[wielkosc_planszy/2][wielkosc_planszy/2]->isblocked=1;
    gtk_image_set_from_file(GTK_IMAGE(przyciski[wielkosc_planszy/2][wielkosc_planszy/2]->obrazek),"czarny.png");

    przyciski[(wielkosc_planszy/2)+1][(wielkosc_planszy/2)+1]->rodzaj=1;
    przyciski[(wielkosc_planszy/2)+1][(wielkosc_planszy/2)+1]->isblocked=1;
    gtk_image_set_from_file(przyciski[(wielkosc_planszy/2)+1][(wielkosc_planszy/2)+1]->obrazek,"czarny.png");

    przyciski[(wielkosc_planszy/2)+1][wielkosc_planszy/2]->rodzaj=0;
    przyciski[(wielkosc_planszy/2)+1][wielkosc_planszy/2]->isblocked=1;
    gtk_image_set_from_file(przyciski[(wielkosc_planszy/2)+1][wielkosc_planszy/2]->obrazek,"bialy.png");

    przyciski[wielkosc_planszy/2][(wielkosc_planszy/2)+1]->rodzaj=0;
    przyciski[wielkosc_planszy/2][(wielkosc_planszy/2)+1]->isblocked=1;
    gtk_image_set_from_file(przyciski[wielkosc_planszy/2][(wielkosc_planszy/2)+1]->obrazek,"bialy.png");

    gtk_label_set_text(GTK_LABEL(historia_label),"");

    podswietl();
}


void przekaz_tekst( GtkWidget *widget,GtkWidget *text)
{
    char moj_id[6];
    strcpy(moj_id,"Ty");
    moj_id[2]='\0';
    strcpy(moj_id+2," : ");

    gchar wejscie[MAKS_DL_TEKSTU+5];
    char temp[MAKS_DL_TEKSTU+5];
    sprintf(temp,"!%s",gtk_entry_get_text(GTK_ENTRY(text)));

    sendStringToPipe(potoki, temp);

    strcpy(wejscie,moj_id);
    strcpy(wejscie+strlen(wejscie),gtk_entry_get_text (GTK_ENTRY (text)));
    strcat(wejscie,"\n");

    gtk_text_buffer_insert_at_cursor (GTK_TEXT_BUFFER(buffor),wejscie,-1);

    gtk_entry_set_text(GTK_ENTRY(text), "");

}


void pobierz_tekst(char *data)
{
    char twoj_id[14];
    strcpy(twoj_id,"Przeciwnik");
    twoj_id[10]='\0';
    strcpy(twoj_id+10," : ");

    gchar wejscie[MAKS_DL_TEKSTU+5];

    strcpy(wejscie,twoj_id);
    strcpy(wejscie+strlen(wejscie),data);
    strcat(wejscie,"\n");
    gtk_text_buffer_insert_at_cursor (GTK_TEXT_BUFFER(buffor),wejscie,-1);

}


int wczyta_czy_pusty()
{
     FILE *fp;
    if ((fp=fopen("historia.txt", "r"))==NULL)
    {
        return 1;
    }

    int c = fgetc(fp);
    if (c == EOF) {
        return 1;
    }
    return 0;
    fclose (fp);
}
