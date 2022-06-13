#include "interface.h"

enum {
  COLUMN_INDEX,
  COLUMN_NAME,
  COLUMN_TIME,
  N_COLUMNS
};

typedef struct Informations
{
    GtkWindow* main_window;
    GtkWindow* sub_window;
    GtkWindow* username_window;
    GtkWindow* result_window;

    GtkButton* main_new;
    GtkButton* main_new_comp;
    GtkButton* sub_confirm;
    GtkButton* sub_cancel;
    GtkButton* main_confirm;
    GtkButton* result_save;
    GtkButton* username_confirm;

    GtkEntry* sub_nom;
    GtkEntry* sub_duree;
    GtkEntry* sub_anterio;
    GtkEntry* nb_tot_comp;
    GtkEntry* username_get;

    GtkLabel* main_number;
    GtkLabel* sub_index;
    GtkLabel* username_error;
    GtkLabel* main_error;

    SDL_Surface* image;

    GtkTreeView* main_tree;

    GtkScrollbar* scroll;

    GtkListStore* list;   

    unsigned long nb;
    unsigned long nb_tot;

    char* username;

    struct list *sentinel;
    
} Informations;

void save(GtkButton *button, gpointer user_data)
{
    (void)(button);
    Informations* info = user_data;
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Save File",
                                        info->result_window,
                                        action,
                                        ("_Cancel"),
                                        GTK_RESPONSE_CANCEL,
                                        ("_Save"),
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);
    chooser = GTK_FILE_CHOOSER (dialog);

    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

    gtk_file_chooser_set_current_name (chooser,"Untitled document");

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (chooser);
        SDL_SaveBMP(info->image,filename);
        free(filename);
    }

    gtk_widget_destroy (dialog);
}

void get_user (GtkButton *button, gpointer user_data)
{
    (void)(button);
    Informations* info = user_data;
    info->username = (char *) gtk_entry_get_text(GTK_ENTRY(info->username_get));

    char* nb_to = (char *) gtk_entry_get_text(GTK_ENTRY(info->nb_tot_comp));
    char* ptr;
    unsigned long nb_tot = strtoul(nb_to,&ptr,0);

    if (nb_tot == 0)
    {
        gtk_entry_set_text(GTK_ENTRY(info->nb_tot_comp),"");
        gtk_widget_show(GTK_WIDGET(info->username_error));
    }
    else
    {
        info->nb_tot = nb_tot;
        gtk_widget_hide(GTK_WIDGET(info -> username_window));
        gtk_widget_show(GTK_WIDGET(info->main_tree));
        gtk_widget_show(GTK_WIDGET(info->scroll));
        gtk_widget_show(GTK_WIDGET(info -> main_number));
        gtk_widget_show(GTK_WIDGET(info -> main_new_comp));
        
    }
}

void new(GtkButton *button, gpointer user_data)
{
    (void)(button);
    Informations* info = user_data;
    gtk_widget_hide(GTK_WIDGET(info -> main_new));
    gtk_widget_show(GTK_WIDGET(info -> username_window));
    list* senti = malloc(sizeof(list));
    senti->data_pert = NULL;
    senti->next = NULL;
    
    info->sentinel = senti;
}

void new_comp(GtkButton *button, gpointer user_data)
{
    (void)(button);
    Informations* info = user_data;
    gtk_widget_hide(GTK_WIDGET(info -> main_new_comp));
    gtk_widget_hide(GTK_WIDGET(info -> main_confirm));

    //Changement label index
    unsigned long temp = info->nb+1;
    unsigned long nb = 7;
    while (temp > 0)
    {
        nb++;
        temp = temp / 10;
    }

    char str[nb+1];
    sprintf(str,"Index: %ld",info->nb+1);
    gtk_label_set_text(info->sub_index,str);


    gtk_widget_show(GTK_WIDGET(info -> sub_window));
}

void sub_close(GtkButton *button, gpointer user_data)
{
    (void)(button);
    Informations* info = user_data;

    if (info->nb < info->nb_tot)
        gtk_widget_show(GTK_WIDGET(info -> main_new_comp));
    else
        gtk_widget_show(GTK_WIDGET(info -> main_confirm));
    gtk_widget_hide(GTK_WIDGET(info -> sub_window));

    //Clear les entry
    gtk_entry_set_text(GTK_ENTRY(info->sub_nom),"");
    gtk_entry_set_text(GTK_ENTRY(info->sub_duree),"");
    gtk_entry_set_text(GTK_ENTRY(info->sub_anterio),"");
}

