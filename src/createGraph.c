// ©Copyright 2020 University of Florida Research Foundation, Inc. All Rights Reserved.
/* create graph from the spice file of the design */
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "array.h"
#include "graph.h"
#include <inttypes.h>
#include <dirent.h>
#include <errno.h>
#include<unistd.h>
#define IN 0
#define OUT 1

#define IN_RES 500 // voltage source resistance value

/* createNewNet: if the net is never found before, insert it into Gate->netArr */
net *createNewNet(char *word, Type *Gate)
{
    net *Net;
    array_t *NetArr;

    NetArr = Gate->netArr;
    Net = findNet(word, NetArr);
    if (Net == NULL)
    {
        Net = (net *)malloc(sizeof(net));
        Net->pchArr = array_alloc(trans *, 1);
        Net->nchArr = array_alloc(trans *, 1);
        Net->name = word;
        array_insert_last(net *, Gate->netArr, Net);
    }
    return Net;
}

/* getResistance: determines min & max width for min & max delay of a gate */
void getResistance(Type *Gate)
{
    net *Net;
    array_t *transArr;
    array_t *prmNetArr;
    net *prmNet;
    int i, n;
    array_t *NetArr;
    trans *tran;

    /* WMax induces Min Delay; WMin does Max Delay; */

    if (strncasecmp(Gate->name, "nnd", 3) == 0)
    {
        transArr = Gate->pChannel;
        tran = array_fetch(trans *, transArr, 0);
        Gate->WpMax = (tran->width) * (array_n(transArr)); // Min. Resistance //
        Gate->WpMin = (tran->width);                       // Max. Resistance //
        Gate->WpOrg = tran->width;

        transArr = Gate->nChannel;
        tran = array_fetch(trans *, transArr, 0);
        Gate->WnMax = (tran->width) / (array_n(transArr));
        Gate->WnMin = Gate->WnMax;
        Gate->WnOrg = tran->width;

        Gate->NoTr = array_n(transArr);
    }

    else if (strncasecmp(Gate->name, "nor", 3) == 0)
    {
        transArr = Gate->pChannel;
        tran = array_fetch(trans *, transArr, 0);
        Gate->WpMax = (tran->width) / (array_n(transArr));
        Gate->WpMin = Gate->WpMax;
        Gate->WpOrg = tran->width;

        transArr = Gate->nChannel;
        tran = array_fetch(trans *, transArr, 0);
        Gate->WnMax = (tran->width) * (array_n(transArr));
        Gate->WnMin = (tran->width);
        Gate->WnOrg = tran->width;

        Gate->NoTr = array_n(transArr);
    }

    else if (strncasecmp(Gate->name, "i1", 2) == 0)
    {
        transArr = Gate->pChannel;
        tran = array_fetch(trans *, transArr, 0);
        Gate->WpMax = (tran->width);
        Gate->WpMin = Gate->WpMax;
        Gate->WpOrg = tran->width;

        transArr = Gate->nChannel;
        tran = array_fetch(trans *, transArr, 0);
        Gate->WnMax = (tran->width);
        Gate->WnMin = Gate->WnMax;
        Gate->WnOrg = tran->width;

        Gate->NoTr = array_n(transArr);
    }
}

/* createNewEdge: Edge structure initializatioin */
edge *createNewEdge(array_t *tempArr, graph *G, array_t *globEdges, int i)
{
    char *edgName;
    edge *edg;

    edgName = array_fetch(char *, tempArr, i);
    edg = findEdge(edgName, globEdges);

    if (edg == NULL) /* no edge present, create one */
    {
        edg = (edge *)malloc(sizeof(edge));
        edg->toNodes = array_alloc(vertex *, 1);
        edg->cplCap = array_alloc(parasite *, 1);
        edg->switchTimeArr = array_alloc(float, 1);
        edg->assnValues = array_alloc(int, 1);
        edg->prmEdgArr = array_alloc(edge *, 1);
        edg->timeEdgArr = array_alloc(edge *, 1);
        edg->crtEdgArr = array_alloc(edge *, 1);
        edg->switchArr = array_alloc(int, 1);
        edg->tmpSwitchArr = array_alloc(int, 1);
        edg->swTimeArr = array_alloc(float, 1);
        edg->tmpSwTimeArr = array_alloc(float, 1);
        edg->rDlyMaxArr = array_alloc(float, 1);
        edg->rDlyMinArr = array_alloc(float, 1);
        edg->fDlyMaxArr = array_alloc(float, 1);
        edg->fDlyMinArr = array_alloc(float, 1);
        edg->gateDlyRArr = array_alloc(float, 1);
        edg->gateDlyFArr = array_alloc(float, 1);
        edg->TtimeRmaxArr = array_alloc(float, 1);
        edg->TtimeRminArr = array_alloc(float, 1);
        edg->TtimeFmaxArr = array_alloc(float, 1);
        edg->TtimeFminArr = array_alloc(float, 1);
        edg->TtimeRArr = array_alloc(float, 1);
        edg->TtimeFArr = array_alloc(float, 1);
        edg->tmpPropArr = array_alloc(int, 1);
        edg->rDelayMaxArr = array_alloc(float, 1);
        edg->rDelayMinArr = array_alloc(float, 1);
        edg->fDelayMaxArr = array_alloc(float, 1);
        edg->fDelayMinArr = array_alloc(float, 1);

        edg->fromNode = NULL;
        edg->maxCcEdg = NULL;
        edg->mainPrmIn = NULL;
        edg->name = edgName;
        edg->cplCapFlag = -1;
        edg->gndCap = 0.0;
        edg->maxCc = 0;
        edg->maxCcConflict = 0;
        edg->tmpMaxDelay = 0;
        edg->rDelayMax = 0;
        edg->rDelayMaxOrg = 0;
        edg->rDelayMaxTmp1 = 0;
        edg->rDelayMin = 0;
        edg->rDelayMinOrg = 0;
        edg->rDelayMinTmp1 = 0;
        edg->fDelayMax = 0;
        edg->fDelayMaxOrg = 0;
        edg->fDelayMaxTmp1 = 0;
        edg->fDelayMin = 0;
        edg->fDelayMinOrg = 0;
        edg->fDelayMinTmp1 = 0;
        edg->maxDelay = 0;
        edg->minDelay = 0;
        edg->gateDelayR = 0;
        edg->gateDelayRTmp = 0;
        edg->gateDelayROrg = 0;
        edg->gateDelayF = 0;
        edg->gateDelayFTmp = 0;
        edg->gateDelayFOrg = 0;
        edg->weight = 55;
        edg->TtimeR = 20e-12;
        edg->TtimeROrg = edg->TtimeR;
        edg->TtimeRTmp = edg->TtimeR;
        edg->TtimeF = 20e-12;
        edg->TtimeFOrg = edg->TtimeF;
        edg->TtimeFTmp = edg->TtimeF;
        edg->TtimeRmax = 20e-12;
        edg->TtimeRmaxOrg = edg->TtimeRmax;
        edg->TtimeRmaxTmp = edg->TtimeRmax;
        edg->TtimeRmin = 20e-12;
        edg->TtimeRminOrg = edg->TtimeRmin;
        edg->TtimeRminTmp = edg->TtimeRmin;
        edg->TtimeFmax = 20e-12;
        edg->TtimeFmaxOrg = edg->TtimeRmax;
        edg->TtimeFmaxTmp = edg->TtimeRmax;
        edg->TtimeFmin = 20e-12;
        edg->TtimeFminOrg = edg->TtimeRmin;
        edg->TtimeFminTmp = edg->TtimeRmin;
        edg->assnFlag = 0;
        edg->assnTran = 55; /* Unassigned Transistion */
        edg->tmpAssign1 = 55;
        edg->tmpAssign2 = 55;
        edg->tmpAssign2Org = 55;
        edg->tmpAssign3 = 55;
        edg->tmpTranTime = 0;
        edg->tmpTranTime2 = 55;
        edg->switchTime = 55;
        edg->logicProp = 55;
        edg->crEdgConflict = 0;
        edg->timeOutFlag = 55;
        edg->progressEdgIndex = -1;
        edg->tpd = -1;
        edg->bPrimIn = 0;    /* 0: not Primary Input */
        edg->bPrimOut = 0;   /* 0: not Primary outPut */
        edg->oldVal = -1;    /* value -1 means undefined */
        edg->newVal = -1;    /* value -1 means undefined */
        edg->errOldVal = -1; /* value -1 means undefined */
        edg->errNewVal = -1; /* value -1 means undefined */
        edg->signal0Prob = 0;
        edg->signal1Prob = 0;

        /* variances for process variation */
        edg->rDelMaxVar = 0;
        edg->fDelMaxVar = 0;
        edg->rDelMinVar = 0;
        edg->fDelMinVar = 0;

        edg->lamda = 0;

        array_insert_last(edge *, globEdges, edg);
        array_insert_last(edge *, G->edgArr, edg);
    }
    return edg;
}

net *findNet(char *netName, array_t *netArr)
{
    int i;
    net *Net;

    for (i = 0; i < array_n(netArr); i++)
    {
        Net = array_fetch(net *, netArr, i);
        if (strcmp(Net->name, netName) == 0)
            return Net;
    }
    return NULL;
}

edge *findEdge(char *edgName, array_t *globEdges)
{
    int i;
    edge *edg;

    for (i = 0; i < array_n(globEdges); i++)
    {
        edg = array_fetch(edge *, globEdges, i);
        if (strcmp(edg->name, edgName) == 0)
            return edg;
    }
    return NULL;
}

// libs* findCell(char* cellName, array_t* libArr)
//{
//	int i;
//	libs* cell;
//	for(i=0;i<array_n(libArr);i++){
//	cell=array_fetch(libs*,libArr,i);
//	if(strcmp(cell->name,cellName)==0)
//		return cell;
//	}
//	printf("Cell %s not found ...\n",cellName);
//	return NULL;
// }

Type *findType(char *typeName, array_t *typArr)
{
    int i;
    Type *gateType;

    for (i = 0; i < array_n(typArr); i++)
    {
        gateType = array_fetch(Type *, typArr, i);
        //        printf("Looking up: %s\n",gateType->name);
        if (strcmp(gateType->name, typeName) == 0)
            return gateType;
    }
    printf("Gate Type %s not found ...\n", typeName);
    return NULL;
}

vertex *findVertex(char *vertName, array_t *lvlvrtArr)
{
    int i;
    vertex *vertTest;

    for (i = 0; i < array_n(lvlvrtArr); i++)
    {
        vertTest = array_fetch(vertex *, lvlvrtArr, i);
        if (strcmp(vertTest->name, vertName) == 0)
            return vertTest;
    }
    return NULL;
}

int getEdgeInfo(array_t *arr, int idx)
{
    char *type = array_fetch_last(char *, arr);

    if (!strncasecmp(type, "dff", 3) || !strncasecmp(type, "latch", 5))
    {
        if (idx > 1)
            return IN;
        else
            return OUT;
    }
    else
    {
        if (idx > 0)
            return IN;
        else
            return OUT;
    }

    /*char* edgName = array_fetch((char*), arr, idx);*/

    /*if(!strncasecmp(type, "nor", 3) || !strncasecmp(type, "or", 2) ||
       !strncasecmp(type, "and", 3) || !strncasecmp(type, "nnd", 3) ||
       !strncasecmp(type, "hi", 2) || !strncasecmp(type, "i1", 2) ||
       !strncasecmp(type, "mx", 2) || !strncasecmp(type, "tib", 3) ||
       !strncasecmp(type, "xnr", 3) || !strncasecmp(type, "xor", 3) ||
       !strncasecmp(type, "aoi", 3) || !strncasecmp(type, "oai", 3))
    {
        if(idx > 0)
            return IN;
        else
            return OUT;
    }
    else if(!strncasecmp(type, "fadd1", 4))
    {
        if(idx > 1)
            return IN;
        else
            return OUT;
    }
    else
    {
      // add some more cells
        printf("WARNING: Edge type not determined.\n");
        return IN;
    }*/
}

void printGraph(graph *G)
{
    array_t *vertArr;
    array_t *edgArr;
    int i, j, k;
    edge *edg;
    vertex *vert, *tvert;

    vertArr = G->vertexArr;
    edgArr = G->edgArr;

    printf("Graph Description\n");
    printf("\n No of Vertices: %d \n", array_n(vertArr));

    for (i = 0; i < array_n(vertArr); i++)
    {
        vert = array_fetch(vertex *, vertArr, i);
        if (vert->type)
            printf("Name: %s Type: %s Idx: %d PrimIn %d\n", vert->name,
                   vert->type->name, vert->idx, vert->bPrimIn);
        else
            printf("Name: %s Type: undefined Idx: %d PrimIn %d\n", vert->name,
                   vert->idx, vert->bPrimIn);
        for (j = 0; j < array_n(vert->outEdges); j++)
        {
            edg = array_fetch(edge *, vert->outEdges, j);
            printf("Out edgIdx: %d, edgName: %s\n", edg->idx, edg->name);
            printf("Fanout nodes: ");
            for (k = 0; k < array_n(edg->toNodes); k++)
            {
                tvert = array_fetch(vertex *, edg->toNodes, k);
                if (!tvert)
                    continue;
                printf(" %s ", tvert->name);
            }
            printf("\n");
        }
        for (j = 0; j < array_n(vert->inEdges); j++)
        {
            edg = array_fetch(edge *, vert->inEdges, j);
            printf("In edgIdx: %d, edgName: %s\n", edg->idx, edg->name);
        }
    }

    printf("\n No of Edges: %d \n", array_n(edgArr));
    for (i = 0; i < array_n(edgArr); i++)
    {
        edg = array_fetch(edge *, edgArr, i);
        if (edg->fromNode == NULL)
        {
            printf("edgIdx: %d edgName: %s Primary input: fromNode: NULL toNodes: ",
                   edg->idx, edg->name);
            for (j = 0; j < array_n(edg->toNodes); j++)
            {
                vert = array_fetch(vertex *, edg->toNodes, j);
                printf("%s ", vert->name);
            }
            printf("\n");
        }
        else if (array_n(edg->toNodes) == 0)
        {
            printf("edgIdx: %d edgName: %s fromNode: %s toNodes: NULL \n",
                   edg->idx, edg->name, edg->fromNode->name);
        }
        else
        {
            printf("edgIdx: %d edgName: %s fromNode: %s toNodes: ",
                   edg->idx, edg->name, edg->fromNode->name);
            for (j = 0; j < array_n(edg->toNodes); j++)
            {
                vert = array_fetch(vertex *, edg->toNodes, j);
                printf("%s ", vert->name);
            }
            printf("\n");
        }
    }

    return;
}

/* words are broken into list */
array_t *breakInWords(char *header)
{
    array_t *wordArr = array_alloc(char *, 1);
    char *word = (char *)malloc(sizeof(char) * 120);
    int i, j, k;

    word[0] = '\0';
    i = j = 0;

    /* get rid of initial blanks */
    while (header[i] == ' ')
        i++;

    while (i < strlen(header))
    {
        if (header[i] == ' ')
        {
            word[j] = '\0';
            array_insert_last(char *, wordArr, strdup(word));
            j = 0;

            i++;
            while (header[i] == ' ' || header[i] == '(' || header[i] == ')' || header[i] == '{')
            {
                i++;
            }
        }
        else
        {
            while (header[i] == ' ' || header[i] == '(' || header[i] == ')' || header[i] == '{' || header[i] == ':' || header[i] == ';' || header[i] == '"' || header[i] == '\\')
            {
                i++;
            }

            word[j++] = header[i];
            // printf("%s\n",header);
            i++;
        }
    }

    /* add the last word */
    word[j] = '\0';
    array_insert_last(char *, wordArr, strdup(word));

    return wordArr;
}

array_t *breakInWords2(char *header)
{
    array_t *wordArr = array_alloc(char *, 1);
    char *word = (char *)malloc(sizeof(char) * 120);
    int i, j, k;

    word[0] = '\0';
    i = j = 0;

    /* get rid of initial blanks */
    while (header[i] == ' ')
        i++;

    while (i < strlen(header))
    {
        if (header[i] == ' ')
        {
            word[j] = '\0';
            array_insert_last(char *, wordArr, strdup(word));
            j = 0;

            i++;
            while (header[i] == ' ' || header[i] == '(' || header[i] == ')' || header[i] == '{')
            {
                i++;
            }
        }
        else
        {
            while ( header[i] == '(' || header[i] == ')' || header[i] == '{' || header[i] == ':' || header[i] == ';' || header[i] == '"' || header[i] == '\\')
            {
                i++;
            }

            word[j++] = header[i];
            // printf("%s\n",header);
            i++;
        }
    }

    /* add the last word */
    word[j] = '\0';
    array_insert_last(char *, wordArr, strdup(word));

    return wordArr;
}

void initializeEdgInputs(array_t *globEdges)
{
    int i;
    edge *edg;

    for (i = 0; i < array_n(globEdges); i++)
    {
        edg = array_fetch(edge *, globEdges, i);
        edg->oldVal = -1;
        edg->newVal = -1;
        edg->tpd = -1;
    }
}

edge *createPrimInEdge(char *edgName, graph *G, array_t *globEdges)
{
    edge *edg;

    edg = findEdge(edgName, globEdges);

    if (edg == NULL) /* no edge present, create one */
    {
        edg = (edge *)malloc(sizeof(edge));
        edg->toNodes = array_alloc(vertex *, 1);
        edg->cplCap = array_alloc(parasite *, 1);
        edg->switchTimeArr = array_alloc(float, 1);
        edg->assnValues = array_alloc(int, 1);
        edg->prmEdgArr = array_alloc(edge *, 1);
        edg->timeEdgArr = array_alloc(edge *, 1);
        edg->crtEdgArr = array_alloc(edge *, 1);
        edg->switchArr = array_alloc(int, 1);
        edg->tmpSwitchArr = array_alloc(int, 1);
        edg->swTimeArr = array_alloc(float, 1);
        edg->tmpSwTimeArr = array_alloc(float, 1);
        edg->rDlyMaxArr = array_alloc(float, 1);
        edg->rDlyMinArr = array_alloc(float, 1);
        edg->fDlyMaxArr = array_alloc(float, 1);
        edg->fDlyMinArr = array_alloc(float, 1);
        edg->gateDlyRArr = array_alloc(float, 1);
        edg->gateDlyFArr = array_alloc(float, 1);
        edg->TtimeRmaxArr = array_alloc(float, 1);
        edg->TtimeRminArr = array_alloc(float, 1);
        edg->TtimeFmaxArr = array_alloc(float, 1);
        edg->TtimeFminArr = array_alloc(float, 1);
        edg->TtimeRArr = array_alloc(float, 1);
        edg->TtimeFArr = array_alloc(float, 1);
        edg->tmpPropArr = array_alloc(int, 1);
        edg->fromNode = NULL;
        edg->maxCcEdg = NULL;
        edg->mainPrmIn = NULL;
        edg->name = edgName;
        edg->cplCapFlag = -1;
        edg->gndCap = 0.0;
        edg->maxCc = 0;
        edg->maxCcConflict = 0;
        edg->rDelayMax = 0;
        edg->rDelayMaxOrg = 0;
        edg->rDelayMaxTmp1 = 0;
        edg->rDelayMin = 0;
        edg->rDelayMinOrg = 0;
        edg->rDelayMinTmp1 = 0;
        edg->fDelayMax = 0;
        edg->fDelayMaxOrg = 0;
        edg->fDelayMaxTmp1 = 0;
        edg->fDelayMin = 0;
        edg->fDelayMinOrg = 0;
        edg->fDelayMinTmp1 = 0;
        edg->maxDelay = 0;
        edg->minDelay = 0;
        edg->gateDelayR = 0;
        edg->gateDelayRTmp = 0;
        edg->gateDelayROrg = 0;
        edg->gateDelayF = 0;
        edg->gateDelayFTmp = 0;
        edg->gateDelayFOrg = 0;
        edg->weight = 55;
        edg->TtimeR = 0.1e-9;
        edg->TtimeROrg = edg->TtimeR;
        edg->TtimeRTmp = edg->TtimeR;
        edg->TtimeF = 0.1e-9;
        edg->TtimeFOrg = edg->TtimeF;
        edg->TtimeFTmp = edg->TtimeF;
        edg->TtimeRmax = 0.1e-9;
        edg->TtimeRmaxOrg = edg->TtimeRmax;
        edg->TtimeRmaxTmp = edg->TtimeRmax;
        edg->TtimeRmin = 0.1e-9;
        edg->TtimeRminOrg = edg->TtimeRmin;
        edg->TtimeRminTmp = edg->TtimeRmin;
        edg->TtimeFmax = 0.1e-9;
        edg->TtimeFmaxOrg = edg->TtimeRmax;
        edg->TtimeFmaxTmp = edg->TtimeRmax;
        edg->TtimeFmin = 0.1e-9;
        edg->TtimeFminOrg = edg->TtimeRmin;
        edg->TtimeFminTmp = edg->TtimeRmin;
        edg->assnFlag = 0;
        edg->assnTran = 55; /* Unassigned Transistion */
        edg->tmpAssign1 = 55;
        edg->tmpAssign2 = 55;
        edg->tmpAssign2Org = 55;
        edg->tmpAssign3 = 55;
        edg->tmpTranTime = 0;
        edg->tmpTranTime2 = 55;
        edg->switchTime = 55;
        edg->logicProp = 55;
        edg->crEdgConflict = 0;
        edg->timeOutFlag = 55;
        edg->progressEdgIndex = -1;
        edg->tpd = -1;
        edg->bPrimIn = 0;
        edg->bPrimOut = 0;
        edg->oldVal = -1; /* value -1 means undefined */
        edg->newVal = -1; /* value -1 means undefined */

        /* variances for process variation */
        edg->rDelMaxVar = 0;
        edg->fDelMaxVar = 0;
        edg->rDelMinVar = 0;
        edg->fDelMinVar = 0;

        //array_insert_last(edge *, globEdges, edg); //ATRI: Commented here
        array_insert_last(edge *, G->edgArr, edg);
    }
    return edg;
}

void readSizeFactors(graph *G, char *filename)
{
    int i, j;
    FILE *fptr = fopen(filename, "r");
    array_t *vertexArr = G->vertexArr;
    int nVert = array_n(G->vertexArr);
    int idx;
    float factor;
    vertex *vert;

    while (!feof(fptr))
    {
        fscanf(fptr, "%d %f\n", &idx, &factor);
        if (idx >= nVert)
            break;
        vert = array_fetch(vertex *, vertexArr, idx);
        if (vert->idx != idx)
            printf("ERRRRRRRRRRRRRR......\n");
        vert->sizeFactor = factor;
    }
    fclose(fptr);
}

