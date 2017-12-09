/*
 * Name:		Alexey Obukhov
 * MatrNr:		39456
 */
/* funktions.c */
/* zum Kompilieren bitte folgendes eingeben:
 * gcc funktions.h funktions.c main.c -o phonebook `pkg-config --cflags --libs gtk+-2.0`
 */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funktions.h"

void insert_person(GtkButton *button, Widgets *w)   
{
    book* p; const gchar *tmp=NULL,*tmp1=NULL,*tmp2=NULL,*error="muster";
    unsigned int count,i;
    int k,k1; /* die Variable braucht man um die Buchstaben durchzusuchen , da es zu negativen Werten fuehren kann, nutzt man hier int anstaat unsigned int */
    bool A=true; /* die Variable braucht man um zu kontrolieren grosse/kleine Schreibung bei Name/Vorname, oder um am Anfang der Nummer ein "+" zu erlauben */
    bool IsAlphaName=false, IsAlphaVorname=false, IsDigit=false; /* die Variable braucht man um zu pruefen, ob es koorekte Eingabe oder nicht */
    count=NumberOfPerson(p); 
    p=ReadFromFile(p);
    GtkTextBuffer *buffer;
    GtkTextMark *mark;
    GtkTextIter iter;
    
    buffer = gtk_text_buffer_new (NULL);
    gtk_text_view_set_buffer (GTK_TEXT_VIEW (w->textview),buffer); /* unrefs old buffer and references new */
    g_object_unref (buffer);
    
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (w->textview));
    mark = gtk_text_buffer_get_insert (buffer);
    gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
    
    /* in diesem Teil wird es geprueft, ob die Eingabe ueberhaut fuer die Strukture passt */
    tmp  = gtk_entry_get_text (GTK_ENTRY (w->entry));
    for(k=0;k<strlen(tmp);k++)
        if(isalpha(tmp[k])) IsAlphaName=true;
        tmp1 = gtk_entry_get_text (GTK_ENTRY (w->entry1));
    for(k=0;k<strlen(tmp1);k++)
        if(isalpha(tmp1[k])) IsAlphaVorname=true;
        tmp2 = gtk_entry_get_text (GTK_ENTRY (w->entry2)); 
    for(k=0;k<strlen(tmp2);k++)
        if(isdigit(tmp2[k])) IsDigit=true;
        
        if(strlen(tmp) || strlen(tmp1) || strlen(tmp2)) /* man prueft ob es ueberhaupt etwas eingetippt wurde */
        {  
            p=realloc(p,sizeof(book)*(count+1)); /* man vergroessert Speicher */
            strncpy(p[count].name,tmp,28); /* und kopiert hin die Person */
            for(i=0;i<=strlen(p[count].name);i++) 
                if(p[count].name[i]==32) p[count].name[i]=126; /* waehrend der Eingabe nimmt man anstatt " " ein "~" Symbol, um kein Fehler beim Speichern zu bekommen, was aber spaeter wider zum " " konvertiert wird */
                    strncpy(p[count].vorname, tmp1,28);
                for(i=0;i<=strlen(p[count].vorname);i++) 
                    if(p[count].vorname[i]==32) p[count].vorname[i]=126;
                    strncpy(p[count].nummer, tmp2,18);
                /* dies Teil des Codes ist verantwortlich fuer die korrekte Eingabe( keine Ziffer in den Namen und Vornamen, bzw. keine Symbole ausser '-' oder ' ' */
                /* keine Buchstaben und Symbolen in den Nummern, ausser '+' am Anfang. Falsch eingegebeben Nummern werden gekurzt bis zum ersten falschen Symbole */
                if(IsAlphaName==true) /* falls es ueberhaupt bei dem Name etwas eingegeben wurde bzw. ob es Buchstaben waren */
                {
                    A=true;
                    for(k=0;k<=strlen(p[count].name)-1;k++)
                    { 
                        if((p[count].name[k]>=65 && p[count].name[k]<=90) && A==true) A=false; /* wenn es am Anfang schon die grosse Buchstabe steht */
                            else if((p[count].name[k]>=97 && p[count].name[k]<=122) && A==true) /* alle kleine Buchstaben am Anfang bzw. nach dem "-" oder nach dem " " werden grossen */
                            {
                                p[count].name[k]-=32;
                                A=false;
                            }
                            else if(A==false && p[count].name[k]>=65 && p[count].name[k]<=90) p[count].name[k]+=32;
                            if((k!=strlen(p[count].name)-1) && ((p[count].name[k]==126) || (p[count].name[k]==45))) /* Symbol "~" wird zugelassen bei einer Person in Structure( dadurch wird " " maskiert) und das Symbol "-" ist zugelassen -> wenn es nach dem Symbol " " oder "-" das zweite/dritte/etc Teils des Namens/Vornamens folgt, wird das mit grossen Buchstaben anfangen */
                            {
                                A=true;
                            }
                            if((p[count].name[k]>=33 && p[count].name[k]<=44) || (p[count].name[k]>=46 && p[count].name[k]<=64) || (p[count].name[k]>=91 && p[count].name[k]<=94) || (p[count].name[k]>=123 &&  p[count].name[k]<126)) /* Korrektur der Eingabe */
                            {
                                for(k1=k;k1<=strlen(p[count].name)-1;k1++) /* die falsch eingetippte Symbol wird durch naechstes Symbol eingesetzt */
                                    p[count].name[k1]=p[count].name[k1+1];
                                k--;     
                            }
                    }                    
                } 
                else strcpy(p[count].name,error); /* falls nicht -> wird durch "muster" eingesetzt */
                    
                    if(IsAlphaVorname==true) /* falls es ueberhaupt bei dem Vorame etwas eingegeben wurde bzw. ob es Buchstaben waren */
                    {
                        A=true;
                        for(k=0;k<=strlen(p[count].vorname)-1;k++)
                        { 
                            if((p[count].vorname[k]>=65 && p[count].vorname[k]<=90) && A==true) A=false;
                            else if((p[count].vorname[k]>=97 && p[count].vorname[k]<=122) && A==true) /* alle kleine Buchstaben am Anfang werden grossen */
                            {
                                p[count].vorname[k]-=32;
                                A=false;
                            }
                            else if(A==false && p[count].vorname[k]>=65 && p[count].vorname[k]<=90) p[count].vorname[k]+=32;
                            if((k!=strlen(p[count].vorname)-1) && ((p[count].vorname[k]==126) || (p[count].vorname[k]==45))) /* Symbol "~" wird zugelassen bei einer Person in Structure( dadurch wird " " maskiert) und das Symbol "-" ist zugelassen -> wenn es nach dem Symbol " " oder "-" das zweite/dritte/etc Teil des Namens/Vornamens folgt, wird das mit grossen Buchstaben anfangen */
                            {
                                A=true;
                            }
                            if((p[count].vorname[k]>=33 && p[count].vorname[k]<=44) || (p[count].vorname[k]>=46 && p[count].vorname[k]<=64) || (p[count].vorname[k]>=91 && p[count].vorname[k]<=94) || (p[count].vorname[k]>=123 &&  p[count].vorname[k]<126)) /* Korrektur der Eingabe */
                            {
                                for(k1=k;k1<=strlen(p[count].vorname)-1;k1++) /* das falsch eingetippte Symbol wird durch naechstes Symbol eingesetzt */
                                    p[count].vorname[k1]=p[count].vorname[k1+1];
                                k--;     
                            }
                        }                    
                    } 
                    else strcpy(p[count].vorname,error); /* falls nicht -> wird durch "muster" eingesetzt */
                        if(IsDigit==true) /* falls es ueberhaupt bei der Nummer etwas eingegeben wurde bzw. ob es Zahlen waren */
                        {
                            A=true;
                            for(k=0;k<=strlen(p[count].nummer)-1;k++)
                            {
                                if((p[count].nummer[k]=='+' || (p[count].nummer[k] > 47) && p[count].nummer[k] < 58) && A==true) A=false; /* ab nun ist es erlaubt nur die Zahlen*/
                                    else if(p[count].nummer[k] <= 47 || p[count].nummer[k] >= 58) /* man prueft ob es eine Zahl eingegeben wurde */
                                    {
                                        for(k1=k;k1<=strlen(p[count].nummer)-1;k1++)
                                            p[count].nummer[k1]=p[count].nummer[k1+1];
                                        k--;     
                                    } /* und ignoriert alle Zeichen ausser [0-9] */
                            }
                        }
                        else strcpy(p[count].nummer,error); /* falls nicht -> wird durch "muster" eingesetzt */
                        
                        sortier(p,count+1); 
                        WriteToFile(p,count+1);
                        gtk_text_buffer_insert (buffer, &iter, "\n                  ############# Add  ###############\n\n" , -1);
                        gtk_text_buffer_insert (buffer, &iter, "\n Person wurde erfolgreich hinzufuegt", -1);
                        gtk_entry_set_text(GTK_ENTRY(w->entry), ""); /* reinigt alle w->enries*/
                        gtk_entry_set_text(GTK_ENTRY(w->entry1), "");
                        gtk_entry_set_text(GTK_ENTRY(w->entry2), "");
        }
        else
        {
            gtk_text_buffer_insert (buffer, &iter, "\n                  ############# Add  ###############\n\n" , -1);
            gtk_text_buffer_insert (buffer, &iter, "\n Um eine Person hinzufuegen passen Sie bitte auf folgendes:", -1);  
            gtk_text_buffer_insert (buffer, &iter, "\n\n WICHTIG! Name soll ins erste Feld eingetippt werden", -1);
            gtk_text_buffer_insert (buffer, &iter, "\n          Vorname soll ins zweite Feld eingetippt werden", -1);
            gtk_text_buffer_insert (buffer, &iter, "\n          Nummer soll ins dritte Feld eingetippt werden", -1);
            gtk_text_buffer_insert (buffer, &iter, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n             Name               Vorname              Nummer", -1);
        }
        
}