void confirm_m(GtkButton *button, gpointer user_data)
{
    //TODO
    (void)(button);
    Informations* info = user_data;

    gtk_widget_hide(GTK_WIDGET(info -> main_window));

    traitement(info->sentinel,info->nb_tot);

    gtk_widget_show(GTK_WIDGET(info -> result_window));
}

void list_actualis(list* sentinel, data_pert *donnee)
{
    struct list *temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;

    struct list *new_c = malloc(sizeof(list));
    new_c -> data_pert = donnee;
    new_c -> next = NULL;
    
    temp->next = new_c;
}

int parseur(struct data_pert* data, char* str, unsigned long nb_compo)
{
    data->anterio = calloc(nb_compo, sizeof(unsigned char));
    char current = str[0];
    int index = 0;
    unsigned long anter = 0;
    int passed = 0;
    int in_number = 0;
    while (current != '\0')
    {
        //We read the number
        if(current >= 48 && current <= 57 && !passed)
        {
            anter *= 10;
            anter += current - 48;
            in_number = 1;
        }
        //There is a space between numbers
        else if(current >= 48 && current <= 57 && passed)
        {
            return 0;
        }
        //end of anterior
        else if(current == '/')
        {
            if(anter >= data->index)
                return 0;
            data->anterio[anter] = 1;
            anter = 0;
            passed = 0;
            in_number = 0; 
        }
        //spaces
        else if(current == ' ')
        {
            if(in_number)
            {
                passed = 1;
            } 
        }
        else
        {
            return 0;
        }
        index += 1;
        current = str[index]; 
    }
    if(in_number)
    {
        if(anter >= data->index)
            return 0;
        data->anterio[anter] = 1; 
    } 
        
    return 1;
}

void confirm_s(GtkButton *button, gpointer user_data)
{
    (void)(button);
    Informations* info = user_data;

    info->nb++;

    //Actualisation du nombre sur le main
    unsigned long temp = info->nb;
    unsigned long nb = 24;
    while (temp > 0)
    {
        nb++;
        temp = temp / 10;
    }

    char str[nb+1];
    sprintf(str,"Nombre de composantes:\n\n%ld",info->nb);

    //Récupération des entry
    char* nom = (char*) gtk_entry_get_text(GTK_ENTRY(info->sub_nom));
    char* duree_s = (char *)gtk_entry_get_text(GTK_ENTRY(info->sub_duree));
    char* anterio = (char*) gtk_entry_get_text(GTK_ENTRY(info->sub_anterio));
    if (strcmp(anterio,"") == 0)
        anterio = "0";  

    char* ptr;
    unsigned long duree = strtoul(duree_s,&ptr,0);

    //Création d'un data_pert

    data_pert *donnee = malloc(sizeof(data_pert));
    donnee->anterio = NULL;
    donnee->duree = duree;
    donnee->index = info->nb;
    donnee->name = malloc(strlen(nom)+1);
    strcpy(donnee->name,nom);


    int pars = parseur(donnee,anterio,info->nb_tot);
    if (pars != 1)
    {
        free(donnee->anterio);
        free(donnee);
        info->nb--;
        gtk_widget_show(GTK_WIDGET(info->main_error));

    }
    else
    {
        list_actualis(info->sentinel,donnee);        
        gtk_label_set_text(info->main_number,str);
        gtk_widget_hide(GTK_WIDGET(info->main_error));
    }

    //Ajout de la nouvelle data à la liste
    GtkTreeIter iter;
    gtk_list_store_append (info->list, &iter);
    gtk_list_store_set (info->list, &iter,
                        COLUMN_INDEX, info->nb,
                        COLUMN_NAME, donnee->name,
                        COLUMN_TIME,  duree,
                        -1);

    //Fermeture
    sub_close(info->sub_cancel,user_data);
}

