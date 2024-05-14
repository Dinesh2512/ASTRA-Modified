// ©Copyright 2020 University of Florida Research Foundation, Inc. All Rights Reserved.
/* define the graph structures */


#ifndef GRAPH_H
#define GRAPH_H

#define DUMPALL 0
#define true 1
#define false 0
#define TRUE 1
#define FALSE 0
#define MAXLEVEL 10 
#define MAXSIZESTEP 3 
#define NSIZESPERCELL 50
#define MAX_ITER_NO 5
#define DONT_TOUCH_SLACK 1.00e-12

// Two compile options - don't delete
 #define TESTGEN    // enable N-Detect test generation


#define NUM_ORG_FFS 30
#define setupDelay 0
#define clk2QDelay 0


#define lowWp   300e-9
#define lowWn   150e-9
#define uppWp   4e-6
#define uppWn   2e-6
#define INVAREA 8.2e-7

#include "cfg_parse.h"

/* global variables -  mainly options */
int      NDETECT;
int      bVecFilePresent;
int      SEQSTATES;
int      NTESTPTS;
int      maxTests;
int      fullScan;
int      extStimulus;
int      traceCount;
float    TRIGTHRESH;
float    inputActivity;
char*    inputFile;
char*    libFile;
char*    moduleName;
char*    cktType;
char*    someFileName; 
char* 	 someFileName2; 
char*    outFile;
char*    subckt;
char*    extStimulusFileName;
int      nPrimIn;
char     keepLog[20];
float    timeFactor;
int      isCombLoop;
int      areaTolerance;
int      delayTolerance;
float    slackMargin;
float    slackWindow;
float    targetDelay;
int      bDebug;
int      maxSizeStep;
int      numSizePerCell;
int      maxIterationNumber;
int      nMaxPatterns;
int      systemVdd;
int      numStages;
int      nParallel;
int      maxNodes;
int      MAXIN;
int      MAXOUT;
int      extraDffIn;
int      btopoOrder;
int      globalVertIdx;
int      globalEdgIdx;
int      globalPufIndex;


typedef enum direction {INPUT=1, OUTPUT=2} dirType;
typedef enum size {UPSIZE=1, DOWNSIZE=2, NOSIZE=3} sizeType;

typedef struct partitionSt
{
   int id;
   array_t*  vertexList;
   array_t*  inList;
   array_t*  outList;
   array_t*  markedInputs;
   array_t*  markedOutputs;
   array_t*  nfanout;
} partitionType;

typedef struct graphSt 
{
   char*     header;
   array_t*  vertexArr;
   array_t*  edgArr;
   array_t*  combGates;
   array_t*  seqGates;
   array_t*  nonscanDFF;      //added by Pravin on 10/7/19
   array_t*  scanDFF;         //added by Pravin on 10/7/19
   array_t*  capArr;        /* Array of capacitances in the circuit */
   array_t*  resArr;        /* Array of Resistances in the circuit */
   array_t*  primInEdges;   /* Array of Primary Inputs */
   array_t*  primOutEdges;  /* Array of Primary Outputs */
   array_t*  lvlVertArr;    /* Array of Vertex based on their level */
   array_t*  listOfPath;    /* List of Critical Path */
   array_t*  fNodes;        /* define faulty and fault-free nodes */
   array_t*  typeArr;       /* Array of type of gates in library */
   array_t*  trueLvlVertArr; // Vertex list sorted in terms of its 'levelized' levels: different from lvlVertArr
   array_t*  lowprobEdges;
   array_t*  libArr; // array of gates in the circuit read from standard cell library 
   char*     eqnFile;

   float     gatingDecoderArea;  // used in shannon's expansion
   int       isGated;
   int       hierLevel;
   int       doExpand; //indicates the controller to expand this graph
   char*     gatingCtrl;
   int       processed;

   float     highVt; 
   float     lowVt;
   float     data_required_time ;

   //atri additions
   array_t* comments; //this will store the comments that are to shown at the header of the verilog netlist
} graph;

