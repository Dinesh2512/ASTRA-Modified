#ifndef CONFIG_PARAM_H
#define CONFIG_PARAM_H

#define MAXLEN 80
#define CONFIG_FILE "config.txt"
//#include "graph.h"
#include "array.h"

typedef struct graphSt graph;

typedef struct config_paramSt
{

  //input file details

  char* inputFile; 
  char* libFile ; 
  char* toolLib ;
  char* toolPrimitive;
  char* configFile;
  char* moduleName;
  char* cktType; 
  char* clock;
  char* reset;
  int rstEdge;
  int slack_aware; //this decides slack aware insertion or dont_care
  int constrained; //if this is constrained or unconstrained
  char* timingFile; //this is the report_timing as dumped by design compiler
  int pufTemplateInsertionType; // if all of the puf types:nand,nor,inv are to be used for puf insertion. 1 for yes 0 for no. When the puf size is in excess inv type pufs will be inserted at the end of selected gates. Default -> 0 -> no mixture
  int pufTemplateType; //this is the type of PUF template that is to be inserted : nand,nor,inv. Default is inv. 0->inv,1->nand,2->nor
  int reuseLogic; //this is for using the existing logic elements to form a combinational latch to behave as a puf circuit. Default it is switched off
  int randomSeed; //this contains the seed value for random value. By default it contains time(0), this is set when reset parms is triggered.
  int debug; //this suppresses some warning messages when set to false
  int numTest; //this is the number of tests that the generated testbench file will run.
  int regression; //marks if regression mode is true or false
  array_t* regression_puf_numbers; //this will store the number of pufs that are stored for regression. It is fetched and then passed to number_of_puf each regression run
  array_t* nodesTraversed; //this is a list of nodes traversed during slacked aware puf insertion. This is used/modified in the insert_puf function only.

  //puf insertion details 

  int number_of_puf ; //the number of PUFs to be inserted in the design
  int period ; //the clock period that is to be inserted in the tcl script for genus when generating reports 
  char* dont_touch ; //pragmas for dont touch for different tools 

  //pointer to the graph
  graph*      G; 
  graph*      oldG; //in case of regression the original G is stored in here while the duplicated graph is stored in G  
  graph*    puf_template;
  graph*    inv_template; //store the different puf templates for reuse logic 
  graph*    nand_template;
  graph*    nor_template;

  //PUF template delay
  float TemplateDelay; //this needs to be set before using the check violation delay function
  float genericTemplateDelay; //currently all of them are default set to 0.11 in resetParms function
  float reuseInvTemplateDelay;
  float reuseNandTemplateDelay;
  float reuseNorTemplateDelay;

  //Output file pointer
  FILE *out;
  char* reports; //default reports folder  
  char* outFile; //name of the puf output file, without its complete path

  //suppress info,warning,error messages
  int *suppress;

  //verification
  int verification;
  int lec;
  int trace;
  array_t* control_ports;
  array_t* status_ports;

} config_param;



#endif
