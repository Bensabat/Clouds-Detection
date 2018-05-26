#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include "compute.h"


// Check if a char* is a bmp path
int is_bmp_path(char* path)
{
    int path_length = strlen(path);
    return (path[path_length - 1] == 'p' && path[path_length - 2] == 'm' && path[path_length - 3] == 'b');
}

// Call compute function giving a path
void auto_compute_call(char* pcFileName)
{
    int NbCol, NbLine;
    guchar *pucImaOrig, *pucImaRes;

    /* chargement en memoire de l'image */
    GdkPixbuf* pGdkPixbufImaIn, *pGdkPixbufImaRes;
    pGdkPixbufImaIn = gdk_pixbuf_new_from_file (pcFileName, NULL);

    /* echec du chargement de l'image en memoire */
    if (pGdkPixbufImaIn == NULL)
    {
        printf("Impossible de charger le fichier %s\n", pcFileName);
        exit(0);
    }

    /* copie du pixbuff original */
    pGdkPixbufImaRes=gdk_pixbuf_copy(pGdkPixbufImaIn);
    /* recuperation du nombre de lignes et de colonnes de l'image */
    NbCol = gdk_pixbuf_get_width(pGdkPixbufImaIn); 
    NbLine = gdk_pixbuf_get_height(pGdkPixbufImaIn);
    /* recuperation du tableau des pixels de l'image originale */
    pucImaOrig=gdk_pixbuf_get_pixels(pGdkPixbufImaIn);
    /* recuperation du tableau des pixels de l'image resultat */
    pucImaRes=gdk_pixbuf_get_pixels(pGdkPixbufImaRes);

    ComputeImage(pucImaOrig, NbLine, NbCol, pucImaRes);
}

int main (int argc, char **argv)
{
    char *dir_path = "../EXE/";
    printf("\n========================\n");
    printf(BLUE "CLOUDS" RESET " DETECTION PROJECT\n");
    printf("========================\n\n");    
    printf("Compute cloud percentage of .bmp files from " CYAN "%s " RESET "directory.\n", dir_path);
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dir_path)) != NULL)
    {
        // Through files and directories within
        while ((ent = readdir (dir)) != NULL)
        {
            char path[256];
            char *pre_path = dir_path;
            snprintf(path, sizeof path, "%s%s", pre_path, ent->d_name);

            if (is_bmp_path(path))
            {
                printf("\nCalculate clouds percentage for " CYAN "%s" RESET ".", ent->d_name);
                auto_compute_call(path);
            }
            else
                continue;
        }
        closedir (dir);
    }
    else
    {
        // Could not open directory
        perror ("");
        return EXIT_FAILURE;
    }

    return 0;
}