typedef struct gateTypeSt
{
   char*     name;
   array_t*  pChannel;
   array_t*  nChannel;
   array_t*  netArr;
   array_t*  primNetArr;
   int       NoTr;        /* number of NMOS(or PMOS) transistors in a gate */
   double    WpMax;         /* Max. On resistance of PUN */
   double    WpMin;         /* Min. On resistance of PUN */
   double    WnMax;         /* Max. On resistance of PDN */
   double    WnMin;         /* Min. On resistance of PDN */
   double     WnOrg;            /* original width of PDN */
   double     WpOrg;            /* original width of PUN */
   double area;            // added by Patanjali. These are supporting datastructures
   double leakage;         //for std cell library format
   int pincount;
   int inputs;
   array_t* pinvals;

} Type;

typedef struct pins{
    char*  pinname;
    char* direction;
    double capacitance;
    double* risedelays;
    double* falldelays; 
    double* index1;
    double* index2;
    double rise_capacitance;
    double fall_capacitance;
    double max_capacitance;
    double* risetxn;
    double* falltxn;
    double* risepower;
    double* fallpower;

} pins;

typedef struct netSt
{
   char*     name;
   array_t*  pchArr;
   array_t*  nchArr;
} net;


typedef struct transistor
{
   char*     name;
   net*      drain;
   net*      gate;
   net*      source;
   double    length;
   double    width;
} trans;

typedef struct pathSt
{
   long pathIndex;
   int validity;        // 1: valid, 0: invalid
   float totalDelay;
   array_t* edgeArr;
   array_t* NodeArr;
   array_t* transArr;
   //array_t* delayArr;
   array_t* segmentDelays;
   float maxDelay;
} path;

typedef struct vertexSt
{
   int idx;
   int lvlIdx;
   int suspect;		//token for identifying suspicious structures
   int visited;
   int topo;
   int topoIdx;
   int bPrimIn;             /* if it's a primary input or not */
   int bPrimOut;
   char* name;
   Type* type;
   int isSeq;
   int bDummy;
   char* gatingCtrl;
   int bDeleted;
   int level;      /* It specifies the level of gate */ 
   int virtualFlag;     /* to detect virtual i/o node */   
   int criticalFlag;    /* to detect critical path nodes*/     
   int fDelay;
   int partitionNumber;
   int partLevel;
   int dontTouch;
   int AM;
   int rDelay;
   int traverseFlag;
   int tmpFlag;
   int borphan;
   int bdeleted;
   int bmoved;
   double delay;
   double riseDelay;
   double fallDelay;
   array_t* delayLRS; /*FIXUP */
   array_t* faultList0; 
   array_t* faultList1; 
   array_t* precludeList0; 
   array_t* precludeList1; 
   double tranLRS;
   array_t* delayArr;
   array_t* inEdges;
   array_t* outEdges;
   array_t* inPorts;
   array_t* outPorts;
   array_t* pathArr;        // list of crossing paths
   double sizeFactor;                /* optimal size multiplication factor */
   double prevSizeFactor;                /* optimal size multiplication factor */
   double mu;
   float alpha;
   double delConst;            // delay constraint: Ai's in the referred paper
   double slackConst;         // slack constraint: Si's in the algorithm
   double resistance;
   double delStd;            // gate delay standard variation(%)
   double outStd;            // output arrival time standard variation(not %, absolute value)
   float  worstSlack;
   path* worstPath;

   //vert->WnLRS represents the equivalent NMOS size of a "gate"
   //this is different from gate->Wn's
   //This value will be set by Lagrangian relaxation sizing algorithm
   double WnLRS;            
   double WnMin;
   double WnMax;
   double lowBound;
   double uppBound;

   double dlyInter;
   double *dlyInterPO;
   double dlyInterMean;
   double dlyInterStd;

   // Intra-die variation
   double dlyIntraMean;
   double dlyIntraStd;
   double tranIntraMean;
   double tranIntraStd;

   float  activity;
   float  nInput00;
   float  nInput01;
   float  nInput10;
   float  nInput11;

   int  vth;
   int isScanDFF;

   //astra instertions
   float worstDelay;
   float sulliedDelay;
   int comment;
   int pufNumber;
   

} vertex;