void show_text(GtkButton *button, Widgets *w)
{
    
    
    book* p;
    unsigned int i,j, count;
    count=NumberOfPerson(p); 
    gchar *text,*text1,*text2, CharTmp[]={" "}; /* die Variable braucht man fuer die Ausgabe eines Persons */
    
    p=ReadFromFile(p);
    GtkTextBuffer *buffer;
    GtkTextMark *mark;
    GtkTextIter iter;
    
    buffer = gtk_text_buffer_new (NULL);
    gtk_text_view_set_buffer (GTK_TEXT_VIEW (w->textview),buffer); /* unrefs old buffer and references new */
    g_object_unref (buffer);
    
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (w->textview));
    mark = gtk_text_buffer_get_insert (buffer);
    gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
    
    gtk_text_buffer_insert (buffer, &iter, "\n                  ########### Phonebook ############\n\n" , -1); 
    for(i=0;i<count;i++)
    {
        if (gtk_text_buffer_get_char_count(buffer))
            gtk_text_buffer_insert (buffer, &iter, "\n", -1);
        /* hier vervollstaendigt man den Name und Vorname mit den Symbolen " ", um die Ausgabe schoen zu machen */
        text  = p[i].name;
        for(j=strlen(text);j<=27;j++)
            text  = strcat(text,CharTmp);
        text1 = p[i].vorname ;
        for(j=strlen(text1);j<=27;j++)
            text1 = strcat(text1,CharTmp);
        
        text2 = p[i].nummer ;
        
        gtk_text_buffer_insert (buffer, &iter, " ", -1); 
        gtk_text_buffer_insert (buffer, &iter, text, -1);
        gtk_text_buffer_insert (buffer, &iter, " ", -1);
        gtk_text_buffer_insert (buffer, &iter, text1, -1);
        gtk_text_buffer_insert (buffer, &iter, " ", -1);
        gtk_text_buffer_insert (buffer, &iter, text2, -1);
    }
    gtk_text_buffer_insert (buffer, &iter, "\n", -1); 
    
}

