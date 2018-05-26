#include <gtk/gtk.h>

#define LargeurImage 360
#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define CYAN    "\033[36m"      /* Cyan */

void PrintArrays(int *array, int N, char* str);
int NearPositionMass(int iValPix, int *boardArray, int N);
void CloudsPercentagePrint(int *nbPixMassArrayNext, int N);

void ComputeImage(guchar *pucImaOrig, 
		  guint uiNbCol, 
		  guint uiNbLine,
		  guchar *pucImaRes);

void ComputeImageKmean(guchar *pucImaOrig, 
		  guint uiNbCol, 
		  guint uiNbLine,
		  guchar *pucImaRes);
