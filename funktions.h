/*
Name:		Alexey Obukhov
MatrNr:		39456
*/
/* funktions.h */
/*---------------------------------------GTK Teil-------------------------------------------*/

#include <gtk/gtk.h>
     
typedef enum { false, true } bool;/*da in C boolean fehlt, sollte man das vorher definieren */
typedef char   gchar;
typedef struct
{
     GtkWidget *entry,*entry1,*entry2, *textview, *textviewName;  /* GTK Widgets fuer text Ein-/Ausgabe    */
     GtkWidget *insert, *show, *search, *del, *quit, *about, *main_screen; /* Buttons       */
} Widgets;                                                                    

/*-----------------------------------Datenstruktur------------------------------------------*/

typedef struct phonebook 	                   /* Strukturtyp			    */
{
    char name[28+1];     	                        /* Strukturkomponenten 		    */
    char vorname[28+1];
    char nummer[18+1];
}book; 			 	     		   /* Datentyp fuer Connector 	    	    */

/*-------------------------Prototypen fuer die Funktionen-----------------------------------*/

void sortier(book* person, unsigned int count);    /* alphabetische Sortierung in Phonebuch */
void search_user(GtkButton *button, Widgets *w);        /* Suche nach Telefonnummer 	    */ 
book* ReadFromFile(book* person);     /* man gibt die Kontakten aus Phonebook ins Struktur  */  
unsigned int NumberOfPerson(book* person);/* zaehlt die Anzahl von Personen im Phonebook    */
void WriteToFile(book* person, unsigned int count); /* ueberschreibt das Phonebook          */
unsigned int DelPerson(book* person,unsigned int count);/* zaehlt die Anzahl von Personen   */ 
void show_text (GtkButton *button, Widgets *w);	      /* anzeigt das Telefonebuch           */
void insert_person(GtkButton *button, Widgets *w);    /* hinzufuegt die Person ins Buch     */
void delete_person(GtkButton *button, Widgets *w);    /* loescht die Person aus phonebook   */
void about_app(GtkButton *button,Widgets *w);	      /* die Info ueber app                 */
void about_app_main(Widgets *w);             /* schreibt aufs Bildschirm die Info ueber app */
GdkPixbuf *load_pixbuf_from_file (const char *filename);/* background von der Anwendung     */
void start_screen(Widgets *w);                          /* main screen                      */    
void start_screen_main(GtkButton *button, Widgets *w);  /* main screen                      */
                              






