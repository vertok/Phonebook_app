/*
 * Name:		Alexey Obukhov
 * MatrNr:		39456
 */
/* main.c */
/*------------------------------------------------------------------------------------------*/

#include "funktions.h"

int main (int   argc, char *argv[])
{
    GtkWidget *window, *scrolled_win, *hbox, *hbox2, *vbox;
    GdkPixbuf *image = NULL;
    GdkPixmap *background = NULL;
    GdkColor   colorHellBlau, colorBlack;
    GtkStyle *style = NULL;
    PangoFontDescription *font_desc;
    GList *list;
    Widgets *w = g_slice_new (Widgets);

    gdk_color_parse ("#9DBCB0", &colorHellBlau);
    gdk_color_parse ("black", &colorBlack);
    gtk_init (&argc, &argv);

    image = load_pixbuf_from_file ("./background.jpg"); /* man setzt den background ein */
    gdk_pixbuf_render_pixmap_and_mask (image, &background, NULL, 0);
    style = gtk_style_new ();
    style->bg_pixmap [0] = background;
    
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "phonebook");
    
    gtk_signal_connect(GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (gtk_main_quit), NULL);
    
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_widget_set_size_request (window, 610, 500);
    gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
    gtk_widget_set_style (GTK_WIDGET(window), GTK_STYLE (style));
    gtk_window_set_transient_for (GTK_WINDOW (window), NULL);
    
    
    bool text_is_editable = false; /* Flag indicating if w->textview field is editable or display only */
    w->textview = gtk_text_view_new ();
    if (!text_is_editable)
	{
	    GTK_WIDGET_UNSET_FLAGS(w->textview, GTK_CAN_FOCUS);
	}
    font_desc = pango_font_description_from_string ("Monospace 10"); /* man aendert hier den Schrift im w->textview */
    gtk_widget_modify_font (w->textview, font_desc);                 /* in dem die alle Buchstaben gleichgross sind */
    pango_font_description_free (font_desc);
    
    
    w->entry  = gtk_entry_new ();
    w->entry1 = gtk_entry_new ();    
    w->entry2 = gtk_entry_new ();

    start_screen(w);
    
    /* Bestimmung von den Tasten */
    w->main_screen = gtk_button_new_with_label ("main"); /*button main without bold text */
    list = gtk_container_get_children (GTK_CONTAINER (w->main_screen));
    gtk_label_set_markup (GTK_LABEL (list->data), "<b>main</b>"); /* wenn alles hat gut geklappt, dann wird show mit fettem Text */
    g_signal_connect (G_OBJECT (w->main_screen), "clicked",G_CALLBACK (start_screen_main),(gpointer) w);/* man laedt die Daten aus phonebook.txt ins structure */
    
    w->show = gtk_button_new_with_label ("show"); /*button show without bold text */
    list = gtk_container_get_children (GTK_CONTAINER (w->show));
    gtk_label_set_markup (GTK_LABEL (list->data), "<b>show</b>"); /* wenn alles hat gut geklappt, dann wird show mit fettem Text */
    g_signal_connect (G_OBJECT (w->show), "clicked",G_CALLBACK (show_text),(gpointer) w);/* man laedt die Daten aus phonebook.txt ins structure */
    
    w->insert = gtk_button_new_with_label ("add"); /*button insert */
    list = gtk_container_get_children (GTK_CONTAINER (w->insert));
    gtk_label_set_markup (GTK_LABEL (list->data), "<b>add</b>"); 
    g_signal_connect (G_OBJECT (w->insert), "clicked",G_CALLBACK (insert_person),(gpointer) w);/* add person */
    gtk_widget_set_size_request(w->insert, 50, 25);
    
    w->search = gtk_button_new_with_label ("search"); /*button search */
    list = gtk_container_get_children (GTK_CONTAINER (w->search));
    gtk_label_set_markup (GTK_LABEL (list->data), "<b>search</b>"); 
    g_signal_connect (G_OBJECT (w->search), "clicked",G_CALLBACK (search_user),(gpointer) w);
    
    w->del = gtk_button_new_with_label ("delete"); /*button del */
    list = gtk_container_get_children (GTK_CONTAINER (w->del));
    gtk_label_set_markup (GTK_LABEL (list->data), "<b>delete</b>"); 
    g_signal_connect (G_OBJECT (w->del), "clicked",G_CALLBACK (delete_person),(gpointer) w); /* man loescht eine Person aus dem Buch */
    
    w->quit = gtk_button_new_with_label ("quit"); /*button quit */
    list = gtk_container_get_children (GTK_CONTAINER (w->quit));
    gtk_label_set_markup (GTK_LABEL (list->data), "<b>quit</b>"); 
    g_signal_connect (G_OBJECT (w->quit),"clicked",G_CALLBACK (gtk_main_quit),NULL);
    
    w->about = gtk_button_new_with_label ("about"); /*button about */
    list = gtk_container_get_children (GTK_CONTAINER (w->about));
    gtk_label_set_markup (GTK_LABEL (list->data), "<b>about</b>"); 
    g_signal_connect (G_OBJECT (w->about), "clicked",G_CALLBACK (about_app),(gpointer) w);/* about das Programm */
    gtk_widget_set_size_request(w->about, 70, 25);
        
    scrolled_win = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_add (GTK_CONTAINER (scrolled_win), w->textview);
    
    hbox = gtk_hbox_new (FALSE, 5);
    gtk_box_pack_start_defaults (GTK_BOX (hbox), w->insert);
    gtk_box_pack_start_defaults (GTK_BOX (hbox), w->entry);
    gtk_box_pack_start_defaults (GTK_BOX (hbox), w->entry1);
    gtk_box_pack_start_defaults (GTK_BOX (hbox), w->entry2);
    gtk_box_pack_start_defaults (GTK_BOX (hbox), w->about);
          
    hbox2 = gtk_hbox_new (FALSE, 5);
    gtk_box_pack_start_defaults (GTK_BOX (hbox2), w->main_screen);     
    gtk_box_pack_start_defaults (GTK_BOX (hbox2), w->show);
    gtk_box_pack_start_defaults (GTK_BOX (hbox2), w->search);
    gtk_box_pack_start_defaults (GTK_BOX (hbox2), w->del);
    gtk_box_pack_start_defaults (GTK_BOX (hbox2), w->quit);
        
    vbox = gtk_vbox_new (FALSE, 5);
    gtk_box_pack_start (GTK_BOX (vbox), scrolled_win, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox2, FALSE, TRUE, 0);
    
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);  
    gtk_window_set_icon_from_file(GTK_WINDOW (window),"./icon.png",NULL); 
    
    /* Visualization */
    gtk_widget_modify_text(w->textview, GTK_STATE_NORMAL, &colorBlack); 
    
    gtk_widget_modify_base (w->show, GTK_STATE_NORMAL, &colorHellBlau);
    gtk_widget_modify_bg (w->show, GTK_STATE_PRELIGHT, &colorHellBlau);
    gtk_widget_modify_base (w->insert, GTK_STATE_NORMAL, &colorHellBlau);
    gtk_widget_modify_bg (w->insert, GTK_STATE_PRELIGHT, &colorHellBlau);
    gtk_widget_modify_base (w->search, GTK_STATE_NORMAL, &colorHellBlau);
    gtk_widget_modify_bg (w->search, GTK_STATE_PRELIGHT, &colorHellBlau);
    gtk_widget_modify_base (w->del, GTK_STATE_NORMAL, &colorHellBlau);
    gtk_widget_modify_bg (w->del, GTK_STATE_PRELIGHT, &colorHellBlau);
    gtk_widget_modify_base (w->about, GTK_STATE_NORMAL, &colorHellBlau);
    gtk_widget_modify_bg (w->about, GTK_STATE_PRELIGHT, &colorHellBlau);
    gtk_widget_modify_base (w->main_screen, GTK_STATE_NORMAL, &colorHellBlau);
    gtk_widget_modify_bg (w->main_screen, GTK_STATE_PRELIGHT, &colorHellBlau);
    gtk_widget_modify_base (w->quit, GTK_STATE_NORMAL, &colorHellBlau);
    gtk_widget_modify_bg (w->quit, GTK_STATE_PRELIGHT, &colorHellBlau);
    
    gtk_widget_show_all (window);
    
    gtk_main();
    
    return 0;
}


