#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-svg.h>
#include <gtk/gtk.h>

#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
 {
  char*        cases;
  unsigned int nbLignes;
  unsigned int nbColonnes;
 }
 Labyrinthe;

Labyrinthe* labyrinthe;

void lireLabyrinthe (char* nomFichier)
 {
  labyrinthe = (Labyrinthe*) calloc (sizeof(Labyrinthe), 1);

  int numLigne=0;
  int numColonne=0;

  if (labyrinthe)
        {
         FILE* fichier = fopen (nomFichier, "r");

         if (fichier)
               {
                int caractere;

                printf ("Lecture du fichier %s\n", nomFichier);


                do
                 {
                  caractere = fgetc (fichier);

                  if (caractere == '\n' || caractere == EOF)
                        {
                         labyrinthe->nbLignes   += 1;

                         if (numColonne > labyrinthe->nbColonnes)
                          labyrinthe->nbColonnes = numColonne;

                         numLigne++;
                         numColonne=0;
                        }
                   else if (caractere >= 32)
                         {
                          labyrinthe->cases = (char*) realloc (labyrinthe->cases,
                                                labyrinthe->nbLignes * labyrinthe->nbColonnes + 1);

                          if (labyrinthe->cases != NULL)
                                 {
                                  labyrinthe->cases [numLigne * labyrinthe->nbColonnes + numColonne]
                                                                        = (char) caractere;
                                  numColonne++;
                                 }
                            else {
                                  fprintf (stderr, "Probleme de redimensionnement de cases\n");
                                 }
                         }
                 }
                 while (caractere != EOF);

                fclose (fichier);
               }
          else {
                fprintf (stderr, "On ne peut pas ouvrir le fichier %s\n", nomFichier);
               }
        }
   else {
         fprintf (stderr, "On ne peut pas creer la structure Labyrinthe\n");
        }
 }



#define TAILLE 50

cairo_t* canvas;


void dessinerMur (int x, int y)
 {
  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_rectangle       (canvas, x*TAILLE,  y*TAILLE, TAILLE, TAILLE);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 0.5, 0.5, 0.5);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);
 }


void dessinerSol (int x, int y)
 {
  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_rectangle       (canvas, x*TAILLE,  y*TAILLE, TAILLE, TAILLE);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 0, 1, 0);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);
 }








void dessiner ()
 {
  int colonne, ligne;

  if (labyrinthe)
   {
    for (ligne=0; ligne<labyrinthe->nbLignes; ligne++)
     {
      for (colonne=0; colonne<labyrinthe->nbColonnes; colonne++)
       {
        switch (labyrinthe->cases [ligne * labyrinthe->nbColonnes + colonne])
         {
          case '#' : dessinerMur (colonne, ligne);
                     break;
          case ' ' : dessinerSol (colonne, ligne);
                     break;
         }
       }
     }
   }
 }

gboolean on_draw_event (GtkWidget* widget,
                        cairo_t*   contexte,
                        gpointer   user_data)
 {
  canvas = contexte;

  dessiner ();

  return FALSE;
 }


int main (int argc, char *argv[])
 {
  GtkWidget* fenetre;
  GtkWidget* canvas;

  gtk_init (&argc, &argv);

  fenetre = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  gtk_window_set_position      (GTK_WINDOW (fenetre), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size  (GTK_WINDOW (fenetre), 400, 400);
  gtk_window_set_title         (GTK_WINDOW (fenetre), "Pacman GTK Cairo");

  g_signal_connect (G_OBJECT (fenetre), "destroy",         G_CALLBACK (gtk_main_quit), NULL);
//  g_signal_connect (G_OBJECT (fenetre), "key_press_event", G_CALLBACK (on_key_press),  canvas);

  canvas  = gtk_drawing_area_new ();

  gtk_container_add (GTK_CONTAINER (fenetre), canvas);

  g_signal_connect (G_OBJECT (canvas), "draw", G_CALLBACK (on_draw_event), NULL);

  gtk_widget_show_all (fenetre);

  lireLabyrinthe ("laby.txt");

  gtk_main();
 }