graph *createGraphFromVerilog(char *inputFileName, char *libFileName, char *moduleName, int *primInCnt, graph *rootG)
{
    int dCnt = 0, i, j, k, nFanout, icnt = 0, inidx = 5, ocnt = 0;
    ;
    int gCnt = 0, nInVar = 0, nOutVar = 0, bAssnFlag = 0, assigncnt = 0;
    char *word, *header, *invout;
    char *Qnet, *outputVar, *gatetype, *gatename, *invname;
    int uniqueInvs = 0, uniqueNors = 0, uniqueNands = 0;
    int *marked = NULL;
    graph *G = (graph *)malloc(sizeof(graph));
    array_t *wordArr, *inVarArr, *outVarArr, *globEdges, *newPrimOutEdges;
    edge *edg, *outedg;
    char *pstr = (char *)malloc(sizeof(char) * 80000000);
    char *pstr1 = (char *)malloc(sizeof(char) * 80000000);
    FILE *fptr = fopen(inputFileName, "r");
    FILE *libFile;
    int libertyflag = 0; // give me library or give me freedom

    FILE *libertyFile = fopen(libFileName, "r");

    if (rootG != NULL){
        printf("INFO: Reading Library File: %s\n", libFileName);
    }
    
    if (strstr(libFileName, "gscl45nm.lib"))
        libertyflag = 1;

    Type *Gate;
    trans *Trans;
    pins *pin; // Patanjali
    net *Net;
    vertex *v;

    globalVertIdx = globalEdgIdx = *primInCnt = 0;

    if (!fptr || !libertyFile)
    {
        printf("Either input file of library file is not present. Exiting ...\n");
        exit(0);
    }

    inVarArr = array_alloc(char *, 1);
    outVarArr = array_alloc(char *, 1);
    header = (char *)malloc(sizeof(char) * 20000);
    gatename = (char *)malloc(sizeof(char) * 400);
    gatetype = (char *)malloc(sizeof(char) * 400);

    int nvector = 0; // jonathan: if vector in verilog file

    /* initialize the graph */
    G->vertexArr = array_alloc(vertex *, 1);
    G->typeArr = array_alloc(Type *, 1); // use this if you are using the spice deck
    // G->libArr=array_alloc(libs*,1); // use this if you are using the std lib
    G->comments = array_alloc(char*,1);
    G->edgArr = array_alloc(edge *, 1);
    G->capArr = array_alloc(parasite *, 1);
    G->resArr = array_alloc(parasite *, 1);
    G->primInEdges = array_alloc(edge *, 1);
    G->primOutEdges = array_alloc(edge *, 1);
    G->fNodes = array_alloc(vertex *, 1);
    G->seqGates = array_alloc(vertex *, 1);
    G->nonscanDFF = array_alloc(vertex *, 1); // added by Pravin on 10/7/19
    G->scanDFF = array_alloc(vertex *, 1);
    G->combGates = array_alloc(vertex *, 1);
    G->processed = false;
    G->gatingDecoderArea = 0;
    G->gatingCtrl = "";
    G->header = strdup(moduleName);
    double *temparray;
    int inputcount = 0, index2count = 0, index = 0; // Patanjali
    int riseflag = 0, fallflag = 0, rtf = 0, ftf = 0, valueflag = 0, linecount = 0, linelimit = 0;
    G->data_required_time = 0 ;
    G->lowprobEdges = array_alloc(edge *, 1);
    if (rootG)
        G->typeArr = rootG->typeArr;
    else
    {
        if (libertyflag == 1)
        {
            while (!feof(libertyFile))
            {

                fscanf(libertyFile, "%[^\n]\n", header);
                if (strncasecmp(header, "cell (", 5) == 0)
                {

                    wordArr = breakInWords(header);
                    Gate = (Type *)malloc(sizeof(Type));

                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                    }

                    //            cell=(libs*)malloc(sizeof(libs));
                    Gate->name = array_fetch(char *, wordArr, 1);
                    // printf("Gate name:%s\n",Gate->name);
                    Gate->pinvals = array_alloc(pins *, 1);

                    array_insert_last(Type *, G->typeArr, Gate);

                    inputcount = 0;
                }
                else if (strncasecmp(header, "area :", 6) == 0)
                {
                    wordArr = breakInWords(header);
                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                    }
                    Gate->area = atof(array_fetch(char *, wordArr, 1));
                }
                else if (strncasecmp(header, "pin(", 4) == 0)
                {
                    pin = (pins *)malloc(sizeof(pins));
                    wordArr = breakInWords(header);
                    for (i = 0; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                    }

                    pin->pinname = word+3;
                    array_insert_last(pins *, Gate->pinvals, pin);
                }
                else if (strncasecmp(header, "direction", 9) == 0)
                {
                    wordArr = breakInWords(header);
                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                    }
                    if (strncasecmp(header, "direction : input", 17) == 0){
                        inputcount++;
                        pin->direction = strdup("input");
                    }else{
                        pin->direction = strdup("output");
                    }
                        
                }
                else if (strncasecmp(header, "cell_leakage_power", 18) == 0)
                {
                    wordArr = breakInWords(header);
                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                    }
                    Gate->leakage = atof(array_fetch(char *, wordArr, 1));
                }

                else if (strncasecmp(header, "capacitance :", 13) == 0)
                {
                    // printf("capacitance found\n");
                    wordArr = breakInWords(header);
                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                        pin->capacitance = atof(word);
                    }
                }
                else if (strncasecmp(header, "rise_capacitance", 15) == 0)
                {
                    wordArr = breakInWords(header);
                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                        pin->rise_capacitance = atof(word);
                    }
                }
                else if (strncasecmp(header, "fall_capacitance", 15) == 0)
                {
                    //	printf("fall cap found\n");
                    wordArr = breakInWords(header);
                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                        pin->fall_capacitance = atof(word);
                        // printf ("fall capacitance: %lf\n",pin->fall_capacitance);
                    }
                }
                else if (strncasecmp(header, "max_capacitance", 15) == 0)
                {
                    wordArr = breakInWords(header);
                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                        pin->max_capacitance = atof(word);
                    }
                }

                else if (strncasecmp(header, "cell_rise", 9) == 0)
                {

                    riseflag = 1;
                    fallflag = 0;
                    rtf = 0;
                    ftf = 0;
                    linecount = 0;
                    valueflag = 0;
                    index = 0;
                }
                else if (strncasecmp(header, "cell_fall", 9) == 0)
                {
                    pin->risetxn = temparray; // printf("cell fall found\n");
                    riseflag = 0;
                    fallflag = 1;
                    rtf = 0;
                    ftf = 0;
                    linecount = 0;
                    valueflag = 0;
                    index = 0;
                }
                else if (strncasecmp(header, "rise_transition", 15) == 0)
                {
                    pin->risedelays = temparray;
                    riseflag = 0;
                    fallflag = 0;
                    rtf = 1;
                    ftf = 0;
                    linecount = 0;
                    valueflag = 0;
                    index = 0;
                }
                else if (strncasecmp(header, "fall_transition", 15) == 0)
                {
                    pin->falldelays = temparray; //	printf("fall txn found\n");
                    riseflag = 0;
                    fallflag = 0;
                    rtf = 0;
                    ftf = 1;
                    linecount = 0;
                    valueflag = 0;
                    index = 0;
                }
                else if (strncasecmp(header, "index_1", 7) == 0)
                {
                    int indexcount = 0;
                    wordArr = breakInWords(header);
                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                        indexcount++;
                    }
                    linelimit = indexcount;
                }
                else if (strncasecmp(header, "index_2", 7) == 0)
                {
                    int indexcount = 0;
                    wordArr = breakInWords(header);
                    for (i = 1; i < array_n(wordArr); i++)
                    {
                        word = array_fetch(char *, wordArr, i);
                        indexcount++;
                    }
                    index2count = indexcount;
                }
                else if (strncasecmp(header, "values", 6) == 0)
                {

                    valueflag = 1;
                    int totalelem = linelimit * index2count;
                    temparray = (double *)malloc(totalelem * sizeof(double));
                }
                else if (valueflag == 1 && ftf == 1 && linecount >= linelimit)
                {
                    pin->falltxn = temparray;
                }
                else if (valueflag == 1 && linecount < linelimit)
                {
                    linecount++;
                    if (riseflag == 1)
                    {

                        wordArr = breakInWords(header);
                        for (i = 0; i < array_n(wordArr) - 1; i++)
                        {
                            temparray[index] = atof(array_fetch(char *, wordArr, i));
                            index++;
                        }
                    }
                    else if (rtf == 1)
                    {
                        wordArr = breakInWords(header);
                        for (i = 0; i < array_n(wordArr) - 1; i++)
                        {
                            temparray[index] = atof(array_fetch(char *, wordArr, i));
                            index++;
                        }
                    }
                    else if (fallflag == 1)
                    {
                        wordArr = breakInWords(header);
                        for (i = 0; i < array_n(wordArr) - 1; i++)
                        {
                            temparray[index] = atof(array_fetch(char *, wordArr, i));
                            index++;
                        }
                    }
                    else if (ftf == 1)
                    {
                        wordArr = breakInWords(header);
                        for (i = 0; i < array_n(wordArr) - 1; i++)
                        {
                            temparray[index] = atof(array_fetch(char *, wordArr, i));
                            index++;
                        }
                    }
                }
            }
        }
        else
        {
            while (!feof(libertyFile))
            {
                fscanf(libertyFile, "%[^\n]\n", header);
                if (strncasecmp(header, ".SUBCKT", 7) == 0)
                {
                    /* Create New Gate */

                    wordArr = breakInWords(header);
                    Gate = (Type *)malloc(sizeof(Type));
                    Gate->pChannel = array_alloc(trans *, 1);
                    Gate->nChannel = array_alloc(trans *, 1);
                    Gate->netArr = array_alloc(net *, 1);
                    Gate->primNetArr = array_alloc(net *, 1);
                    Gate->name = array_fetch(char *, wordArr, 1);
                    for (i = 2; i < array_n(wordArr); i++)
                    {

                        word = array_fetch(char *, wordArr, i);
                        Net = createNewNet(word, Gate);
                        array_insert_last(net *, Gate->primNetArr, Net);
                    }
                    array_insert_last(Type *, G->typeArr, Gate);
                }

                else if (strncasecmp(header, "m", 1) == 0) // now it's a transistor
                {
                    /* Gate Description */

                    Trans = (trans *)malloc(sizeof(trans));
                    //    printf("reading gates\n");
                    wordArr = breakInWords(header);
                    Trans->name = array_fetch(char *, wordArr, 0);
                    word = array_fetch(char *, wordArr, 6);
                    word[strlen(word) - 1] = '\0';
                    Trans->width = atof(word + 2) * 1e-6; // transistor width is stored

                    word = array_fetch(char *, wordArr, 5); // type of transistor
                    if (strncasecmp(word, "pch", 3) == 0)
                    {
                        array_insert_last(trans *, Gate->pChannel, Trans); // pull-up network

                        word = array_fetch(char *, wordArr, 1);
                        Net = createNewNet(word, Gate);
                        array_insert_last(trans *, Net->pchArr, Trans);
                        Trans->drain = Net;

                        word = array_fetch(char *, wordArr, 2);
                        Net = createNewNet(word, Gate);
                        array_insert_last(trans *, Net->pchArr, Trans);
                        Trans->gate = Net;

                        word = array_fetch(char *, wordArr, 3);
                        Net = createNewNet(word, Gate);
                        array_insert_last(trans *, Net->pchArr, Trans);
                        Trans->source = Net;
                    }

                    else if (strncasecmp(word, "nch", 3) == 0)
                    {
                        array_insert_last(trans *, Gate->nChannel, Trans); // pull-down network

                        word = array_fetch(char *, wordArr, 1);
                        Net = createNewNet(word, Gate);
                        array_insert_last(trans *, Net->nchArr, Trans);
                        Trans->drain = Net;

                        word = array_fetch(char *, wordArr, 2);
                        Net = createNewNet(word, Gate);
                        array_insert_last(trans *, Net->nchArr, Trans);
                        Trans->gate = Net;

                        word = array_fetch(char *, wordArr, 3);
                        Net = createNewNet(word, Gate);
                        array_insert_last(trans *, Net->nchArr, Trans);
                        Trans->source = Net;
                    }
                }
                else if (strncasecmp(header, ".ENDS", 5) == 0) // one gate is done
                {
                    getResistance(Gate); // calculate the effective Wmin and Wmax for Max & Min delay
                }
            }
        }
        fclose(libertyFile); // library parsing is done
    }

    globEdges = G->edgArr;
    fscanf(fptr, "%s", pstr);
    while (!strstr(pstr, "module"))
        fscanf(fptr, "%s", pstr);

    // now read the input file
    while (!feof(fptr))
    {
        fscanf(fptr, "%[^\n]\n", pstr);

        if (pstr[0] == '/' && pstr[1] == '/')
            continue; // line comment

        if (pstr[0] != '/' || pstr[1] != '/')
        {
            // remove trailing white spaces
            for (i = strlen(pstr) - 1; i >= 0; i--)
            {
                if (pstr[i] == ' ')
                    pstr[i] = '\0';
                else
                    break;
            }

            while (pstr[strlen(pstr) - 1] != ';' && !strstr(pstr, "endmodule"))
            {
                fscanf(fptr, "%[^\n]\n", pstr1);

                // remove trailing white spaces
                for (i = strlen(pstr1) - 1; i >= 0; i--)
                {
                    if (pstr1[i] == ' ')
                        pstr1[i] = '\0';
                    else
                        break;
                }

                strcat(pstr, pstr1);
            }
        }

        wordArr = breakInWordsVlog(pstr);
        word = array_fetch(char *, wordArr, 0);
        nvector = 0;
        if (!strncasecmp(word, "input", 5))
        {
            nInVar += array_n(wordArr) - 1;
            for (i = 1; i < array_n(wordArr); i++)
            {
                word = array_fetch(char *, wordArr, i);
                if (word[0] == '[') // jonathan: vector identified
                {

                    nvector = strtoul(&word[1], NULL, 10);
                    continue;
                }

                else if (nvector > 0)
                {
                    char *temp_word = (char *)malloc(sizeof(char) * 400);
                    char *buffer = (char *)malloc(sizeof(char) * 400);
                    int j;
                    for (j = 0; j < nvector + 1; j++)
                    {

                        strcpy(temp_word, word);
                        strcat(temp_word, "[%d]\0");
                        sprintf(buffer, temp_word, j);
                        array_insert_last(char *, inVarArr, strdup(buffer));
                    }
                    nInVar += nvector - 1;
                    nvector = 0;
                    free(temp_word);
                }
                else
                {
                    array_insert_last(char *, inVarArr, strdup(word));
                }
            }
        }
        else if (!strncasecmp(word, "output", 6))
        {
            nOutVar += array_n(wordArr) - 1;
            for (i = 1; i < array_n(wordArr); i++)
            {
                word = array_fetch(char *, wordArr, i);
                if (word[0] == '[') // jonathan: vector identified
                {

                    nvector = strtoul(&word[1], NULL, 10); // retrieve size of list
                    continue;
                }

                else if (nvector > 0)
                {
                    char *temp_word = (char *)malloc(sizeof(char) * 400);
                    char *buffer = (char *)malloc(sizeof(char) * 400);
                    int j;
                    for (j = 0; j < nvector + 1; j++)
                    {

                        strcpy(temp_word, word);
                        strcat(temp_word, "[%d]\0");
                        sprintf(buffer, temp_word, j);
                        array_insert_last(char *, outVarArr, strdup(buffer));
                    }
                    nOutVar += nvector - 1;
                    nvector = 0;
                    free(temp_word);
                }
                else
                {
                    array_insert_last(char *, outVarArr, strdup(word));
                }
            }
        }
        else if (!strncasecmp(word, "assign", 6))
        {

            if (!libertyflag)
            {
                strcpy(gatetype, "nb1s1");
                sprintf(gatename, "AU%d", assigncnt++);
            }
            else
            {
                strcpy(gatetype, "BUFX2");
                sprintf(gatename, "AU%d", assigncnt++);
            }
            createNewVertexForVlog(G, wordArr, gatetype, strdup(gatename), G->edgArr, libertyflag);
        }
        else if (!strncasecmp(word, "wire", 4) // || !strncasecmp(word, "assign", 6)
                 || !strncasecmp(word, "endmodule", 9) || !strncasecmp(word, "tri", 3) || (word[0] == '/' && word[1] == '/'))
        {
            continue;
        }
        else if (nInVar && nOutVar)
        {
            gatetype = array_fetch(char *, wordArr, 0);
            gatename = array_fetch(char *, wordArr, 1);
            /*if (strstr(gatetype, "dff") && strstr(array_fetch(char*, wordArr, 2), "QN"))
            {
                inidx = 5;
                extraDffIn++;
                if (strstr(array_fetch(char*, wordArr, 4), "Q"))
                {
                    outputVar = array_fetch(char*, wordArr, 5);
                    inidx = 7;
                }
                else  // create a new edge
                {
                    Qnet = (char*) malloc(sizeof(char) * 120);
                    sprintf(Qnet, "qtmp%d", extraDffIn);
                    outputVar = Qnet;
                }
                createNewVertexForVlog(G, wordArr, gatetype, gatename, G->edgArr, libertyflag);

                // insert an inverter
                invout = array_fetch(char*, wordArr, 3);
                invname = (char*) malloc(sizeof(char) * 120);
                sprintf(invname, "IU%d", extraDffIn);
                createInverter(G, outputVar, invout, invname, globEdges);
            }
            else
            {
                // for one output variable
                outputVar = array_fetch(char*, wordArr, 3);
                createNewVertexForVlog(G, wordArr, gatetype, gatename, G->edgArr);
            }*/
            createNewVertexForVlog(G, wordArr, gatetype, gatename, G->edgArr, libertyflag);
        }
    }
    fclose(fptr);

    /* find the set of primary input and output edges in the graph */
    /* first break the header into words & store in an arr */
    //printf("InVar: %d \t OutVar: %d\n", nInVar, nOutVar);
    for (i = 0; i < nInVar; i++)
    {
        word = array_fetch(char *, inVarArr, i);
        edg = findEdge(word, globEdges);
        if (!edg)
            printf("Warning: Unconnected primary input edge %s\n", word);
        else if (edg->fromNode != NULL)
            printf("ERROR: Inprimary edge assertion failure: %s\n", word);
        else
        {
            edg->bPrimIn = true;
            array_insert_last(edge *, G->primInEdges, edg);
            (*primInCnt)++;
        }
    }

    for (i = 0; i < nOutVar; i++)
    {
        word = array_fetch(char *, outVarArr, i);
        edg = findEdge(word, globEdges);
        if (!edg)
            printf("Warning: Unconnected primary output edge %s\n", word);
        else
        {
            edg->bPrimOut = true;
            array_insert_last(edge *, G->primOutEdges, edg);
            ocnt++;
        }
    }

    // printf("Number of primary inputs: %d primary outputs: %d\n", icnt, ocnt);
    array_free(inVarArr);
    array_free(outVarArr);
    free(pstr);
    free(pstr1);
    // free(header);
    // free(gatename);
    // free(gatetype);
    return G;
}

array_t *breakInWordsVlog(char *header)
{
    array_t *wordArr = array_alloc(char *, 1);
    char *word = (char *)malloc(sizeof(char) * 120);
    int i, j, k;
    int cnt = 0;

    word[0] = '\0';
    i = j = 0;

    /* get rid of initial blanks */
    while (header[i] == ' ')
        i++;

    while (i < strlen(header))
    {
        if (header[i] == ' ' || header[i] == '(' || header[i] == ')' ||
            header[i] == ',' || header[i] == ';')
        {
            word[j] = '\0';
            array_insert_last(char *, wordArr, strdup(word));
            word = (char *)malloc(sizeof(char) * 120);
            j = 0;

            i++;
            while (header[i] == ' ' || header[i] == '(' || header[i] == ')' ||
                   header[i] == ',' || header[i] == ';')
                i++;
        }
        else
        {
            word[j++] = header[i];
            i++;
        }
    }

    /* add the last word */
    if (j)
    {
        word[j] = '\0';
        array_insert_last(char *, wordArr, strdup(word));
    }

    free(word);
    return wordArr;
}

