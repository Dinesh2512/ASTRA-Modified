
/*
 * parse: parse simple name/value pairs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <stdint.h>
#include "cfg_parse.h"
#include "array.h"
#include "graph.h"
#include <readline/readline.h>
#include <readline/history.h>

#define BINARY_NAME "astra"

int decomposeArg ( char* line, config_param* parms );
int checkRequirements(config_param* parms);

void resetParms(config_param* params){
	FREE(params->inputFile);
	FREE(params->libFile);
	FREE(params->toolLib);
	FREE(params->toolPrimitive);
	FREE(params->moduleName);
	FREE(params->cktType);
	FREE(params->clock);
	FREE(params->reset);
	FREE(params->reports);
	FREE(params->outFile);
	params->rstEdge = -1;
	FREE(params->configFile);
	params->dont_touch = strdup("cadence");
	params->number_of_puf = 0 ;
	params->period = 0 ;
	params->slack_aware = 0 ;
	params->constrained = 0 ;
	FREE(params->timingFile);
	params->randomSeed = time(0);
	params->debug = 0;
	params->numTest = 1000;

	params->pufTemplateInsertionType = 0 ;
	params->pufTemplateType = 0 ;
	params->reuseLogic = 0 ;

	params->genericTemplateDelay = 0.11;
	params->reuseInvTemplateDelay = 0.11;
	params->reuseNandTemplateDelay = 0.11;
	params->reuseNorTemplateDelay = 0.11;
	params->TemplateDelay = params->genericTemplateDelay;

	params->regression = 0 ;

	if(params->regression_puf_numbers != NULL){
		array_free(params->regression_puf_numbers);
	}
	

	params->G = NULL ;  //free the G instead of null assigning the same
	if(params->nodesTraversed != NULL){
		array_free(params->nodesTraversed);
		params->nodesTraversed = NULL;
	}

	params->reports = strdup("../reports");

	globalPufIndex = 0 ;

	//suppress error warning info messages

	int numberOfMessages = 100; //just a placeholder number, adjust according to need

	if(params->suppress == NULL){
		params->suppress = calloc(numberOfMessages, sizeof(int));
	}else{
		free(params->suppress);
		params->suppress = calloc(numberOfMessages, sizeof(int));
	}

	//free graphs
	if(params->oldG != NULL)
		freeG(params->oldG);
	if(params->G != NULL)
		freeG(params->G);
	if(params->puf_template != NULL)		
		freeG(params->puf_template);
	if(params->inv_template != NULL)
		freeG(params->inv_template);
	if(params->nand_template != NULL)
		freeG(params->nand_template);
	if(params->nor_template != NULL)
		freeG(params->nor_template);

	//verification
	params->verification = 0;
	params->lec = 0;
	params->trace = 0 ;
	if(params->control_ports != NULL){
		array_free(params->control_ports);
	}
	if(params->status_ports != NULL){
		array_free(params->status_ports);
	}

}

int readConfig(config_param* parms){
	
	FILE * fp;
	char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
	int exit_f;

	fp = fopen(parms->configFile, "r");
    if (fp == NULL){
		printError(3);
		return 1;
	}
	
	while((read = getline(&line, &len, fp)) != -1){
		line[strcspn( line, "\n" )] = '\0';
		line[strcspn( line, "\r" )] = '\0';
		exit_f = decomposeArg(line,parms);
		if(exit_f){
			break;
		}
	}

	return 0;

}

void report_params(config_param* parms){

	char str[50];

	printInfoMsg("Input File",parms->inputFile);
	printInfoMsg("Library File ",parms->libFile);
	printInfoMsg("Tool Specific Library File ",parms->toolLib);
	printInfoMsg("Config File",parms->configFile);
	printInfoMsg("Module name",parms->moduleName);
	printInfoMsg("Circuit type",parms->cktType);
	printInfoMsg("Clock",parms->clock);
	printInfoMsg("Reset",parms->reset);
	sprintf(str,"%d",parms->rstEdge);
	printInfoMsg("Reset Edge",str);
	sprintf(str,"%d",parms->number_of_puf);
	printInfoMsg("Pufs",str);
	printInfoMsg("Puf insertion type:",parms->pufTemplateInsertionType? "Mixed mode insertion": "Non mixed mode insertion");
	sprintf(str,"%d",parms->pufTemplateType);
	printInfoMsg("Puf template type( 0-> inverter, 1-> nand, 2-> nor):",str);
	printInfoMsg("Re-use Logic:",parms->reuseLogic ? "Yes" : "No");
	sprintf(str,"%d",parms->period);
	printInfoMsg("Period",str);
	printInfoMsg("Dont Touch",parms->dont_touch);
	printInfoMsg("Puf Insertion type: ",parms->slack_aware ? "slack aware insertion":"dont care");
	printInfoMsg("Constrained: ",parms->constrained ? "Constrained":"Unconstrained");
	printInfoMsg("Timing File: ",parms->timingFile);


}


void decomposeInputArg (int argc, char** argv, config_param* parms)
{
	int i;
	int exit_f = 0 ;



	exit_f = printHelp (argc, argv);

	if(exit_f){
		exit(0);
	}

	//mandatory inputs
	
	if(argc < 3){
		if(parms->suppress[24] != 1){
			printf("ERROR[24]: Please specify config file.\n Usage : ./%s -source <path to config file>\n",BINARY_NAME);
		}
		exit(0);
	}

	

	if(!strcmp(argv[1],"-source")){
		parms->configFile = strdup(argv[2]);
		if(parms->suppress[25] != 1){
			printf("INFO[25]: Config File: %s\n",parms->configFile);
		}
			
		exit_f = readConfig(parms);

		if(exit_f){
			printError(4);
			exit(0);
		}        

	}else{
		if(parms->suppress[26] != 1){
			printf("ERROR[26]: Please specify config file.\n Usage : ./%s -source <path to config file>\n",BINARY_NAME);
		}
		exit(0);
	}
	
}

char **command_completion(const char *text, int start, int end) {
  rl_attempted_completion_over = 0;
  return rl_completion_matches(text, command_generator);
}

char* command_generator(const char *text, int state)
{
    static int list_index, len;
    char *name;

    char *character_names[] = {
    "set_hdl",
    "set_library",
    "set_tool_library",
	"set_library_primitive",
    "set_top",
	"set_clk",
	"set_rst",
	"source",
	"set_circuit_type",
	"set_puf_size",
	"set_dont_touch",
	"set_default_output_directory",
	"set_random_seed",
	"set_puf_template",
	"set_debug",
	"set_verification",
	"set_trace",
	"set_number_of_tests",
	"set_control_ports",
	"set_status_ports",
	"set_reuse_logic",
	"set_suppress_message",
	"reset_suppress_message",
	"report_params",
	"clear",
	"help",
	"remove_design",
	"start",
	"set_regression",
	"set_insertion_type",
	"set_timing_file",
	"exit",
    NULL
};

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = character_names[list_index++])) {
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }

    return NULL;
}


int decomposeArg ( char* line, config_param* parms ){
	    
		char* token ;

		if(line[0] == '#'){
			return 0;
		}

		printf("[CONFIG] >> %s\n",line);
		
		while ((token = strtok_r(line, " ", &line))){
			if(!strcmp(token,"set_hdl")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->inputFile = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_library")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->libFile = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_tool_library")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->toolLib = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_library_primitive")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->toolPrimitive = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_top")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->moduleName = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_clk")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->clock = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_rst")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->reset = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"source")){
				token = strtok_r(line, " ", &line);
				if(token != NULL){
					parms->configFile = strdup(token);
					readConfig(parms);
				}else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_circuit_type")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->inputFile = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_puf_size")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->number_of_puf = atoi(token);	
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_dont_touch")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->dont_touch = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_default_output_directory")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->reports = strdup(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_random_seed")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->randomSeed = atoi(token);
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_puf_template")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					if(!strcmp(token,"mixed_mode")){
						parms->pufTemplateInsertionType = 1;
					}else{
						parms->pufTemplateInsertionType = 0;
						//token = strtok_r(line, " ", &line);
						if(token != NULL){
							if(!strcmp(token,"inverter")){
								parms->pufTemplateType = 0 ;
							}else if(!strcmp(token,"nand")){
								parms->pufTemplateType = 1 ;
							}else if(!strcmp(token,"nor")){
								parms->pufTemplateType = 2 ;
							}else{
								printError(5);	
							}
						}	
						else
							printError(26);	
					}
					
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_debug")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					if(!strcmp(token,"true")){
						parms->debug = 1;
					}else{
						parms->debug = 0;
					}
					
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_verification")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					if(!strcmp(token,"true")){
						parms->verification = 1;
					}else{
						parms->verification = 0;
					}
					
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_lec")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					if(!strcmp(token,"true")){
						parms->lec = 1;
					}else{
						parms->lec = 0;
					}
					
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_trace")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					if(!strcmp(token,"true")){
						parms->trace = 1;
					}else{
						parms->trace = 0;
					}
					
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_number_of_tests")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->numTest = atoi(token);	
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_control_ports")){ //this generates multiple designs with the number of pufs specified with spaces 
				token = strtok_r(line, " ", &line);
				//int number = 1 ;
				if(token != NULL){
					parms->control_ports = array_alloc(char*,1);
				}else{
					printError(28);
					return 0;
				}
				
				while(token != NULL){
					array_insert_last(char*,parms->control_ports,strdup(token));					
					token = strtok_r(line, " ", &line);
				}

			}else if(!strcmp(token,"set_status_ports")){ //this generates multiple designs with the number of pufs specified with spaces 
				token = strtok_r(line, " ", &line);
				//int number = 1 ;
				if(token != NULL){
					parms->status_ports = array_alloc(char*,1);
				}else{
					printError(28);
					return 0;
				}
				
				while(token != NULL){
					array_insert_last(char*,parms->status_ports,strdup(token));					
					token = strtok_r(line, " ", &line);
				}

			}else if(!strcmp(token,"set_reuse_logic")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					if(!strcmp(token,"true")){
						parms->reuseLogic = 1;
					}else{
						parms->reuseLogic = 0;
					}
					
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_suppress_message")){ //this generates multiple designs with the number of pufs specified with spaces 
				token = strtok_r(line, " ", &line);
				if(token != NULL){
					while(token != NULL){
						parms->suppress[atoi(token)] = 1 ;
						token = strtok_r(line, " ", &line);
					}
				}else{
					printError(28);
					return 0;
				}						
			}else if(!strcmp(token,"reset_suppress_message")){ //this generates multiple designs with the number of pufs specified with spaces 
				token = strtok_r(line, " ", &line);
				if(token != NULL){
					while(token != NULL){
						parms->suppress[atoi(token)] = 0 ;
						token = strtok_r(line, " ", &line);
					}
				}else{
					printError(28);
					return 0;
				}						
			}else if(!strcmp(token,"report_params")){
				report_params(parms);
			}else if(!strcmp(token,"clear")){
				system("clear");
			}else if(!strcmp(token,"help")){
				//printInfoMsg("Men at work.","");
				char *help[] = {"-help","-help"};
				printHelp(2,&help);
			}else if(!strcmp(token,"remove_design")){
				printInfoMsg("All parameters and designs removed.","");
				resetParms(parms);
			}else if(!strcmp(token,"start")){
				if(checkRequirements(parms)){
					//obfuscate(parms);
					start(parms);
				}
				
			}else if(!strcmp(token,"set_regression")){ //this generates multiple designs with the number of pufs specified with spaces 
				token = strtok_r(line, " ", &line);
				//int number = 1 ;
				if(token != NULL){
					parms->regression = 1 ;
					parms->regression_puf_numbers = array_alloc(int,1);
				}else{
					printError(28);
					return 0;
				}
				
				while(token != NULL){
					array_insert_last(int,parms->regression_puf_numbers,atoi(token));					
					token = strtok_r(line, " ", &line);
				}

			}else if(!strcmp(token,"set_insertion_type")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					if(!strcmp(token,"slack_aware")){
						parms->slack_aware = 1;
						parms->constrained = 1 ;
						//token = strtok_r(line, " ", &line);
						// if(token != NULL){
						// 	if(!strcmp(token,"constrained")){
						// 		parms->constrained = 1 ;
						// 	}else{
						// 		parms->constrained = 0 ;
						// 	}
						// }else{
						// 		parms->constrained = 0 ;
						// }
					}else{
						parms->slack_aware = 0;
						parms->constrained = 0;
					}
					
				else
					printError(26);
				//parms->allDone++ ;
			}else if(!strcmp(token,"set_timing_file")){
				token = strtok_r(line, " ", &line);
				if(token != NULL)
					parms->timingFile = strdup(token);										
				else
					printError(26);
				//parms->allDone++ ;
			}else{
				printErrorMsg(token);
				printError(5);
			}
		}

		return 0;
        
}

void printErrorMsg(char* errorMsg)
{
	printf("ERROR:%s\n",errorMsg);
}

void printWarningMsg(char* errorMsg){
	printf("WARNING:%s\n",errorMsg);
}

void printInfoMsg(char* errorMsg, char* errorMsg1)
{
	printf("INFO: %s %s\n",errorMsg,errorMsg1);
}


void printError(int errorCode)
{
	switch(errorCode) {

	case -1:
		printf("ERROR: Unknown flag(s).\n");
		break; 
	case -2 :
		printf("ERROR: Both -clk and -rst flags need to be specified.\n");
		break;
	
	case 1 :
		printf("ERROR: All required flags are not specified.\n");
		break;
	case 2 :
		printf("ERROR: Number of flags exceeds the maximum.\n");
		break;
	case 3 :
		printf("ERROR: Flag error: -config. Could not read config file.\n");
		break;
	case 4 :
		printf("ERROR: Exiting.\n");
		break;
	case 5 :
		printf("ERROR: Unknown argument.\n");
		break;	
	case 6 :
		printf("ERROR: There are no obfuscatable if blocks in the top module. Cannot proceed with obfuscation.\n");
		break;	
	case 7 :
		printf("ERROR: Cannot open the key vector file. Could not write the keys.\n");
		break;								
	case 21 :
		printf("ERROR: Flag error: -inputFile not present.\n");
		break;
	case 22 :
		printf("ERROR: Flag error: -moduleName not specified.\n");
		break;
	case 23 :
		printf("ERROR: Flag error: -clock not specified.\n");
		break;
	case 24 :
		printf("ERROR: Flag error: -reset not specified.\n");
		break;
	case 25 :
		printf("ERROR: Flag error: -libFile not present.\n");
		break;
	case 26 :
		printf("ERROR: Argument value missing.\n");
		break;	
	case 27 :
		printf("ERROR: Flag error: -number_of_puf not specified.\n");
		break;	
	case 28 :
		printf("ERROR: Flag error: -Regression flag specified with no other parameters.\n");
		break;	
	case 29 :
		printf("ERROR: Could not open synopsis timing file.\n");
		break;					
	default : 
		printf("ERROR: Unknown error.\n");
	}
	
}


int printHelp (int argc, char **argv)
{
	int i;
	for(i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-help")) // || !strcmp(argv[i], "-h"))
		{
			printf("Usage: ./%s [option1 option2 ...]\n", BINARY_NAME);
			printf("Options:\n");
			printf("\t-config <Location/of/Configuration/File.txt>\n");		
			printf("\t-help\n");
			//printf("\t-debug\n");
			//printf("\t-hier\n");
			//printf("\t-verify\n");	
			//printf("Exiting...\n");			
			//exit(0);
			return 1 ;
		}
	}	

	return 0;	
}



int checkRequirements(config_param* parms){

	int exit_f = 0 ;
	int i;
	int numberConditions = 7 ;

	for(i=0 ; i < numberConditions ; i++){
		switch(i){
			case 0:
				if(parms->inputFile == NULL){
					printError(21);	
					exit_f = 1;	
				}
				
				break;
			case 1:
				if(parms->moduleName == NULL){
					printError(22);
					exit_f = 1;
				}
				
				break;
			case 2:
				if(parms->clock == NULL){
					printError(23);
					exit_f = 1;
				}
				
				break;
			case 3:
				if(parms->reset == NULL){
					printError(24);	
					exit_f = 1;
				}
				
				break;
			case 4:
				if(parms->libFile == NULL){
					printError(25);
					exit_f = 1;
				}				
				break;
			case 5:
				if(parms->regression == 0 && parms->number_of_puf == 0){
					printError(27);
					exit_f = 1;
				}				
				break;	
			case 6:
				if(parms->regression){
					
					if(array_n(parms->regression_puf_numbers) == 0){
						printError(28);
						exit_f = 1;
					}
					
				}				
				break;			
		}
	}

	if(exit_f){
		printError(1);
	}

	return !exit_f;

}