void search_user(GtkButton *button, Widgets *w) /* suche nach der Telefonnummer */
{
    book* p;
    unsigned int i,j,count;
    bool tmp=false; /* mit der Variable prueft man, ob Nutzer im Phonebook existiert */
    const gchar *wort;
    gchar *text,*text1,*text2, CharTmp[]={" "};
    count=NumberOfPerson(p);  
    p=ReadFromFile(p);
    GtkTextBuffer *buffer;
    GtkTextMark *mark;
    GtkTextIter iter;
    
    buffer = gtk_text_buffer_new (NULL);
    gtk_text_view_set_buffer (GTK_TEXT_VIEW (w->textview),buffer); /* unrefs old buffer and references new */
    g_object_unref (buffer);
    
    
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (w->textview));
    mark = gtk_text_buffer_get_insert (buffer);
    gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
    /* man muss den Nummer korrekt eingeben */
    wort=gtk_entry_get_text (GTK_ENTRY (w->entry2));
    gtk_text_buffer_insert (buffer, &iter, "\n", -1);
    
    for(i=0;i<count;i++) 
        if(strcmp(p[i].nummer,wort)==0) /* erstmal ueberhaupt prueft man ob gesuchtes Nutzer im Phonebook existiert */
        {
            gtk_text_buffer_insert (buffer, &iter, "                  ###### Gesuchte Person(-en) ######\n\n" , -1); 
            tmp=true;  /* d.h. gesuchter Nummer existiert im Phonebook */
            break;
        }
        if(tmp==true)
        {
            for(i=0;i<count;i++) /* gibt die gesuchte Personen aufs Bildschirm aus */
            {
                if(strcmp(p[i].nummer,wort)==0) 
                {            
                    text  = p[i].name;
                    for(j=strlen(text);j<=27;j++)
                        text  = strcat(text,CharTmp);
                    text1 = p[i].vorname ;
                    for(j=strlen(text1);j<=27;j++)
                        text1 = strcat(text1,CharTmp);
                    
                    text2 = p[i].nummer ;
                    
                    gtk_text_buffer_insert (buffer, &iter, "\n ", -1);
                    gtk_text_buffer_insert (buffer, &iter, text, -1);
                    gtk_text_buffer_insert (buffer, &iter, " ", -1);
                    gtk_text_buffer_insert (buffer, &iter, text1, -1);
                    gtk_text_buffer_insert (buffer, &iter, " ", -1);
                    gtk_text_buffer_insert (buffer, &iter, text2, -1);
                    
                } 
            }
            gtk_entry_set_text(GTK_ENTRY(w->entry), "");
            gtk_entry_set_text(GTK_ENTRY(w->entry1), "");
            gtk_entry_set_text(GTK_ENTRY(w->entry2), "");
        }
        else
        {
            gtk_text_buffer_insert (buffer, &iter, "                  ############ Search ##############\n\n" , -1);
            gtk_text_buffer_insert (buffer, &iter, "\n Person mit der eingetippten Nummer wurde nicht gefunden", -1);  
            gtk_text_buffer_insert (buffer, &iter, "\n\n WICHTIG! Gesuchte Nummer soll ins dritte Feld eingetippt werden", -1);  
            gtk_text_buffer_insert (buffer, &iter, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                                                     Nummer", -1);
        }
}