/* create a vertex of given type - written for generating graph from gate-level verilog mapped to LEDA library */
vertex *createNewVertexForVlog(graph *G, array_t *wordArr, char *cellName, char *nodeName, array_t *globEdges, int libertyflag)
{
    int i, j, k, bdff = false;
    char *str, *tstr, *word, *lit1, *lit2;
    edge *edg;
    vertex *vert;
    vert = (vertex *)malloc(sizeof(vertex));
    vert->type = (Type *)malloc(sizeof(Type));
    vert->idx = globalVertIdx++;
    vert->suspect = 0;
    vert->topo = 0;
    vert->visited = 0;
    vert->bPrimIn = 0;
    vert->bPrimOut = 0;
    vert->isSeq = 0;
    vert->bDummy = 0;
    vert->gatingCtrl = "";
    vert->bDeleted = 0;
    vert->type = findType(cellName, G->typeArr);
    vert->level = 0;
    vert->virtualFlag = 0;
    vert->alpha = 1;
    vert->criticalFlag = 0;
    vert->slackConst = 0;
    vert->sizeFactor = 1;
    vert->activity = 0;
    vert->nInput00 = 0;
    vert->nInput01 = 0;
    vert->nInput10 = 0;
    vert->nInput11 = 0;
    vert->name = strdup(nodeName);
    vert->inEdges = array_alloc(edge *, 1);
    vert->outEdges = array_alloc(edge *, 1);
    vert->inPorts = array_alloc(char *, 1);
    vert->outPorts = array_alloc(char *, 1);
    vert->delayArr = array_alloc(int, 1);
    vert->delayLRS = array_alloc(float, 1); /* FIXUP */
    vert->isScanDFF = 0;
    vert->worstDelay = 0 ;
    vert->sulliedDelay = 0;
    // insert buffer for assign statement
    word = array_fetch(char *, wordArr, 2);
    if (strchr(word, '='))
    {
        edg = createEdge(G, array_fetch(char *, wordArr, 1)); // output edge
        edg->fromNode = vert;
        array_insert_last(edge *, vert->outEdges, edg);
        if (!libertyflag)
            array_insert_last(char *, vert->outPorts, ".Q");
        else
            array_insert_last(char *, vert->outPorts, ".Y");
        word = array_fetch(char *, wordArr, 3);
        if (!strcmp(word, "1'b0") || !strcmp(word, "0"))
            word = "GND";
        if (!strcmp(word, "1'b1") || !strcmp(word, "1"))
            word = "VDD";
        edg = createEdge(G, word); // input edge
        array_insert_last(vertex *, edg->toNodes, vert);
        array_insert_last(edge *, vert->inEdges, edg);
        if (!libertyflag)
            array_insert_last(char *, vert->inPorts, ".DIN");
        else
            array_insert_last(char *, vert->inPorts, ".A");
        array_insert_last(vertex *, G->vertexArr, vert);
        array_insert_last(vertex *, G->combGates, vert);
        return vert;
    }

    if (strcasestr(cellName, "dff") || strcasestr(cellName, "latch"))
    {
        vert->isSeq = true;
        if (strcasestr(cellName, "sdff") || fullScan)
        { // updated by Pravin on 10/7/19
            vert->isScanDFF = 1;
            array_insert_last(vertex *, G->scanDFF, vert);
        }
        else
            array_insert_last(vertex *, G->nonscanDFF, vert);
        array_insert_last(vertex *, G->seqGates, vert);
    }
    else
        array_insert_last(vertex *, G->combGates, vert);
    array_insert_last(vertex *, G->vertexArr, vert);

    // first create the output edges
    for (i = 2; i < array_n(wordArr); i += 2)
    {
        tstr = array_fetch(char *, wordArr, i);
        if (!strcasecmp(tstr, ".q", 2) || !strncasecmp(tstr, ".out", 4) || !strncasecmp(tstr, ".y", 2) || !strncasecmp(tstr, ".zn", 3))
        {
            word = array_fetch(char *, wordArr, i + 1);
            edg = createEdge(G, word);
            edg->fromNode = vert;
            array_insert_last(edge *, vert->outEdges, edg);
            array_insert_last(char *, vert->outPorts, tstr);
        }
    }

    // now create the input edges
    for (i = 2; i < array_n(wordArr); i += 2)
    {
        tstr = array_fetch(char *, wordArr, i);
        // easier to enumerate outputs than inputs. Therefore, check for NOT an output match
        if (!(!strcasecmp(tstr, ".q", 2) || !strncasecmp(tstr, ".out", 4) || !strncasecmp(tstr, ".y", 2) || !strncasecmp(tstr, ".zn", 3)))
        {
            word = array_fetch(char *, wordArr, i + 1);

            if (word[0] == '!')
                word += 1;
            if (!strcmp(word, "0") || !strcmp(word, "1'b0"))
            {
                word = "GND";
                // printf("INFO: Ground inserted\n");
            }
            else if (!strcmp(word, "1") || !strcmp(word, "1'b1"))
            {
                word = "VDD";
                // printf("INFO: VDD inserted \n");
            }
            edg = createEdge(G, word);
            array_insert_last(vertex *, edg->toNodes, vert);
            array_insert_last(edge *, vert->inEdges, edg);
            array_insert_last(char *, vert->inPorts, tstr);
        }
    }

    char *tempword1;
    array_t *tempPorts = array_alloc(char *, 1);
    tempPorts = array_dup(vert->inPorts); // temp arrays
    array_t *tempEdges = array_alloc(edge *, 1);
    tempEdges = array_dup(vert->inEdges); // temp arrays
    int vertLen = array_n(vert->inPorts);
    for (i = 0; i < vertLen - 1; i++)
    {
        // word = array_fetch(char*,vert->inPorts, i);
        // iter = i;
        for (int j = 0; j < vertLen - i - 1; j++)
        {
            word = array_fetch(char *, vert->inPorts, j);
            tempword1 = array_fetch(char *, vert->inPorts, j + 1);

            if (strcmp(tempword1, word) < 0)
            {
                array_insert(char *, vert->inPorts, j, tempword1);
                array_insert(char *, vert->inPorts, j + 1, word);
                // iter = j;
            }
        }
    }

    // map edge positions to sorted ports
    for (i = 0; i < array_n(vert->inPorts); i++)
    {
        word = array_fetch(char *, vert->inPorts, i);
        for (j = 0; j < array_n(tempPorts); j++)
        {
            tempword1 = array_fetch(char *, tempPorts, j);
            if ((strlen(word) == strlen(tempword1)) && strstr(word, tempword1))
            {
                break;
            }
        }
        edg = array_fetch(edge *, tempEdges, j);
        array_insert(edge *, vert->inEdges, i, edg);
    }

    return vert;
}

/* createEdge: Edge structure initializatioin */
edge *createEdge(graph *G, char *name)
{
    edge *edg;
    char *modName = name;

    edg = findEdge(name, G->edgArr);

    if (edg == NULL) /* no edge present, create one */
    {
        edg = (edge *)malloc(sizeof(edge));
        edg->idx = globalEdgIdx++;
        edg->toNodes = array_alloc(vertex *, 1);
        edg->cplCap = array_alloc(parasite *, 1);
        edg->switchTimeArr = array_alloc(float, 1);
        edg->assnValues = array_alloc(int, 1);
        edg->prmEdgArr = array_alloc(edge *, 1);
        edg->timeEdgArr = array_alloc(edge *, 1);
        edg->crtEdgArr = array_alloc(edge *, 1);
        edg->switchArr = array_alloc(int, 1);
        edg->tmpSwitchArr = array_alloc(int, 1);
        edg->swTimeArr = array_alloc(float, 1);
        edg->tmpSwTimeArr = array_alloc(float, 1);
        edg->rDlyMaxArr = array_alloc(float, 1);
        edg->rDlyMinArr = array_alloc(float, 1);
        edg->fDlyMaxArr = array_alloc(float, 1);
        edg->fDlyMinArr = array_alloc(float, 1);
        edg->gateDlyRArr = array_alloc(float, 1);
        edg->gateDlyFArr = array_alloc(float, 1);
        edg->TtimeRmaxArr = array_alloc(float, 1);
        edg->TtimeRminArr = array_alloc(float, 1);
        edg->TtimeFmaxArr = array_alloc(float, 1);
        edg->TtimeFminArr = array_alloc(float, 1);
        edg->TtimeRArr = array_alloc(float, 1);
        edg->TtimeFArr = array_alloc(float, 1);
        edg->tmpPropArr = array_alloc(int, 1);
        edg->rDelayMaxArr = array_alloc(float, 1);
        edg->rDelayMinArr = array_alloc(float, 1);
        edg->fDelayMaxArr = array_alloc(float, 1);
        edg->fDelayMinArr = array_alloc(float, 1);
        edg->valueArr = array_alloc(uint8_t, 1);
        edg->fromNode = NULL;
        edg->maxCcEdg = NULL;
        edg->mainPrmIn = NULL;
        edg->name = modName;
        edg->cplCapFlag = -1;
        edg->gndCap = 0.0;
        edg->maxCc = 0;
        edg->maxCcConflict = 0;
        edg->tmpMaxDelay = 0;
        edg->rDelayMax = 0;
        edg->rDelayMaxOrg = 0;
        edg->rDelayMaxTmp1 = 0;
        edg->rDelayMin = 0;
        edg->rDelayMinOrg = 0;
        edg->rDelayMinTmp1 = 0;
        edg->fDelayMax = 0;
        edg->fDelayMaxOrg = 0;
        edg->fDelayMaxTmp1 = 0;
        edg->fDelayMin = 0;
        edg->fDelayMinOrg = 0;
        edg->fDelayMinTmp1 = 0;
        edg->maxDelay = 0;
        edg->minDelay = 0;
        edg->gateDelayR = 0;
        edg->gateDelayRTmp = 0;
        edg->gateDelayROrg = 0;
        edg->gateDelayF = 0;
        edg->gateDelayFTmp = 0;
        edg->gateDelayFOrg = 0;
        edg->weight = 55;
        edg->TtimeR = 20e-12;
        edg->TtimeROrg = edg->TtimeR;
        edg->TtimeRTmp = edg->TtimeR;
        edg->TtimeF = 20e-12;
        edg->TtimeFOrg = edg->TtimeF;
        edg->TtimeFTmp = edg->TtimeF;
        edg->TtimeRmax = 20e-12;
        edg->TtimeRmaxOrg = edg->TtimeRmax;
        edg->TtimeRmaxTmp = edg->TtimeRmax;
        edg->TtimeRmin = 20e-12;
        edg->TtimeRminOrg = edg->TtimeRmin;
        edg->TtimeRminTmp = edg->TtimeRmin;
        edg->TtimeFmax = 20e-12;
        edg->TtimeFmaxOrg = edg->TtimeRmax;
        edg->TtimeFmaxTmp = edg->TtimeRmax;
        edg->TtimeFmin = 20e-12;
        edg->TtimeFminOrg = edg->TtimeRmin;
        edg->TtimeFminTmp = edg->TtimeRmin;
        edg->assnFlag = 0;
        edg->assnTran = 55; /* Unassigned Transistion */
        edg->tmpAssign1 = 55;
        edg->tmpAssign2 = 55;
        edg->tmpAssign2Org = 55;
        edg->tmpAssign3 = 55;
        edg->tmpTranTime = 0;
        edg->tmpTranTime2 = 55;
        edg->switchTime = 55;
        edg->logicProp = 55;
        edg->crEdgConflict = 0;
        edg->timeOutFlag = 55;
        edg->progressEdgIndex = -1;
        edg->tpd = -1;
        edg->bPrimIn = 0;  /* 0: not Primary Input */
        edg->bPrimOut = 0; /* 0: not Primary Input */
        edg->oldVal = -1;  /* value -1 means undefined */
        edg->newVal = -1;  /* value -1 means undefined */
        edg->signal0Prob = 0;
        edg->signal1Prob = 0;
        edg->activity01 = 0;
        edg->activity10 = 0;

        /* variances for process variation */
        edg->rDelMaxVar = 0;
        edg->fDelMaxVar = 0;
        edg->rDelMinVar = 0;
        edg->fDelMinVar = 0;
        edg->lamda = 0;
        array_insert_last(edge *, G->edgArr, edg);
    }
    return edg;
}

vertex *createInverter(graph *G, char *inputVar, char *outputVar, char *nodeName, array_t *globEdges)
{
    int i, j, k;
    char *str, *tstr, *word, *lit1, *lit2;
    edge *edg;
    vertex *vert;

    vert = (vertex *)malloc(sizeof(vertex));
    vert->type = (Type *)malloc(sizeof(Type));
    vert->idx = globalVertIdx++;
    vert->suspect = 0;
    vert->topo = 0;
    vert->visited = 0;
    vert->bPrimIn = 0;
    vert->type = findType("hi1s1", G->typeArr);
    vert->level = 0;
    vert->virtualFlag = 0;
    vert->alpha = 1;
    vert->criticalFlag = 0;
    vert->slackConst = 0;
    vert->sizeFactor = 1;
    vert->activity = 0;
    vert->nInput00 = 0;
    vert->nInput01 = 0;
    vert->nInput10 = 0;
    vert->nInput11 = 0;
    vert->inPorts = array_alloc(char *, 1);
    vert->outPorts = array_alloc(char *, 1);
    vert->name = nodeName;
    vert->inEdges = array_alloc(edge *, 1);
    vert->outEdges = array_alloc(edge *, 1);
    vert->delayArr = array_alloc(int, 1);
    vert->delayLRS = array_alloc(float, 1); /* FIXUP */

    // create output edges
    edg = createEdge(G, outputVar);
    edg->fromNode = vert;
    array_insert_last(edge *, vert->outEdges, edg);
    array_insert_last(char *, vert->outPorts, ".Q");

    // create input edge
    edg = createEdge(G, inputVar);
    array_insert_last(char *, vert->inPorts, ".DIN");
    array_insert_last(vertex *, edg->toNodes, vert);
    array_insert_last(edge *, vert->inEdges, edg);
    array_insert_last(vertex *, G->vertexArr, vert);

    return vert;
}
// write out hmetis file for graph representation

