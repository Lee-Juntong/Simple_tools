//this this edited from a 32bit bmp->coe program from WBY
//LJT has edited it to allow conversion from 24bit bmp->coe
//and the output file can be used directly by verilog and output to a 96*64 oled screen
//as I disabled some information text 


#include <windows.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
FILE *outfp;

int BMPtoCOE(char *BMPfilename, int isfinished)
{   int carry2;
    FILE *fp;
    if ((fp = fopen(BMPfilename, "rb")) == NULL)
    {
        printf("no picture found under this path\n");
        return 1;
    }
    
    char bm[2];
    fseek(fp, sizeof(BITMAPFILEHEADER), 0);
    BITMAPINFOHEADER head;//an object of BITMAPINFOHEADER type to hold several vaules to be used
    fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
    int picwidth = head.biWidth, picheight = head.biHeight;//find width and height of the picture
    short ind=head.biBitCount;//bitcount, whether it's 24bit or 32bit
    fseek(fp, 0L, 0);
    fread(bm, 2, 1, fp);

    if (bm[0] != 'B' || bm[1] != 'M' )//checks whether it's bmp
    {
        printf("this is not a bmp\n");
        return 1;
    }
    if (picheight < 0)
        picheight = -picheight;
    fseek(fp, 0x36L, 0);//0x36L == sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER),where the actual picture starts
    printf("picture dimensions:%d*%d\n", picwidth, picheight);//for checking purpose (I guess you should know the dimensions before using this code)

    int i, j;
    unsigned char r, g, b, t ;
    for (i = 0; i < picheight; i++)
        for (j = 0; j < picwidth; j++)
        {   
            
            fread(&b, 1, 1, fp);
            fread(&g, 1, 1, fp);
            fread(&r, 1, 1, fp);

            //seperate to 4 cases and ensure that the output are always 4 bit in hexadecimal i.e. add zeros in front
            if ((int)(r / 256.0 * 32)*32*64+ (int)(g / 256.0 * 64)*32+ (int)(b / 256.0 * 32)>=4096)
            fprintf(outfp, "%x", (int)(r / 256.0 * 32)*32*64+ (int)(g / 256.0 * 64)*32+ (int)(b / 256.0 * 32)); 
            else if((int)(r / 256.0 * 32)*32*64+ (int)(g / 256.0 * 64)*32+ (int)(b / 256.0 * 32)>=256)
            fprintf(outfp, "0%x", (int)(r / 256.0 * 32)*32*64+ (int)(g / 256.0 * 64)*32+ (int)(b / 256.0 * 32)); 
            else if((int)(r / 256.0 * 32)*32*64+ (int)(g / 256.0 * 64)*32+ (int)(b / 256.0 * 32)>=16)
            fprintf(outfp, "00%x", (int)(r / 256.0 * 32)*32*64+ (int)(g / 256.0 * 64)*32+ (int)(b / 256.0 * 32));
            else fprintf(outfp, "000%x", (int)(r / 256.0 * 32)*32*64+ (int)(g / 256.0 * 64)*32+ (int)(b / 256.0 * 32));


            carry2++;
            if (carry2 == 16)
                fprintf(outfp, "\n"), carry2 = 0;
            //this converts BGR in bmp to RGB in oled, 565 format. add them together to provide an 16 bit binary array.
            fprintf(outfp, " ");
 /*           if (i == picheight - 1 && j == picwidth - 1 && isfinished)
                fprintf(outfp, ";");
            else
                fprintf(outfp, ",");
           I'm using it in verilog so I don't need , and ;*/
            if(ind==32) fread(&t, 1, 1, fp);//take out transparency character in 32 bit bmp
        }

    fclose(fp);
    return 0;
}
int main()
{   
    char s[20];
    char sout[20];
    //allows only 20 characters in the string, so pls adjust it accordingly
    printf("please enter the path of bmp picture\n");
    scanf("%s",s);
   
    printf("please enter the path of the out put file\n");
    scanf("%s",sout);
    outfp = fopen(sout, "w");
    
    // fprintf(outfp, "memory_initialization_radix=16;\nmemory_initialization_vector =\n");
    BMPtoCOE(s, 1);
    system("pause");
    return 0;
}