int main()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new ();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "UI.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* main_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.main_window"));
    GtkWindow* sub_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.sub_window"));
    GtkWindow* username_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.username"));
    GtkWindow* result_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.result"));

    GtkButton* main_new = GTK_BUTTON(gtk_builder_get_object(builder, "main_new"));
    GtkButton* main_new_comp = GTK_BUTTON(gtk_builder_get_object(builder, "main_new_comp"));
    GtkButton* sub_confirm = GTK_BUTTON(gtk_builder_get_object(builder, "sub_confirm"));
    GtkButton* sub_cancel = GTK_BUTTON(gtk_builder_get_object(builder, "sub_cancel"));
    GtkButton* main_confirm = GTK_BUTTON(gtk_builder_get_object(builder, "main_confirm"));
    GtkButton* result_save = GTK_BUTTON(gtk_builder_get_object(builder, "result_save"));
    GtkButton* username_confirm = GTK_BUTTON(gtk_builder_get_object(builder, "username_confirm"));

    GtkEntry* sub_nom = GTK_ENTRY(gtk_builder_get_object(builder, "sub_nom"));
    GtkEntry* nb_tot_comp = GTK_ENTRY(gtk_builder_get_object(builder, "nb_tot_comp"));
    GtkEntry* username_get = GTK_ENTRY(gtk_builder_get_object(builder, "username_get"));
    GtkEntry* sub_duree = GTK_ENTRY(gtk_builder_get_object(builder, "sub_duree"));
    GtkEntry* sub_anterio = GTK_ENTRY(gtk_builder_get_object(builder, "sub_anterio"));

    GtkLabel* main_number = GTK_LABEL(gtk_builder_get_object(builder, "main_number"));
    GtkLabel* sub_index = GTK_LABEL(gtk_builder_get_object(builder, "sub_index"));
    GtkLabel* username_error = GTK_LABEL(gtk_builder_get_object(builder, "username_error"));
    GtkLabel* main_error = GTK_LABEL(gtk_builder_get_object(builder, "main_error"));

    GtkScrollbar* scroll = GTK_SCROLLBAR(gtk_builder_get_object(builder, "scroll"));

    GtkTreeView* main_tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "main_tree"));
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    //GtkWidget *main_tree = gtk_tree_view_new();


    /* --- Column #1 --- */

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Index:",renderer, "text", COLUMN_INDEX, NULL);

    /* pack tree view column into tree view */
    gtk_tree_view_append_column(GTK_TREE_VIEW(main_tree), col);



    /* --- Column #2 --- */

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Nom:",renderer, "text", COLUMN_NAME, NULL);
    /* pack tree view column into tree view */
    gtk_tree_view_append_column(GTK_TREE_VIEW(main_tree), col);



    /* --- Column #3 --- */

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Temps:",renderer, "text", COLUMN_TIME, NULL);


    /* pack tree view column into tree view */
    gtk_tree_view_append_column(GTK_TREE_VIEW(main_tree), col);


    //GtkListStore* list = GTK_LIST_STORE(gtk_builder_get_object(builder, "list"));
    GtkListStore* list = gtk_list_store_new(N_COLUMNS, G_TYPE_ULONG, G_TYPE_STRING, G_TYPE_ULONG);
    
    gtk_tree_view_set_model (main_tree, GTK_TREE_MODEL(list));

    Informations info = 
    {
        .main_window = main_window,
        .sub_window = sub_window,
        .username_window = username_window,
        .result_window = result_window,

        .main_new = main_new,
        .main_new_comp = main_new_comp,
        .sub_confirm = sub_confirm,
        .sub_cancel = sub_cancel,
        .main_confirm = main_confirm,
        .result_save = result_save,
        .username_confirm = username_confirm,

        .sub_nom = sub_nom,
        .sub_duree = sub_duree,
        .sub_anterio = sub_anterio,
        .nb_tot_comp = nb_tot_comp,
        .username_get = username_get,

        .main_number = main_number,
        .sub_index = sub_index,
        .username_error = username_error,
        .main_error = main_error,

        .main_tree = main_tree,

        .scroll = scroll,

        .list = list,

        .nb = 0,
        .nb_tot = 0,

        .image = NULL,

        .username = NULL,

        .sentinel = NULL,
    };

    // Connects event handlers.
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(sub_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(username_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(result_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(main_new, "clicked", G_CALLBACK(new),&info);
    g_signal_connect(main_new_comp, "clicked", G_CALLBACK(new_comp),&info);
    g_signal_connect(result_save, "clicked", G_CALLBACK(save),&info);
    g_signal_connect(sub_confirm, "clicked", G_CALLBACK(confirm_s), &info);
    g_signal_connect(sub_cancel, "clicked", G_CALLBACK(sub_close), &info);
    g_signal_connect(main_confirm, "clicked", G_CALLBACK(confirm_m), &info);
    g_signal_connect(username_confirm, "clicked", G_CALLBACK(get_user), &info);

    g_object_unref(G_OBJECT(builder));
    gtk_widget_show(GTK_WIDGET(main_window));
    gtk_widget_hide(GTK_WIDGET(sub_window));
    gtk_widget_hide(GTK_WIDGET(main_tree));
    gtk_widget_hide(GTK_WIDGET(scroll));
    gtk_widget_hide(GTK_WIDGET(username_window));
    gtk_widget_hide(GTK_WIDGET(result_window));
    gtk_widget_hide(GTK_WIDGET(main_number));
    gtk_widget_hide(GTK_WIDGET(main_confirm));
    gtk_widget_hide(GTK_WIDGET(main_new_comp));


    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}