void writeHMETIS(graph *G, char *moduleName)
{
    FILE *fptr;
    edge *edg;
    vertex *v;
    char *hMetisFileName, *vertexIDFileName, *edgeIDFileName;
    hMetisFileName = (char *)malloc(sizeof(char) * 400);
    vertexIDFileName = (char *)malloc(sizeof(char) * 400);
    edgeIDFileName = (char *)malloc(sizeof(char) * 400);
    sprintf(hMetisFileName, "%s_hmetis.txt", moduleName);
    sprintf(vertexIDFileName, "%s_vertexID.txt", moduleName);
    sprintf(edgeIDFileName, "%s_edgeID.txt", moduleName);
    fptr = fopen(hMetisFileName, "w");
    int i, j;
    printf("Writing HMETIS file to %s...\n", hMetisFileName);
    fprintf(fptr, "%d %d\n", array_n(G->edgArr), array_n(G->vertexArr));
    for (i = 0; i < array_n(G->edgArr); i++)
    {
        edg = array_fetch(edge *, G->edgArr, i);
        if (!strncasecmp(edg->name, "gnd", 3) || !strncasecmp(edg->name, "vcc", 3)) // ignore ground and vcc
            continue;
        if (!edg->bPrimOut)
        {
            for (j = 0; j < array_n(edg->toNodes) - 1; j++)
            {
                v = array_fetch(vertex *, edg->toNodes, j);
                fprintf(fptr, "%d ", v->idx);
            }
            v = array_fetch(vertex *, edg->toNodes, array_n(edg->toNodes) - 1);
            fprintf(fptr, "%d ", v->idx);
        }
        if (!edg->bPrimIn)
        {
            v = edg->fromNode;
            fprintf(fptr, "%d", v->idx);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
    fptr = fopen(vertexIDFileName, "w");
    for (i = 0; i < array_n(G->vertexArr); i++)
        fprintf(fptr, "%d:%s\n", i, array_fetch(vertex *, G->vertexArr, i)->name);
    fclose(fptr);
    fptr = fopen(edgeIDFileName, "w");
    for (i = 0; i < array_n(G->edgArr); i++)
    {
        edg = array_fetch(edge *, G->edgArr, i);
        if (!strncasecmp(edg->name, "gnd", 3) || !strncasecmp(edg->name, "vcc", 3)) // ignore ground and vcc
            continue;
        fprintf(fptr, "%s\n", edg->name);
    }
    fclose(fptr);
}

array_t* findVector(array_t* vector,char* name){
    
    int i;
    vectorList* fetchedVector;
    for(i=0;i<array_n(vector);i++){
       fetchedVector = array_fetch(vectorList*,vector,i);
       if(!strcmp(fetchedVector->name,name)){
           return fetchedVector;  
       }
    }

    return NULL;
}

array_t* vectorExtractor(array_t* edgeArray){

    array_t* store = array_alloc(vectorList*,1); //store the extracted input output ports in here 
    int i,n;
    edge* edg;
    char* port;
    char* substr;
    vectorList* fetchedVector;

    for(i=0;i<array_n(edgeArray);i++){
        edg = array_fetch(edge*,edgeArray,i);
        port = strdup(array_fetch(edge*,edgeArray,i)->name);
        substr = strtok(port,"[");
        fetchedVector = findVector(store,substr);
        if(fetchedVector == NULL){
            vectorList* newVector = (vectorList*)malloc(sizeof(vectorList));
            newVector->name = strdup(substr);
            substr = strtok(NULL,"]");
            if(substr != NULL){
                sscanf(substr,"%d",&newVector->max);
                sscanf(substr,"%d",&newVector->min);
                newVector->bPrim = (edg->bPrimIn || edg->bPrimOut) ;
            }else{
                newVector->max = 0 ;
                newVector->min = 0 ;
                newVector->bPrim = (edg->bPrimIn || edg->bPrimOut) ;     
            }
            
            array_insert_last(vectorList*,store,newVector);            
        }else{
            substr = strtok(NULL,"]");
            if(substr != NULL){
                sscanf(substr,"%d",&n);
                if(n > fetchedVector->max){
                    fetchedVector->max = n ;
                }else if(n < fetchedVector->min){
                    fetchedVector->min = n ;
                }
            }            
        }

    }

    return store;

}

void writeVerilog(config_param* param){

    array_t* store;
    array_t* vertArr;
    vectorList* fetchedVector;
    graph *G;
    vertex* vert;
    edge* edg;
    FILE *out;
    int i,j,n;
    char tempString[10000];

    G = param->G ;
    vertArr = G->vertexArr;
    out = param->out;

    //print out the comments

    //print out the module and input/output parameter list of the verilog file
    store = vectorExtractor(G->primInEdges);

    fprintf(out, "module %s(\n",G->header);
    
    for(i=0;i<array_n(store);i++){
        fprintf(out,"    input ");
        fetchedVector = array_fetch(vectorList*,store,i);                
        if(fetchedVector->max == 0 && fetchedVector->min == 0){
            fprintf(out,"%s,\n",fetchedVector->name);
        }else{
            fprintf(out,"[%d:%d] %s,\n",fetchedVector->max,fetchedVector->min,fetchedVector->name);
        }        
    }
    array_free(store);
    store = vectorExtractor(G->primOutEdges);
    
    for(i=0;i<array_n(store);i++){
        fprintf(out,"    output ");
        fetchedVector = array_fetch(vectorList*,store,i);                
        if(fetchedVector->max == 0 && fetchedVector->min == 0){
            fprintf(out,"%s",fetchedVector->name);
        }else{
            fprintf(out,"[%d:%d] %s",fetchedVector->max,fetchedVector->min,fetchedVector->name);
        } 
        if(i!=array_n(store)-1){
            fprintf(out,",\n");
        }       
    }
    fprintf(out, ");\n\n");

    array_free(store);

    //print out the wires and input output ports of the verilog file
    
    n=0;
    store = vectorExtractor(G->edgArr);    

    for(i=0;i<array_n(store);i++){
        fetchedVector = array_fetch(vectorList*,store,i);
        if(fetchedVector->bPrim || (fetchedVector->max != 0 && fetchedVector->max != 0) ){
            continue;
        }

        if(n==0){
            fprintf(out,"    wire %s",fetchedVector->name);
        }else if(n < 20){
            fprintf(out,", %s",fetchedVector->name);
        }else{
            fprintf(out,";\n    wire %s",fetchedVector->name);
            n = 0;
        }
        
        n++;
    }

    fprintf(out,";\n");

    for(i=0;i<array_n(store);i++){        
        fetchedVector = array_fetch(vectorList*,store,i);                
        if(fetchedVector->max == 0 && fetchedVector->min == 0){
            continue;
        }else if(fetchedVector->bPrim != 1){
            fprintf(out,"    wire [%d:%d] %s;\n",fetchedVector->max,fetchedVector->min,fetchedVector->name);
        }               
    }

    fprintf(out,"\n\n");
    

    array_free(store);
        
    //print out the body of the verilog file

    for(i=0;i<array_n(vertArr);i++){
        vert = array_fetch(vertex*,vertArr,i);
        fprintf(out,"   %s%s %s (",vert->comment?"//":"",vert->type->name,vert->name);
        for(j=0;j<array_n(vert->inPorts);j++){
            char* portname;
            int freePortName = 0;
            if(!strcmp(array_fetch(edge*,vert->inEdges,j)->name,"VDD")){
                portname = strdup("1'b1");
                freePortName = 1;
            }else if(!strcmp(array_fetch(edge*,vert->inEdges,j)->name,"GND")){
                portname = strdup("1'b0");
                freePortName = 1;
            } else{
                portname = array_fetch(edge*,vert->inEdges,j)->name;
            }

            fprintf(out," %s(%s), ",array_fetch(char*,vert->inPorts,j),portname);

            if(freePortName){
                free(portname);
            }
        }
        for(j=0;j<array_n(vert->outPorts);j++){
            if(j!=0){
                fprintf(out,", ");
            }
            fprintf(out,"%s(%s)",array_fetch(char*,vert->outPorts,j),array_fetch(edge*,vert->outEdges,j)->name);
        }
        if(vert->comment){
            sprintf(tempString," // this node is a part of PUF number: %d",vert->pufNumber);
        }
        fprintf(out,");%s\n",vert->comment?tempString:"");
    }

    //print out the endmodule of the verilog file

    fprintf(out, "endmodule\n\n\n");

}

void vertex_edge_duplicate(array_t* oldVertArr,array_t* oldEdgeArr,array_t* newVertArr,array_t* newEdgeArr){

    //newVertArr = array_alloc(vertex*,1);
    //newEdgeArr = array_alloc(edge*,1);
    int i,j;
    vertex* vert;
    vertex* tempVertex;
    vertex* newVert;
    edge* edg;
    edge* tempEdg;
    edge* newEdge;

    for(i=0;i<array_n(oldVertArr);i++){
        vert = array_fetch(vertex*,oldVertArr,i);

        vertex* newVertex = (vertex*)malloc(sizeof(vertex));                
        
        char* dupName;

        dupName = strdup(vert->name);
        if(dupName == NULL){
            int nameLen = strlen(vert->name);
            
            dupName = (char*) malloc((nameLen+10)*sizeof(char));
            if (dupName == NULL) {
                printf("ATRI DEBUG: Serious Problem. Unable to strdup and then malloc the vertex name : %s.\n",vert->name);
            }
            sprintf(dupName,"%s",vert->name);
        }
        // newVertex->name = strdup(vert->name); // For big designs this is unable to strdup some nodes.
        newVertex->name = strdup(dupName);
        newVertex->bPrimIn = vert->bPrimIn;
        newVertex->bPrimOut = vert->bPrimOut;
        newVertex->type = vert->type;
        newVertex->inPorts = array_alloc(char*,1);
        newVertex->outPorts = array_alloc(char*,1);
        newVertex->inEdges = array_alloc(edge*,1);
        newVertex->outEdges = array_alloc(edge*,1);
        newVertex->worstSlack = vert->worstSlack;
        newVertex->worstDelay = vert->worstDelay;
        newVertex->sulliedDelay = vert->sulliedDelay;
        newVertex->comment = vert->comment;
        newVertex->pufNumber = vert->pufNumber;
        newVertex->idx = vert->idx;

        for(j=0;j<array_n(vert->inPorts);j++){
            char* portname = array_fetch(char*,vert->inPorts,j);            
            array_insert_last(char*,newVertex->inPorts,strdup(portname));
        }

        for(j=0;j<array_n(vert->outPorts);j++){
            char* portname = array_fetch(char*,vert->outPorts,j);            
            array_insert_last(char*,newVertex->outPorts,strdup(portname));
        }
        
        //array_t* inEdges;
        //array_t* outEdges;      

        array_insert_last(vertex*,newVertArr,newVertex);
    }

    for(i=0;i<array_n(oldEdgeArr);i++){
        edg = array_fetch(edge*,oldEdgeArr,i);

        edge* newEdge = (edge*)malloc(sizeof(edge));
               
        newEdge->name = strdup(edg->name);                
        newEdge->bPrimIn = edg->bPrimIn;
        newEdge->bPrimOut = edg->bPrimOut;
        newEdge->toNodes = array_alloc(vertex*,1);
        newEdge->idx = edg->idx;
        newEdge->done = edg->done;

        if(edg->bPrimIn != 1){
            if(edg->fromNode != NULL){            
                tempVertex = findVertex(edg->fromNode->name,newVertArr);
                if(tempVertex != NULL){
                    newEdge->fromNode = tempVertex ;
                }else{
                    printf("ERROR: Null returned when querying: %s from the new vertexArr nested 0.\n",edg->fromNode->name);
                }
            }
        }     
                    
        for(j=0;j<array_n(edg->toNodes);j++){
            vert = array_fetch(vertex*,edg->toNodes,j);

            tempVertex = findVertex(vert->name,newVertArr);

            if(tempVertex != NULL){
                array_insert_last(vertex*,newEdge->toNodes,tempVertex);
            }else{
                printf("ERROR: Null returned when querying: %s from the new vertexArr.nested 1\n",vert->name);
            }
        }

        array_insert_last(edge*,newEdgeArr,newEdge);

    }

    for(i=0;i<array_n(oldVertArr);i++){

        vert = array_fetch(vertex*,oldVertArr,i);
        newVert = array_fetch(vertex*,newVertArr,i);

        // newVert->inEdges = array_alloc(edge*,1);
        // newVert->outEdges = array_alloc(edge*,1);

        for(j=0;j<array_n(vert->inEdges);j++){
            edg = array_fetch(edge*,vert->inEdges,j);
            newEdge = findEdge(edg->name,newEdgeArr);

            if(newEdge != NULL){
                array_insert_last(edge*,newVert->inEdges,newEdge);
            }else{
                printf("ERROR: Null returned when querying: %s from newEdgeArr.\n",edg->name);
            }

        }

        for(j=0;j<array_n(vert->outEdges);j++){
            edg = array_fetch(edge*,vert->outEdges,j);
            newEdge = findEdge(edg->name,newEdgeArr);

            if(newEdge != NULL){
                array_insert_last(edge*,newVert->outEdges,newEdge);
            }else{
                printf("ERROR: Null returned when querying: %s from newEdgeArr.\n",edg->name);
            }

        }

    }    

}

void freeG(graph* G){

    free(G->header);
    freeVertArr(G->vertexArr);
    freeEdgeArr(G->edgArr);    
    // array_free(G->vertexArr);
    //array_free(G->edgArr);    
    array_free(G->primInEdges);
    array_free(G->primOutEdges);
    // if(array_n(G->comments) > 0){  //creates seg fault fix later
    //     array_free(G->comments);
    // }
    
    free(G);
    G = NULL;

}

void freeVertArr(array_t* vertexArr){

    vertex* vert;

    for(int i=0;i<array_n(vertexArr);i++){
        vert = array_fetch(vertex*,vertexArr,i);
        free(vert->name);
        
        // for(int j=0; j<array_n(vert->inPorts);j++){
        //     free(array_fetch(char*,vert->inPorts,j));
        // }
        
        // for(int j=0; j<array_n(vert->outPorts);j++){
        //     free(array_fetch(char*,vert->outPorts,j));
        // }
        array_free(vert->inEdges);
        array_free(vert->outEdges);
        array_free(vert->inPorts); //Uncomment here
        array_free(vert->outPorts); //Uncomment here
        
        free(vert); //Uncomment here
        
    }

    array_free(vertexArr); //Uncomment here

}

void freeEdgeArr(array_t* edgArr){

    edge* edg;

    for(int i=0;i<array_n(edgArr);i++){
        edg = array_fetch(edge*,edgArr,i);
        // printf("ATRI Degib: Line 2039, edge name = %s.\n",edg->name);
        // free(edg->name);
        if(array_n(edg->toNodes) != 0){
            array_free(edg->toNodes);
        }
        
        if(!(edg->bPrimIn || edg->bPrimOut)){
            // printf("ATRI Degib: Line 2045, free edge iterable i = %d.\n",i);
            free(edg);
        }      
        
    }

    array_free(edgArr);

}

void duplicateGraph(graph* oldG,graph* newG){

    array_t* vertArr = array_alloc(vertex*,1);
    array_t* edgArr = array_alloc(edge*,1);
    array_t* comments = array_alloc(char*,1);
    newG->primInEdges = array_alloc(edge*,1);
    newG->primOutEdges = array_alloc(edge*,1);
    edge* edg;
    char* string;

    vertex_edge_duplicate(oldG->vertexArr,oldG->edgArr,vertArr,edgArr);

    newG->header = strdup(oldG->header);
    newG->vertexArr = vertArr;
    newG->edgArr = edgArr;
    newG->typeArr = oldG->typeArr;
    newG->data_required_time = oldG->data_required_time ;

    for(int i=0; i<array_n(oldG->primInEdges); i++){
        edg = findEdge(array_fetch(edge*,oldG->primInEdges,i)->name,edgArr);
        array_insert_last(edge*,newG->primInEdges,edg);
    }

    for(int i=0; i< array_n(oldG->primOutEdges); i++){
        edg = findEdge(array_fetch(edge*,oldG->primOutEdges,i)->name,edgArr);
        array_insert_last(edge*,newG->primOutEdges,edg);
    }

    for(int i=0; i< array_n(oldG->comments); i++){
        string = array_fetch(char*,oldG->comments,i);
        array_insert_last(char*,newG->comments,strdup(string));
    }

}

void modifyGraph(graph* G,modifyInstructions* modify){

    int i,j,k,l,n;
    vertex *vert,*fetchedVert,*tempVert,*tempVert2;
    vertex *vert1,*vert2;
    edge *edg,*edg1,*edg2,*fetchedEdge,*selEdge,*newEdge;
    reuseIdList* fetchedResueID;
    char name[10000];
    char tempName[10000];
    char tempName2[10000];
    graph* template = modify->template;

    edg = createEdge(G,strdup("sel"));
    selEdge = edg;

    edg->bPrimIn = 1 ;
    array_insert_last(edge*,G->edgArr,edg);
    array_insert_last(edge*,G->primInEdges,edg);
    
    if(modify->operation == 0){ //this takes care of the puf insertion at the end

        for(i=0;i<array_n(modify->targetCells);i++){
            
            array_t* vertArr = array_alloc(vertex*,1);
            array_t* edgArr = array_alloc(edge*,1);

            //vertex* vivadoVertex;
            
            vert = array_fetch(vertex*,modify->targetCells,i);
            edg = array_fetch(edge*,vert->outEdges,0);
            edg2 = edg; //this contains the original edge, this should be connected to the output of the melpuf template        

            if(modify->dummy_insertion){ //inserting black boxes for vivado implementation
                vertex* vivadoVertex = (vertex*)malloc(sizeof(vertex));
                vivadoVertex->type = (Type*)malloc(sizeof(Type));
                vivadoVertex->type->name = template->header;
                sprintf(tempName,"puf_instance_%d",globalPufIndex);
                vivadoVertex->name = strdup(tempName);
                vivadoVertex->comment = 0; //this comments out the whole vertex. Need to rectify this later
                vivadoVertex->pufNumber = globalPufIndex;
                vivadoVertex->bPrimIn = 0;
                vivadoVertex->bPrimOut = 0;
                vivadoVertex->inPorts = array_alloc(char *, 1);
                vivadoVertex->outPorts = array_alloc(char *, 1);
                vivadoVertex->inEdges = array_alloc(edge *, 1);
                vivadoVertex->outEdges = array_alloc(edge *, 1);

                //add the primary input and output ports of the mentioned template

                for(j=0;j<array_n(template->edgArr);j++){
                    fetchedEdge = array_fetch(edge*,template->edgArr,j);
                    if(fetchedEdge->bPrimIn){
                        if(!strcmp(fetchedEdge->name,"sel")){
                            sprintf(tempName,".%s",fetchedEdge->name);
                            array_insert_last(char*,vivadoVertex->inPorts,strdup(tempName));
                            array_insert_last(edge*,vivadoVertex->inEdges,selEdge);
                        }else{
                            sprintf(tempName,".%s",fetchedEdge->name);
                            array_insert_last(char*,vivadoVertex->inPorts,strdup(tempName));
                            sprintf(tempName,"temp_puf_edge_%d",globalPufIndex);
                            newEdge = createEdge(G,strdup(tempName));
                            array_insert(edge*,vert->outEdges,0,newEdge);
                            newEdge->fromNode = vert;
                            array_insert_last(vertex*,newEdge->toNodes,vivadoVertex);
                            array_insert_last(edge*,vivadoVertex->inEdges,newEdge);
                        }
                    }else if(fetchedEdge->bPrimOut){
                        sprintf(tempName,".%s",fetchedEdge->name);
                        array_insert_last(char*,vivadoVertex->outPorts,strdup(tempName));
                        array_insert_last(edge*,vivadoVertex->outEdges,edg2);
                    }

                }

                array_insert_last(vertex*,G->vertexArr,vivadoVertex);

                //assign the output port to the puf out port
                sprintf(tempName,"puf_out_instance_%d",globalPufIndex);

                vertex* newVertex = (vertex *)malloc(sizeof(vertex));

                newVertex->name = strdup(tempName);
                newVertex->type = findType("BUFX2", G->typeArr);
                newVertex->bPrimIn = 0;
                newVertex->bPrimOut = 0;
                newVertex->inEdges = array_alloc(edge *, 1);
                newVertex->outEdges = array_alloc(edge *, 1);
                newVertex->inPorts = array_alloc(char *, 1);
                newVertex->outPorts = array_alloc(char *, 1);
                newVertex->comment = 0 ;
                array_insert_last(char*,newVertex->inPorts,strdup(".A"));
                array_insert_last(char*,newVertex->outPorts,strdup(".Y"));
                array_insert_last(edge*,newVertex->inEdges,edg2);
                array_insert_last(vertex*,edg2->toNodes,newVertex);

                sprintf(tempName,"pufout[%d]",globalPufIndex);
                fetchedEdge = createEdge(G,strdup(tempName));
                fetchedEdge->bPrimOut = 1;
                fetchedEdge->fromNode = newVertex;
                array_insert_last(edge*,newVertex->outEdges,fetchedEdge);
                array_insert_last(edge*,G->primOutEdges,fetchedEdge);
                array_insert_last(vertex*,G->vertexArr,newVertex);

            }else{            

                vertex_edge_duplicate(template->vertexArr,template->edgArr,vertArr,edgArr);

                //modify the name of all of the puf template vertexes
                for(j=0;j<array_n(vertArr);j++){
                    fetchedVert = array_fetch(vertex*,vertArr,j);
                    free(fetchedVert->name);
                    sprintf(tempName,"puf_%d_instance_%d",globalPufIndex,j);
                    fetchedVert->name = strdup(tempName);
                    array_insert_last(vertex*,G->vertexArr,fetchedVert);
                }

                //modify the name of all of the puf template edges
                for(j=0;j<array_n(edgArr);j++){
                    fetchedEdge = array_fetch(edge*,edgArr,j);
                    if(!strcmp(fetchedEdge->name,"sel")){
                        fetchedEdge->bPrimIn = 0 ; //make it so that it is not shown as a primary input in the main Graph
                        continue;
                    }
                    if(fetchedEdge->bPrimIn){
                        fetchedEdge->bPrimIn = 0 ;
                        array_insert(edge*,vert->outEdges,0,fetchedEdge);
                        //edg = fetchedEdge;
                        //free(fetchedEdge->fromNode);
                        fetchedEdge->fromNode = vert;
                    }
                    if(fetchedEdge->bPrimOut){
                        fetchedEdge->bPrimOut = 0 ;
                        //fetchedEdge = edg2 ; 
                        edg2->fromNode = fetchedEdge->fromNode ;
                        tempVert = edg2->fromNode;
                        for(k=0;k<array_n(tempVert->outEdges);k++){
                            if(!strcmp(fetchedEdge->name,array_fetch(edge*,tempVert->outEdges,k)->name)){
                                array_insert(edge*,tempVert->outEdges,k,edg2);
                                break;
                            }
                        }

                        for(k=0;k<array_n(fetchedEdge->toNodes);k++){
                            tempVert2 = array_fetch(vertex*,fetchedEdge->toNodes,k);
                            for(l=0;l<array_n(tempVert2->inEdges);l++){
                                if(!strcmp(fetchedEdge->name,array_fetch(edge*,tempVert2->inEdges,l)->name)){
                                    array_insert(edge*,tempVert2->inEdges,l,edg2);
                                }
                            }
                        }

                        array_insert(edge*,edgArr,j,edg2);
                        free(fetchedEdge);
                        //assign the output port to the puf out port
                        sprintf(tempName,"puf_out_instance_%d",globalPufIndex);

                        vertex* newVertex = (vertex *)malloc(sizeof(vertex));

                        newVertex->name = strdup(tempName);
                        newVertex->type = findType("BUFX2", G->typeArr);
                        newVertex->bPrimIn = 0;
                        newVertex->bPrimOut = 0;
                        newVertex->inEdges = array_alloc(edge *, 1);
                        newVertex->outEdges = array_alloc(edge *, 1);
                        newVertex->inPorts = array_alloc(char *, 1);
                        newVertex->outPorts = array_alloc(char *, 1);
                        newVertex->comment = 0 ;
                        array_insert_last(char*,newVertex->inPorts,strdup(".A"));
                        array_insert_last(char*,newVertex->outPorts,strdup(".Y"));
                        array_insert_last(edge*,newVertex->inEdges,edg2);
                        array_insert_last(vertex*,edg2->toNodes,newVertex);

                        sprintf(tempName,"pufout[%d]",globalPufIndex);
                        fetchedEdge = createEdge(G,strdup(tempName));
                        fetchedEdge->bPrimOut = 1;
                        fetchedEdge->fromNode = newVertex;
                        array_insert_last(edge*,newVertex->outEdges,fetchedEdge);
                        array_insert_last(edge*,G->primOutEdges,fetchedEdge);
                        array_insert_last(vertex*,G->vertexArr,newVertex);
                        continue;
                    }
                    free(fetchedEdge->name);
                    sprintf(tempName,"puf_%d_wire_%d",globalPufIndex,j);
                
                    fetchedEdge->name = strdup(tempName);
                    array_insert_last(edge*,G->edgArr,fetchedEdge);
                }

            }

            globalPufIndex++; //finally increase the global puf index
            
        }

    }else{//this takes care of puf insertion by reusing existing logic cells
        for(i=0;i<array_n(modify->targetCells);i++){
           
            array_t* vertArr = array_alloc(vertex*,1); //this stores the vertices of the puf template
            array_t* edgArr = array_alloc(edge*,1); //this stores the edges of the puf template

            fetchedResueID = array_fetch(reuseIdList*,modify->targetCells,i);
            vert1 = fetchedResueID->vert1; //this stores the 1st target cell
            vert2 = fetchedResueID->vert2; //this stores the 2nd target cell
            vert1->comment = 1 ;
            vert2->comment = 1 ;
            vert1->pufNumber = globalPufIndex;
            vert2->pufNumber = globalPufIndex;
            
            switch(fetchedResueID->pufTemplateType){
                case 0:
                    template = modify->inv_template;
                    break;

                case 1:
                    template = modify->nand_template;
                    break;

                case 2:
                    template = modify->nor_template;
                    break;                    
            }

            vertex_edge_duplicate(template->vertexArr,template->edgArr,vertArr,edgArr);

            //modify the name of all of the puf template vertexes
            for(j=0;j<array_n(vertArr);j++){
               fetchedVert = array_fetch(vertex*,vertArr,j);
               free(fetchedVert->name);
               sprintf(tempName,"puf_%d_instance_%d",globalPufIndex,j);
               fetchedVert->name = strdup(tempName);
               array_insert_last(vertex*,G->vertexArr,fetchedVert);
            }

            //modify the name of all of the puf template edges and then make and break all of the connections 
            for(j=0;j<array_n(edgArr);j++){
                
                fetchedEdge = array_fetch(edge*,edgArr,j);
                
                if(!strcmp(fetchedEdge->name,"sel")){
                    fetchedEdge->bPrimIn = 0 ; //make it so that it is not shown as a primary input in the main Graph
                    continue;
                }

                if(fetchedEdge->bPrimIn){
                    
                    fetchedEdge->bPrimIn = 0 ;
                    // array_insert(edge*,vert->outEdges,0,fetchedEdge);
                    // //edg = fetchedEdge;
                    // free(fetchedEdge->fromNode);
                    // fetchedEdge->fromNode = vert;

                    if(fetchedResueID->pufTemplateType == 0){ //this is the inverter puf template. this will have 2 input ports and a sel port
                        
                        //make the connections for the input ports
                        if(!strcmp(fetchedEdge->name,"in0")){
                            //vert1 connections are to be made
                            //vert1->inEdges connect this to in0 nodes;

                            //this contains the input edge of inv1
                            edg1 = array_fetch(edge*,vert1->inEdges,0); //there should only be 1 in edge for an inverter

                            for(k=0;k<array_n(fetchedEdge->toNodes);k++){
                                tempVert = array_fetch(vertex*,fetchedEdge->toNodes,k);
                                for(l=0;l<array_n(tempVert->inEdges);l++){
                                    if(!strcmp(array_fetch(edge*,tempVert->inEdges,l)->name,"in0")){
                                        array_insert(edge*,tempVert->inEdges,l,edg1); //insert the 
                                    }
                                }
                            }


                        }else if(!strcmp(fetchedEdge->name,"in1")){
                            //vert2 connections are to be made

                            //this contains the input edge of inv2
                            edg1 = array_fetch(edge*,vert2->inEdges,0); //there should only be 1 in edge for an inverter

                            for(k=0;k<array_n(fetchedEdge->toNodes);k++){
                                tempVert = array_fetch(vertex*,fetchedEdge->toNodes,k);
                                for(l=0;l<array_n(tempVert->inEdges);l++){
                                    if(!strcmp(array_fetch(edge*,tempVert->inEdges,l)->name,"in1")){
                                        array_insert(edge*,tempVert->inEdges,l,edg1); //insert the 
                                    }
                                }
                            }
                            
                        }
                        

                    }else{ //this is the nand and nor puf template. these have 4 input port and a sel port
                        
                        //make the connections for the input ports

                        if(!strcmp(fetchedEdge->name,"in0")){ //this is for vert1

                            //this contains the input edge of nand/nor
                            edg1 = array_fetch(edge*,vert1->inEdges,0); //there should be 2 in edge for a nand or nor gate
                            edg2 = array_fetch(edge*,vert1->inEdges,1); //there should be 2 in edge for a nand or nor gate

                            for(k=0;k<array_n(fetchedEdge->toNodes);k++){
                                tempVert = array_fetch(vertex*,fetchedEdge->toNodes,k);
                                for(l=0;l<array_n(tempVert->inEdges);l++){
                                    if(!strcmp(array_fetch(edge*,tempVert->inEdges,l)->name,"in0")){
                                        array_insert(edge*,tempVert->inEdges,l,edg1);  
                                    }
                                }
                            }

                        }else if(!strcmp(fetchedEdge->name,"in1")){ //this is for vert1

                            //this contains the input edge of nand/nor
                            edg1 = array_fetch(edge*,vert1->inEdges,0); //there should be 2 in edge for a nand or nor gate
                            edg2 = array_fetch(edge*,vert1->inEdges,1); //there should be 2 in edge for a nand or nor gate

                            for(k=0;k<array_n(fetchedEdge->toNodes);k++){
                                tempVert = array_fetch(vertex*,fetchedEdge->toNodes,k);
                                for(l=0;l<array_n(tempVert->inEdges);l++){
                                    if(!strcmp(array_fetch(edge*,tempVert->inEdges,l)->name,"in1")){
                                        array_insert(edge*,tempVert->inEdges,l,edg2);  
                                    }
                                }
                            }

                        }else if(!strcmp(fetchedEdge->name,"in2")){ //this is for vert2

                            //this contains the input edge of nand/nor
                            edg1 = array_fetch(edge*,vert2->inEdges,0); //there should be 2 in edge for a nand or nor gate
                            edg2 = array_fetch(edge*,vert2->inEdges,1); //there should be 2 in edge for a nand or nor gate

                            for(k=0;k<array_n(fetchedEdge->toNodes);k++){
                                tempVert = array_fetch(vertex*,fetchedEdge->toNodes,k);
                                for(l=0;l<array_n(tempVert->inEdges);l++){
                                    if(!strcmp(array_fetch(edge*,tempVert->inEdges,l)->name,"in2")){
                                        array_insert(edge*,tempVert->inEdges,l,edg1);  
                                    }
                                }
                            }

                        }else if(!strcmp(fetchedEdge->name,"in3")){ //this is for vert2

                            //this contains the input edge of nand/nor
                            edg1 = array_fetch(edge*,vert2->inEdges,0); //there should be 2 in edge for a nand or nor gate
                            edg2 = array_fetch(edge*,vert2->inEdges,1); //there should be 2 in edge for a nand or nor gate

                            for(k=0;k<array_n(fetchedEdge->toNodes);k++){
                                tempVert = array_fetch(vertex*,fetchedEdge->toNodes,k);
                                for(l=0;l<array_n(tempVert->inEdges);l++){
                                    if(!strcmp(array_fetch(edge*,tempVert->inEdges,l)->name,"in3")){
                                        array_insert(edge*,tempVert->inEdges,l,edg2);  
                                    }
                                }
                            }

                        }

                    }

                    continue;

                }

                if(fetchedEdge->bPrimOut){

                    edg1 = array_fetch(edge*,vert1->outEdges,0); //if this is a inv,nand, nor gate these should only have 1 output edge. Else something is super wrong
                    edg2 = array_fetch(edge*,vert2->outEdges,0); //if this is a inv,nand, nor gate these should only have 1 output edge. Else something is super wrong

                    if(!strcmp(fetchedEdge->name,"out0")){ //connect the output wires to the output of the 1st instance

                        for(k=0;k<array_n(fetchedEdge->fromNode->outEdges);k++){
                            if(!strcmp(array_fetch(edge*,fetchedEdge->fromNode->outEdges,k)->name,"out0")){
                                array_insert(edge*,fetchedEdge->fromNode->outEdges,k,edg1);
                            }
                        }

                        for(k=0;k<array_n(fetchedEdge->toNodes);k++){
                            
                            tempVert = array_fetch(vertex*,fetchedEdge->toNodes,k);

                            for(l=0;l<array_n(tempVert->inEdges);l++){
                                if(!strcmp(array_fetch(edge*,tempVert->inEdges,l)->name,"out0")){
                                    array_insert(edge*,tempVert->inEdges,l,edg1);
                                }
                            }
                        }

                        //assign the output port to the puf out port
                        sprintf(tempName,"puf_out_instance_%d",globalPufIndex);

                        vertex* newVertex = (vertex *)malloc(sizeof(vertex));

                        newVertex->name = strdup(tempName);
                        newVertex->type = findType("BUFX2", G->typeArr);
                        newVertex->bPrimIn = 0;
                        newVertex->bPrimOut = 0;
                        newVertex->inEdges = array_alloc(edge *, 1);
                        newVertex->outEdges = array_alloc(edge *, 1);
                        newVertex->comment = 0 ;
                        newVertex->inPorts = array_alloc(char *, 1);
                        newVertex->outPorts = array_alloc(char *, 1);
                        array_insert_last(char*,newVertex->inPorts,strdup(".A"));
                        array_insert_last(char*,newVertex->outPorts,strdup(".Y"));
                        array_insert_last(edge*,newVertex->inEdges,edg1);
                        array_insert_last(vertex*,edg1->toNodes,newVertex);

                        sprintf(tempName,"pufout[%d]",globalPufIndex);
                        fetchedEdge = createEdge(G,strdup(tempName));
                        fetchedEdge->bPrimOut = 1;
                        fetchedEdge->fromNode = newVertex;
                        array_insert_last(edge*,newVertex->outEdges,fetchedEdge);
                        array_insert_last(edge*,G->primOutEdges,fetchedEdge);
                        array_insert_last(vertex*,G->vertexArr,newVertex); 
                        
                    }else if(!strcmp(fetchedEdge->name,"out1")){ //connect the output wires to the output of the 2nd instance

                        for(k=0;k<array_n(fetchedEdge->fromNode->outEdges);k++){
                            if(!strcmp(array_fetch(edge*,fetchedEdge->fromNode->outEdges,k)->name,"out1")){
                                array_insert(edge*,fetchedEdge->fromNode->outEdges,k,edg2);
                            }
                        }

                        for(k=0;k<array_n(fetchedEdge->toNodes);k++){
                            
                            tempVert = array_fetch(vertex*,fetchedEdge->toNodes,k);

                            for(l=0;l<array_n(tempVert->inEdges);l++){
                                if(!strcmp(array_fetch(edge*,tempVert->inEdges,l)->name,"out1")){
                                    array_insert(edge*,tempVert->inEdges,l,edg2);
                                }
                            }
                        }

                    }else{
                        printf("ULTRA ERROR: THIS SHOULD NOT HAVE HAPPENED.\n");
                    }                  

                    continue;

                }

                //change the name of the edge over here

                free(fetchedEdge->name);
                sprintf(tempName,"puf_%d_wire_%d",globalPufIndex,j);
           
                fetchedEdge->name = strdup(tempName);
                array_insert_last(edge*,G->edgArr,fetchedEdge);

            }

            globalPufIndex++; //finally increase the global puf index

        }

        
    }

}

int check_if_index_is_present(int inverter_instance_index_store_for_puf_replacement[], int i, int index_of_vertex)
{

    int j = 0;

    for (j = 0; j < i; j++)
    {

        if (inverter_instance_index_store_for_puf_replacement[j] == index_of_vertex)
        {
            return 1;
        }
    }

    return 0;
}

void module_and_wire_printer(char *inputFile, char *top, FILE *out, int sel_non_primary_input)
{ // this prints out the module and wire statements from the input netlist/file
    // printf("entered the module and wire printer function\n");
    // char *header;
    // char header[20000];
    char module[1000];
    sprintf(module, "module %s", top);
    char *header = malloc(sizeof(char) * 1000);

    FILE *fp1;
    fp1 = fopen(inputFile, "r");
    if (fp1 == NULL)
    {
        printf("ERROR: Cannot open input file.......Exiting.\n");
        exit(1);
    }

    while (!feof(fp1))
    {
        if (fgets(header, 1000, fp1) != NULL)
        {

            if (strstr(header, "//") != NULL)
            {
                continue;
            }
            else if ((strstr(header, module) != NULL || strstr(header, "input ") != NULL || strstr(header, "output ") != NULL || strstr(header, "wire ") != NULL) && strstr(header, "endmodule") == NULL)
            {

                if (sel_non_primary_input)
                { // this will add the wire sel at the start of the module element when sel_non_primary_input == 0

                    fprintf(out, "%s", header); // prints the header if sel is non primary as it is
                }
                else
                {

                    if (strstr(header, module) != NULL)
                    { // this inserts the sel as the first argument when module is encountered else it will print the header as it is
                        int z = 0;
                        while (header[z] != '\0')
                        {
                            if (header[z] == '(')
                            {
                                fprintf(out, "(sel,puf_out,");
                            }
                            else
                            {
                                fprintf(out, "%c", header[z]);
                            }
                            z++;
                        }
                    }
                    else
                    {
                        fprintf(out, "%s", header); // prints the header if sel is non primary as it is
                    }
                }

                while (!feof(fp1))
                {
                    // fscanf(fp, "%[^\n]\n",header);

                    if (strstr(header, ";") != NULL)
                    {
                        // fprintf(out,"%s",header);
                        // printf("%s\n",header);
                        break;
                    }
                    else
                    {
                        // printf("%s\n",header);
                        fgets(header, 1000, fp1);
                        fprintf(out, "%s", header); // continue printing until ; is encountered
                    }
                }
            }
        }
    }
    // fclose(fp1);
    free(header);
}

void insert_reuse_melpuf(FILE *out, array_t* reuseVertices,config_param *param)
{

    char* puf_instance_name[] = {"inverter_reuse_instance","nand_reuse_instance","nor_reuse_instance","melpuf"};
    char* puf_name;
    char sel[] = "sel";
    char *in0,*in1,*in2,*in3,*out0,*out1,*out2,*out3;
    reuseIdList* fetched_reuseID;
    int inverter_puf_number, nand_puf_number, nor_puf_number;

    vertex  *fetched_vert1,*fetched_vert2;
    array_t* selectedVertexArr;
    vertex* selectedVertex;
    edge* fetched_edge;

    inverter_puf_number = 0;
    nand_puf_number = 0;
    nor_puf_number = 0;

    // if(!strcmp(param->dont_touch,"vivado")){
    //     fprintf(out, "    //-------------------------------------------------------------------------------\n");
    //     fprintf(out, "    melpuf puf_inst_%d(.sel(%s),.inn(%s),.out(%s));//vivado checkpoint\n",inserted_puff_number,sel,input_edge_name,output_edge_name);
    //     fprintf(out, "    //-------------------------------------------------------------------------------\n");
    //     return 0;
    // }

    fprintf(out, "    //-------------------------------------------------------------------------------\n");

    for(int i=0; i< array_n(reuseVertices); i++){

        fetched_reuseID = array_fetch(reuseIdList*,reuseVertices,i);

        if(fetched_reuseID->pufExcess){
            continue;
        }

        fetched_vert1 = array_fetch(vertex*,param->G->vertexArr,fetched_reuseID->idx1);
        fetched_vert2 = array_fetch(vertex*,param->G->vertexArr,fetched_reuseID->idx2);

        if(fetched_reuseID->pufTemplateType != 0){
            
            fetched_edge = array_fetch(edge*,fetched_vert1->inEdges,0);
            in0 = fetched_edge->name;
            fetched_edge = array_fetch(edge*,fetched_vert1->inEdges,1);
            in1 = fetched_edge->name;
            fetched_edge = array_fetch(edge*,fetched_vert2->inEdges,0);
            in2 = fetched_edge->name;
            fetched_edge = array_fetch(edge*,fetched_vert2->inEdges,1);
            in3 = fetched_edge->name;

        }else{

            fetched_edge = array_fetch(edge*,fetched_vert1->inEdges,0);
            in0 = fetched_edge->name;            
            fetched_edge = array_fetch(edge*,fetched_vert2->inEdges,0);
            in1 = fetched_edge->name;           

        }

        fetched_edge = array_fetch(edge*,fetched_vert1->outEdges,0);
        out0 = fetched_edge->name ;            
        fetched_edge = array_fetch(edge*,fetched_vert2->outEdges,0);
        out1 = fetched_edge->name ;

        
        if(!strcmp(param->dont_touch,"vivado")){
            switch(fetched_reuseID->pufTemplateType){
                case 0:
                    fprintf(out, "    %s %s_%d(.%s(%s), .in0(%s), .in1(%s), .out0(%s), .out1(%s));//vivado checkpoint\n",puf_instance_name[fetched_reuseID->pufTemplateType],puf_instance_name[fetched_reuseID->pufTemplateType],inverter_puf_number,sel,sel,in0,in1,out0,out1);
                    inverter_puf_number++;
                    break;
                case 1:
                    fprintf(out, "    %s %s_%d(.%s(%s), .in0(%s), .in1(%s), .in2(%s), .in3(%s), .out0(%s), .out1(%s));//vivado checkpoint\n",puf_instance_name[fetched_reuseID->pufTemplateType],puf_instance_name[fetched_reuseID->pufTemplateType],nand_puf_number,sel,sel,in0,in1,in2,in3,out0,out1);
                    nand_puf_number++;
                    break;
                case 2:
                    fprintf(out, "    %s %s_%d(.%s(%s), .in0(%s), .in1(%s), .in2(%s), .in3(%s), .out0(%s), .out1(%s));//vivado checkpoint\n",puf_instance_name[fetched_reuseID->pufTemplateType],puf_instance_name[fetched_reuseID->pufTemplateType],nor_puf_number,sel,sel,in0,in1,in2,in3,out0,out1);
                    nor_puf_number++;
                    break;                    

            }
        }else{

            switch(fetched_reuseID->pufTemplateType){
                case 0:
                    //selectedVertexArr = param->                    
                    break;
                case 1:
                    break;
                case 2:
                    break;                                        
            }            

        }

    }


    fprintf(out, "    //-------------------------------------------------------------------------------\n");
}

void insert_mel_puf(FILE *out, char input_edge_name[], char output_edge_name[], int inserted_puff_number, int number_of_pufs, char inv_name[], graph *puf_template,config_param *param)
{

    array_t *vertArr;
    array_t *edgArr;
    array_t *gateArr;
    vertArr = puf_template->vertexArr;
    edgArr = puf_template->edgArr;
    int number_of_vertices = array_n(vertArr); // this stores the number of vertices of the netlist
    int number_of_edges = array_n(edgArr);     // this stores the number of vertices of the netlist
    char *portname;
    char *temp_edge_name; // this will store the name of the input/output edge name of a inverter selected for puf insertion
    temp_edge_name = NULL;

    int i, j, k;
    edge *edg;
    vertex *vert, *tvert;

    inserted_puff_number++;
    char sel[] = "sel";

    if(!strcmp(param->dont_touch,"vivado")){
        fprintf(out, "    //-------------------------------------------------------------------------------\n");
        fprintf(out, "    melpuf puf_inst_%d(.sel(%s),.inn(%s),.out(%s));//vivado checkpoint\n",inserted_puff_number,sel,input_edge_name,output_edge_name);
        fprintf(out, "    //-------------------------------------------------------------------------------\n");
        return 0;
    }

    fprintf(out, "    //-------------------------------------------------------------------------------\n");

    char instance_name[2000]; // this is the name that will be used for naming the puf instance names

    for (i = 0; i < number_of_vertices; i++)
    {
        vert = array_fetch(vertex *, vertArr, i);

        if (vert->type)
        {

            // this  line is for commenting out the instance name incase of inverter identified for puf insertion

            sprintf(instance_name, "puf_%d_inst_%d", inserted_puff_number, i);

            fprintf(out, "    %s %s ( ", vert->type->name, instance_name); // this prints the vertex type

            for (j = 0; j < array_n(vert->outEdges); j++)
            { // this will print out the name of the output pinsname followed by the output wire names

                edg = array_fetch(edge *, vert->outEdges, j);
                portname = array_fetch(char *, vert->outPorts, j);

                if (temp_edge_name != NULL)
                {
                    free(temp_edge_name);
                }
                temp_edge_name = strdup(edg->name);
                if (!strcmp(temp_edge_name, "VDD"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup("1'b1");
                }
                else if (!strcmp(temp_edge_name, "GND"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup("1'b0");
                }
                else if (!strcmp(temp_edge_name, "in0"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup(input_edge_name);
                }
                else if (!strcmp(temp_edge_name, "out0"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup(output_edge_name);
                }
                else if (!strcmp(temp_edge_name, "sel"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup("sel");
                }
                else
                {
                    char puf_indexing[100];
                    sprintf(puf_indexing, "%s_p_%d", temp_edge_name, inserted_puff_number);
                    if (temp_edge_name != NULL)
                    {
                        free(temp_edge_name);
                    }
                    temp_edge_name = strdup(puf_indexing);
                }
                fprintf(out, "%s(%s), ", portname, temp_edge_name);
            }

            for (j = 0; j < array_n(vert->inEdges); j++)
            { // this will print the input port and wire names

                edg = array_fetch(edge *, vert->inEdges, j);
                portname = array_fetch(char *, vert->inPorts, j);

                if (temp_edge_name != NULL)
                {
                    free(temp_edge_name);
                }
                temp_edge_name = strdup(edg->name);
                if (!strcmp(temp_edge_name, "VDD"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup("1'b1");
                }
                else if (!strcmp(temp_edge_name, "GND"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup("1'b0");
                }
                else if (!strcmp(temp_edge_name, "in0"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup(input_edge_name);
                }
                else if (!strcmp(temp_edge_name, "out0"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup(output_edge_name);
                }
                else if (!strcmp(temp_edge_name, "sel"))
                {
                    free(temp_edge_name);
                    temp_edge_name = strdup("sel");
                }
                else
                {
                    char puf_indexing[100];
                    sprintf(puf_indexing, "%s_p_%d", temp_edge_name, inserted_puff_number);
                    if (temp_edge_name != NULL)
                    {
                        free(temp_edge_name);
                    }
                    temp_edge_name = strdup(puf_indexing);
                }

                fprintf(out, "%s(%s)", portname, temp_edge_name);
                if (j != array_n(vert->inEdges) - 1)
                {
                    fprintf(out, ", ");
                }
            }

            fprintf(out, " );\n");
        }
    }

    fprintf(out, "    //-------------------------------------------------------------------------------\n");
}

int search_index(int i, int inverter_instance_index_store_for_puf_replacement[], int number_of_pufs)
{

    for (int j = 0; j < number_of_pufs; j++)
    {
        if (inverter_instance_index_store_for_puf_replacement[j] == i)
        {
            return 1;
        }
    }

    return 0;
}

char *node_name_change(char *node_name, int *puf_instance)
{

    int i = 0, n = 0;
    while (*(node_name + i) != '\0')
    {
        n++;
        i++;
    }

    char dup_node_name[n + 1];

    i = 0;

    while (*(node_name + i) != '\0')
    {

        if ((*(node_name + i) == '\\') || (*(node_name + i) == '[') || (*(node_name + i) == ']'))
        {
            dup_node_name[i] = '_';
        }
        else
        {
            dup_node_name[i] = *(node_name + i);
        }

        i++;
    }

    dup_node_name[i] = '\0';

    if (strncasecmp(node_name, "puf_", 3) == 0)
    {
        *puf_instance = 1;
    }
    else
    {
        *puf_instance = 0;
    }

    return strdup(dup_node_name);
}

void create_graph(graph *G, char *name, char *file_type)
{ // file typ is if the graph is original or melpuf inserted one

    FILE *fp;
    char graph_file[200];
    char *old_name;
    old_name = strdup(name);
    if (!strcmp(file_type, "melpuf"))
    {
        char new_name[200];
        sprintf(new_name, "%s_melpuf", name);
        name = strdup(new_name);
    }
    int puf_instance = 0, dummy_instance;
    sprintf(graph_file, "../reports/%s/%s.gv", old_name, name);
    fp = fopen(graph_file, "w");
    if (fp == NULL)
    {
        printf("ERROR: Cannot open graph file %s .......Exiting.\n", graph_file);
        exit(1);
    }

    array_t *vertArr;
    array_t *edgArr;
    int i, j, k;
    edge *edg;
    vertex *vert, *tvert;

    vertArr = G->vertexArr;
    edgArr = G->edgArr;
    char *node_name;

    fprintf(fp, "digraph %s {\n", name);

    for (i = 0; i < array_n(vertArr); i++)
    {
        vert = array_fetch(vertex *, vertArr, i);
        node_name = vert->name;
        if (!strcmp(node_name, "*"))
        {
            continue;
        }

        node_name = node_name_change(node_name, &puf_instance);

        if (vert->type)
        {
            fprintf(fp, "%s -> {", node_name);
        }
        else
        {
            fprintf(fp, "%s -> {", node_name);
        }

        for (j = 0; j < array_n(vert->outEdges); j++)
        {
            edg = array_fetch(edge *, vert->outEdges, j);

            for (k = 0; k < array_n(edg->toNodes); k++)
            {
                tvert = array_fetch(vertex *, edg->toNodes, k);
                if (!tvert)
                    continue;
                node_name = node_name_change(tvert->name, &dummy_instance);

                if (!strcmp(node_name, "*"))
                {
                    continue;
                }

                fprintf(fp, " %s", node_name);

                if (dummy_instance)
                {
                    fprintf(fp, " [color=red] ");
                }
                else
                {
                    fprintf(fp, " ");
                }
            }
        }

        fprintf(fp, "}");
        if (puf_instance)
        {
            fprintf(fp, " [color=red] ;\n");
        }
        else
        {
            fprintf(fp, " [color=blue] ; \n");
        }
    }

    fprintf(fp, "\n}");
    fclose(fp);

    char gen_graph[200];
    sprintf(gen_graph, "dot -Tjpg ../reports/%s/%s.gv -o ../reports/%s/%s.jpg", old_name, name, old_name, name);
    // system(gen_graph);

    free(old_name);

    return;
}

/*
void generate_overhead_report(char* output_file_name,int select){ //select 1 is for puf and select 2 is for the original design

    FILE *fp_mp;
    char output_tcl[200] ;
    sprintf(output_tcl,"tcl_scripts/%s_melpuf.tcl",moduleName);

    fp_mp = fopen(output_tcl, "w+");
    if(fp_mp == NULL){
        printf("ERROR : Failed in creating tcl script for melpuf design(generate_overhead_report). The overhead reports will not be generated. Exiting......\n");
        exit(0);
    }

    fprintf(fp_mp,"set_db library %s\n",libFile);


    if(select == 1){

        fprintf(fp_mp,"set_db write_vlog_unconnected_port_style none\nset_db print_ports_nets_preserved_for_cb true\n");
        fprintf(fp_mp,"read_hdl %s\n",output_file_name);
        fprintf(fp_mp,"elaborate\n");

        if(!strcmp(cktType,"seq")){
            fprintf(fp_mp, "\n\ncreate_clock -name %s -period %.2f -waveform {0 %.2f} [get_ports %s]\n",clk,period,period/2,clk);
            fprintf(fp_mp, "set_input_delay  0 -clock %s [all_inputs]\nset_output_delay 0 -clock %s [all_outputs]\nset_clock_uncertainty 0 %s\n\n",clk,clk,clk);
        }

        fprintf(fp_mp,"remove_cdn_loop_breaker\n");
        fprintf(fp_mp,"report_area > ../reports/%s/%s_melpuf_area_%d.rpt\n",moduleName,moduleName,number_of_pufs);
        fprintf(fp_mp,"remove_cdn_loop_breaker\n");
        fprintf(fp_mp,"report_timing > ../reports/%s/%s_melpuf_timing_%d.rpt\n",moduleName,moduleName,number_of_pufs);
        fprintf(fp_mp,"remove_cdn_loop_breaker\n");
        fprintf(fp_mp,"report_power > ../reports/%s/%s_melpuf_power_%d.rpt\n",moduleName,moduleName,number_of_pufs);

    }else if(select == 2){

        //this part reads in the original netlist and then generates its overheads

        fprintf(fp_mp,"set_db write_vlog_unconnected_port_style none\nset_db print_ports_nets_preserved_for_cb true\n");
        fprintf(fp_mp,"read_hdl %s\n",inputFile);
        fprintf(fp_mp,"elaborate\n");

        if(!strcmp(cktType,"seq")){
            fprintf(fp_mp, "\n\ncreate_clock -name %s -period %.2f -waveform {0 %.2f} [get_ports %s]\n",clk,period,period/2,clk);
            fprintf(fp_mp, "set_input_delay  0 -clock %s [all_inputs]\nset_output_delay 0 -clock %s [all_outputs]\nset_clock_uncertainty 0 %s\n\n",clk,clk,clk);
        }

        fprintf(fp_mp,"remove_cdn_loop_breaker\n");
        fprintf(fp_mp,"report_area > ../reports/%s/%s_area_%d.rpt\n",moduleName,moduleName,number_of_pufs);
        fprintf(fp_mp,"remove_cdn_loop_breaker\n");
        fprintf(fp_mp,"report_timing > ../reports/%s/%s_timing_%d.rpt\n",moduleName,moduleName,number_of_pufs);
        fprintf(fp_mp,"remove_cdn_loop_breaker\n");
        fprintf(fp_mp,"report_power > ../reports/%s/%s_power_%d.rpt\n",moduleName,moduleName,number_of_pufs);

    }



    fprintf(fp_mp,"exit\n");

    fclose(fp_mp);

    char genus_script[200];

    printf("\nINFO: Generating melpuf inserted design reports.\n");

    sprintf(genus_script,"genus -files %s -log /dev/null -abort_on_error >> tcl_scripts/genus_log.txt",output_tcl);

    system(genus_script);


}
*/

int search_vertices(array_t *vertices, int idx, int *idx_stored_location)
{

    idList *retrievedId;

    for (int i = 0; i < array_n(vertices); i++)
    {
        retrievedId = array_fetch(idList *, vertices, i);
        if (retrievedId->idx == idx)
        {
            *idx_stored_location = i;
            return 1;
        }
    }

    *idx_stored_location = -1;

    return 0;
}

int search_reuseVertices(array_t *vertices, int idx, int *idx_stored_location)
{

    reuseIdList *retrievedId;

    for (int i = 0; i < array_n(vertices); i++)
    {
        retrievedId = array_fetch(reuseIdList *, vertices, i);
        if (retrievedId->idx1 == idx)
        {
            *idx_stored_location = i;
            return 1;
        }else if(retrievedId->idx2 == idx){
            *idx_stored_location = i;
            return 1;
        }        
    }

    *idx_stored_location = -1;

    return 0;
}

// synthesis if the dont touch has been set to synopsys
void resynthesize(config_param *param)
{

    if (!strcmp(param->dont_touch, "synopsys"))
    {

        if (param->toolLib == NULL)
        {
            printf("ERROR: Cannot create resynthesized design. Tool Library(db file) is not specified.\n");
            return (0);
        }

        FILE *fp;

        fp = fopen("./tcl_scripts/resynth.tcl", "w+");

        fprintf(fp, "set_app_var target_library %s \n", param->toolLib);
        fprintf(fp, "set_app_var link_library %s \n", param->toolLib);
        fprintf(fp, "read_file -format verilog {../reports/%s/%s_puf_%d.v}\n", param->moduleName, param->moduleName, param->number_of_puf);
        fprintf(fp, "compile -map_effort high -area_effort high \nungroup -all -flatten -all_instances\n");
        fprintf(fp, "write -f verilog -o ../reports/%s/%s_puf_%d_resynth.v\n", param->moduleName, param->moduleName, param->number_of_puf);
        // fprintf(fp,"report_area > ../reports/%s/%s_%d_area_resynth.rpt\n",param->moduleName,param->moduleName,param->number_of_puf);
        // fprintf(fp,"report_power >  ../reports/%s/%s_%d_power_resynth.rpt\n",param->moduleName,param->moduleName,param->number_of_puf);
        // fprintf(fp,"report_timing > ../reports/%s/%s_%d_timing_resynth.rpt\n",param->moduleName,param->moduleName,param->number_of_puf);
        fprintf(fp, "exit\n");

        fclose(fp);

        system("dc_shell -f ./tcl_scripts/resynth.tcl > /dev/null");
    }
}
array_t *extract_paths(char *line, config_param *params)
{

    char *token;

    array_t *wordArr;

    wordArr = array_alloc(char *, 1);

    while ((token = strtok_r(line, " ", &line)))
    {
        array_insert_last(char *, wordArr, token);
    }

    return wordArr;
}

// pathList *createPath()
// {
//     pathList *newNode = (pathList *)malloc(sizeof(pathList));

//     newNode->pathNumber = 0;
//     newNode->slackViolated = 0;
//     newNode->pathSlack = 0;
//     newNode->constrained = 0;

//     newNode->node = array_alloc(nodeList *, 1);

//     return newNode;
// }

// search the vertex array of graph and pass the pointer back of the searched vertex
vertex *findVertexByNameSynDumpFile(config_param *params, char *word)
{

    array_t *vertexArr = params->G->vertexArr;
    vertex *fetchedVert;

    char *token;
    char *gateName;

    token = strtok(word, "/");
    gateName = strdup(token);

    for (int i = 0; i < array_n(vertexArr); i++)
    {

        fetchedVert = array_fetch(vertex *, vertexArr, i);

        if (!strcmp(fetchedVert->name, gateName))
        {
            return fetchedVert;
        }
    }

    return NULL;
}

// return 1 if the passed word is a int,float etc
int check_number(char *word)
{
    int i = 0;
    int isNumber = 0;

    while (*(word + i) != '\0')
    {

        if (*(word + i) == '0' || *(word + i) == '1' || *(word + i) == '2' || *(word + i) == '3' || *(word + i) == '4' || *(word + i) == '5' || *(word + i) == '6' || *(word + i) == '7' || *(word + i) == '8' || *(word + i) == '9' || *(word + i) == '.')
        {
            isNumber = 1;
        }
        else
        {
            return 0;
        }

        i++;
    }

    return isNumber;
}

// this is for STORING the different paths
int store_timing_paths(config_param *params)
{

    if (params->timingFile == NULL)
    {
        if(params->suppress[3] != 1){
            printf("ERROR[3]: Synopsis timing file is not specified. It is required for slack aware constrained puf insertion.\n");
        }        
        return 1;
    }

    if(params->suppress[4] != 1){
        printf("INFO[4]: Reading the different timing paths from %s.\n", params->timingFile);
    }

    FILE *fp;
    char *line;
    char *token;
    size_t len = 0;
    ssize_t read = 0;
    //array_t *path_list;
    array_t *wordArr;
    //pathList *pathN; // this stores the nth path parsed from the synopsis file
    vertex *fetchedVert;

    char *word;
    int createNewPath = 1;
    int startAdding = 0;
    char *startPoint;
    char *endPoint;
    int skip_startpoint = 0;
    int skip_next_line = 0;
    int pathNumber = 0;
    float pathDelay;
    char *delayValue;
    float required_time = 0 ;

    //path_list = array_alloc(pathList *, 1); // params->G->paths

    fp = fopen(params->timingFile, "r");
    if (fp == NULL)
    {
        printError(29);
        return 1;
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        line[strcspn(line, "\n")] = '\0';
        line[strcspn(line, "\r")] = '\0';

        if (!strcmp(line, "1"))
        {
            createNewPath = 1;
            startAdding = 0;
            continue;
        }

        if (createNewPath)
        {
            createNewPath = 0;
            //pathN = createPath();
            //pathN->pathNumber = pathNumber;
            pathNumber++;
            startAdding = 0;
        }

        if (skip_next_line)
        {
            skip_next_line--;
            continue;
        }

        wordArr = extract_paths(line, params);

        for (int i = 0; i < array_n(wordArr); i++)
        {
            word = array_fetch(char *, wordArr, i);

            if (startAdding < 2)
            {
                skip_startpoint = 0;
                if (!strcmp(word, "Startpoint:"))
                {
                    if (startAdding == 0)
                    {
                        startPoint = strdup(array_fetch(char *, wordArr, i + 1)); // free wordArr to use less memmory
                        // printf("Startpoint:%s\n",startPoint);        //take care of startAdding , create NewPath variables
                        ++startAdding;
                    }
                }
                else if (!strcmp(word, "Endpoint:"))
                {
                    if (startAdding == 1)
                    {
                        endPoint = strdup(array_fetch(char *, wordArr, i + 1));
                        // printf("Endpoint:%s\n",endPoint);
                        ++startAdding;
                        goto fetch_next_line; // very strange thing was happening like the line was not incrementing and it was directly going into the else if endpoint if statement, i will buy cofee for whoever finds my stupidity :p
                    }
                }
            }
            else if (startAdding == 2)
            {
                if (!skip_startpoint && strstr(word, startPoint))
                {

                    //if the input is a register and the the clock port time is encountered '#' in the incr column then fetch next line
                    if(array_n(wordArr) == 2){
                        
                        //fetch the next line 
                        read = getline(&line, &len, fp);
                        
                        line[strcspn(line, "\n")] = '\0';
                        line[strcspn(line, "\r")] = '\0';
                        
                        wordArr = extract_paths(line, params);
                        word = array_fetch(char*,wordArr,1);
                        if(!strcmp(word,"#")){
                            continue;
                        }else{
                            skip_startpoint = 1;
                        }
                    }else if(array_n(wordArr) == 3){ //test
                        word = array_fetch(char*,wordArr,2);
                        if(!strcmp(word,"<-")){

                            if(array_n(wordArr) == 3){
                                 //fetch the next line 
                                read = getline(&line, &len, fp);
                                
                                line[strcspn(line, "\n")] = '\0';
                                line[strcspn(line, "\r")] = '\0';
                                
                                wordArr = extract_paths(line, params);
                                word = array_fetch(char*,wordArr,1);

                                //update the path delay from the next line
                            }else{
                                //update the path delay from the same line
                            }
                           
                           skip_next_line = 1 ;

                        }
                        skip_startpoint = 1;
                    }else{
                        word = array_fetch(char*,wordArr,3);
                        if(!strcmp(word,"#")){
                            continue;
                        }else{
                            word = array_fetch(char*,wordArr,2);

                            if(!strcmp(word,"<-")){
                                skip_next_line = 1 ;
                            }

                            skip_startpoint = 1;
                        }
                    }
                    //skip_startpoint = 1;
                    // printf("Startpoint Encountered in the path.(%s, full word:%s)\n",startPoint,word);
                }
                else if (skip_startpoint && i == 0)
                {
                    if (!strcmp(word, endPoint) || strstr(word, endPoint))
                    {
                        skip_startpoint = 0;

                        //add the path delay of the endpoints
                        fetchedVert = findVertexByNameSynDumpFile(params, word);
                        
                        
                        if(fetchedVert != NULL){        
                            
                            if(array_n(wordArr) == 2){
                                    //fetch the next line 
                                    read = getline(&line, &len, fp);

                                    line[strcspn(line, "\n")] = '\0';
                                    line[strcspn(line, "\r")] = '\0';
                                    
                                    wordArr = extract_paths(line, params);

                                    char* fetchedWord;
                                    fetchedWord = array_fetch(char*,wordArr,1);
                                    if(check_number(fetchedWord)){
                                        pathDelay = atof(delayValue);
                                        if(fetchedVert->worstDelay < pathDelay){
                                            fetchedVert->worstDelay = pathDelay ;
                                            //printf("INFO1:vert name:%s path delay:%.2f\n",fetchedVert->name,pathDelay);
                                        }
                                    }else{
                                        fetchedWord = array_fetch(char*,wordArr,2);
                                        pathDelay = atof(delayValue);
                                        if(fetchedVert->worstDelay < pathDelay){
                                            fetchedVert->worstDelay = pathDelay ;
                                            //printf("INFO2:vert name:%s path delay:%.2f\n",fetchedVert->name,pathDelay);
                                        }
                                    }
                                }else{
                                    delayValue = array_fetch(char*,wordArr,3);
                                    if(check_number(delayValue)){
                                        pathDelay = atof(delayValue);
                                        if(fetchedVert->worstDelay < pathDelay){
                                            fetchedVert->worstDelay = pathDelay ;
                                            //printf("INFO4:vert name:%s path delay:%.2f\n",fetchedVert->name,pathDelay);
                                        }
                                    }else{
                                        printf("ERROR: delayValue is not a number, %s. \n",delayValue);
                                    }

                                }

                        }
                        

                        while (!required_time && (!(!strcmp(line, "1") || !strcmp(line, "0"))))
                        { // wait till the report finishing with 1/0
                            array_free(wordArr);
                            read = getline(&line, &len, fp);
                            line[strcspn(line, "\n")] = '\0';
                            line[strcspn(line, "\r")] = '\0';
                            wordArr = extract_paths(line, params);

                            if (array_n(wordArr) == 0)
                            {
                                continue;
                            }

                            word = array_fetch(char *, wordArr, 0);

                            if (!strcmp(word, "data"))
                            {
                                word = array_fetch(char *, wordArr, 1);
                                if (!strcmp(word, "required"))
                                {
                                    word = array_fetch(char *, wordArr, 2);
                                    if (!strcmp(word, "time")){
                                        word = array_fetch(char *, wordArr, 3);
                                        params->G->data_required_time = atof(word);
                                        required_time = 1 ;
                                    }
                                }
                                
                            }
                        }

                        //array_insert_last(pathList *, path_list, pathN);

                        createNewPath = 1;
                        // printf("Endpoint Encountered in the path.(end point:%s, full word:%s, path_list number of nodes:%d)\n",endPoint,word,array_n(pathN->node));
                        // printf("resetting startpoint and endpoint in the paths\n");
                        free(startPoint);
                        free(endPoint);
                        startAdding = 0;
                        // array_free(wordArr);
                        // goto fetch_next_line;
                    }
                    else{
                    
                        if (array_n(wordArr) >= 3 && check_number(word)){
                         // come back here lator and make adequate changes
                            continue;
                        }
                        // printf("Potential gate:%s\n",word);
                        // char* pilot_word = array_fetch(char*,wordArr,1) ;

                        fetchedVert = findVertexByNameSynDumpFile(params, word);

                        if (fetchedVert != NULL){
                            if (array_n(wordArr) >= 3 && !strcmp("<-", array_fetch(char *, wordArr, 2))){ //this is for when <- is encountered in the line
                                if (array_n(wordArr) == 3){ //this is when the name is too large and the rest of the line is written in the next line 
                                    //fetch the next line 
                                    read = getline(&line, &len, fp);

                                    line[strcspn(line, "\n")] = '\0';
                                    line[strcspn(line, "\r")] = '\0';
                                    wordArr = extract_paths(line, params);

                                    char* fetchedWord;
                                    fetchedWord = array_fetch(char*,wordArr,1);
                                    if(check_number(fetchedWord)){
                                        pathDelay = atof(delayValue);
                                        if(fetchedVert->worstDelay < pathDelay){
                                            fetchedVert->worstDelay = pathDelay ;
                                            //printf("INFO1:vert name:%s path delay:%.2f\n",fetchedVert->name,pathDelay);
                                        }
                                    }else{
                                        fetchedWord = array_fetch(char*,wordArr,2);
                                        pathDelay = atof(delayValue);
                                        if(fetchedVert->worstDelay < pathDelay){
                                            fetchedVert->worstDelay = pathDelay ;
                                            //printf("INFO2:vert name:%s path delay:%.2f\n",fetchedVert->name,pathDelay);
                                        }
                                    }

                                    skip_next_line = 1;                                    
                                }else{ //this is the normal case when there is <- and also thne rest of the line is also present in the same line 
                                    delayValue = array_fetch(char*,wordArr,4);
                                    if(check_number(delayValue)){
                                        pathDelay = atof(delayValue);
                                        if(fetchedVert->worstDelay < pathDelay){
                                            fetchedVert->worstDelay = pathDelay ;
                                            //printf("INFO3:vert name:%s path delay:%.2f\n",fetchedVert->name,pathDelay);
                                        }
                                    }else{
                                        printf("ERROR: delayValue is not a number, %s. \n",delayValue);
                                    }  

                                    skip_next_line = 1;                              
                                }
                            }else{ //this when <- is not there and the rest if the line is there 

                                if(array_n(wordArr) == 2){
                                    //fetch the next line 
                                    read = getline(&line, &len, fp);

                                    line[strcspn(line, "\n")] = '\0';
                                    line[strcspn(line, "\r")] = '\0';
                                    wordArr = extract_paths(line, params);

                                    char* fetchedWord;
                                    fetchedWord = array_fetch(char*,wordArr,1);
                                    if(check_number(fetchedWord)){
                                        pathDelay = atof(delayValue);
                                        if(fetchedVert->worstDelay < pathDelay){
                                            fetchedVert->worstDelay = pathDelay ;
                                            //printf("INFO1:vert name:%s path delay:%.2f\n",fetchedVert->name,pathDelay);
                                        }
                                    }else{
                                        fetchedWord = array_fetch(char*,wordArr,2);
                                        pathDelay = atof(delayValue);
                                        if(fetchedVert->worstDelay < pathDelay){
                                            fetchedVert->worstDelay = pathDelay ;
                                            //printf("INFO2:vert name:%s path delay:%.2f\n",fetchedVert->name,pathDelay);
                                        }
                                    }
                                }else{
                                    delayValue = array_fetch(char*,wordArr,3);
                                    if(check_number(delayValue)){
                                        pathDelay = atof(delayValue);
                                        if(fetchedVert->worstDelay < pathDelay){
                                            fetchedVert->worstDelay = pathDelay ;
                                            //printf("INFO4:vert name:%s path delay:%.2f\n",fetchedVert->name,pathDelay);
                                        }
                                    }else{
                                        printf("ERROR: delayValue is not a number, %s. \n",delayValue);
                                    }

                                }

                            }
                        }else{
                        
                            printf("ERROR: Word:%s not found in the vertex array.\n", word);
                        }

                        // if (array_n(wordArr) >= 3 && !strcmp("<-", array_fetch(char *, wordArr, 2))){
                        
                        //     if (array_n(wordArr) == 3){
                            
                        //         skip_next_line = 2;
                        //     }
                        //     else{
                            
                        //         skip_next_line = 1;
                        //     }
                        // }
                    }
                }
            }
        }
    fetch_next_line:;
        array_free(wordArr);
    }

    // printf("Number of paths discovered:%d.\n",array_n(path_list));

    fclose(fp);

    //---------------------------DELETE-------------------------------------------------

    // vertex* testVert;
    // array_t *testVertArr = params->G->vertexArr;

    // for(int r=0;r<array_n(testVertArr);r++){
    //     testVert = array_fetch(vertex*,testVertArr,r);
    //     printf("%d) name:%s  worstDelay:%.2f\n",r,testVert->name,testVert->worstDelay);
    // }

    // printf("Data Required time:%0.2f\n",params->G->data_required_time);

    // exit(0);

    //---------------------------DELETE-------------------------------------------------

    // params->G->paths =  path_list; //link the path list to the paths array_t poinyter in the graph G

    return 0;
}

int searchVertexIDbyName(array_t *vertArr, char *name)
{
    vertex *vert;

    for (int i = 0; i < array_n(vertArr); i++)
    {
        vert = array_fetch(vertex *, vertArr, i);
        if (!strcmp(vert->name, name))
        {
            return i;
        }
    }

    return -1;
}

void storeVertex(array_t *vertices, int idx, char *inverter_cell_name[], array_t *vertArr)
{

    int number_of_inverter_cells = sizeof(inverter_cell_name) / sizeof(char *);
    int found = -1;
    vertex *fetchedVert;

    fetchedVert = array_fetch(vertex *, vertArr, idx);

    idList *ID = (idList *)malloc(sizeof(idList));
    ;

    for (int i = 0; i < number_of_inverter_cells; i++)
    {
        if (!strcmp(fetchedVert->type->name, inverter_cell_name[i]))
        {
            found = i;
            break;
        }
    }

    if (found < 0)
    {

        ID->idx = idx;
        ID->type = 0;
        ID->inv_type = 0;
    }
    else
    {

        ID->idx = idx;
        ID->type = 1;
        ID->inv_type = found;
    }

    array_insert_last(idList *, vertices, ID);
}

int searchVertexinverticesbyID(array_t *vertices, int idx)
{

    idList *fetchID;

    for (int i = 0; i < array_n(vertices); i++)
    {
        fetchID = array_fetch(idList *, vertices, i);
        if (fetchID->idx == idx)
        {
            return 1;
        }
    }

    return 0;
}

void restore_delay(config_param *param)
{

    vertex* vert;

    for(int i=0; i < array_n(param->G->vertexArr);i++){
        vert = array_fetch(vertex*,param->G->vertexArr,i);
        vert->sulliedDelay = 0 ;
    }

}

//this checks if the given idx is already present in the array_t
int checkTraversedNodes(array_t* nodesTraversed, int idx){

    traversedNodes* fetchedNode;

    if(array_n(nodesTraversed)==0){
        return 1 ;
    }

    for(int i = 0 ; i < array_n(nodesTraversed); i++){
        fetchedNode = array_fetch(traversedNodes*,nodesTraversed,i);
        if(fetchedNode->idx == idx){
            return 1 ;
        }
    }

    return 0 ;

}


//return type: 0 : vertex untouched, 1: selected for insertion at the end, 2: timing path ended without violation, 3: timing void, not selected
//same goes for array values 
int checkTiming(vertex* vert, int *array, int *selected, int *rejected, config_param* param){

    int ret_val = 0 ;
    float dataRequiredTime = param->G->data_required_time ;
    float newDelay ; //this is the delay value that will be updated if timing is not violated 
    vertex *fetchedVert;
    edge* edg;

    array_t* vertArr = param->G->vertexArr ;

    if(vert->sulliedDelay == 0){ //if sullied delay is 0 that means it has not been visited 

        newDelay = vert->worstDelay + param->TemplateDelay ; //0.11 is the delay incurred due to puf insertion

    }else{ //else this has been visited and the new delay is updated over here 

        newDelay = vert->sulliedDelay + param->TemplateDelay ;

    }

    if(newDelay > dataRequiredTime){
        if(param->debug && param->suppress[0] == 0){
            printf("WARNING[0]: Timing path have been violated for node:%s. Puf will not be inserted in this path.\n",vert->name);
        }
        
        *rejected = *rejected + 1;

        for(int i=0; i<array_n(vertArr); i++){
            fetchedVert = array_fetch(vertex*,vertArr,i);
            if(!strcmp(vert->name,fetchedVert->name)){
                array[i] = 3 ;
            }
        }

        return 3;        
    }

    for(int i=0;i<array_n(vert->outEdges);i++){
        edg = array_fetch(edge*,vert->outEdges,i);
        for(int j=0; j<array_n(edg->toNodes);j++){
            fetchedVert = array_fetch(vertex*,edg->toNodes,j);
            if(strcasestr(fetchedVert->type->name,"dff")){
                if(param->debug && param->suppress[1] == 0){
                    printf("INFO[1]: A DFF : %s has been encountered. Path has ended without violation.Continuing.\n",fetchedVert->name);
                }
                continue ; //continue when a dff is encountered. a timing path ends at either a dff or a primary output
            }else{
                ret_val = checkTiming(fetchedVert,array,selected,rejected,param);
                if(ret_val == 3){
                    if(param->debug && param->suppress[2] == 0){
                        printf("WARNING[2]: Some node timing has been violated in the fan out cone of the node:%s. Puf will not be inserted in this timing path.\n",fetchedVert->name);
                    }
                    *rejected = *rejected + 1;

                    for(int k=0; k<array_n(vertArr); k++){                        
                        if(!strcmp(fetchedVert->name,array_fetch(vertex*,vertArr,k)->name)){
                            array[k] = 3 ;
                            break;
                        }
                    }

                    return 3;

                }
            }

        }
    }

    vert->sulliedDelay = newDelay ;
    return 2 ;


}

// int checkTimingViolation(config_param* param, int idx){
//     array_t* vertArr = param->G->vertexArr;
//     vertex* fetchedVert;
//     vertex* fetchedSubVert;
//     edge* fetchedEdg ;
//     int selected = 0 ;
//     float dataRequiredTime = param->G->data_required_time ;

//     float newDelay ; //this is the delay value that will be updated if timing is not violated 

//     fetchedVert = array_fetch(vertex*,vertArr,idx);

//     if(fetchedVert->sulliedDelay == 0){ //if sullied delay is 0 that means it has not been visited 

//         newDelay = fetchedVert->worstDelay + param->TemplateDelay ; //0.11 is the delay incurred due to puf insertion

//     }else{ //else this has been visited and the new delay is updated over here 

//         newDelay = fetchedVert->sulliedDelay + param->TemplateDelay ;

//     }

//     if(newDelay > dataRequiredTime){
//         printf("WARNING: Timing path have been violated for node:%s. Puf will not be inserted in this path.\n",fetchedVert->name);
        
//         if(!checkTraversedNodes(param->nodesTraversed,fetchedVert->idx)){
//             traversedNodes* newNode = (traversedNodes*)malloc(sizeof(traversedNodes));
//             newNode->idx = fetchedVert->idx ;
//             newNode->selected = 0 ;
//             array_insert_last(traversedNodes*,param->nodesTraversed,newNode);
//         }
        
//         return 0 ; //return zero on timing path violation
//     }

//     //first fetch the out edges followed by fetching all of its toNodes
//     for(int i=0; i < array_n(fetchedVert->outEdges); i++){
//         fetchedEdg = array_fetch(edge*,fetchedVert->outEdges,i);
//         for(int j=0; j < array_n(fetchedEdg->toNodes); j++){
//             fetchedSubVert = array_fetch(vertex*,fetchedEdg->toNodes,j);
//             if(strcasestr(fetchedSubVert->type->name,"dff")){ //do this graph traversal until a dff flop is encountered 
//                 printf("INFO: A DFF : %s has been encountered. Path has ended without violation.Continuing.\n",fetchedSubVert->name);
//                 continue ; //continue when a dff is encountered. a timing path ends at either a dff or a primary output
//             }else{
//                 selected = checkTimingViolation(param,fetchedSubVert->idx);
//                 if(selected == 0){
//                     printf("WARNING: Some node timing has been violated in the fan out cone of the node:%s. Puf will not be inserted in this timing path.\n",fetchedSubVert->name);
                    
//                     if(!checkTraversedNodes(param->nodesTraversed,fetchedSubVert->idx)){
//                         traversedNodes* newNode = (traversedNodes*)malloc(sizeof(traversedNodes));
//                         newNode->idx = fetchedSubVert->idx ;
//                         newNode->selected = 0 ;
//                         array_insert_last(traversedNodes*,param->nodesTraversed,newNode);
//                     }

//                     return 0 ;
//                 }
//             }
            
//         }
//     }

//     fetchedVert->sulliedDelay = newDelay ;
//     return 1 ;
    
// }

modifyInstructions* createMI(array_t *miTargetCells,config_param *param){

    modifyInstructions* mi = (modifyInstructions*)malloc(sizeof(modifyInstructions));
    mi->operation = 1 ;
    mi->template =  param->puf_template;
    mi->inv_template = param->inv_template; 
    mi->nand_template = param->nand_template;
    mi->nor_template = param->nor_template;
    mi->targetCells = miTargetCells;

    if(!strcmp(param->dont_touch,"vivado")){
        mi->dummy_insertion = 1;
    }else{
        mi->dummy_insertion = 0;
    }

    return mi;

}

modifyInstructions* createMI2(array_t *miVert, config_param *param){

    modifyInstructions* mi2 = (modifyInstructions*)malloc(sizeof(modifyInstructions));

    mi2->template = param->puf_template;
    mi2->inv_template = param->inv_template;
    mi2->nand_template = param->nand_template;
    mi2->nor_template = param->nor_template;
    mi2->operation = 0; //this is for doing the puf insertion at the end 
    mi2->targetCells = miVert; //provide the target cells for puf insertion

    if(!strcmp(param->dont_touch,"vivado")){
        mi2->dummy_insertion = 1;
    }else{
        mi2->dummy_insertion = 0;
    }

    return mi2;

}

// 0-> check if directory is present else create the same,1-> create a file
//void* return type... do you know what chaos you can do with it ?? :D 
void* checkCreateDirectoryFile(int condition,char* name){

    char tempName[10000];

    if(condition == 0){
        DIR* dir = opendir(name);
        if (dir) {
            /* Directory exists. */
            closedir(dir);
        } else if (ENOENT == errno) {
            /* Directory does not exist. */
            printf("INFO: Creating %s directory.\n",name);
            sprintf(tempName,"mkdir %s",name);
            system(tempName);
        } else {
            printf("ERROR: Could not create the %s directory, please create it manually. Exiting.....\n",name);
            return NULL;
        }
    }else if(condition == 1){
        
        FILE* out;
        out = fopen(name, "w+");
        if (out == NULL){
            printf("ERROR: File : %s could not be created.\n", name);
            return NULL;
        }else{
            printf("INFO: Output file name : %s.\n",name);
        }

        return out;

    }

    return 1 ;
    
}

//flag 0: means use all of the cells, 1 means replace/insert the chosen nodes only
void createListForResueCells(array_t* miTargetCells, array_t *selectedVertexes,int* array,int possiblePairs,int *selectedPufNumber,int templateType, int flag){

    int i;

    for(i=0;i<possiblePairs;i++){
        if(flag && array[i] == 0){
            continue;
        }

        *selectedPufNumber = *selectedPufNumber + 1;
        reuseIdList* newReuseID = (reuseIdList*)malloc(sizeof(reuseIdList));
        newReuseID->vert1 = array_fetch(vertex*,selectedVertexes,2*i);
        newReuseID->vert2 = array_fetch(vertex*,selectedVertexes, ((2*i) + 1));
        newReuseID->pufExcess = 0 ;
        newReuseID->pufTemplateType = templateType ;
        array_insert_last(reuseIdList*,miTargetCells,newReuseID); 

    }

}

int* randomlyChoose(int range,int number){

    int* array = calloc(range, sizeof(int));
    int selected = 0;
    int jockey;
    int i=0;

    if(number > range){
        for(i=0;i<range;i++){
            array[i] = 1 ;
        }
        
    }else{
        while(selected < number){
            jockey = rand()%range;
            if(array[jockey] != 1){
                selected++;
                array[jockey] = 1;
            }
        }
    }

    return array;

}

void endInsertion(int *vertID, int selectedPufNumber, array_t *miVert, config_param* param){

    //count the number of 0 in vertID
    //int i = 0 ;
    int count = 0;
    int selected = 0 ; //placeholder variables, has no significance here
    int rejected = 0 ; //placeholder variables, has no significance here
    int ret_value = 0 ;
    int numberOfVertices = array_n(param->G->vertexArr);
    vertex* vert;
    graph* G = param->G;

    for(int i=0;i<numberOfVertices;i++){
        if(vertID[i] == 0){
            count++;
        }
    }

    if(count > param->number_of_puf - selectedPufNumber){ //then randomly choose nodes for puf insertion

        int *tempVertID = calloc(count,sizeof(int));
        int *tempID;
        int j=0 ;

        for(int i=0;i<numberOfVertices;i++){
            if(vertID[i] == 0){
                if(param->slack_aware /*&& param->reuseLogic*/){ //come back here problematic
                    vert = array_fetch(vertex*,G->vertexArr,i);
                    //return type: 0 : vertex untouched, 1: selected for insertion at the end, 2: timing path ended without violation, 3: timing void, not selected
                    ret_value = checkTiming(vert, vertID, &selected, &rejected, param);
                    if(ret_value == 3){
                        vertID[i] = 3;
                        continue;
                    }
                }
                tempVertID[j] = i;
                j++;
            }
        }

        if(param->slack_aware /*&& param->reuseLogic*/){
            tempID = randomlyChoose(j,param->number_of_puf - selectedPufNumber); //come back here problematic
            count = j ;
        }else{
            tempID = randomlyChoose(count,param->number_of_puf - selectedPufNumber);
        }

        for(int i=0;i<count;i++){
            if(tempID[i] == 1){
                vert = array_fetch(vertex*,G->vertexArr,tempVertID[i]);
                array_insert_last(vertex*,miVert,vert);
            }
        }

        free(tempVertID);
        free(tempID);


    }else{ //choose all of the nodes for puf insertion

        for(int i=0;i<numberOfVertices;i++){
            if(vertID[i] == 0){
                vert = array_fetch(vertex*,G->vertexArr,i);
                if(param->slack_aware /*&& param->reuseLogic*/){
                    ret_value = checkTiming(vert, vertID, &selected, &rejected, param);
                    if(ret_value == 3){
                        vertID[i] = 3;
                        continue;
                    }
                }
                array_insert_last(vertex*,miVert,vert);
            }
        }

    }

}

                    

void mixedInsertion(int *vertID, array_t* miTargetCells, array_t* miVert, array_t *invVertexes, array_t *nandVertexes, array_t *norVertexes, config_param* param){


    int possibleInvPairs = 0 ;
    int possibleNandPairs = 0 ;
    int possibleNorPairs = 0 ;
    int selectedPufNumber = 0 ;
    int mi2Flag = 0 ;
    int i=0;

    vertex* vert;
    graph* G = param->G;

    int numberOfVertices = array_n(param->G->vertexArr);
    
    possibleInvPairs =  array_n(invVertexes)/2;
    possibleNandPairs =  array_n(nandVertexes)/2;
    possibleNorPairs =  array_n(norVertexes)/2;

    if(possibleInvPairs > param->number_of_puf){

        int* array = randomlyChoose(possibleInvPairs,param->number_of_puf);

        createListForResueCells(miTargetCells,invVertexes,array,possibleInvPairs,&selectedPufNumber,0,1);

        free(array);                    

    }else if(possibleInvPairs + possibleNandPairs > param->number_of_puf){

        int* array = randomlyChoose(possibleNandPairs,param->number_of_puf - possibleInvPairs);

        createListForResueCells(miTargetCells,invVertexes,array,possibleInvPairs,&selectedPufNumber,0,0);
        createListForResueCells(miTargetCells,nandVertexes,array,possibleNandPairs,&selectedPufNumber,1,1);

        free(array);

    }else if(possibleInvPairs + possibleNandPairs + possibleNorPairs > param->number_of_puf){

        int* array = randomlyChoose(possibleNorPairs,param->number_of_puf - possibleInvPairs - possibleNandPairs);

        createListForResueCells(miTargetCells,invVertexes,array,possibleInvPairs,&selectedPufNumber,0,0);
        createListForResueCells(miTargetCells,nandVertexes,array,possibleNandPairs,&selectedPufNumber,1,0);
        createListForResueCells(miTargetCells,norVertexes,array,possibleNorPairs,&selectedPufNumber,2,1);

        free(array);

    }else{
        mi2Flag = 1 ;

        createListForResueCells(miTargetCells,invVertexes,NULL,possibleInvPairs,&selectedPufNumber,0,0);
        createListForResueCells(miTargetCells,nandVertexes,NULL,possibleNandPairs,&selectedPufNumber,1,0);
        createListForResueCells(miTargetCells,norVertexes,NULL,possibleNorPairs,&selectedPufNumber,2,0);

        endInsertion(vertID, selectedPufNumber, miVert, param);
    }

    modifyInstructions* mi = createMI(miTargetCells,param);
    modifyInstructions* mi2 = createMI2(miVert, param);

    modifyGraph(G,mi);
    if(mi2Flag){
        modifyGraph(G,mi2);
    }                
    free(miVert);
    free(mi);
    free(mi2);
    array_free(miTargetCells);

}

void nonMixedInsertion(int *vertID, array_t* miTargetCells, array_t *selectedVertexes, config_param* param){


    int selected = 0 ; //placeholder variables, has no significance here
    int rejected = 0 ; //placeholder variables, has no significance here
    int ret_value = 0 ;

    vertex* vert;
    graph* G = param->G;
    graph* puf_template = param->puf_template;
    graph* inv_template = param->inv_template; 
    graph* nand_template = param->nand_template;
    graph* nor_template = param->nor_template;

    int i=0;
    int j;
    int numberOfVertices = array_n(param->G->vertexArr);
    int possiblePairs = array_n(selectedVertexes)/2 ;

    if(param->debug && param->suppress[5] != 1){
        printf("INFO[5]: Number of reuse-logic node pairs: %d.\n",possiblePairs);
    }
    

    if(param->number_of_puf > possiblePairs){

        int numberOfPUFSelected = possiblePairs ;
        int test = 0 ;
        array_t* miVert = array_alloc(vertex*,1);

        createListForResueCells(miTargetCells,selectedVertexes,NULL,possiblePairs,&test,param->pufTemplateType,0);       

        endInsertion(vertID, numberOfPUFSelected, miVert, param);

        modifyInstructions* mi = createMI(miTargetCells,param);
        modifyInstructions* mi2 = createMI2(miVert, param);

        modifyGraph(G,mi);
        modifyGraph(G,mi2);
        array_free(miTargetCells);
        free(miVert);
        free(mi);
        free(mi2);


    }else{
        
        int* pairID;
        int test = 0 ;
        pairID = randomlyChoose(possiblePairs,param->number_of_puf);

        createListForResueCells(miTargetCells,selectedVertexes,pairID,possiblePairs,&test,param->pufTemplateType,1);

        modifyInstructions* mi = createMI(miTargetCells,param);

        modifyGraph(G,mi);
        free(pairID);
        free(selectedVertexes);
        free(mi);
        array_free(miTargetCells);

    }
 

}


void readPufTemplates(config_param *param){

    FILE *fptr ;
    char *libFile = param->libFile;
    graph *puf_template,*inv_template,*nand_template,*nor_template;

    //---------------------------------------------- read all of the puf templates file ---------------------------------------------------------
    if(param->suppress[7] != 1){
        printf("INFO[7]: Reading puf template file -> puf_template/puf_template.v \n"); // Atri : This is for reading in the puf_template
    }
    fptr = fopen("puf_template/puf_template.v", "r");
    if (!fptr){
        if(param->suppress[8] != 1){
            printf("ERROR[8]: The template file could not be opened. Aborting exection.\n");     
        }
        return;           
    }else{
        fclose(fptr);
        puf_template = createGraphFromVerilog("puf_template/puf_template.v", libFile, "puf_template", &pufPrimIn, param->oldG);
        param->puf_template = puf_template;
    }

    if(param->suppress[9] != 1){
        printf("INFO[9]: Reading puf template file -> puf_template/puf_template_inverter.v \n"); // Atri : This is for reading in the puf_template
    }
    fptr = fopen("puf_template/puf_template_inverter.v", "r");
    if (!fptr){
        if(param->suppress[10] != 1){
            printf("ERROR[10]: The template file could not be opened. Aborting exection.\n");     
        }
        return;           
    }else{
        fclose(fptr);
        inv_template = createGraphFromVerilog("puf_template/puf_template_inverter.v", libFile, "puf_template_inverter", &pufPrimIn, param->oldG);
        param->inv_template = inv_template ;
    }

    if(param->suppress[11] != 1){
        printf("INFO[11]: Reading puf template -> puf_template/puf_template_nand.v \n"); // Atri : This is for reading in the puf_template
    }
    fptr = fopen("puf_template/puf_template_nand.v", "r");
    if (!fptr){
        if(param->suppress[12] != 1){
            printf("ERROR[12]: The template file could not be opened. Aborting exection.\n");    
        }
        return;            
    }else{
        fclose(fptr);
        nand_template = createGraphFromVerilog("puf_template/puf_template_nand.v", libFile, "puf_template_nand", &pufPrimIn, param->oldG);
        param->nand_template = nand_template ;
    }

    if(param->suppress[13] != 1){
        printf("INFO[13]: Reading puf template file -> puf_template/puf_template_nor.v \n"); // Atri : This is for reading in the puf_template
    }
    fptr = fopen("puf_template/puf_template_nor.v", "r");
    if (!fptr){
        if(param->suppress[14] != 1){
            printf("ERROR[14]: The template file could not be opened. Aborting exection.\n");   
        }
        return;             
    }else{
        fclose(fptr);
        nor_template = createGraphFromVerilog("puf_template/puf_template_nor.v", libFile, "puf_template_nor", &pufPrimIn, param->oldG);
        param->nor_template = nor_template ;
    }

}

void createTB(FILE *fptr2,config_param *param){

    int i,j,n;
    int flag=0;
    array_t* store;
    array_t* portNames;
    array_t* portNames1;
    array_t* portNames2;
    array_t* vertArr;
    char* name;
    vectorList* fetchedVector;
    graph *G;
    vertex* vert;
    edge* edg;    
    char tempString[10000];

    G = param->G ;
    vertArr = G->vertexArr;
    portNames = array_alloc(char*,1);
    portNames1 = array_alloc(char*,1);
    portNames2 = array_alloc(char*,1);

    fprintf(fptr2,"module %s_tb;\n",param->moduleName);

    fprintf(fptr2," localparam NUM_TESTS = %d ;\n",param->numTest);
    fprintf(fptr2," localparam clk_change = 5;\n");
    fprintf(fptr2,"\n");
    fprintf(fptr2," int count;\n");
    fprintf(fptr2," int test_passed;\n");
    fprintf(fptr2,"\n");

    store = vectorExtractor(G->primInEdges);

    for(i=0;i<array_n(store);i++){
        fprintf(fptr2,"    reg ");
        fetchedVector = array_fetch(vectorList*,store,i);                
        if(fetchedVector->max == 0 && fetchedVector->min == 0){
            fprintf(fptr2,"%s;\n",fetchedVector->name);
        }else{
            fprintf(fptr2,"[%d:%d] %s;\n",fetchedVector->max,fetchedVector->min,fetchedVector->name);
        }   
        array_insert_last(char*,portNames,fetchedVector->name);
        array_insert_last(char*,portNames1,fetchedVector->name);
    }
    array_free(store);
    store = vectorExtractor(G->primOutEdges);
    
    for(i=0;i<array_n(store);i++){
        fprintf(fptr2,"    wire ");
        fetchedVector = array_fetch(vectorList*,store,i);                
        if(fetchedVector->max == 0 && fetchedVector->min == 0){
            fprintf(fptr2,"%s, %s_puf;\n",fetchedVector->name,fetchedVector->name);
        }else{
            fprintf(fptr2,"[%d:%d] %s, %s_puf;\n",fetchedVector->max,fetchedVector->min,fetchedVector->name,fetchedVector->name);
        }    
        array_insert_last(char*,portNames,fetchedVector->name); 
        array_insert_last(char*,portNames2,fetchedVector->name);
    }

    array_free(store);


    fprintf(fptr2,"\n");
    fprintf(fptr2,"\n");
    fprintf(fptr2," %s orig(",param->moduleName);

    for(i=0;i<array_n(portNames);i++){
        name = array_fetch(char*,portNames,i);
        if(!strcmp(name,"sel") || !strcmp(name,"pufout")){
            continue;
        }
        fprintf(fptr2,"%s.%s(%s)",i == 0 ? " ":", ",name,name);
    }

    fprintf(fptr2,");\n");

        fprintf(fptr2," %s dut(",param->G->header);

    for(i=0;i<array_n(portNames1);i++){
        name = array_fetch(char*,portNames1,i);
        fprintf(fptr2,".%s(%s), ",name,name);
    }

    for(i=0;i<array_n(portNames2);i++){
        name = array_fetch(char*,portNames2,i);
        fprintf(fptr2,".%s(%s_puf)%s",name,name,i==array_n(portNames2)-1 ? " ":", ");
    }

    fprintf(fptr2,");\n");

    

    fprintf(fptr2,"\n");
    fprintf(fptr2,"\n");
    fprintf(fptr2," initial begin\n");
    fprintf(fptr2,"     %s <= 0 ;\n",param->clock);
    fprintf(fptr2,"     while (1)\n");
    fprintf(fptr2,"         #5 %s = ~%s;\n",param->clock,param->clock);
    fprintf(fptr2," end\n");
    fprintf(fptr2,"\n");

    if(param->trace){
        fprintf(fptr2," initial begin\n");
        fprintf(fptr2,"     $dumpfile(\"%s.vcd\");\n",param->G->header);
        fprintf(fptr2,"     $dumpvars;\n");  
        fprintf(fptr2,"end\n");
        fprintf(fptr2,"\n");      
    }   

    fprintf(fptr2," initial begin\n");
    fprintf(fptr2,"     $timeformat(-9, 0, \"ns\");\n");
    fprintf(fptr2,"     $display(\"INFO: Initializing Tests for %s.\");  \n",param->G->header);
    fprintf(fptr2,"     @(posedge %s);\n",param->clock);
    fprintf(fptr2,"     test_passed = 0 ;\n");
    fprintf(fptr2,"     %s <= 1 ;\n\n",param->reset);

    for(i=0;i<array_n(portNames1);i++){
        name = array_fetch(char*,portNames1,i);
        if(!strcmp(name,param->clock) || !strcmp(name,param->reset)){
            continue;
        }else if(!strcmp(name,"sel")){
            fprintf(fptr2,"     %s <= 1 ;\n",name);
        }else{
            fprintf(fptr2,"     %s <= 0 ;\n",name);
        }
        
    }

    fprintf(fptr2,"\n");
    fprintf(fptr2,"     count <= 0;\n");
    fprintf(fptr2,"     $display(\"INFO: Setting resets for 10 clock cycles.\"); \n");
    fprintf(fptr2,"     for(int i=0; i< 10 ; i++) begin\n");
    fprintf(fptr2,"         @(posedge %s);\n",param->clock);
    fprintf(fptr2,"     end\n");
    fprintf(fptr2,"\n");
    fprintf(fptr2,"     $display(\"INFO: Deasserting reset\"); \n");
    fprintf(fptr2,"     @(posedge %s);\n",param->clock);
    fprintf(fptr2,"     %s <= 0 ;\n\n",param->reset);
    fprintf(fptr2,"\n");
    fprintf(fptr2,"         $display(\"INFO: Starting Random Test.\"); \n");
    fprintf(fptr2,"     for (int i=0; i < NUM_TESTS; i++) begin\n");
    if(param->suppress[26] == 1){
        fprintf(fptr2,"//");
    }
    fprintf(fptr2,"         $display(\"INFO[26]: Running Test:%%d.\",i); \n");
    fprintf(fptr2,"         @(posedge %s);\n",param->clock);

    for(i=0;i<array_n(portNames1);i++){
        name = array_fetch(char*,portNames1,i);
        flag = 0 ;
        for(j=0;j<array_n(param->control_ports);j++){
            if(!strcmp(name,array_fetch(char*,param->control_ports,j))){
                flag = 1;
            }
        }
        if(!strcmp(name,param->clock) || !strcmp(name,param->reset) || !strcmp(name,"sel")){
                flag = 1;
        }
        if(flag == 1){
            continue;
        }        
        fprintf(fptr2,"         %s <= $random ;\n",name);
    }

    fprintf(fptr2,"\n");
    fprintf(fptr2,"         @(posedge %s);\n",param->clock);

    for(i=0;i<array_n(param->control_ports);i++){
        name = array_fetch(char*,param->control_ports,i);
        fprintf(fptr2,"         %s <= $random ;\n",name);
    }

    fprintf(fptr2,"\n");
    fprintf(fptr2,"         @(posedge %s);\n",param->clock);

    for(i=0;i<array_n(param->control_ports);i++){
        name = array_fetch(char*,param->control_ports,i);
        fprintf(fptr2,"         %s <= 0 ;\n",name);
    }

    fprintf(fptr2,"\n");
    fprintf(fptr2,"         count <= 0;\n");
    if(param->suppress[27] == 1){
        fprintf(fptr2,"//");
    }
    fprintf(fptr2,"         $display(\"INFO[27]: Polling the status ports for 100 clock cycles.\"); \n");
    fprintf(fptr2,"         while(count < 100) begin\n");
    fprintf(fptr2,"             @(posedge %s);\n",param->clock);
    fprintf(fptr2,"             if(");

    for(i=0;i<array_n(param->status_ports);i++){
        name = array_fetch(char*,param->status_ports,i);
        fprintf(fptr2,"%s%s",name,i==array_n(param->status_ports)-1 ? "":" && ");
    }

    fprintf(fptr2,") begin\n");
    fprintf(fptr2,"                 break;\n");
    fprintf(fptr2,"             end\n");
    fprintf(fptr2," \n");
    fprintf(fptr2,"             count++;\n");
    fprintf(fptr2,"\n");
    fprintf(fptr2,"         end\n");
    fprintf(fptr2,"\n");
    if(param->suppress[28] == 1){
        fprintf(fptr2,"//");
    }
    fprintf(fptr2,"         $display(\"INFO[28]: Validating the output values.\");\n");
    fprintf(fptr2,"\n");
    fprintf(fptr2,"         if(count < 100) begin\n");
    fprintf(fptr2,"             if((");
    
    for(i=0;i<array_n(param->status_ports);i++){
        name = array_fetch(char*,param->status_ports,i);
        fprintf(fptr2,"%s%s_puf",i==0 ? "":" && ",name);
    }

    fprintf(fptr2,") != 1) begin\n");
    if(param->suppress[29] == 1){
        fprintf(fptr2,"//");
    }
    fprintf(fptr2,"                 $display(\"ERROR[29] (time %%0t): The out valid of the dut module has not been aserted.\", $realtime);\n");
    fprintf(fptr2,"             end\n");
    fprintf(fptr2,"             else begin\n");
    fprintf(fptr2,"             if(");

    for(i=0;i<array_n(portNames2);i++){
        name = array_fetch(char*,portNames2,i);
        
        if(!strcmp(name,"pufout")){
            continue;
        }

        for(j=0;j<array_n(param->status_ports);j++){
            if(!strcmp(name,array_fetch(char*,param->status_ports,j))){
                continue;
            }
        }
     
        fprintf(fptr2,"%s(%s == %s_puf)",i==0 ? "":" && ",name,name);
    }

    fprintf(fptr2,") begin\n");
    if(param->suppress[30] == 1){
        fprintf(fptr2,"//");
    }
    fprintf(fptr2,"                 $display(\"INFO[30] (time %%0t): The output of the two designs match.\", $realtime);\n");
    fprintf(fptr2,"                 test_passed++;\n");
    fprintf(fptr2,"             end\n");
    fprintf(fptr2,"             else begin\n");
    if(param->suppress[31] == 1){
        fprintf(fptr2,"//");
    }
    fprintf(fptr2,"                 $display(\"ERROR[31] (time %%0t): The output of the two designs does not match.\", $realtime);\n");
    fprintf(fptr2,"             end\n");
    fprintf(fptr2,"         end\n");
    fprintf(fptr2,"     end\n");
    fprintf(fptr2,"     else begin\n");
    if(param->suppress[32] == 1){
        fprintf(fptr2,"//");
    }
    fprintf(fptr2,"         $display(\"ERROR[32] (time %%0t): The output of the original design has itself not been asserted.\", $realtime);\n");
    fprintf(fptr2,"     end\n");
    fprintf(fptr2," end\n");
    fprintf(fptr2,"\n");
    if(param->suppress[33] == 1){
        fprintf(fptr2,"//");
    }
    fprintf(fptr2," $display(\"INFO[33]: Tests passed: %%d/%%d.\",test_passed,NUM_TESTS); \n");
    fprintf(fptr2," $display(\"Tests completed.\"); \n");
    fprintf(fptr2," $finish;\n");
    fprintf(fptr2,"\n");
    fprintf(fptr2,"end \n");    
    fprintf(fptr2,"\n");
    fprintf(fptr2,"endmodule\n");

    array_free(portNames);
    array_free(portNames1);
    array_free(portNames2);
    
}

void insert_puf(config_param *param)
{

    graph* G = param->G ;
    vertex *vert;
    int number_of_pufs = param->number_of_puf;
    char *libFile = param->libFile;
    char *name,*current_path;
    FILE *fptr ;   
    FILE *fptr2 ; 
    int i; 

    srand(param->randomSeed);

    if (param->number_of_puf > array_n(G->vertexArr)){
        if(param->suppress[6] != 1){
            printf("ERROR[6]: The number of PUF cannot exceed the number of nodes in the given design. Cannot insert PUFs in the design.\n");
        }
        return;
    } 

    if(param->slack_aware){ //do operations for slack aware puf insertion

        if(param->reuseLogic){ //this is for reuse logic is required

            if(param->pufTemplateInsertionType){ //Mixed mode insertion. When the puf size is in excess puf_template type pufs will be inserted at the end of selected gates. Default -> 0 -> no mixture

                if(param->suppress[15] != 1){
                    printf("INFO[15]: Performing slack aware mixed mode logic-reuse mel-puf insertion.\n");
                }

                int selected = 0 ; //placeholder variables, has no significance here
                int rejected = 0 ; //placeholder variables, has no significance here
                int ret_value = 0;
                int numberOfVertices = array_n(G->vertexArr);
                int possiblePairs = 0 ;
                char *targetTypeNames[] = {"INVX1","NAND2X1","NOR2X1"};
                char *typeName;
                array_t *invVertexes = array_alloc(vertex*,1);
                array_t *nandVertexes = array_alloc(vertex*,1);
                array_t *norVertexes = array_alloc(vertex*,1);
                array_t* miTargetCells = array_alloc(reuseIdList*,1);
                array_t* miVert = array_alloc(vertex*,1);

                int *vertID = calloc(numberOfVertices,sizeof(int));
                //return type: 0 : vertex untouched, 1: selected for insertion at the end, 2: timing path ended without violation, 3: timing void, not selected
                for(i=0;i<numberOfVertices;i++){
                    vert = array_fetch(vertex*,G->vertexArr,i);
                    if(!strcmp(vert->type->name,"INVX1")){
                        ret_value = checkTiming(vert, vertID, &selected, &rejected, param);
                        if(ret_value == 2){
                            array_insert_last(vertex*,invVertexes,vert);
                            vertID[i] = 2 ; //mark this id as used for reuse logic node
                        }
                    }else if(!strcmp(vert->type->name,"NAND2X1")){
                        ret_value = checkTiming(vert, vertID, &selected, &rejected, param);
                        if(ret_value == 2){
                            array_insert_last(vertex*,nandVertexes,vert);
                            vertID[i] = 2 ; //mark this id as used for reuse logic node
                        }
                    }else if(!strcmp(vert->type->name,"NOR2X1")){
                        ret_value = checkTiming(vert, vertID, &selected, &rejected, param);
                        if(ret_value == 2){
                            array_insert_last(vertex*,norVertexes,vert);
                            vertID[i] = 2 ; //mark this id as used for reuse logic node
                        }
                    }
                }

                mixedInsertion(vertID, miTargetCells, miVert, invVertexes, nandVertexes, norVertexes, param);
                
                free(vertID);
            

            }else{ //this is when there is no mixture and only a given template type is used for puf insertion. When puf size is in excess, puf_template type pufs will be inserted at the end of selected gates.
                
                if(param->suppress[16] != 1){
                    printf("INFO[16]: Performing slack aware template specific logic-reuse mel-puf insertion.\n");
                }

                int numberOfVertices = array_n(G->vertexArr);
                int possiblePairs = 0 ;
                char *targetTypeNames[] = {"INVX1","NAND2X1","NOR2X1"};
                char *typeName;
                int selected = 0 ; //placeholder variables, has no significance here
                int rejected = 0 ; //placeholder variables, has no significance here
                int ret_value = 0 ;
                array_t *selectedVertexes = array_alloc(vertex*,1); //this will store the nodes that are potential candidates for melpuf insertion.
                array_t* miTargetCells = array_alloc(reuseIdList*,1);  

                //count the number of gates of the selected template and then mark them for melpuf insertion
                switch(param->pufTemplateType){
                    case 0:
                        typeName = targetTypeNames[0];
                        break;
                    case 1:
                        typeName = targetTypeNames[1];
                        break;
                    case 2:
                        typeName = targetTypeNames[2];
                        break;    
                }

                int *vertID = calloc(numberOfVertices,sizeof(int));   

                for(i=0;i<numberOfVertices;i++){
                    vert = array_fetch(vertex*,G->vertexArr,i);
                    if(!strcmp(vert->type->name,typeName)){
                        //return type: 0 : vertex untouched, 1: selected for insertion at the end, 2: timing path ended without violation, 3: timing void, not selected
                        ret_value = checkTiming(vert, vertID, &selected, &rejected, param);
                        if(ret_value == 2){
                            array_insert_last(vertex*,selectedVertexes,vert);
                            vertID[i] = 2 ; //mark this id as used for reuse logic node
                        }
                    }
                }
                //checkpoint1
                nonMixedInsertion(vertID,miTargetCells,selectedVertexes,param);
                           
                free(vertID);
            }

        }else{ //this is when reuse logic is not required. Pufs are inserted at the end of the selected node.
            if(param->suppress[17] != 1){
                printf("INFO[17]: Performing slack aware non-mixed mode mel-puf insertion.\n");
            }

            int selectedPufNumber = 0; //this is not required in purely endinsertion mode 
            int numberOfVertices = array_n(G->vertexArr);
            int* vertID = calloc(numberOfVertices,sizeof(int));
            array_t* miVert = array_alloc(vertex*,1);

            endInsertion(vertID, selectedPufNumber, miVert, param);

            modifyInstructions* mi2 = createMI2(miVert,param);
 
            modifyGraph(G,mi2); //modify the nodes
            free(miVert); //free the array
            free(mi2);
            free(vertID);

        }

    }else{ //do operation for slack unaware operation

        if(param->reuseLogic){ //this is for reuse logic is required

            if(param->pufTemplateInsertionType){ //Mixed mode insertion. When the puf size is in excess puf_template type pufs will be inserted at the end of selected gates. Default -> 0 -> no mixture

                if(param->suppress[18] != 1){
                    printf("INFO[18]: Performing generic mixed mode logic-reuse mel-puf insertion.\n");
                }

                int numberOfVertices = array_n(G->vertexArr);
                int possiblePairs = 0 ;
                char *targetTypeNames[] = {"INVX1","NAND2X1","NOR2X1"};
                char *typeName;
                array_t *invVertexes = array_alloc(vertex*,1);
                array_t *nandVertexes = array_alloc(vertex*,1);
                array_t *norVertexes = array_alloc(vertex*,1);
                array_t* miTargetCells = array_alloc(reuseIdList*,1);
                array_t* miVert = array_alloc(vertex*,1);

                int *vertID = calloc(numberOfVertices,sizeof(int));

                for(i=0;i<numberOfVertices;i++){
                    vert = array_fetch(vertex*,G->vertexArr,i);
                    if(!strcmp(vert->type->name,"INVX1")){
                        array_insert_last(vertex*,invVertexes,vert);
                        vertID[i] = 3 ;
                    }else if(!strcmp(vert->type->name,"NAND2X1")){
                        array_insert_last(vertex*,nandVertexes,vert);
                        vertID[i] = 4 ;
                    }else if(!strcmp(vert->type->name,"NOR2X1")){
                        array_insert_last(vertex*,norVertexes,vert);
                        vertID[i] = 5 ;
                    }
                }

                mixedInsertion(vertID, miTargetCells, miVert, invVertexes, nandVertexes, norVertexes, param);
                
                free(vertID);


            }else{ //this is when there is no mixture and only a given template type is used for puf insertion. When puf size is in excess, puf_template type pufs will be inserted at the end of selected gates.
                
                if(param->suppress[19] != 1){
                    printf("INFO[19]: Performing generic template specific logic-reuse mel-puf insertion.\n");
                }

                int numberOfVertices = array_n(G->vertexArr);
                int possiblePairs = 0 ;
                char *targetTypeNames[] = {"INVX1","NAND2X1","NOR2X1"};
                char *typeName;
                array_t *selectedVertexes = array_alloc(vertex*,1); //this will store the nodes that are potential candidates for melpuf insertion.
                array_t* miTargetCells = array_alloc(reuseIdList*,1);

                //count the number of gates of the selected template and then mark them for melpuf insertion
                switch(param->pufTemplateType){
                    case 0:
                        typeName = targetTypeNames[0];
                        break;
                    case 1:
                        typeName = targetTypeNames[1];
                        break;
                    case 2:
                        typeName = targetTypeNames[2];
                        break;    
                }

                int *vertID = calloc(numberOfVertices,sizeof(int));

                for(i=0;i<numberOfVertices;i++){
                    vert = array_fetch(vertex*,G->vertexArr,i);
                    if(!strcmp(vert->type->name,typeName)){
                        array_insert_last(vertex*,selectedVertexes,vert);
                        vertID[i] = 2 ; //mark this id as used for reuse logic node
                    }
                }

                nonMixedInsertion(vertID,miTargetCells,selectedVertexes,param);

                free(vertID);

            }

        }else{ //this is when reuse logic is not required. Pufs are inserted at the end of the selected node.
            
            if(param->suppress[20] != 1){
                printf("INFO[20]: Performing generic non-mixed mode mel-puf insertion.\n");
            }
            
            int selectedPufNumber = 0; //this is not required in purely endinsertion mode 
            int numberOfVertices = array_n(G->vertexArr);
            int* vertID = calloc(numberOfVertices,sizeof(int));
            array_t* miVert = array_alloc(vertex*,1);

            endInsertion(vertID, selectedPufNumber, miVert, param);

            modifyInstructions* mi2 = createMI2(miVert,param);
 
            modifyGraph(G,mi2); //modify the nodes
            free(miVert); //free the array
            free(mi2);
            free(vertID);

        }

    }

    
    if(checkCreateDirectoryFile(0,param->reports) == NULL){
        return;
    }
    name = calloc(10000,sizeof(char));
    current_path = calloc(10000,sizeof(char));
    sprintf(name,"%s/%s",param->reports,param->moduleName);
    if(checkCreateDirectoryFile(0,name) == NULL){
        return;
    }
    sprintf(name,"%s_puf_%d.v",param->moduleName, param->number_of_puf);
    param->outFile = strdup(name);
    sprintf(name,"%s/%s/%s",param->reports,param->moduleName, param->outFile);
    param->out = checkCreateDirectoryFile(1,name);
    free(name);
    if(param->out == NULL){
        return;
    }

    if(param->suppress[21] != 1){
        printf("INFO[21]: Output file name : %s/%s/%s\n",param->reports,param->moduleName,param->outFile);
    }
    if(param->suppress[22] != 1){
        printf("INFO[22]: Generating the Mel-PUF inserted design.\n");
    }

    writeVerilog(param);

    fclose(param->out);
    
    if(param->suppress[23] != 1){
        printf("INFO[23]: Successfully generated the Mel-PUF inserted design.\n");
    }

    name = calloc(10000,sizeof(char));

    if(param->verification == 1){
        printf("INFO: Generating verification setup.\n");
        sprintf(name,"%s/%s/testbench",param->reports,param->moduleName);
        checkCreateDirectoryFile(0,name);
        sprintf(name,"%s/%s/testbench/%s_%d_tb.sv",param->reports,param->moduleName,param->moduleName,param->number_of_puf);
        fptr2 = checkCreateDirectoryFile(1,name);
        if(fptr2 != NULL){
            sprintf(name,"%s/%s/testbench/%s_puf_%d.sv",param->reports,param->moduleName,param->moduleName,param->number_of_puf);
            param->out = checkCreateDirectoryFile(1,name);
            if(param->out == NULL){
                return;
            }           
            
            sprintf(name,"%s_puf_%d",param->moduleName,param->number_of_puf);
            param->G->header = name;
            
            printf("INFO: Generating verification puf inserted file.\n");
            writeVerilog(param);
            graph* tempG = param->G;
            param->G = param->puf_template;
            writeVerilog(param);
            param->G = param->inv_template;
            writeVerilog(param);
            param->G = param->nand_template;
            writeVerilog(param);
            param->G = param->nor_template;
            writeVerilog(param);
            param->G = tempG;
            printf("INFO: Generating verification testbench file.\n");
            createTB(fptr2,param);
            sprintf(name,"cp %s %s/%s/testbench/%s.sv",param->inputFile,param->reports,param->moduleName,param->moduleName);
            system(name);
            printf("INFO: Successfully generated the verification setup.\n");

            param->G->header = param->moduleName;
            
            fclose(fptr2);
            fclose(param->out);

            getcwd(current_path, 10000);
            printf("INFO: Starting VCS simulation.\n");            
            sprintf(name,"vcs %s/%s/testbench/%s_%d_tb.sv %s/%s/testbench/%s_puf_%d.sv %s/%s/testbench/%s.sv %s -sverilog -o %s/%s/testbench/simv %s",param->reports,param->moduleName,param->moduleName,param->number_of_puf,param->reports,param->moduleName,param->moduleName,param->number_of_puf,param->reports,param->moduleName,param->moduleName,param->toolPrimitive,param->reports,param->moduleName,param->debug ? " ":"> /dev/null");
            printf("INFO: vcs command: %s\n",name);
            system(name);
            sprintf(name,"./%s/%s/testbench/simv",param->reports,param->moduleName);
            system(name);
            printf("INFO: VCS simulation finished.\n");
        }
        
    }

    if(param->lec == 1){
        char verificationFile [1000];
        printf("INFO: Starting LEC.\n");
        sprintf(name,"%s/%s/LEC",param->reports,param->moduleName);
        checkCreateDirectoryFile(0,name);
        sprintf(name,"%s/%s/LEC/%s_%d_lec.do",param->reports,param->moduleName,param->moduleName,param->number_of_puf);
        fptr2 = checkCreateDirectoryFile(1,name);
        if(fptr2 != NULL){
            
            fprintf(fptr2,"set dofile abort exit\n");
            fprintf(fptr2,"setenv module_name \"%s\"\n",param->moduleName);
            fprintf(fptr2,"setenv verify_lec_path \"%s/%s/LEC\"\n",param->reports,param->moduleName);
            fprintf(fptr2,"read library -liberty %s\n",param->libFile);
            // fprintf(fptr2,"read design -golden \"\$verify_lec_path\"/original_flat.v\n");
            // fprintf(fptr2,"read design -revised \"\$verify_lec_path\"/revised_flat.v\n");
            fprintf(fptr2,"read design -golden original_flat_%d.v\n",param->number_of_puf);
            fprintf(fptr2,"read design -revised revised_flat_%d.v\n",param->number_of_puf);
            fprintf(fptr2,"set system mode setup \n");
            // fprintf(fptr2,"set screen display off\n");
            // fprintf(fptr2,"add primary input /%s/\"sel\" -pin -revised\n",param->moduleName);
            // fprintf(fptr2,"add pin constraints 1 /%s/\"sel\" -revised\n",param->moduleName);

            fprintf(fptr2,"add pin constraints 1 sel  -module %s  -revised\n",param->moduleName);        
            fprintf(fptr2,"add ignore outputs pufout* -module %s  -revised\n",param->moduleName);                  

            fprintf(fptr2,"report primary outputs > \"\$module_name\"_%d.primary_outputs.rpt\n",param->number_of_puf);                       

            fprintf(fptr2,"set system mode lec > \"\$module_name\"_%d.system_mode_lec.rpt\n",param->number_of_puf);
            fprintf(fptr2,"add compared points -all -golden\n");
            fprintf(fptr2,"compare\n");

            // diagnose -summary > "$verify_lec_path"/reports/"$module_name".summary.rpt
            // diagnose -noneq > "$verify_lec_path"/reports/"$module_name".noneq.rpt
            fprintf(fptr2,"report verification -verbose > \"\$module_name\"_%d.verification.rpt\n",param->number_of_puf);
            sprintf(verificationFile,"%s_%d.verification.rpt",param->moduleName,param->number_of_puf);
            fprintf(fptr2,"report statistics > \"\$module_name\"_%d.statistics.rpt\n",param->number_of_puf);            
            fprintf(fptr2,"exit -f\n");

            fclose(fptr2);
        
        }

        // copy the original file to the LEC folder as original_flat.v and the puf output as revised_flat.v
        printf("INFO: Copying the original and revised files to the LEC folder.\n");        
        sprintf(name,"cp %s %s/%s/LEC/original_flat_%d.v",param->inputFile,param->reports,param->moduleName,param->number_of_puf);
        system(name);
        printf("DEBUG: %s\n",name);
        sprintf(name,"cp %s/%s/%s %s/%s/LEC/revised_flat_%d.v",param->reports,param->moduleName,param->outFile,param->reports,param->moduleName,param->number_of_puf);
        system(name);
        printf("DEBUG: %s\n",name);

        // Move inside the LEC folder and run the LEC command
        printf("INFO: Moving inside the LEC folder\n");
        sprintf(name,"%s/%s/LEC",param->reports,param->moduleName);
        printf("DEBUG: %s\n",name);
        chdir(name);
        getcwd(current_path, 10000);
        printf("INFO: Current working directory: %s\n",current_path);
        
        // Run the LEC command
        sprintf(name,"lec -nogui -dofile %s_%d_lec.do > ./%s_%d_lec_dump.log",param->moduleName,param->number_of_puf,param->moduleName,param->number_of_puf);
        printf("INFO: Running LEC command : %s\n",name);
        system(name);        
        printf("INFO: LEC finished.\n");      

        //Open the verifiacation report       
        // system("pwd");
        // system("ls -l");
        printf("INFO: Opening the verification report : %s for the LEC\n",verificationFile);
        fptr2 = fopen(verificationFile,"r");

        if(fptr2 != NULL){
            printf("INFO: Verification report for the LEC : ");
            array_t* wordArr;
            while(fgets(name,10000,fptr2) != NULL){
                wordArr = breakInWords2(name);
                int flag = 0;

                for(i=0;i<array_n(wordArr);i++){
                    if(!strcmp(array_fetch(char*,wordArr,i),"6.")){
                        flag = 1;   
                        i = 3;                        
                    }
                    if(flag == 1){
                        printf("%s ",array_fetch(char*,wordArr,i));
                    }
                }

                if(flag == 1){
                    printf("\n");
                }
                

                array_free(wordArr);                  
            }
            fclose(fptr2);
        } else {
            printf("ERROR: Could not open the verification report for the LEC\n");
        }

    }

    free(name);

}
