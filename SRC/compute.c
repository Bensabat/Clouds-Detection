#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "compute.h"

/*******************************************************
IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
DES FONCTIONS
*******************************************************/


/*---------------------------------------
  Proto: 

  
  But: 

  Entrees: 
    --->le tableau des valeurs des pixels de l'image d'origine
	      (les lignes sont mises les unes à la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
    --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes à la suite des autres)


  Sortie:

  Rem: 

  Voir aussi:

  ---------------------------------------*/

// Print given array - Usefull for debug
void PrintArrays(int *array, int N, char *str)
{
  printf("Printing array %s: ", str);
  size_t i;
  for(i = 0; i < N; i++)
    printf("%d ", array[i]);
  printf("\n");
}

// Return the near pixel position
int NearPositionMass(int iValPix, int *boardArray, int N)
{
  size_t i;
  for(i = 0; i < N; i++)
  {
    if (boardArray[i] <= iValPix && iValPix <= boardArray[i + 1])
      break;
    else
      continue;
  }
  return i;
}

// Pretty print cloud percentage
void CloudsPercentagePrint(int *nbPixMassArrayNext, int N)
{
  size_t i;
  int acc = 0;
  for(i = 0; i < N; i++)
  {
    acc += nbPixMassArrayNext[i];
  }
  float percentage;
  float nbPix = nbPixMassArrayNext[N - 1];
  percentage = nbPix / (float)acc;
  printf("\nThis image of " CYAN "%d" RESET " pixels contains " CYAN "%d" RESET " pixels of clouds.\n", (int)acc, (int)nbPix);
  printf("That\'s around " GREEN "%.8f%%" RESET ".\n", percentage * 100);
}

// Main compute function
void ComputeImage(guchar *pucImaOrig, guint NbLine, guint NbCol, guchar *pucImaRes)
{
  int iNbPixelsTotal, iNumPix;
  int iNumChannel, iNbChannels=3; /* on travaille sur des images couleurs*/
  guchar ucMeanPix;

  //printf("Segmentation de l'image.... A vous!\n");
  
  iNbPixelsTotal=NbCol*NbLine;
  for(iNumPix=0; iNumPix<iNbPixelsTotal*iNbChannels; iNumPix=iNumPix+iNbChannels)
  {  
    /* Moyenne sur les composantes RVB */
    ucMeanPix=(unsigned char)((*(pucImaOrig+iNumPix) + *(pucImaOrig+iNumPix+1) + *(pucImaOrig+iNumPix+2))/3);
    
    /* sauvegarde du resultat */
    for(iNumChannel=0; iNumChannel<iNbChannels; iNumChannel++)
      *(pucImaRes+iNumPix+iNumChannel)= ucMeanPix;
  }

  /* --- INITIALISATION --- */
  
  int N = 5; // Number of classes
  size_t i;

  // Array that contain the N + 1 borders
  int boardArray[N + 1];
  float denominator = (float)255 / (float)N;
  for(i = 0; i < N; i++)
    boardArray[i] = denominator * i;
  boardArray[N] = 255;
  //PrintArrays(boardArray, N + 1, "boardArray");

  // Array that contain the N masses centers
  int massArray[N];
  for(i = 0; i < N; i++)
    massArray[i] = (boardArray[i] + boardArray[i + 1]) / 2;
  //PrintArrays(massArray, N, "massArray");

  // Array that will contain the N next masses centers
  int massArrayNext[N];  
  memset(massArrayNext, 0, N * sizeof(int));

  // Arrat that will contain the nb of pix at each masses classes
  int nbPixMassArrayNext[N];
  memset(nbPixMassArrayNext, 0, N * sizeof(int));
  
  /* --- CLASSIFICATION --- */
  
  // Main loop, iterate while stability
  int stability = 2;
  int debug = 100;
  
  while (debug > 0)
  {
    debug--;
    // Calculates of the near position of each pixel and acc it in array,
    // calculates of the number of pixel at each masses classes
    for(iNumPix = 0; iNumPix < iNbPixelsTotal * iNbChannels; iNumPix += iNbChannels)
    {
      int iValPix = (int)(*(pucImaRes + iNumPix));
      int near = NearPositionMass(iValPix, boardArray, N); // 0 <= near < N
      massArrayNext[near]      += iValPix;
      nbPixMassArrayNext[near] += 1;
    }

    // Calculates real masses centers by divided each of them by nb pix
    for(i = 0; i < N; i++)
    {
      if (nbPixMassArrayNext[i] == 0)
        massArrayNext[i] = 0;
      else
        massArrayNext[i] /= nbPixMassArrayNext[i];
    }
    
    // Calculates the new boarders
    for(i = 1; i < N; i++)
      boardArray[i] = (massArrayNext[i] + massArrayNext[i - 1]) / 2;

    // Calculate of the new epsilon to know if it stable or not
    int epsilon = 0;
    for(i = 0; i < N; i++)
    {
      epsilon += abs(massArrayNext[i] - massArray[i]);
    }

    // Is it stable?
    if (epsilon > stability)
    {
      // swap massArray with massArrayNext
      for(i = 0; i < N; i++)
      {
        int temp = massArray[i];
        massArray[i] = massArrayNext[i];
        massArrayNext[i] = temp;
      }

      // Reinitialisation of massArrayNext and nbPixMassArrayNext
      memset(massArrayNext, 0, N * sizeof(int));
      memset(nbPixMassArrayNext, 0, N * sizeof(int));  
      continue;
    }
    else
      break;        

  } // End while

  /* --- % CLOUDS CALCULATE AND DISPLAY --- */

  // Printing percentage of clouds
  //PrintArrays(nbPixMassArrayNext, N, "nbPixMassArrayNext");
  CloudsPercentagePrint(nbPixMassArrayNext, N);

  // Displaying classes
  
  for(iNumPix = 0; iNumPix < iNbPixelsTotal * iNbChannels; iNumPix += iNbChannels)
  {
    int iValPix = (int)(*(pucImaRes + iNumPix));
    int near = NearPositionMass(iValPix, boardArray, N); // 0 <= near < N
    int massVal = massArrayNext[near];

    // Transforming pixel to mass center
    for(iNumChannel = 0; iNumChannel < iNbChannels; iNumChannel++)
      *(pucImaRes + iNumPix + iNumChannel) = (unsigned char)massVal;
  }
  
}