void delete_person(GtkButton *button, Widgets *w)
{
    book* p;
    unsigned int i,n,count;
    bool check=false; /* mit der Variable prueft man, ob Nutzer im Phonebook existiert */
    const gchar *PersonName,*PersonVorname;
    count=NumberOfPerson(p);  
    p=ReadFromFile(p);
    GtkTextBuffer *buffer;
    GtkTextMark *mark;
    GtkTextIter iter;
    
    buffer = gtk_text_buffer_new (NULL);
    gtk_text_view_set_buffer (GTK_TEXT_VIEW (w->textview),buffer); /* unrefs old buffer and references new */
    g_object_unref (buffer);
    
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (w->textview));
    mark = gtk_text_buffer_get_insert (buffer);
    gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
    /* man muss den Name und den Vorname korrekt eingeben */
    PersonName =gtk_entry_get_text (GTK_ENTRY (w->entry));
    PersonVorname=gtk_entry_get_text (GTK_ENTRY (w->entry1));
    for(i=0;i<count;i++)
        if((strcmp(p[i].name,PersonName) == 0) & (strcmp(p[i].vorname,PersonVorname) == 0)) 
        { 
            n=i;
            check=true;
            break;
        }
        if(check==true) /* d.h. Nutzer existiert im Buch  */
        { /* Verschiebung allen Elementen um eins, um eine Person zu loeschen */
            for(;n<(count-1);n++)
                p[n]=p[n+1];
            count--; /* dann verkuerzt man count um eins une ueberschreibt die Datei, um Ergebniss zu speichern */
            WriteToFile(p,count);
            gtk_text_buffer_insert (buffer, &iter, "\n                  ############ Delete ##############\n\n" , -1);
            gtk_text_buffer_insert (buffer, &iter, "\n Person wurde erfolgreich geloescht ",-1);
            gtk_entry_set_text(GTK_ENTRY(w->entry), "");
            gtk_entry_set_text(GTK_ENTRY(w->entry1), "");
            gtk_entry_set_text(GTK_ENTRY(w->entry2), "");
        }
        else
        {
            gtk_text_buffer_insert (buffer, &iter, "\n                  ############ Delete ##############\n" , -1);
            gtk_text_buffer_insert (buffer, &iter, "\n\n Person mit den eingetippten Namen und Vornamen wurde nicht gefunden",-1); 
            gtk_text_buffer_insert (buffer, &iter, "\n\n WICHTIG! Name soll ins erste, Vorname in zweite Feld eingegeben werden",-1);  
            gtk_text_buffer_insert (buffer, &iter, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n             Name               Vorname", -1);
        }
}

