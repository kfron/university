#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "reversi.h"
#include "fifo.h"

#define MAKS_DL_TEKSTU 10000

void pokazBlad(char *komunikat)
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

int main(int argc,char *argv[])
{
    if(argc < 2)
    {
        printf("\nArgumentem wywołania programu powinno być A lub B\n");
        return 1;
    }
    if(argv[1][0]=='A'){id='A';myid="A - ";yid="B - "; potoki=initPipes(argc,argv);}
    else if(argv[1][0]=='B'){id='B';myid="B - ";yid="A - "; potoki=initPipes(argc,argv);}
    else{printf("\nArgumentem wywołania powinno byc A lub B\n"); return 0;}

    gtk_init (&argc, &argv);

    //inicjalizacja widgetu window
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Reversi");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),30);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(quit),NULL);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

    //stworzenie menu
    menu=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,30);
    GtkWidget *menu_grid=gtk_grid_new();
    GtkWidget *reversi=gtk_label_new("REVERSI");

    GtkWidget *newGame_menu=gtk_button_new_with_label("Nowa gra");
    g_signal_connect(newGame_menu,"clicked",G_CALLBACK(rozp_nowa_gre_menu),NULL);

    wczytajGre=gtk_button_new_with_label("Wczytaj gre");
    g_signal_connect(wczytajGre,"clicked",G_CALLBACK(rozp_wczytywanie),NULL);
    if(wczyta_czy_pusty()==1) gtk_widget_set_sensitive(GTK_WIDGET(wczytajGre),FALSE);

    char *uri = "https://www.kurnik.pl/reversi/zasady.phtml";
    GtkWidget *informacje=gtk_link_button_new_with_label(uri,"Zasady");

    GtkWidget *wyjdz=gtk_button_new_with_label("Wyjdź");
    g_signal_connect(wyjdz,"clicked",G_CALLBACK(quit),NULL);

    gtk_grid_attach(GTK_GRID(menu_grid),reversi,0,0,4,2);
    gtk_grid_attach(GTK_GRID(menu_grid),newGame_menu,0,2,4,2);
    gtk_grid_attach(GTK_GRID(menu_grid),wczytajGre,0,4,4,2);
    gtk_grid_attach(GTK_GRID(menu_grid),informacje,0,6,4,2);
    gtk_grid_attach(GTK_GRID(menu_grid),wyjdz,0,8,4,2);
    gtk_container_add(GTK_CONTAINER(menu), menu_grid);



    //boxy na wszystkie widgety
    glowne=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,30);
    GtkWidget *srodek=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    GtkWidget *lewe=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    GtkWidget *prawe=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

    //grid na historie
    GtkWidget *lewyGrid=gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(lewyGrid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(lewyGrid), TRUE);

    //historia
    GtkWidget *historia_theme=gtk_label_new("     POPRZEDNIE RUCHY     ");
    GtkWidget *historia=gtk_scrolled_window_new(NULL,NULL);
    historia_label=gtk_label_new("Brak.");
    gtk_container_add(GTK_CONTAINER(historia), historia_label);
    gtk_grid_attach(GTK_GRID(lewyGrid),historia,0,3,2,10);
    gtk_grid_attach(GTK_GRID(lewyGrid),historia_theme,0,2,2,1);



    //Grid na przyciski nowa gra i cofnij oraz na nick
    GtkWidget *srodekGrid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(srodekGrid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(srodekGrid), TRUE);

    //nowa gra
    newGame=gtk_button_new_with_label("Nowa gra");
    gtk_grid_attach(GTK_GRID(srodekGrid),newGame,0,0,1,1);
    g_signal_connect(newGame,"clicked",G_CALLBACK(rozp_nowa_gre),NULL);
    czyjruch=0;
    numer_tury=1;

    //zapisz
    GtkWidget *cofnij = gtk_button_new_with_label("Zapisz");
    gtk_grid_attach(GTK_GRID(srodekGrid),cofnij,1,0,1,1);
    g_signal_connect(G_OBJECT(cofnij),"clicked",G_CALLBACK(zapis),NULL);

    //nick gracza
    GtkWidget *nick;
    if(strcmp(myid,"A - ")==0)
        nick=gtk_label_new("Gracz Bialy");
    else
        nick=gtk_label_new("Gracz Czarny");
    gtk_box_pack_start(GTK_BOX(srodek), nick, TRUE, TRUE, 0);

    //grid ustawiajacy pola planszy
    pola=gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(pola), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(pola), TRUE);


    //czat
    GtkWidget *prawyGrid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(prawyGrid), 1);
    gtk_grid_set_row_homogeneous(GTK_GRID(prawyGrid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(prawyGrid), TRUE);

    buffor = (GtkWidget *)gtk_text_buffer_new (NULL);
    GtkWidget *text_view = gtk_text_view_new_with_buffer (GTK_TEXT_BUFFER(buffor));
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);

    GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER (scrolled_window), text_view);
    gtk_container_set_border_width (GTK_CONTAINER(scrolled_window), 1);
    gtk_grid_attach(GTK_GRID(prawyGrid), scrolled_window, 0, 0, 10, 6);

    GtkWidget *text = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(text), MAKS_DL_TEKSTU);
    gtk_entry_set_text(GTK_ENTRY(text), "");
    g_signal_connect(G_OBJECT(text), "activate",G_CALLBACK(przekaz_tekst),(gpointer) text);
    gtk_grid_attach(GTK_GRID(prawyGrid), text, 0, 6, 10, 1);

    GtkWidget *czat=gtk_label_new("Czat");
    gtk_box_pack_start(GTK_BOX(prawe),czat,0,0,0);


    //dodanie gridu do boxa i boxa do window

    gtk_container_add(GTK_CONTAINER(lewe), lewyGrid);
    gtk_container_add(GTK_CONTAINER(glowne),lewe);

    gtk_container_add(GTK_CONTAINER(srodek), srodekGrid);
    gtk_container_add(GTK_CONTAINER(srodek), pola);
    gtk_container_add(GTK_CONTAINER(glowne), srodek);

    gtk_container_add(GTK_CONTAINER(prawe), prawyGrid);
    gtk_container_add(GTK_CONTAINER(glowne),prawe);

    gtk_container_add(GTK_CONTAINER(window), menu);

    g_timeout_add(100,odbierz_sygnal,NULL);

    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}

