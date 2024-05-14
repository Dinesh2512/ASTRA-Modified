 //©Copyright 2020 University of Florida Research Foundation, Inc. All Rights Reserved.

/**********************************************************/
/*                                                        */
/*             Activity monitor at test points             */
/*                                                        */
/*             Last update on 11/16/2018                 */
/*                                                        */
/**********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "array.h"
#include "graph.h"
#include "cfg_parse.h"
#include <readline/readline.h>
#include <readline/history.h>

// tunable parameters
#define MEMOPT 0  // optimize memory
#define PLP 1  // optimize PLP
#define ANNEAL_PARTITION 1  // anneal partitions - only one of these three annealing should be ON
#define ANNEAL_FANOUT 0  // anneal fanouts
#define ANNEAL_CAMMEM 0

#define WORDSIZE 12
#define CROSSBAR_SIZE 32
#define NLEVEL 16
#define NUMVECTORS 100
#define VOPT 8
#define BE_FLEXIBLE 0  // with respect to input and weight

//#define BINARY_NAME "Astra"

//function declarations
int countFaninFanout(graph* G, vertex* vert, dirType dir, int nLevel);
void printHeader();

int nMaxInputs, totMinterms;
int nTruthTableEntry;
int INWEIGHT, OUTWEIGHT;
array_t* prtnInList, *prtnOutList, *prtnMintermCnt, *lvlPartitionList;


FILE* intresults;
FILE* intresults1;
FILE* noofpartf;
FILE* tmp_testgen;
int noofpart;
int noofcycles;
double memreq, cammem;
int PRTNPERLEVEL;
int LOCPRTNPERLEVEL;

/* main function */
int main(int argc, char** argv)
{
   char *line = NULL;
   int exit_f;
   clock_t     start, finish;
   double      duration, mips;

    start = clock();
    
    printHeader();

    config_param* params = (config_param*)malloc(sizeof(config_param));	
    params->suppress = NULL;
	resetParms(params);
    
    rl_attempted_completion_function = command_completion;

    // Inputs to the tool	

	if(argc > 1){

		decomposeInputArg(argc, argv, params);

		printf("\n\n%-45s : %s \n", "-- User Specified Input Design File Name\n", params->inputFile);
		//printf("(module %s)\n", topModuleName);
		if(params->configFile){
			printf("\n%-45s : %s \n", "-- User Specified Configuration File Name\n", params->configFile);
			//resetParms(params);
			//parse_config(params,params->configFile);
		}else{
			printf("\nERROR: Config file not found/not declared. Cannot proceed with RTL Locking....Exiting.\n");
			exit(0);
		}
		
	}else{ //this leads into an interactive mode like design compiler and genus tools

		while(true){

			line = readline("astra> ");
			
			if (strlen(line) != 0) {
				add_history(line);
			}


			// printf("rtl_sl> ");
			// read = getline(&line, &len, stdin);
			line[strcspn( line, "\n" )] = '\0';
			line[strcspn( line, "\r" )] = '\0';
			
			if(!strcmp("quit",line) || !strcmp("q",line) || !strcmp("exit",line)){
				break;
			}

			exit_f = decomposeArg(line,params);

			if(exit_f){
				break;
			}
			
		}

		free(line);

	}

   finish = clock();
   duration = ((double)(finish - start)) / CLOCKS_PER_SEC;
   printf("Total Runtime: %4.2f seconds\n", duration );
   printf("--------------------------------\n");
   printf("Job Finished. Exiting ... \n\n");

   return 1;
}