typedef struct edgeSt
{
   int idx;
   char* name;
   int oldVal;
   int newVal;
   int errNewVal;
   int errOldVal;
   int lowProbVal;
   struct edge* prmEdgTran;  /* primary edge responsible for transition */
   struct edgeSt* maxCcEdg;
   struct edgeSt* mainPrmIn;      /* Main PriIn for switching */
   array_t* impEdgArr;
   array_t* crtEdgArr;       /* list of critical path edges coupled with */
   int tmpAssign1;           /* Temporarily Assigned */
   int tmpAssign2;           /* Desired Transition */
   int tmpAssign2Org;
   int tmpAssign3;           /* For virtual propagation */
   float tmpTranTime; 
   float tmpTranTime2;       
   int assnTran;            /* Transition Assigned */
   int assnFlag;            /* Transition Assignment Flag */
   int tpd;                 /* propagation delay */
   int bPrimIn;
   int bPrimOut;
   int bSeqIn;
   int bSeqOut;
   int cplCapFlag;          /* Check if Cc exists */
   int gndCapFlag;          /* Check if ground Cap existx */
   vertex* fromNode;
   array_t* toNodes;
   array_t* cplCap;         /* Array of Coupling Capacitors */
   float    maxCc;
   float    maxCcConflict;
   double   gndCap;         /* Ground Capacitor */
   double   rDelayMax;
   float    rDelayMaxOrg;   /* To store the original value during simulation */
   array_t* rDlyMaxArr;
   
   array_t* rDelayMaxArr;   /* Max. Pin-to-Pin rising delay list at the output edge */ 
   array_t* rDelayMinArr;   /* Min. Pin-to-Pin rising delay list at the output edge */
  
   array_t* fDelayMaxArr;   /* Max. Pin-to-Pin falling delay list at the output edge */ 
   array_t* fDelayMinArr;   /* Min. Pin-to-Pin falling delay list at the output edge */
   double   tmpMaxDelay;    /* To temporarily store the delay for finding critical path */
   
   double tmpslack;         /* To store the relative slack information */
 
   
   float    rDelayMaxTmp1;
   double   rDelayMin;
   double   rDelayMinOrg;
   array_t* rDlyMinArr;
   float    rDelayMinTmp1;
   double   fDelayMax;
   float    fDelayMaxOrg;
   array_t* fDlyMaxArr;
   float    fDelayMaxTmp1;
   double   fDelayMin;
   double   fDelayMinOrg;
   array_t* fDlyMinArr;
   float    fDelayMinTmp1;
   double   maxDelay;
   double   gateDelayR;
   float    gateDelayRTmp;
   array_t* gateDlyRArr;
   float    gateDelayROrg;
   double   gateDelayF;
   float    gateDelayFTmp;
   array_t* gateDlyFArr;
   float    gateDelayFOrg;
   int      weight;
   float    TtimeR;         /* Output Transition time */
   float    TtimeROrg;
   float    TtimeRTmp;
   array_t* TtimeRArr;
   float    TtimeRmax;      /* Output Transition time */
   float    TtimeRmaxOrg;
   float    TtimeRmaxTmp;
   array_t* TtimeRmaxArr;
   float    TtimeRmin;      /* Output Transition time */
   float    TtimeRminOrg;
   float    TtimeRminTmp;
   array_t* TtimeRminArr;
   float    TtimeF;         /* Output Transition time */
   float    TtimeFOrg;
   float    TtimeFTmp;
   array_t* TtimeFArr;
   float    TtimeFmax;      /* Output Transition time */
   float    TtimeFmaxOrg;
   float    TtimeFmaxTmp;
   array_t* TtimeFmaxArr;
   float    TtimeFmin;      /* Output Transition time */
   float    TtimeFminOrg;
   float    TtimeFminTmp;
   array_t* TtimeFminArr;
   int      lastSample;    /* to flag last sampling time */
   array_t* switchArr;     /* to record the switching in a edge */
   array_t* valueArr; //to store values for each edge
   array_t* tmpSwitchArr;
   array_t* swTimeArr;  /* to record the time at which switching takes place */
   array_t* tmpSwTimeArr;
   array_t* prmEdgArr;     /* List of Prime Edge responsible for switch time */
   array_t* timeEdgArr;    /* List of edges affected by any primary edge */
   array_t* switchTimeArr;    /* To keep track on all assigned tran time */
   float    switchTime;       /* Switching time */
   array_t* assnValues;       /* Values Assigned to any PI */
   int      outTranMaxDelay;  /* Reqd. Transition for Max. Delay */
   double   minDelay;
   int      outTranMinDelay;  /* Reqd. Transition for Min. Delay */
   array_t* tmpPropArr;          /* To record propagation status of each tran */
   int      logicProp;        /* tmporary propagation status */
   int      crEdgConflict;     /* To record any conflict at critical edg */
   int      timeOutFlag;     /* 1: if last sampling time > desired sw time */
   int      progressEdgIndex; /* For PrimINEdg to track pregress in podem */
   float    rDelMaxVar;       /* variance of delay variation */
   float    fDelMaxVar;
   float    rDelMinVar;
   float    fDelMinVar;

   float    signal0Prob;
   float    signal1Prob;
   float    activity01;
   float    activity10;

   float    lamda;
   int done;
} edge;

