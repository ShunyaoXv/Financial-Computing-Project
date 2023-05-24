#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gnuplot_linux.h"
#include <vector>
#include "Vector.h"
#include <cstdio>

using namespace std;

    void gnuplot_func::inityValue_CAAR(vector<vector<Vector>> V){
        // orginal data structure vector<vector<Vec>> V; 
        // alocate memory for y
        yData = (double*) malloc((nIntervals+1)*sizeof(double));
        yData2 = (double*) malloc((nIntervals+1)*sizeof(double));
        yData3 = (double*) malloc((nIntervals+1)*sizeof(double));
            
        Vec BeatV = V[0][2].data;
        Vec MissV = V[2][2].data;
        Vec MeetV = V[1][2].data;
    
        //init the y value for Beat, yData
        Vec::iterator itr1;
        int count1 = 0;
        for(itr1 = BeatV.begin(); itr1 != BeatV.end(); itr1++){
            yData[count1] = *itr1;
            count1 ++;
        }
    
        //init the y value for Miss, yData1
        Vec::iterator itr2;
        int count2 = 0;
        for(itr2 = MissV.begin(); itr2 != MissV.end(); itr2++){
            yData2[count2] = *itr2;
            count2 ++;
        }
    
        //init the y value for Meet, yData2
        Vec::iterator itr3;
        int count3 = 0;
        for(itr3 = MeetV.begin(); itr3 != MeetV.end(); itr3++){
            yData3[count3] = *itr3;
            count3 ++;
        }
    
    }
    
    
    void gnuplot_func::inityValue_CAARstd(vector<vector<Vector>> V){
        // orginal data structure vector<vector<Vec>> V; 
        // alocate memory for y
        yData = (double*) malloc((nIntervals+1)*sizeof(double));
        yData2 = (double*) malloc((nIntervals+1)*sizeof(double));
        yData3 = (double*) malloc((nIntervals+1)*sizeof(double));
            
        Vec BeatV = V[0][3].data;
        Vec MissV = V[2][3].data;
        Vec MeetV = V[1][3].data;
    
        //init the y value for Beat, yData
        Vec::iterator itr1;
        int count1 = 0;
        for(itr1 = BeatV.begin(); itr1 != BeatV.end(); itr1++){
            yData[count1] = *itr1;
            count1 ++;
        }
    
        //init the y value for Miss, yData1
        Vec::iterator itr2;
        int count2 = 0;
        for(itr2 = MissV.begin(); itr2 != MissV.end(); itr2++){
            yData2[count2] = *itr2;
            count2 ++;
        }
    
        //init the y value for Meet, yData2
        Vec::iterator itr3;
        int count3 = 0;
        for(itr3 = MeetV.begin(); itr3 != MeetV.end(); itr3++){
            yData3[count3] = *itr3;
            count3 ++;
        }
    }
    
    
    void gnuplot_func::plotResults_CAAR() {
        int dataSize = nIntervals-1;
        FILE *gnuplotPipe,*tempDataFile;
        const char *BeatDataFile = "Beat";
        const char *MissDataFile = "Miss";
        const char *MeetDataFile = "Meet";
    
        double x, y, x2, y2, x3, y3;
        int i;
        const char *title = "CAAR Mean of 3 Groups";
        const char *yLabel = "CAAR Mean Value";
    
        gnuplotPipe = popen("gnuplot -persist", "w");
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "set xrange [-%d:%d]\n", N-1, N);
        fprintf(gnuplotPipe, "set title '%s'\n", title);
        fprintf(gnuplotPipe, "set arrow from 0,graph(0,0) to 0,graph(1,1) nohead lc rgb 'red'\n");
        fprintf(gnuplotPipe, "set xlabel 'n Days(# Days from Announcement Date)'\nset ylabel '%s'\n", yLabel);
        if (gnuplotPipe) {
            fprintf(gnuplotPipe,"plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n",BeatDataFile, 
                    MissDataFile, MeetDataFile);
            fflush(gnuplotPipe);
        
            //plot Beat Group
            tempDataFile = fopen(BeatDataFile,"w");
            for (i=0; i <= dataSize; i++) {
                x = xData[i];
                y = yData[i];
                fprintf(tempDataFile,"%lf %lf\n",x,y);
            }
            fclose(tempDataFile);
        
            //plot Miss Group
            tempDataFile = fopen(MissDataFile,"w");
            for (i=0; i <= dataSize; i++) {
                x2 = xData[i];
                y2 = yData2[i];
                fprintf(tempDataFile,"%lf %lf\n",x2,y2);
            }
            fclose(tempDataFile);
        
            //plot Meet Group
            tempDataFile = fopen(MeetDataFile,"w");
            for (i=0; i <= dataSize; i++) {
                x3 = xData[i];
                y3 = yData3[i];
                fprintf(tempDataFile,"%lf %lf\n",x3,y3);
            }
            fclose(tempDataFile);
        
            printf("press enter to continue...");
            getchar();
            remove(BeatDataFile);
            remove(MissDataFile);
            remove(MeetDataFile);
            fprintf(gnuplotPipe,"exit \n");
        } else {        
            printf("gnuplot not found...");    
        }
    } 
    
    
    void gnuplot_func::plotResults_CAARstd() {
        int dataSize = nIntervals-1;
        FILE *gnuplotPipe1,*tempDataFile1;
        const char *BeatDataFile1 = "Beat";
        const char *MissDataFile1 = "Miss";
        const char *MeetDataFile1 = "Meet";
    
        double x, y, x2, y2, x3, y3;
        int i;
        const char *title = "std for CAAR of 3 Groups";
        const char *yLabel = "std for CAAR";
    
        gnuplotPipe1 = popen("gnuplot -persist", "w");
        fprintf(gnuplotPipe1, "set grid\n");
        fprintf(gnuplotPipe1, "set xrange [-%d:%d]\n", N-1, N);
        fprintf(gnuplotPipe1, "set title '%s'\n", title);
        fprintf(gnuplotPipe1, "set arrow from 0,graph(0,0) to 0,graph(1,1) nohead lc rgb 'red'\n");
        fprintf(gnuplotPipe1, "set xlabel 'n Days(# Days from Announcement Date)'\nset ylabel '%s'\n", yLabel);
        if (gnuplotPipe1) {
            fprintf(gnuplotPipe1,"plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n",BeatDataFile1, 
                    MissDataFile1, MeetDataFile1);
            fflush(gnuplotPipe1);
        
            //plot Beat Group
            tempDataFile1 = fopen(BeatDataFile1,"w");
            for (i=0; i <= dataSize; i++) {
                x = xData[i];
                y = yData[i];
                fprintf(tempDataFile1,"%lf %lf\n",x,y);
            }
            fclose(tempDataFile1);
        
            //plot Miss Group
            tempDataFile1 = fopen(MissDataFile1,"w");
            for (i=0; i <= dataSize; i++) {
                x2 = xData[i];
                y2 = yData2[i];
                fprintf(tempDataFile1,"%lf %lf\n",x2,y2);
            }
            fclose(tempDataFile1);
        
            //plot Meet Group
            tempDataFile1 = fopen(MeetDataFile1,"w");
            for (i=0; i <= dataSize; i++) {
                x3 = xData[i];
                y3 = yData3[i];
                fprintf(tempDataFile1,"%lf %lf\n",x3,y3);
            }
            fclose(tempDataFile1);
        
            printf("press enter to continue...");
            getchar();
            remove(BeatDataFile1);
            remove(MissDataFile1);
            remove(MeetDataFile1);
            fprintf(gnuplotPipe1,"exit \n");
        } else {        
            printf("gnuplot not found...");    
        }
    } 