//this is the function that calls the puf insertion function
void start(config_param* param){

   int         i, j;
   float       MINCAP;
   int         iter = 0, terminate = 0, lastLvlIdx, nowLevel, nSeqGates = 0;
   edge*       edg;
   vertex*     vert, *v, *tmpVrt;
   path*       crtPath;
   array_t*    pathArr;
   array_t*    vertArr, *sizedCells, *topoArr;
   array_t*    edgArr, *finalPrtnList, *preFinalPrtnList;
   
   array_t*    globalEdges = array_alloc(edge*, 1);
   graph*      G;   

   int exit_f = 0 ;
   
   SEQSTATES = 0;
   globalPufIndex = 0 ;
   
   //printf("CRITICAL INFO: double check the create graph from verilog function and then and make sure that it functions properly.\n");
   G = createGraphFromVerilog(param->inputFile, param->libFile, param->moduleName, &nPrimIn, NULL);
   readPufTemplates(param);

   param->G = G ;//link the graph to the parameter config file.
   param->oldG = G;
   
   for(i = 0; i < array_n(G->vertexArr); i++)
   {
       v = array_fetch(vertex*, G->vertexArr, i);
       if (!strncasecmp(v->type->name, "dff",3) || !strncasecmp(v->type->name,"latch",5))
           nSeqGates++;
   }

    if(param->slack_aware){
       exit_f =  store_timing_paths(param);

       if(exit_f){
           printf("ERROR: Cannot proceed with Puf Insertion due to unsuccessfull operation regarding synopsis timing file. Stopping operation.\n");
           return;
       }
    }

    if(param->regression){
        int regression_size = array_n(param->regression_puf_numbers);
        param->oldG = G;
        param->G = NULL;
        
        for(int i = 0 ; i < regression_size ; i++){
            
            if(i > 0){
                freeG(param->G);
                param->moduleName = strdup(param->oldG->header); //dont know y but moduleName is cleared after freeG. param->G and param->moduleName are the same object. searcg where.
                param->G = (graph *)malloc(sizeof(graph)); //create a new graph object
            }

            globalPufIndex = 0 ;
            param->G = (graph *)malloc(sizeof(graph));
            duplicateGraph(param->oldG,param->G);

            restore_delay(param); //restore the value of delay that has been dirtied during regression
            param->number_of_puf = array_fetch(int,param->regression_puf_numbers,i);
            printf("INFO: Starting PUF insertion for requested number: %d.\n",param->number_of_puf);
            insert_puf(param);
        }
    }else{
        printf("INFO: Starting PUF insertion for requested number: %d.\n",param->number_of_puf);
        insert_puf(param);
    }
    

   
}


/* initialize vertices to be unmarked for traversal for sizing */
void initializeTraverseFlag(graph* G)
{
    int i;
    array_t* vertArr = G->vertexArr;
    vertex* vert;

    for(i = 0; i < array_n(vertArr); i++)
    {
        vert = array_fetch(vertex*, vertArr, i);
        vert->traverseFlag = false;
        vert->tmpFlag = 0;
    }
}









char* dec2bin(int number, int num)
{
   int x = 0, y = 0;
   char *str = (char*) malloc(sizeof(char) * (num+1));

   for(y = num-1; y >= 0; y--)
   {
       x = number / (1 << y);
       number = number - x * (1 << y);
           str[num-1-y] = (x) ? '1' : '0';
   }
   str[num] = '\0';
   return str;
}

int bin2dec(char* str, int num)
{
   int x = 0, m, y;

   for(y = 0; y < num; y++)
   {
           m = (str[y] == '1') ? 1 : 0;
       x += m * (1 << y);
   }

   return x;
}


// mark the input/output logic cone of sized cell. nLevel
// indicates the number of levels to mark from target cell
int countFaninFanout(graph* G, vertex* orgvert, dirType dir, int nLevel)
{
    int i, j, k, l, cnt = 0;
    array_t* edges;
    array_t* tempArr = array_alloc(vertex*, 1);
    array_t* newArr;
    vertex* vert;
    edge* edg;

    for (k = 0; k < array_n(G->vertexArr); k++)
    {
        vert = array_fetch(vertex*, G->vertexArr, k);
        vert->traverseFlag = false;
    }

    array_insert_last(vertex*, tempArr, orgvert);
    for (l = 0; l < nLevel; l++)
    {
        newArr = array_alloc(sizeof(vertex*), 1);
        for (k = 0; k < array_n(tempArr); k++)
        {
            vert = array_fetch(vertex*, tempArr, k);
            if (vert->traverseFlag) continue;
            vert->traverseFlag = true;
            cnt++;
            edges = (dir == INPUT) ? vert->inEdges : vert->outEdges;
            for(i = 0; i < array_n(edges); i++)
            {
                edg = array_fetch(edge*, edges, i);
                if (dir == INPUT && edg->fromNode)
                    array_insert_last(vertex*, newArr, edg->fromNode);
                else if (dir == OUTPUT && array_n(edg->toNodes))
                {
                    for(j = 0; j < array_n(edg->toNodes); j++)
                        array_insert_last(vertex*, newArr, array_fetch(vertex*, edg->toNodes, j));
                }
            }
        }
        array_free(tempArr);
        if (!array_n(newArr))
            break;
        tempArr = array_dup(newArr);
        array_free(newArr);
    }

    for (k = 0; k < array_n(G->vertexArr); k++)
    {
        vert = array_fetch(vertex*, G->vertexArr, k);
        vert->traverseFlag = false;
    }

    return cnt;
}

void printHeader()
{
	printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
	printf("                            ASTRA                              \n");
	printf("                   A MEL-PUF INSERTION Tool                    \n");
	printf("                         Version: 0.4\n");
	printf("                     EDA Flow: %s\n", "SYNOPSYS");
	printf("         Copyright : University of Florida, FL, USA            \n");
	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
	
}