typedef struct parasiteSt
{
   char* name;
   edge* fromEdge;
   edge* toEdge;
   double value;
} parasite;


/*typedef struct partList
{
   int id;
   array_t* vertexList;
   array_t* inList;
   array_t* outList;
} partitionType; */

typedef struct edgListSt
{
   array_t* edgListTran;
   array_t* edgListTime;
} edgList;

typedef struct jFrontSt
{
   vertex*  node;
   int      tran;
   float    time;
   array_t* edgArr;   /* Edges already tried */
}  jFront;


//Atri - global variables
int      pufPrimIn;
//int      puf_insert; //Atri: This flag is for enabling the insertion of puf 
//int      synthesize;//Atri: This is for synthesizing the given netlist
//int      number_of_pufs; //Atri: This is the default number of pufs
//float    period; //Atri: This is the default period of the clock used in the tcl script
//char*    isSynth; //Atri: This is for determining if the input design file needs to be synthesised or not 
//char*    dont_touch;//Atri: Which tool to implement the dont_touch feature for 

//Atri:structures
// struct inv_instance_record{//this has the same number of variables hardcoded as puf_target_instance[] 
//     int *empty; //this says if there are any inverter cell have been identified from the netlist. starting value should be 1 and if any is identified then it should be updated
//     int *type_num; //this would record the number of unique inverters cells present in the netlist 
//     int **type_index; //this stores the index number of the that specific type of inverter cell encounter in the verilog netlist 
// };

typedef struct idListSt { //this is a list of all of the idx that have have been selected for puf insertion when puf excess = 1
   int idx; //this stores the id of the vertices that have been selected for puf insertion
   int type; // 1 = inverter instance, 0 = non inverter instance
   int inv_type; //this stores the type of inverter instance eg inx1,invx2 and so on

} idList;

typedef struct traversedNodesST{
   int idx; //the id of the node/vertex
   int selected; // 0 = not selected , 1 = selected
}traversedNodes;

typedef struct reuseIdListST{

   int idx1,idx2;
   vertex *vert1,*vert2; // this can additionally store the vertex pointer of the selected gates
   int pufTemplateType;//this is the type of PUF template that is to be inserted : nand,nor,inv. Default is inv. 0->inv,1->nand,2->nor
   int pufExcess; // this is true when a pair of gates are not available and now the excess pufs are to be inserted at the end of the selected gate
                  // in this both the idx are same. use any 1. Puf template of invx1 will be inserted at the end of the identifieed gate.
}reuseIdList;

typedef struct vectorListST{
   char* name;
   int max,min;
   int bPrim;
}vectorList;

typedef struct modifyInstructionsST{

   int operation; //0->insert at the end , 1-> replace.this is if the template needs to be replaced or inserted at the end of the target node.
   int dummy_insertion; //0->means no dummy insertion, 1-> dummy insertion. Dummy insertion just creates a dummy instance and then prints the ports of the template 
   graph* template; //this will hold the template that is to be used
   graph *inv_template,*nand_template,*nor_template;
   array_t* targetCells; //if insert at the end then it will be initialized as an array of vertexes selected for insertion at the end
                         //if replace is used other data structure will be used to replace the said gate units.

}modifyInstructions;