void about_app(GtkButton *button, Widgets *w)
{
    about_app_main(w);
}

void about_app_main(Widgets *w)
{
    GtkTextBuffer *buffer;
    GtkTextMark *mark;
    GtkTextIter iter;
    
    buffer = gtk_text_buffer_new (NULL);
    gtk_text_view_set_buffer (GTK_TEXT_VIEW (w->textview),buffer); // unrefs old buffer and references new
    g_object_unref (buffer);
    
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (w->textview));
    mark = gtk_text_buffer_get_insert (buffer);
    gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
    gtk_text_buffer_insert (buffer, &iter, "\n                  ############ About  ##############\n\n" , -1);
    gtk_text_buffer_insert (buffer, &iter, "\n Wenn Sie:\n",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n   -das haupt Bildschirm sehen wollen, die Taste \"main\" zu druecken",-1); 
    gtk_text_buffer_insert (buffer, &iter, "\n   -das Buch anschauen wollen, druecken Sie bitte die Taste \"show\"",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n   -ein Kontakt hinzufuegen wollen, druecken die Taste \"add\"",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n   (um eine Person ins Phonebook hinzufuegen, tippen Sie ins erste - ",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n    Name( max. 28 Symbolen), zweites - Vorname( max. 28 Symbolen) und" ,-1);
    gtk_text_buffer_insert (buffer, &iter, "\n    in drittes Feld - Phonenummer( max. 18 Symbolen) ein)",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n   -fuer die Suche nach der Telefonnummer - die Taste \"search\" druecken",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n   (waehrend der Suche die gesuchte Nummer sollte im dritten Feld sein)",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n   -fuer das Loeschen eines Kontaktes druecken Sie die Taste \"delete\"",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n   (Name von Person, die Sie loeschen wollen, soll im ersten Feld sein,",-1); 
    gtk_text_buffer_insert (buffer, &iter, "\n    Vorame - soll im zweiten Feld sein)",-1); 
    gtk_text_buffer_insert (buffer, &iter, "\n   -um Info ueber das app zu bekommen druecken die Taste \"about\"",-1); 
    gtk_text_buffer_insert (buffer, &iter, "\n    ( oder fuer die ausfuehrliche Information lesen Sie readme.txt)",-1); 
    gtk_text_buffer_insert (buffer, &iter, "\n   -und um das Programm zu verlassen tippen Sie die Taste \"quit\" ein. \n", -1);
    gtk_text_buffer_insert (buffer, &iter, "\n                                                 .....aaaaand have fun!", -1);
}

void start_screen(Widgets *w)
{
    GtkTextBuffer *buffer;
    GtkTextMark *mark;
    GtkTextIter iter;
    
    buffer = gtk_text_buffer_new (NULL);
    gtk_text_view_set_buffer (GTK_TEXT_VIEW (w->textview),buffer); /* unrefs old buffer and references new */
    g_object_unref (buffer);
    
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (w->textview));
    mark = gtk_text_buffer_get_insert (buffer);
    gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
    gtk_text_buffer_insert (buffer, &iter, "\n                  ##### Welcome to phonebook!  #####\n\n",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n ",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      #############   ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      #############   ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###       ###   ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###       ###   ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###       ###   ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###       ###   ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      #############   #############",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      #############   #############",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###             ###       ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###             ###       ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###             ###       ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###             ###       ###",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###             #############",-1);
    gtk_text_buffer_insert (buffer, &iter, "\n                      ###             #############",-1);  
    
    
}

void start_screen_main(GtkButton *button, Widgets *w)
{
    start_screen(w);
}

void sortier(book* person, unsigned int count) /* alphabetische Sortierung in phonebuch */
{  
    book* tmp; /* temporale Variable zum Umtauschen */
    unsigned int n,m; /* da nur pisitive Werte nutzt man hier( und nicht nur hier) unsigned */
    short code; /* -1, 0, 1 */
    /* erstmal nach Name */
    for(n=1;n<count;n++)
        for(m=0;m<count-n;m++)
        {
            code=strcmp(person[m+1].name,person[m].name); /* Vergleich von Elementen */          
            if(code<0)
            {
                tmp=malloc(sizeof(book));
                if(tmp!=NULL)
                {            
                    *tmp=person[m];
                    person[m]=person[m+1];
                    person[m+1]=*tmp;
                    free(tmp);                    
                } 
                else 
                {
                    exit(1);
                }
            }
        }
        /* dann nach Vorname */
        for(n=1;n<count;n++)
            for(m=0;m<count-n;m++)
            {
                code=strcmp(person[m+1].name,person[m].name); /* Vergleich von Elementen */          
                if(code==0)
                    if(strcmp(person[m+1].vorname,person[m].vorname)<0)
                    {   
                        tmp=malloc(sizeof(book));
                        if(tmp!=NULL)
                        {            
                            *tmp=person[m];
                            person[m]=person[m+1];
                            person[m+1]=*tmp;
                            free(tmp);
                        } 
                        else 
                        {
                            exit(2);
                        }
                    }
            }
}



book* ReadFromFile(book* person) /* man gibt die Kontakten aus Phonebook in die Strukture */
{
    unsigned int i=0,j;
    FILE *myfile; 
    myfile = fopen ("phonebook.txt", "r");  /* man liest info aus dem File(falls er gibt) */
    person=malloc(sizeof (book)); 
    if(person==NULL)
    {
        exit(3);
    } 
    while (fscanf (myfile, "%s %s %s", person[i].name, person[i].vorname, person[i].nummer) != EOF)
    {
        for(j=0;j<=strlen(person[i].name)-1;j++) 
            if(person[i].name[j]==126) person[i].name[j]=32;
            for(j=0;j<=strlen(person[i].vorname)-1;j++) 
                if(person[i].vorname[j]==126) person[i].vorname[j]=32;
                i++;
            person=realloc(person,sizeof(book)*(i+1));
    }
    fclose(myfile); 
    return person;
}

unsigned int NumberOfPerson(book* person) /* zaehlt die Anzahl von Personen im Phonebook.txt */
{
    unsigned int i=0; /* die Variable nutzt man als counter */
    FILE *myfile; 
    myfile = fopen ("phonebook.txt", "r");  /* man liest info aus dem File(falls er gibt) */
    person=malloc(sizeof (book)); 
    if(person==NULL)
    {
        exit(4);
    } 
    
    while (fscanf (myfile, "%s %s %s", person[i].name, person[i].vorname, person[i].nummer) != EOF)
    {
        i++;
        person=realloc(person,sizeof(book)*(i+1));
    }
    free(person);  /* da man nur die Anzahl von Personen braucht, sollte man Speicher entleeren */
    fclose(myfile); 
    return i;
}


void WriteToFile(book* person, unsigned int count) /* man schreibt das neu Phonebook in die Datei */
{
    FILE *f = fopen("phonebook.txt", "w");
    if (f == NULL)
    {
        exit(5);
    }
    unsigned int i,j;
    for(i=0;i<count;i++)
    {
        for(j=0;j<=strlen(person[i].name)-1;j++) 
            if(person[i].name[j]==32) person[i].name[j]=126;  /* beim Schreiben wird das Symbol " " durch das "~" maskiert */
                for(j=0;j<=strlen(person[i].vorname)-1;j++)           /* damit es kein Fehler beim Ablesen vorkommt */
                    if(person[i].vorname[j]==32) person[i].vorname[j]=126;
                    fprintf(f, "%-28s %-28s %-18s\n",person[i].name,person[i].vorname,person[i].nummer); /* man schreibt vorbereitene Person in die Datei ein */
    }
    fclose(f);
}

GdkPixbuf *load_pixbuf_from_file (const char *filename) /* um ein background einzusetzen */
{
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file (filename, &error);
    
    if (pixbuf == NULL)
    {
        g_print ("Error loading file: %d : %s\n", error->code, error->message);
        g_error_free (error);
        exit (6);
    }
    return pixbuf;
}



