#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>
#define  PICSIZE 256
#define  MAXMASK 100

         int    pic[PICSIZE][PICSIZE];
         double outpicx[PICSIZE][PICSIZE];
         double outpicy[PICSIZE][PICSIZE];
         int    edgeflag[PICSIZE][PICSIZE];
         double maskx[MAXMASK][MAXMASK];
         double masky[MAXMASK][MAXMASK];
         double peaks[PICSIZE][PICSIZE];
         double ival[PICSIZE][PICSIZE];
         double finalEdges[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
        int     i,j,l,k,p,q,s,t,mr,centx,centy,moretodo;
        double  maskxval,maskyval,sum1,sum2,sig,maxival,minival,maxval,slope,low_threshold,high_threshold,Percent;
        FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
        char    *foobar;

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
        sig = atof(foobar);

        argc--; argv++;
        foobar = *argv;
        Percent = atof(foobar);

        mr = (int)(sig * 3);
        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);

        for(i=0; i<15;i++)
          getc(fp1);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  pic[i][j]  =  getc (fp1);
                }
        }

        for (p=-mr;p<=mr;p++)
        {  for (q=-mr;q<=mr;q++)
           {
              maskxval = (q)*(exp((-1*((p*p)+(q*q)))/(2*(sig*sig))));
              maskyval = (p)*(exp((-1*((p*p)+(q*q)))/(2*(sig*sig))));
              (maskx[p+centy][q+centx]) = maskxval;
              (masky[p+centy][q+centx]) = maskyval;
           }
        }

        for (i=mr;i<=255-mr;i++)
        { for (j=mr;j<=255-mr;j++)
          {
             sum1 = 0;
             sum2 = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+centy][q+centx];
                   sum2 += pic[i+p][j+q] * masky[p+centy][q+centx];
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

        fprintf(fo1,"P5\n");
        fprintf(fo1,"%d %d\n", 256, 256);
        fprintf(fo1,"255\n");

        for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {
             ival[i][j] = (ival[i][j] / maxival) * 255;
             fprintf(fo1,"%c",(char)((int)(ival[i][j])));
            }
          }

        //part 2
        //find peaks
        for (i=mr;i<256-mr;i++)
        {
            for (j=mr;j<256-mr;j++)
            {
                if((outpicx[i][j]) == 0.0)
                {
                    outpicx[i][j] = 0.00001;
                }
                slope = outpicy[i][j]/outpicx[i][j];
                if((slope <= 0.4142) && (slope > -0.4142))
                {
                    if((ival[i][j] > ival[i][j-1]) && (ival[i][j] > ival[i][j+1]))
                    {
                      peaks[i][j] = 255;
                    }
                }
                else if((slope <= 2.4142) && (slope > 0.4142))
                {
                    if((ival[i][j] > ival[i-1][j-1]) && (ival[i][j] > ival[i+1][j+1]))
                    {
                        peaks[i][j] = 255;
                    }
                }
                else if((slope <= -0.4142) && (slope > -2.4142))
                {
                    if((ival[i][j] > ival[i+1][j-1]) && (ival[i][j] > ival[i-1][j+1]))
                    {
                        peaks[i][j] = 255;
                    }
                }
                else
                {
                    if((ival[i][j] > ival[i-1][j]) && (ival[i][j] > ival[i+1][j]))
                    {
                        peaks[i][j] = 255;
                    }
                }
            }
        }

        //Header for file
        fprintf(fo2,"P5\n");
        fprintf(fo2,"%d %d\n", 256, 256);
        fprintf(fo2,"255\n");

        for (i=0;i<PICSIZE;i++)
        {
            for (j=0;j<PICSIZE;j++)
            {
                if (peaks[i][j] == 255)
                    fprintf(fo2,"%c",(char)((int)(255)));
                else
                    fprintf(fo2,"%c",(char)((int)(0)));
            }
        }

        //part 4
        int Histogram[PICSIZE] = { 0 };
        for (i=0; i<256; i++)
        {
            for (j=0; j<256; j++)
            {
                (Histogram[(int)ival[i][j]])++;
            }
        }
        double CutOff = Percent*PICSIZE*PICSIZE;
        double AreaOfTops = 0;
        for(i=255;i>0;i--)
        {
            AreaOfTops += Histogram[i];
            if(AreaOfTops>CutOff)
            {
                break;
            }
        }
        high_threshold = i;
        low_threshold = 0.35*high_threshold;

        //part 3
        for (i=0; i<256; i++)
            {
                for (j=0; j<256; j++)
                {
                    if(peaks[i][j] == 255)
                    {
                        if(ival[i][j] > high_threshold)
                        {
                            peaks[i][j] = 0;
                            finalEdges[i][j] = 255;
                        }
                        else if(ival[i][j] < low_threshold)
                        {
                            finalEdges[i][j] = peaks[i][j] = 0;
                        }
                    }
                }
            }
            moretodo = 1;
            while(moretodo == 1)
            {
                moretodo = 0;
                for (i=0; i<256; i++)
                {
                    for (j=0; j<256; j++)
                    {
                        if(peaks[i][j] == 255)
                        {
                            for (k=-1; k<=1; k++)
                            {
                                for (l=-1; l<=1; l++)
                                {
                                    if(finalEdges[i+k][j+l] == 255)
                                    {
                                        peaks[i][j] = 0;
                                        finalEdges[i][j] = 255;
                                        moretodo = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            fprintf(fo3,"P5\n");
            fprintf(fo3,"%d %d\n", 256, 256);
            fprintf(fo3,"255\n");

            for (i=0; i<256; i++)
            {
                for (j=0; j<256; j++)
                {
                    fprintf(fo3,"%c",(char)((int)(finalEdges[i][j])));
                }
            }
}
