       /*************************************************************/
       /* AUTHOR .... :  Bert van Oortmarssen                       */
       /* NAME ...... :  p2raw     									*/
       /* PROJECTS .. :  zx81                                       */
       /* DATE ...... :                                             */
       /* TASK ...... :  P to raw wav file 22kHz mono 8bits         */
       /*                                                           */
       /*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef char BYTE;


BYTE ZXc [] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";


int  OptZeroBeforeOne = 29;
int	 OptZeroBeforeNul = 29;
int  OptExtraOne      = 76;
int	 OptExtraNul      = 76;
int  OptLeadIn        = 22;
int  OptLeadOut       = 22;
char OptFileNameS[80] = "";



BYTE DoubleWav[13] = { 128, 212, 224, 153, 60, 26, 81, 175, 230, 196, 103, 32, 44 };



void RawOneOrZero (FILE *fo, int one)
{
	static  int nNul;
	static  int nOne;
	int		j, k;
	BYTE 	zero=0x80;

	k = one ? OptZeroBeforeOne : OptZeroBeforeNul;
	
	for (j=0; j< k; j++)
		fwrite (&zero, 1, 1, fo);

	if (one)
	{
		/* 9 waves is 4,5 doubles ... */
		for (j=0; j < 4; j++)
			fwrite (DoubleWav, 1, 13, fo);

 		fwrite (DoubleWav, 1, 6, fo);

		if (OptExtraOne)
		{
			k = nOne / 100;
			nOne += OptExtraOne;

			if ((nOne/100) > k)
			{
				nOne -= k*100;
				fwrite (&zero, 1, 1, fo);
			}
		}
	}
	else
	{
		for (j=0; j < 2; j++)
			fwrite (DoubleWav, 1, 13, fo);

 		if (OptExtraNul)
		{
			k = nNul / 100;
			nNul += OptExtraNul;

			if ((nNul/100) > k)
			{
				nNul -= k*100;
				fwrite (&zero, 1, 1, fo);
			}
		}
	}
}


void RawOut (FILE *fo, BYTE b)
{
	static BYTE c[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
	int			j;


	for (j=0; j < 8; j++)
		RawOneOrZero (fo, b & c[j]);
}	


#define MUCH   30000

void BytesLines (char *szfi, char *szfo)
{
    FILE   *fp;
    FILE   *fo;
    unsigned long n, k;
    BYTE   *big;


    if ((fp=fopen (szfi, "rb"))==NULL)
    {
        fprintf (stderr," *** error ***   Can't read file %s \n", szfi);
        exit(1);
    }

	if ((fo=fopen (szfo, "wt"))==NULL)
	{
		fprintf (stderr," *** error ***   Can't open output file %s \n", szfo);
		fclose (fp);
		exit(1);
	}

    if ((big = (BYTE *) malloc (MUCH))==NULL) exit(1);


	memset (big, 0x80, MUCH);

	for (k=0; k < OptLeadIn; k++)
		fwrite (big, 1, 1000, fo);


		/* do the name */
	n = strlen(OptFileNameS);

	OptFileNameS [n - 1] |= 0x80;     /* inverse character */

	for (k=0; k < n; k++)
		RawOut (fo, OptFileNameS[k]);


	do {
		n = fread (big, 1, MUCH, fp);

		for (k=0; k < n; k++)
			RawOut (fo, big[k]);


	} while (n==MUCH);

	memset (big, 0x80, MUCH);

	for (k=0; k < OptLeadOut; k++)
		fwrite (big, 1, 1000, fo);

    free (big);

	fclose (fo);
    fclose (fp);
}


void Uitleg (char *pszProgname)
{
    printf (" %s  filename.p filename.raw [options] \n", pszProgname);
    printf (" (c) berto@wirehub.nl [Bert van Oortmarssen] %s %s \n\n", __DATE__, __TIME__);
    printf ("     This will generate a raw datfile that can is accepted\n");
    printf ("     by most wave-editors. The format is currently fixed \n");
    printf ("     (mono, 8 bits, 22050 Hz)\n");
    printf (" Options: \n");
    printf ("   filename.p         // the input P-file (no checking if valid P-file)\n");
    printf ("   filename.raw       // the output file with the raw data bytes\n");
    printf ("   i=NN               // intro  silence (*1000 samples) [def=22]\n");
	printf ("   o=NN               // outtro silence (*1000 samples) [def=22]\n");
    printf ("   1=NN               // insert extra sample every Nth/100 '1' [def=76]\n");
	printf ("   0=NN               // insert extra sample every Nth/100 '0' [def=76]\n");
    printf ("   p=NN               // silence before '1' [def=29 samples]\n");
	printf ("   q=NN               // silence before '0' [def=29 samples]\n");
    printf ("   -fZXNAME           // use ZXNAME for saved name\n");
}



void MainOptions (int argc, char **argv)
{
    int n, k, val;

    for (n=3; n < argc; n++)
    {
        if (argv[n][0]=='-')
        {
            k = toupper(argv[n][1]);
            switch (k)
            {		  
               case 'F' : 
					strcpy (OptFileNameS, &argv[n][2]);
					if (OptFileNameS[0]==0)
						fprintf (stderr,"missing ZXNAME %s\n", argv[n]);
					break;

                default :
                    fprintf (stderr,"unknown option %s\n", argv[n]);
            }
        }
        else if (argv[n][1]=='=')
        {
            k = toupper(argv[n][0]);
            val = atoi(&argv[n][2]);
            switch (k)
            {
                case 'I' : OptLeadIn		= val; break;
                case 'O' : OptLeadOut		= val; break;
                case '1' : OptExtraOne		= val; break;
                case '0' : OptExtraNul		= val; break;
                case 'P' : OptZeroBeforeOne = val; break;
                case 'Q' : OptZeroBeforeNul = val; break;
                default :
                    fprintf (stderr,"unknown option %s\n", argv[n]);
            }

        }
    }
}




