// ©Copyright 2020 University of Florida Research Foundation, Inc. All Rights Reserved.

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "array.h"
#include "graph.h"

#define IN 0
#define OUT 1

/*

typedef struct libSt
{
    double area;
    char* name;
    int pincount;
    int inputs;
    array_t* capacitance;
    array_t* leakage;
    array_t* direction;
    array_t* rise_capacitance;
    array_t* fall_capaciatance;
    double max_capacitance;
    array_t* cellrisevalues;
    array_t* cellfallvalues;
    array_t* risetxnvalues;
    array_t* falltxnvalues;
    array_t* risepower;
    array_t* fallpower;

} libs;

*/

array_t* breakInWords(char* header)
{
    array_t* wordArr = array_alloc(char*, 1);
    char* word = (char*)malloc(sizeof(char) * 120);
    int i, j, k;

    word[0] = '\0';
    i = j = 0;

    /* get rid of initial blanks */
    while(header[i] == ' ')
        i++;

    while(i < strlen(header))
    {
        if(header[i] == ' ')
        {
            word[j] = '\0';
            array_insert_last(char*, wordArr, strdup(word));
            j = 0;

            i++;
            while(header[i] == ' ')
                i++;
        }
        else
        {
           word[j++] = header[i];
           i++;
        }
    }

    /* add the last word */
    word[j] = '\0';
    array_insert_last(char*, wordArr, strdup(word));

    return wordArr;
}

int main() {
    
    FILE* libFile=fopen("leda.lib","r");
    char* header = (char*)malloc(sizeof(char)*400);
    array_t* wordArr;
    while(!feof(libFile)){
        fscanf(libFile,"%[^\n]\n",header);
        if(strncasecmp(header,"cell(",7)==0)
	{
		wordArr=breakInWords(header);
        wordArr=breakI
		//printf("%s\n",wordArr);
	}

    }
    return 0;
}

