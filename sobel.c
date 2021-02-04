#include <stdio.h>                          /* Sobel.c */
#include <math.h>

        int pic[256][256];
        int outpicx[256][256];
        int outpicy[256][256];
        int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
        int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
        double ival[256][256], image1[256][256], image2[256][256], maxival;

void make_image_file(FILE *fo, double arr[256][256], double threshold)
{
  int i, j;
  //Header for file
  fprintf(fo,"P5\n");
  fprintf(fo,"%d %d\n", 256, 256);
  fprintf(fo,"255\n");

  for (i=0;i<256;i++)
    { for (j=0;j<256;j++)
      {
       if(threshold == 0) {
         ;//do nothing pass magnitude
       }
       else if(arr[i][j] > threshold) {
         //greater than threshold so mark as edge
         arr[i][j] = 255;
       }
       else if(arr[i][j] < threshold) {
         //less than threshold so do not mark as edge
         arr[i][j] = 0;
       }
       fprintf(fo,"%c",(char)((int)(arr[i][j])));
      }
    }
}

main(argc,argv)
int argc;
char **argv;
{
        int i,j,p,q,mr,sum1,sum2;
        double low_threshold, high_threshold;
        FILE *fo1, *fo2, *fo3, *fp1, *fopen();
        char *foobar;

        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

	       argc--; argv++;
	       foobar = *argv;
	       fo1=fopen(foobar,"wb");

         argc--; argv++;
	       foobar = *argv;
	       fo2=fopen(foobar,"wb");

         argc--; argv++;
	       foobar = *argv;
	       fo3=fopen(foobar,"wb");

         argc--; argv++;
	       foobar = *argv;
	       low_threshold = atof(foobar);

         argc--; argv++;
	       foobar = *argv;
	       high_threshold = atof(foobar);

        for(i=0;i<15;i++)
          getc(fp1);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  pic[i][j]  =  getc (fp1);
                  pic[i][j]  &= 0377;
                }
        }

        mr = 1;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             sum1 = 0;
             sum2 = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                   sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
             }
             outpicx[i][j] = sum1;
             outpicy[i][j] = sum2;
          }
        }

        maxival = 0;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));
             if (ival[i][j] > maxival)
                maxival = ival[i][j];

           }
        }

        for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {
             ival[i][j] = (ival[i][j] / maxival) * 255;
             image1[i][j] = image2[i][j] = ival[i][j];
           }
         }

    make_image_file(fo1, ival, 0);
    make_image_file(fo2, image1, low_threshold);
    make_image_file(fo3, image2, high_threshold);
}