/* declare graph functions */

net* createNewNet(char* word, Type* gate);

void getResistance(Type* gate);

edge* createNewEdge(array_t* tmpArr, graph* G, array_t* gEdges, int i);

edge* createPrimInEdge(char* edgName, graph* G, array_t* globEdges);

net* findNet(char* word, array_t* NetArr);

int getEdgeInfo(array_t* arr, int idx);

edge* findEdge(char* edgName, array_t* gEdges);

Type* findType(char* typeName, array_t* typArr);

vertex* findVertex(char* vertName, array_t* lvlvrtArr);

void levelize(graph* G);

void findLevel(array_t* vertArr, graph* G, FILE* fout);

void getDelay(graph* G, vertex* vert);

int findInTran(int outTran, Type* gate);

double findDelayOrg(int tran, int maxMin, float Wn, float Wp, float CL, edge* edg, float tT);

double findCL(graph* G, edge* outEdg, Type* gate, int rqTran, int i);

double findDelay(int tran, int maxMin, float Wn, float Wp, float CL, edge* edg, float tT);


/* Calculate the Path Delay */
void findPathDelay(graph* G,path* crtPath);

edge* findMaxPOEdg(array_t* edgArr, int rqTran);

edge* findMaxEdg(edge* outEdg, int outTran);

void insertDelay(path* crtpath, edge* edg, vertex* outNode, int tran);
 
void findRealPath(graph* G, float delta);

void findInputTime(graph* G, array_t* edgArr, int n, int delta);

void resetList(graph* G);

void resetTree(graph* G, edge* testEdg);

void initialize(graph* G);

edge* newAssignment(edge* edg);

int nextTran(array_t* tranArr, int tran);

int findTran(array_t* tranArr, int tran);

float findObjTime(array_t* edgArr, int j, edge* edg, int objTran);

int podem(graph* G, edge* edg, array_t* edgArr, int objTran, int objTime,  
                        int conflict, int progressIndex);
 
edge* chgPrmInAssn(graph* G, edge* testEdg, array_t* conflEdgArr);

void freeEdgLst(graph* G, edgList* Lst, int k);

edge* backTrack(graph* G, edge* testEdg, array_t* conflEdgArr);

int getComplement(int value);

array_t* getAggressorEdg(array_t* edgArr, int n);

int forwardPropagate(graph* G, edge* edg, edge* testEdg, 
                     array_t* conflEdgArr, int conflict, int tmpProp);

void findPrmEdg(edge* outEdg);

void getPrmEdg(edge* edg, edge* outEdg);

int findConflict(edge* edg, edge* testEdg, array_t* conflEdgArr);

void findSwTime(vertex* node, edge* outEdg, float swTime);

float findTime(edge* refEdg, vertex* node, float minSwTime, int tmpProp);

float getTime(edge* refEdg, vertex* node, float minSwTime, int tmpProp);

void assignTime(edge* edg, float swTime, float gamma,
                                edge* outEdg, edge* refEdg, int tmpProp);

int getOutTran(vertex* node, edge* testEdg, int tmpProp);

int checkPropagation(vertex* node, int inputSet, edge* testEdg);

int findPropagation(vertex* node, int tmpProp);

int checkEdg(edge* refEdg, edge* outEdg, edge* testEdg, 
                                  array_t* conflEdgArr, int tmpProp);

int prmInCheck(edge* prmEdg, edge* testEdg, array_t* conflEdgArr);

edge* backTrace(graph* G, edge* edg, int assnTran, float objTime);

float getObjTime(edge* edg, int assnTran, float objTime);

edge* selectEdge(array_t* inEdgArr, vertex* vert, 
                                    int inputTran, float objTime);

edge* updateJfront(array_t* tmpArr, vertex* vert, 
                                    int inputTran, float objTime);

edge* getNewObject();

array_t* updateFrontArr(array_t* testArr);

void freeFront(array_t* testArr);

array_t* updateEdgArr(array_t* testArr);

array_t* updateFloatArr(array_t* testArr);

array_t* updateListArr(array_t* testArr);

array_t* removeEdg(array_t* testArr, edge* edg);

void printGraph(graph* G);

void initializeEdgInputs(array_t* globEdges);

void assntime(edge* refEdg, vertex* node, float swTime, int tmpProp);

// new functions added for Lagrangian relaxation

double optSizerOld(array_t* lvlvrtArr, int i);

double optSizer(graph *G, array_t* lvlvrtArr, int i);

void findPath(graph* G, float f, int cFlag);

void SOLVE_LRS(graph *G, array_t* lvlvrtArr);

void SOLVE_LDP(graph* G, float sf);

double **lamdaInit(double **lamda, array_t* trueLvlVrtArr);

double **moveLamda(double **lamda, array_t* trueLvlVrtArr, double rho, double inConst, double outConst);

//int lamdaVecInitF(double **lamda, double *lamdaVec, double **matA, array_t *trueLvlVrtArr);

int lamdaVecInitF(double **lamda, array_t *trueLvlVrtArr);

int lamdaVecInitB(double **lamda, double *lamdaVec, array_t *trueLvlVrtArr);

void moveLamdaVecOld(double **lamda, double *lamdaVec, double **matB, double **projMat, array_t* trueLvlVrtArr, double rho, double inConst, double outConst, int vecSize);

//double moveLamdaVec(double **lamda, double *lamdaVec, double **matB, double **projMat, array_t* trueLvlVrtArr, double rho, double inConst, double outConst, int vecSize);

void moveLamdaVec(double **lamda, array_t* trueLvlVrtArr, double inConst, double outConst, float sf);

// new functions for slack based sizing


double minElement(double **matrix, int sizeMat);

void getProjMat(double **matB, double **projMat, int sizeMat, int vecSize);

// matrix.c

void matInverse(double **matIn, double **matOut, int sizeMat);

void lubksb(double **a, int sizeMat, int *indx, double *b);

void ludcmp(double **a, int sizeMat, int *indx, double *d);

void matMultiply(double **matA, double **matB, double **matOut, int rowA, int colA, int rowB, int colB);

void matVecMultiply(double **matA, double *vecIn, double *vecOut, int rowA, int colA);

void unitMatInit(double **mat, int sizeMat);

void matTrans(double **matIn, double **matOut, int rowA, int colA);

void matSubtract(double **matA, double **matB, double **matOut, int rowA, int colA);

void loadStd(int noTran, int noSize, int noLoad, double ***stdVal);

double calcStd(double trIn,double gateW, double lFactor);

double calcSigmaFac(double yield);

void delayCalcInter(array_t* trueLvlVrtArr, int noSample);

double mean(double *data, int size);

double std(double *dat, double mean, int size);

void delayCalcIntra(array_t* trueLvlVrtArr);

typedef double list8[3][3][3][4][4][4][3][3];
typedef double list4[4][5][4][3];

double calcStdIntra(double arr1Std, double arr2Std, double diffArr, double loadFac, 
                    double tran1, double tran2, double tran1Std, double tran2Std, list8 dataIntra);

double calcStdIntraInv(double loadFac, double gateW, double tran1, double tran1Std, list4 dataIntra);

void readIntra();

void readIntraInv();

int printPaths(graph* G, char* fName, char* hName);

int resetLevelsDelays(graph* G);

void readOptions(char* optFile);

long slackBasedSizing(graph* G, float initMaxDelay, float initArea, 
       int delayTolerance, int areaTolerance, float slackWindow, 
       float tdelay, array_t* sizedCells);

double computeTotalSlack(graph* G, float refDelay);

int assignSlackToCells(graph* G, float refDelay);

int initializeAlphaEtc(graph* G);

int rankVerticesOnSlack(graph* G, array_t*, array_t*);

double computeCircuitArea(graph* G);

float getMaxDelay(graph* G);

void initializeTraverseFlag(graph* G);

void markLogicCone(graph* G, int nLevel);

float getCellTrioDelay(vertex* vert, path* pPath, float* drvDelay);

float whatIfCellTrioDelay(graph* G, vertex* vert, path* pPath, float newSizeFactor, float* drvDelay);

float computeCellDelay(graph* G, vertex* vert, int rqTran, int outTran, edge* edg, float sizeFactor);

double getNextSize(vertex* vert, sizeType sizeDir, int nStep);

float performWhatIfAnalysis(graph* G, vertex* vert, sizeType sizeDir, float targetSlack, float* drvDelayDiff);

long performHealing(graph* G, double targetDelay, array_t* sizedCells);

void markFanInLogicCone(vertex* startVert, int nLevel, float slack, float drvDlyDiff);

void readSizeFactors(graph* G, char* filename);

void generateStat(graph* G, array_t* topoArr, char* inputFile, char* vecFile,int rstEdgeId, int extStimulus, char* extStimulusFileName, int traceCount);

void calculateTVLA(graph* G, array_t* topoArr, char* inputFile, char* vecFile,int rstEdgeId, int traceCount);

void createTestBench(graph *G, array_t *topoArr, char* vecFile);

array_t* getTopoOrder(graph* G);

double getDelayOpt(graph* G, vertex* vert, float Vdd, int input, int outTran);

double getDelayStd(graph* G, vertex* vert, float Vdd, int input, int outTran);

double findCLStd(graph* G, edge* outEdg, Type* gate, int input, double* absCL, float vdd);

double findCLOpt(graph* G, edge* outEdg, Type* gate, int input, double* absCL, float vdd);

double computeStageDelay(graph* G, array_t* topoArr, float vdd, int numStages, int device);

double computeTotalPower(graph* G, array_t* topoArr, float vdd, double freq, int nStages, int device);

array_t* partition(graph* G);

int checkTopoOrder(vertex* v, array_t* tList);

int findInputOutputOfPartition(array_t* tmpList, array_t* inList, array_t* outList);

//void writeVerilog(int nPart, FILE* fpart, array_t* tmpList, array_t* inList, array_t* outList);

void levelizen(graph* G, array_t* topoArr);

array_t* breakInWords(char* str);

array_t* partitionBackward(graph* G);

array_t* partitionBackwardNew(graph* G);

array_t* partitionForward(graph* G);
array_t* partitionForwardComplex(graph* G);

void performReverseTrace(array_t* tlist, array_t* tmpList, int dir, int i);

int calculateOutValues(vertex* s);

graph* createGraphFromVerilog(char* inputFileName, char* libFileName, char* moduleName, int* primInCnt, graph* rootG);

array_t* breakInWordsVlog(char* header);

vertex* createNewVertexForVlog(graph* G, array_t* wordArr, char* cellName, char* nodeName, array_t* globEdges, int libertyflag);

vertex* createNewVertex(graph* G, array_t* wordArr, char* outputVar, char* cellName, char* nodeName, array_t* globEdges);

edge* createEdge(graph* G, char* name);

int computeMaxDelayPath(graph* G);

vertex* createInverter(graph* G, char* inputVar, char* outputVar, char* nodeName, array_t* globEdges);

int countTransistors(graph* G);

int propagateToPO(graph* G, array_t* topoArr, edge* tedg);

//int testgenWR(graph* G, int nPrimIn, int nMaxPatterns, float inputActivity, int clkEdgeId, int rstEdgeId, int rstEdge);

int genStimulus(graph* G, int nPrimIn, int nMaxPatterns, float inputActivity, int clkEdgeId, int rstEdgeId, int rstEdge);

void writeHMETIS(graph* G, char* moduleName);

//astra functions declared below 
void start(config_param*);
void insert_puf(config_param* param); //Atri:functions
int store_timing_paths(config_param*);
void restore_slack(config_param*);
void writeVerilog(config_param*);
array_t* vectorExtractor(array_t*);
void duplicateGraph(graph*,graph*);
void freeG(graph*);
void freeVertArr(array_t*);
void freeEdgeArr(array_t*);
void readPufTemplates(config_param *);
char* command_generator(const char *, int);
char **command_completion(const char *, int, int);

#endif

