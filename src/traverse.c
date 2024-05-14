//©Copyright 2020 University of Florida Research Foundation, Inc. All Rights Reserved.

/* traversing the graph for tracing the faulty region
- this part implements the partitioning */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include "array.h"
#include "graph.h"
#include <inttypes.h>

/* define percentage tolerance in delay value */
#define DELAYTOLERANCE 10
#define MAXVECTORS 10000
#ifndef TESTGEN //Jonathan
#define TESTGEN
#endif
/* global variables */
array_t *contributor;
extern int bDebug;
extern int actualDelay;
extern vertex *faultyCell;

/* forward declaration of the functions */
void propagateValues(graph *G, array_t *topoArr, char *vec);
void propagateOnlyNewValues(graph *G, array_t *topoArr, char *vec);
void DFSVisit(vertex *, array_t *, int *, int *, int, int *);
int calculateOutTransition(vertex *);
int calculateOutTransitionOnlyNewValues(vertex *);
void getValue(char *, array_t *, int *, int *);
void getValue_gsc(char *, char *, array_t *, int *, int *);
edge *findDataEdge(vertex *s);

void DFSVisit(vertex *u, array_t *topoArr, int *color,
              int *d, int time, int *f)
{
    int i, j;
    edge *edg;
    vertex *v;
    array_t *edges = u->outEdges;
    array_t *toVtxArr;

    color[u->idx] = 1;
    time = time + 1;
    d[u->idx] = time;

    for (i = 0; i < array_n(edges); i++)
    {
        edg = array_fetch(edge *, edges, i);
        toVtxArr = edg->toNodes;

        for (j = 0; j < array_n(toVtxArr); j++)
        {
            v = array_fetch(vertex *, toVtxArr, j);
            if (v && color[v->idx] == 0)
                DFSVisit(v, topoArr, color, d, time, f);
        }

        color[u->idx] = 2;
        time = time + 1;
        f[u->idx] = time;
        array_insert_last(vertex *, topoArr, u);
    }
}

int calculateOutTransition(vertex *s)
{
    int *oVal1 = (int *)malloc(sizeof(int));
    int *oVal2 = (int *)malloc(sizeof(int));
    int *nVal1 = (int *)malloc(sizeof(int));
    int *nVal2 = (int *)malloc(sizeof(int));
    array_t *inEdges = s->inEdges;
    array_t *outEdges = s->outEdges;
    edge *edg;
    int i, retval,j,k;
    char* port;
	//printf("\n Vertex : %s, Type : %s\n",s->name,s->type->name);
    array_t *oldInVal = array_alloc(int, 1);
    array_t *newInVal = array_alloc(int, 1);

    /* let's assume that there will be max 2 outputs per cell */
    /* if more, we can pass more oVal to the getValue function */

    for (i = 0; i < array_n(inEdges); i++)
    {
        edg = array_fetch(edge *, inEdges, i);
        
        array_insert_last(int, oldInVal, edg->oldVal);
        array_insert_last(int, newInVal, edg->newVal);
    }
    
    
     for (j = 0; j < array_n(s->outEdges); j++)
        {
            port = array_fetch(char*, s->outPorts, j);
            //printf("\n NAme : %s %s",s->name,port);
        }
    
    

    
    /* compute the value based on gate type */
    //getValue(s->type->name, oldInVal, oVal1, oVal2);
    //getValue(s->type->name, newInVal, nVal1, nVal2);

    /*
    if(!strcmp(s->name,"U2775"))
    {
        for(k=0;k<array_n(oldInVal);k++)
        {
            printf("\nDebug : Old - %d",array_fetch(int,oldInVal,k));
        }
        for(k=0;k<array_n(newInVal);k++)
        {
            printf("\nDebug : New - %d",array_fetch(int,newInVal,k));
        }
    }
    */

    getValue_gsc(s->type->name, port, oldInVal, oVal1, oVal2);
    getValue_gsc(s->type->name, port, newInVal, nVal1, nVal2);


    /* set the output values on output edges */
    if (array_n(outEdges) == 1)
    {
        edg = array_fetch(edge *, outEdges, 0);
        edg->oldVal = *oVal1;
        edg->newVal = *nVal1;
        array_insert_last(uint8_t, edg->valueArr, (uint8_t)edg->newVal);
    }
    else if (array_n(outEdges) == 2)
    {
        edg = array_fetch(edge *, outEdges, 0);
        edg->oldVal = *oVal1;
        edg->newVal = *nVal1;
        array_insert_last(uint8_t, edg->valueArr, (uint8_t)edg->newVal);
        edg = array_fetch_last(edge *, outEdges);
        edg->oldVal = *oVal2;
        edg->newVal = *nVal2;
        array_insert_last(uint8_t, edg->valueArr, (uint8_t)edg->newVal);
    }
    else
        printf("ERROR: out edges > 2 for %s %s in CalculateOutTransition.\n", s->type->name, s->name);

    if (*oVal1 != *nVal1 || *oVal2 != *nVal2)
        retval = 1;
    else
        retval = 0;

    array_free(oldInVal);
    array_free(newInVal);
    free(oVal1);
    free(oVal2);
    free(nVal1);
    free(nVal2);

    return retval;
}

// slightly different from the above, "just calculate the newVal"(save some work)
int calculateOutTransitionOnlyNewValues(vertex *s)
{
    //   int* oVal1 = (int*)malloc(sizeof(int));
    //   int* oVal2 = (int*)malloc(sizeof(int));
    int *nVal1 = (int *)malloc(sizeof(int));
    int *nVal2 = (int *)malloc(sizeof(int));
    array_t *inEdges = s->inEdges;
    array_t *outEdges = s->outEdges;
    edge *edg;
    int i, retval = 0;

    //   array_t* oldInVal = array_alloc(int, 1);
    array_t *newInVal = array_alloc(int, 1);

    /* let's assume that there will be max 2 outputs per cell */
    /* if more, we can pass more oVal to the getValue function */

    for (i = 0; i < array_n(inEdges); i++)
    {
        edg = array_fetch(edge *, inEdges, i);
        //      array_insert_last(int, oldInVal, edg->oldVal);
        array_insert_last(int, newInVal, edg->newVal);
    }

    /* compute the value based on gate type */
    //   getValue(s->type->name, oldInVal, oVal1, oVal2);
    getValue(s->type->name, newInVal, nVal1, nVal2);

    /* set the output values on output edges */
    if (array_n(outEdges) == 1)
    {
        edg = array_fetch(edge *, outEdges, 0);
        //       edg->oldVal = *oVal1;
        edg->newVal = *nVal1;
        if (edg->oldVal != edg->newVal)
            retval = 1;
    }
    else if (array_n(outEdges) == 2)
    {
        edg = array_fetch(edge *, outEdges, 0);
        //       edg->oldVal = *oVal1;
        edg->newVal = *nVal1;
        if (edg->oldVal != edg->newVal)
            retval = 1;
        edg = array_fetch_last(edge *, outEdges);
        //       edg->oldVal = *oVal2;
        edg->newVal = *nVal2;
        if (edg->oldVal != edg->newVal)
            retval = 1;
    }
    else
        printf("ERROR: out edges > 2 for %s in CalculateOutTransition.\n", s->type->name);

    /* if(edg->oldVal != *nVal1 || *oVal2 != *nVal2)
      retval = 1;
   else
      retval = 0;
*/

    //   array_free(oldInVal);
    array_free(newInVal);
    //   free(oVal1);
    //   free(oVal2);
    free(nVal1);
    free(nVal2);

    return retval;
}

int calculateErrOutTransition(vertex *s)
{
    int *oVal1 = (int *)malloc(sizeof(int));
    int *oVal2 = (int *)malloc(sizeof(int));
    int *nVal1 = (int *)malloc(sizeof(int));
    int *nVal2 = (int *)malloc(sizeof(int));
    array_t *inEdges = s->inEdges;
    array_t *outEdges = s->outEdges;
    edge *edg;
    int i, retval;

    array_t *oldInVal = array_alloc(int, 1);
    array_t *newInVal = array_alloc(int, 1);

    /* let's assume that there will be max 2 outputs per cell */
    /* if more, we can pass more oVal to the getValue function */
    for (i = 0; i < array_n(inEdges); i++)
    {
        edg = array_fetch(edge *, inEdges, i);
        array_insert_last(int, oldInVal, edg->errOldVal);
        array_insert_last(int, newInVal, edg->errNewVal);
    }

    /* compute the value based on gate type */
    getValue(s->type->name, oldInVal, oVal1, oVal2);
    getValue(s->type->name, newInVal, nVal1, nVal2);

    /* set the output values on output edges */
    if (array_n(outEdges) == 1)
    {
        edg = array_fetch(edge *, outEdges, 0);
        edg->errOldVal = *oVal1;
        edg->errNewVal = *nVal1;
    }
    else if (array_n(outEdges) == 2)
    {
        edg = array_fetch(edge *, outEdges, 0);
        edg->errOldVal = *oVal1;
        edg->errNewVal = *nVal1;
        edg = array_fetch_last(edge *, outEdges);
        edg->errOldVal = *oVal2;
        edg->errNewVal = *nVal2;
    }
    else
        printf("ERROR: out edges > 2 for %s in CalculateOutTransition.\n", s->type->name);

    if (*oVal1 != *nVal1 || *oVal2 != *nVal2)
        retval = 1;
    else
        retval = 0;

    array_free(oldInVal);
    array_free(newInVal);
    free(oVal1);
    free(oVal2);
    free(nVal1);
    free(nVal2);

    return retval;
}

void getValue(char *type, array_t *inVal, int *val1, int *val2)
{
    int tempVal, i, cnt;
    int x, y, z, w, tmp;
    int DIN[20], DIN1, DIN2, DIN3, DIN4, DIN5, DIN6, DIN7, DIN8;
    *val1 = -1;
    *val2 = -1;

    for (i = 0; i < 20; i++)
        DIN[i] = 0;
    for (i = 0; i < array_n(inVal); i++)
        DIN[i] = array_fetch(int, inVal, i);

    DIN1 = DIN[0];
    DIN2 = DIN[1];
    DIN3 = DIN[2];
    DIN4 = DIN[3];
    DIN5 = DIN[4];
    DIN6 = DIN[5];
    DIN7 = DIN[6];
    DIN8 = DIN[7];
    if (!strncasecmp(type, "nor", 3))
    {
        *val1 = 1;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 1)
                *val1 = 0;
        }
    }
    else if (!strncasecmp(type, "or", 2))
    {
        *val1 = 0;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 1)
                *val1 = 1;
        }
    }
    else if (!strncasecmp(type, "nnd", 3) || !strncasecmp(type, "nand", 4) || !strncasecmp(type, "NAND", 4))
    {
        *val1 = 0;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 0)
                *val1 = 1;
        }
    }
    else if (!strncasecmp(type, "AND", 3))
    {
        *val1 = 1;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 0)
                *val1 = 0;
        }
    }
    else if (!strncasecmp(type, "XOR", 3))
    {
        *val1 = 1;
        cnt = 0;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 1)
                cnt++;
        }
        if (cnt % 2 == 0)
            *val1 = 0;
    }
    else if (!strncasecmp(type, "AOI21X1", 7))
    {
	tempVal = (DIN1 & DIN2) | DIN3;	   //"(!((A B)+C))";
	*val1 = !tempVal;
    }
    else if (!strncasecmp(type, "AOI22X1", 7))
    {
	tempVal = ((DIN3 & DIN4) | (DIN1 & DIN2));	  //"(!((C D)+(A B)))";
	*val1 = !tempVal;
    }
    else if (!strncasecmp(type, "OAI21X1", 7))
    { 
	tempVal = (DIN1 | DIN2) & DIN3;	   //"(!((A+B) C))";
	*val1 = !tempVal;
    }
    /*else if (!strncasecmp(type, "AOI", 3))
    {
        cnt = 2;
        x = y = z = w = 0;
        x = (int)type[3];
        y = (int)type[4];
        if (type[5] != 's')
        {
            z = (int)type[5];
            cnt = 3;
        }
        if (type[6] != 's')
        {
            w = (int)type[6];
            cnt = 4;
        }
        tmp = 1;
        *val1 = 0;
        for (i = 0; i < x; i++)
            tmp = tmp & DIN[i];
        if (tmp)
            return;
        tmp = 1;
        for (i = x; i < x + y; i++)
            tmp = tmp & DIN[i];
        if (tmp)
            return;
        if (cnt >= 3)
        {
            tmp = 1;
            for (i = x + y; i < x + y + z; i++)
                tmp = tmp & DIN[i];
            if (tmp)
                return;
        }
        if (cnt >= 4)
        {
            tmp = 1;
            for (i = x + y + z; i < x + y + z + w; i++)
                tmp = tmp & DIN[i];
            if (tmp)
                return;
        }
        *val1 = 1;
    }
    else if (!strncasecmp(type, "OAI", 3))
    {
        cnt = 2;
        x = y = z = w = 0;
        x = (int)type[3];
        y = (int)type[4];
        if (type[5] != 's')
        {
            z = (int)type[5];
            cnt = 3;
        }
        if (type[6] != 's')
        {
            w = (int)type[6];
            cnt = 4;
        }
        tmp = 0;
        *val1 = 1;
        for (i = 0; i < x; i++)
            tmp = tmp | DIN[i];
        if (!tmp)
            return;
        tmp = 0;
        for (i = x; i < x + y; i++)
            tmp = tmp | DIN[i];
        if (!tmp)
            return;
        if (cnt >= 3)
        {
            tmp = 0;
            for (i = x + y; i < x + y + z; i++)
                tmp = tmp | DIN[i];
            if (!tmp)
                return;
        }
        if (cnt >= 4)
        {
            tmp = 0;
            for (i = x + y + z; i < x + y + z + w; i++)
                tmp = tmp | DIN[i];
            if (!tmp)
                return;
        }
        *val1 = 0;
    }
  */
    else if (!strncasecmp(type, "aoai1112", 8))
    {
        tempVal = (DIN1 & DIN2 & (DIN3 | (DIN4 & DIN5)));
        *val1 = !tempVal;
    }
    else if (!strncasecmp(type, "aoai122", 7))
    {
        tempVal = (DIN1 & ((DIN2 & DIN3) | (DIN4 & DIN5)));
        *val1 = !tempVal;
    }
    else if (!strncasecmp(type, "mx21", 4) || !strncasecmp(type, "mux2", 4))
    {
        *val1 = (DIN1 & ~DIN2) | (DIN3 & DIN2);
    }
    else if (!strncasecmp(type, "dsmx", 4))
    {
        *val1 = (DIN1 & ~DIN2) | (DIN3 & DIN2);
    }
    else if (!strncasecmp(type, "mxi21", 5))
    {
        tempVal = (DIN1 & ~DIN2) | (DIN3 & DIN2);
        *val1 = !tempVal;
    }
    else if (!strncasecmp(type, "mx41", 4) || !strncasecmp(type, "mux4", 4))
    {
        *val1 = ((DIN1 & ~DIN2 & ~DIN3) | (DIN4 & ~DIN3 & DIN2) | (DIN5 & DIN3 & ~DIN2) | (DIN6 & DIN2 & DIN3));
    }
    else if (!strncasecmp(type, "mxi41", 5))
    {
        tempVal = ((DIN1 & ~DIN2 & ~DIN3) | (DIN4 & ~DIN3 & DIN2) | (DIN5 & DIN3 & ~DIN2) | (DIN6 & DIN2 & DIN3));
        *val1 = !tempVal;
    }
    else if (!strncasecmp(type, "fax1", 5))
    {
        tempVal = (DIN1 ^ DIN2 ^ DIN3);
        *val1 = tempVal;
    }
    else if (!strncasecmp(type, "xnr", 3) || !strncasecmp(type, "xnor", 4))
    {
        *val1 = 1;
        cnt = 0;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 1)
                cnt++;
        }
        if (cnt % 2 == 1)
            *val1 = 0;
    }
    else if (!strncasecmp(type, "hi", 2) || !strncasecmp(type, "i1", 2) || !strncasecmp(type, "ib1", 3) || !strncasecmp(type, "inv", 3))
    {
        if (array_fetch_last(int, inVal) == 0)
            *val1 = 1;
        else
            *val1 = 0;
    }
    else if (!strncasecmp(type, "nb1", 3) || !strncasecmp(type, "BUFX1", 4) || !strncasecmp(type, "BUFX2", 3) || !strncasecmp(type, "DFFPOSX1", 8) || !strncasecmp(type, "DFFSR", 4))
    {
        if (array_fetch_last(int, inVal) == 0)
            *val1 = 0;
        else
            *val1 = 1;
    }
    else
        printf("ERROR: unknown type of cell: %s in getValue.\n", type);
}


void getValue_gsc(char *type, char* port, array_t *inVal, int *val1, int *val2)
{
    int tempVal, tempVal2, i, cnt;
    int x, y, z, w, tmp;
    int DIN[20], DIN1, DIN2, DIN3, DIN4, DIN5, DIN6, DIN7, DIN8;
    *val1 = -1;
    *val2 = -1;

    for (i = 0; i < 20; i++)
        DIN[i] = 0;
    for (i = 0; i < array_n(inVal); i++)
        DIN[i] = array_fetch(int, inVal, i);

    DIN1 = DIN[0];
    DIN2 = DIN[1];
    DIN3 = DIN[2];
    DIN4 = DIN[3];
    DIN5 = DIN[4];
    DIN6 = DIN[5];
    DIN7 = DIN[6];
    DIN8 = DIN[7];
    if (!strncasecmp(type, "NOR", 3))
    {
        *val1 = 1;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 1)
                *val1 = 0;
        }
    }
    else if (!strncasecmp(type, "OR", 2))
    {
        *val1 = 0;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 1)
                *val1 = 1;
        }
    }
    else if (!strncasecmp(type, "NND", 3) || !strncasecmp(type, "nand", 4) || !strncasecmp(type, "NAND", 4))
    {
        *val1 = 0;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 0)
                *val1 = 1;
        }
    }
    else if (!strncasecmp(type, "AND", 3))
    {
        *val1 = 1;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 0)
                *val1 = 0;
        }
    }
    else if (!strncasecmp(type, "XOR", 3))
    {
        *val1 = 1;
        cnt = 0;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 1)
                cnt++;
        }
        if (cnt % 2 == 0)
            *val1 = 0;
    }
    else if (!strncasecmp(type, "AOI21X1", 7))
    {
	tempVal = (DIN1 & DIN2) | DIN3;	   //"(!((A B)+C))";
	*val1 = !tempVal;
    }
    else if (!strncasecmp(type, "AOI22X1", 7))
    {
	tempVal = ((DIN3 & DIN4) | (DIN1 & DIN2));	  //"(!((C D)+(A B)))";
	*val1 = !tempVal;
    }
    else if (!strncasecmp(type, "OAI21X1", 7))
    { 
	tempVal = (DIN1 | DIN2) & DIN3;	   //"(!((A+B) C))";
	*val1 = !tempVal;
    }
    else if (!strncasecmp(type, "mx21", 4) || !strncasecmp(type, "mux2", 4))
    {
        //*val1 = (DIN1 & ~DIN2) | (DIN3 & DIN2);
        *val1 = !((!DIN3 & DIN2) | (DIN3 & DIN1));
    }
    else if (!strncasecmp(type, "FAX1", 4))
    {
        //tempVal = (DIN1 ^ DIN2 ^ DIN3);
        // tempVal = (((DIN1 & DIN2) | (DIN2 & DIN3)) | (DIN3 & DIN1));
        // *val1 = tempVal;
        // tempVal2 = ((DIN1 ^ DIN2) ^ DIN3);
        // *val2 = tempVal2;
        if(!strcmp(port,".YC"))
        {
            tempVal = (((DIN1 & DIN2) | (DIN2 & DIN3)) | (DIN3 & DIN1));   
        }
        else if(!strcmp(port,".YS"))
        {
            tempVal = ((DIN1 ^ DIN2) ^ DIN3);   
        }
        *val1 = tempVal;

    }
    else if (!strncasecmp(type, "HAX1", 4))
    {

        if(!strcmp(port,".YC"))
        {
            tempVal = (DIN1 & DIN2);  
        }
        else if(!strcmp(port,".YS"))
        {
             tempVal2 = (DIN1 ^ DIN2);  
        }
        *val1 = tempVal;
        //tempVal = (DIN1 ^ DIN2 ^ DIN3);
        // tempVal = (DIN1 & DIN2);
        // *val1 = tempVal;
        // tempVal2 = (DIN1 ^ DIN2);
        // *val2 = tempVal2;
    }
    else if (!strncasecmp(type, "INV", 3))
    {
        if (DIN1 == 0)
            *val1 = 1;
        else
            *val1 = 0;
    }
    else if (!strncasecmp(type, "xnr", 3) || !strncasecmp(type, "XNOR", 4))
    {
        *val1 = 1;
        cnt = 0;
        for (i = 0; i < array_n(inVal); i++)
        {
            if (array_fetch(int, inVal, i) == 1)
                cnt++;
        }
        if (cnt % 2 == 1)
            *val1 = 0;
    }
    else if (!strncasecmp(type, "BUFX", 4) || !strncasecmp(type, "DFFPOSX1", 8) || !strncasecmp(type, "DFFNEGX1", 8) || !strncasecmp(type, "DFFSR", 5) || !strncasecmp(type, "CLKBUF", 6) || !strncasecmp(type, "LATCH", 5))
    {
        if (array_fetch_last(int, inVal) == 0)
            *val1 = 0;
        else
            *val1 = 1;
    }
    else
	{
        printf("ERROR: unknown type of cell: %s in getValue.\n", type);
		exit(0);
	}
}

array_t *getTopoOrder(graph *G)
{
    int i, j, k, l, time = 0, topoCnt = 0, flag;
    vertex *v, *u, *s, *flagv;
    array_t *orgvList = G->vertexArr;
    array_t *toVtxArr;
    int nNodes = array_n(orgvList);

    int *d, *flagArr, *visited;
    int inpCnt = 0;
    array_t *sorted = array_alloc(vertex *, 1);
    array_t *unsorted = array_alloc(vertex *, 1);
    edge *edg;
    vertex *sVtx, *vtx;
    vertex *toVtx;
    vertex *outVtx;
    array_t *outEdges, *vList, *dffList, *dffNonScan, *dffScan;

    vList = G->combGates;

    dffList = G->seqGates;

    dffNonScan = G->nonscanDFF;
    dffScan = G->scanDFF;
    printf("vList: %d\n", array_n(vList));
    printf("dffScan: %d\n", array_n(dffScan));
    printf("dffNonScan: %d\n", array_n(dffNonScan));
    //vList = array_join(vList, dffList);
    printf("vList: %d\n", array_n(vList));
    nNodes = array_n(vList);
    d = (int *)malloc(sizeof(int) * nNodes);
    flagArr = (int *)malloc(sizeof(int) * nNodes);
    visited = (int *)malloc(sizeof(int) * nNodes);
    /* assign unique id to all vertices */
    for (i = 0; i < nNodes; i++)
    {
        visited[i] = FALSE;
        outVtx = array_fetch(vertex *, vList, i);
        outVtx->idx = i;
    }
    for (i = 0; i < array_n(dffList); i++)
    {
        outVtx = array_fetch(vertex *, dffList, i);
        outVtx->idx = i + array_n(vList);
    }

    /* topological sort of the vertices */
    for (i = 0; i < nNodes; i++)
    {
        outVtx = array_fetch(vertex *, vList, i);
        //printf("\nvertex:%s\t",outVtx->name);
        inpCnt = 0;
        for (j = 0; j < array_n(outVtx->inEdges); j++)
        {
            edg = array_fetch(edge *, outVtx->inEdges, j);
            //if (!strncmp(outVtx->name, "U13472", 6))
            //{
            //    printf("Check : %s\n", edg->fromNode->type->name);
            //}
            if (
                !(!strncmp(edg->name, "1'b0", 4)) &&
                !(!strncmp(edg->name, "1'b1", 4)) &&
                !(!strncmp(edg->name, "GND", 3)) &&
                !(!strncmp(edg->name, "VDD", 3)) &&
                !edg->bPrimIn &&                                                   //if primary do not increase count
                !(edg->fromNode && (strcasestr(edg->fromNode->type->name, "df"))) //if sdTracesFixedf do not increase count
            )
            {
                /*
                   if(strstr(edg->fromNode->type->name, "DFF")){                    //if dff inDegree = 1
                        inpCnt = 1;
                        break;
                   }
                   else     
                   */
                inpCnt++;
            }
            //else if (!strncmp(outVtx->name, "U13472", 6))
            //{
            //    printf("Inside : %s -- From Node %s\n", edg->name, edg->fromNode->name);
            //}
        }
        d[outVtx->idx] = inpCnt;
        //printf("### Dependency : %s\t%d nInputs: %d\n", outVtx->name, inpCnt, array_n(outVtx->inEdges));
        array_insert(vertex *, unsorted, outVtx->idx, outVtx);
    }
    for (i = 0; i < nNodes; i++)
    {
        outVtx = array_fetch(vertex *, vList, i);
        //printf("\nvertex:%s---%d", outVtx->name, d[outVtx->idx]);
    }

    for (i = 0; i < nNodes; i++)
    {
        for (j = 0; j < nNodes; j++)
            flagArr[j] = false;

        for (j = 0; j < nNodes; j++)
        {
            
            
            if (d[j] == 0 && visited[j] == FALSE)
            {
                visited[j] = TRUE;
                sVtx = array_fetch(vertex *, unsorted, j);
                flagArr[j] = true;
            }
            

        }

        for (l = 0; l < nNodes; l++)
        {
            vertex *tempV = array_fetch(vertex *, vList, l); 
            if (!flagArr[l])
                continue;                              //skip if false
            sVtx = array_fetch(vertex *, unsorted, l); //grab vertex
            array_insert_last(vertex *, sorted, sVtx);
            sVtx->topoIdx = ++topoCnt;
            outEdges = sVtx->outEdges;
            //if(strstr(sVtx->name, "U7975"))
            //{
                //printf("Sorted : %s\n", sVtx->name);
            //}
            for (j = 0; j < array_n(outEdges); j++)
            {
                edg = array_fetch(edge *, outEdges, j);
                //printf("edge: %s\n",edg->name);
                toVtxArr = edg->toNodes;
                for (k = 0; k < array_n(toVtxArr); k++)
                {
                    toVtx = array_fetch(vertex *, toVtxArr, k);
                    //printf("%s\t %s\t %d\n",toVtx->name,toVtx->type->name,d[toVtx->idx]);
                     //if (strcasestr(toVtx->type->name, "sdTracesFixedf"))
                    if (strcasestr(toVtx->type->name, "df"))
                        continue;
                    //else if (strstr(toVtx->type->name, "dff") )
                    //d[toVtx->idx] = 0;
                    // else if (d[toVtx->idx] > 0)
                    // {
                        d[toVtx->idx]--;
                        //printf("\t%s\t %s\t %d\n",toVtx->name,toVtx->type->name,d[toVtx->idx]);
                        
                        //printf("%s\t %d\n",toVtx->name,d[toVtx->idx]);
                    // }
                }
            }
        }
        for (l = 0; l < nNodes; l++)
        {
            vertex *tempV = array_fetch(vertex *, vList, l); 
            //printf("\nIter: %d vertex:%s---%d", i, tempV->name, d[tempV->idx]);
        }

    }

    flagv = NULL;
    for (i = 0; i < array_n(vList); i++)
    {
        s = array_fetch(vertex *, vList, i);
        //printf("%s\t%d",s->name,d[s->idx]);
        if (d[s->idx] > 0)
        {
            flagv = s;
            printf("\nERROR: Violation of topological order for vertex %s. Number of untraversed input edges: %d\n", flagv->name, d[flagv->idx]);
            isCombLoop = true;
        }
    }
    //if (flagv)
    //printf("\nERROR: Violation of topological order for vertex %s. Number of untraversed input edges: %d\n", flagv->name, d[flagv->idx]);
    //for(i=0;i<array_n(sorted);i++)
    //{
    //    toVtx=array_fetch(vertex*, sorted, i);
    //    printf("%d: %s\n",i,toVtx->name);
    //}
    //if(d)free(d);
    free(visited);
    //free(flagArr);
    array_free(unsorted);
    return sorted;
}

/*Order the sequential elements based on topo order of combinational. To be performed after topological sort of comb.
*/
array_t *orderSequential(graph *G)
{
    int i, j, k, sortFlag, maxTries, count;
    vertex *v, *s;
    edge *edg;
    array_t *unsorted = array_alloc(vertex *, 1);
    array_t *sorted = array_alloc(vertex *, 1);

    for(i=0;i<array_n(G->seqGates);i++)
    {
        v = array_fetch(vertex*, G->seqGates,i);
        edg = findDataEdge(v);
        //insert only to ff's with combinational input
   // printf("inserted %s:%d into sorted\n",v->name,v->topoIdx);
        if(edg->fromNode == NULL)
        {
            v->topoIdx = 0;
            array_insert_last(vertex*, sorted, v);
            continue;
        }
    }
    for(i=0;i<array_n(G->seqGates);i++)
    {
        v = array_fetch(vertex*, G->seqGates,i);
        edg = findDataEdge(v);
        //insert only to ff's with combinational input
   // printf("inserted %s:%d into sorted\n",v->name,v->topoIdx);
        if(edg->fromNode == NULL)
        {
            continue;
        }
        if(! (strcasestr(edg->fromNode->type->name, "dff"))){

            //printf("inserted %s:%d into sorted\n",v->name, v->topoIdx);
            array_insert_last(vertex*, sorted, v);
            v->topoIdx = edg->fromNode->topoIdx; //update topoIdx of sequential with its combinational idx
        }
        else{
            //printf("inserted %s:%d into unsorted\n",v->name, v->topoIdx);
            array_insert_last(vertex*, unsorted, v);
        }
    }

    /*for (i = 0; i < array_n(G->seqGates); i++)
    {
        v = array_fetch(vertex *, G->seqGates, i);
        edg = findDataEdge(v);
        //insert only to ff's with combinational input
        if (!(edg->fromNode && (strcasestr(edg->fromNode->type->name, "dff"))))
        {
            printf("inserted %s:%d into sorted\n",v->name,v->topoIdx);
            array_insert_last(vertex *, sorted, v);
            v->topoIdx = edg->fromNode->topoIdx; //update topoIdx of sequential with its combinational idx
        }
        else
        {
            printf("inserted %s:%d into unsorted\n",v->name, v->topoIdx);
            array_insert_last(vertex *, unsorted, v);
        }
    }*/
    sortFlag = 1;
    maxTries = array_n(G->seqGates) * array_n(G->seqGates);
    count = 0; //just in case infinite loop
    while (sortFlag && count < maxTries)
    {
        sortFlag = 0;
        for (i = 0; i < array_n(unsorted); i++)
        {
            v = array_fetch(vertex *, unsorted, i);
            edg = findDataEdge(v);
            //insert only to ff's with combinational input
            if (edg->fromNode->topoIdx >= 0 && v->topoIdx == 0)
            {
                array_insert_last(vertex *, sorted, v);
                v->topoIdx = edg->fromNode->topoIdx + 1; //update topoIdx of sequential with its combinational idx
            }
            else
                sortFlag = 1;
        }
        count++;
    }
    array_free(unsorted);
    if (array_n(G->seqGates) < 1)
    {
        printf("No sequential gates to sort\n");
    }
    else if (count !=1 && count >= maxTries)
    {
        printf("ERROR: Unable to properly sort sequential elements\n");
        return G->seqGates; //return original ordering...
    }

    return sorted;
}


void createTestBench(graph *G, array_t *topoArr, char *vectorFile)
{
    array_t *primInEdges = G->primInEdges;
    array_t *primOutEdges = G->primOutEdges;
    int i,j;
    char* test_bench_file_name;
    FILE* test_bench;
    edge* q;
    vertex *s;
    edge* temp;
    char* edg;
    int f = 0;

    test_bench_file_name = (char*) malloc(sizeof(char)*120);
    sprintf(test_bench_file_name, "../testbench/tb_%s.v", G->header);

    test_bench = fopen(test_bench_file_name, "w");

    array_t* inputs = array_alloc(char*, 1);
    array_t* outputs = array_alloc(char*, 1);
    array_t* internal = array_alloc(char*, 1);

    fprintf(test_bench, "module tb_%s();\n",G->header);

    
    for (i = 0; i < array_n(primInEdges); i++)
    {
        q = array_fetch(edge *, primInEdges, i);
        array_insert_last(char*,inputs,q->name);
    }

    for (i = 0; i < array_n(primOutEdges); i++)
    {
        q = array_fetch(edge *, primOutEdges, i);
        array_insert_last(char*,outputs,q->name);
    }

    fprintf(test_bench, "reg ");

    for (i = 0; i < array_n(inputs); i++)
    {
        if(i == array_n(inputs)-1)
            fprintf(test_bench, "%s;\n",array_fetch(char*,inputs,i));
        else
            fprintf(test_bench, "%s, ",array_fetch(char*,inputs,i));
    }


    fprintf(test_bench, "wire ");

    for (i = 0; i < array_n(outputs); i++)
    {
        if(i == array_n(outputs)-1)
            fprintf(test_bench, "%s;\n",array_fetch(char*,outputs,i));
        else
            fprintf(test_bench, "%s, ",array_fetch(char*,outputs,i));
    }

    fprintf(test_bench, "reg clk;\n");
    fprintf(test_bench, "integer key_file_1,key_file_2,scan_keys;\n");
    

    fprintf(test_bench, "%s %s_inst(",G->header,G->header);

    for (i = 0; i < array_n(inputs); i++)
    {
        fprintf(test_bench, ".%s(%s),",array_fetch(char*,inputs,i),array_fetch(char*,inputs,i));
    }

    for (i = 0; i < array_n(outputs); i++)
    {
        if(i == array_n(outputs)-1)
            fprintf(test_bench, ".%s(%s));\n",array_fetch(char*,outputs,i),array_fetch(char*,outputs,i));
        else
            fprintf(test_bench, ".%s(%s),",array_fetch(char*,outputs,i),array_fetch(char*,outputs,i));
    }

    fprintf(test_bench, "always #5 clk = !clk;\n");

    fprintf(test_bench, "initial begin \n");
    fprintf(test_bench, "\tkey_file_1 = $fopen(\"");
    fprintf(test_bench,"%s\", \"r\");\n",vectorFile);
    fprintf(test_bench, "\tkey_file_2 = $fopen(\"output_%s.log\", \"w\");\n",G->header);
    fprintf(test_bench, "\tif(!key_file_1 || !key_file_2)\n\t\tbegin\n");
    fprintf(test_bench, "\t\t\t$display(\"ERROR: Key file could not be opened . Exiting Simulation.\");\n\t\t\t$finish;\n\t\tend\n\t\tclk <= 0;\nend\n");
    
    fprintf(test_bench, "initial begin \n");
    fprintf(test_bench, "\t@(posedge clk);\n");
    fprintf(test_bench, "\twhile(!$feof(key_file_1))\n\tbegin\n");
    fprintf(test_bench, "\t\tscan_keys = $fscanf(key_file_1,\"%b\\n\",{");
    
    for (i = 0; i < array_n(inputs); i++)
    {
        if(i == array_n(inputs)-1)
            fprintf(test_bench, "%s});\n",array_fetch(char*,inputs,i));
        else
            fprintf(test_bench, "%s, ",array_fetch(char*,inputs,i));
    }

    fprintf(test_bench, "\t@(posedge clk);\n");

    fprintf(test_bench, "\t\t$fwrite(key_file_2,\"");

    for (i = 0; i < array_n(topoArr); i++)
    {
       s = array_fetch(vertex *, topoArr, i);
       f=0;
       if (array_n(s->outEdges))
        {
            q = array_fetch(edge *, s->outEdges, 0);
            //temp = findEdge(q->name,outputs);
            //if(!temp)
                //printf("\ntest : %s",q->name);
            for(j = 0; j < array_n(outputs); j++)
            {
                edg = array_fetch(char*, outputs, j);
                if(strcmp(edg, q->name) == 0)
                {
                    f=1;
                    break;
                }
                    
            }
            if(f==0)
            {
                //printf("\ntest : %s",q->name);
                array_insert_last(char*,internal,q->name);
            }
        }
    }


    for (i = 0; i < array_n(internal); i++)
    {
        if(i == array_n(internal)-1)
            fprintf(test_bench, "%s:%b \\n\",",array_fetch(char*,internal,i));
        else
            fprintf(test_bench, "%s:%b ",array_fetch(char*,internal,i));
    }

    for (i = 0; i < array_n(internal); i++)
    {
        if(i == array_n(internal)-1)
            fprintf(test_bench, "%s_inst.%s);\n",G->header,array_fetch(char*,internal,i));
        else
            fprintf(test_bench, "%s_inst.%s,",G->header,array_fetch(char*,internal,i));
    }

    fprintf(test_bench,"\tend\n");
    fprintf(test_bench,"\t$fclose(key_file_1);\n\t$fclose(key_file_2);\n\t$finish;\n");
    fprintf(test_bench,"end\n");
    fprintf(test_bench,"endmodule");

    fclose(test_bench);

}


//read the test patterns and simulate
void calculateTVLA(graph *G, array_t *topoArr, char *inputFile, char *vectorFile, int rstEdgeId, int traceCount)
{
    int i, j, k, l, m, z, outTrans, flag = 0, ncnt = 0, onecnt = 0, zerocnt = 0, resetflag = 0;
    int seed = 15, b1, b2, sens, gcnt = 0, nodeFlag = 0;
    long nPatterns = 0;
    vertex *s, *v;
    char *str1, *port;
    edge *q, *edg1, *edg2, *edg, *clkedg, *edg3;
    char str[1200], *vec1;
    FILE *fvec, *fout, *yptr, *fnodes;
    float activity_store = 0.0, prob;
    array_t *primInEdges = G->primInEdges;
    array_t *primOutEdges = G->primOutEdges;
    char *type;

    // These Variable delarations are used to calculate Node wise TVLA
    
    FILE *final_file_tvla;
    
    FILE *dump_values;

    char* dump_file_name = (char*) malloc(sizeof(char)*120);

    sprintf(dump_file_name, "../cast_output/cast_output_%s.txt", G->header);

    dump_values = fopen(dump_file_name,"w");
    char* tvla_values_file;
    int row = 0, vec_count = 0;
    int vecsize;
    int ii, jj, kk, kk1, jj1;
    vertex *ss;
    double col = 0.0;
    int flagR = 0;
    double loading_cap = 0.0, total_loading_cap = 0.0;
    //int node_level[array_n(topoArr)];
    pins *tp;
    vertex *vi;
    edge *ei;
    edge* r;
    int found = 0;
    
    //double NodePowerMeanRandom [array_n(topoArr)+1];
    //double NodePowerMeanFixed [array_n(topoArr)+1];
    double delta=0.0;
    //double NodePowerCentralSumOrder2Random [array_n(topoArr)+1];
    //double NodePowerCentralSumOrder2Fixed [array_n(topoArr)+1];
    //double IncrementalNnodeTvla[array_n(topoArr)+1];

    int *node_level = (int *)malloc(sizeof(int) * (array_n(topoArr)));
    double *NodePowerMeanRandom = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));
    double *NodePowerMeanFixed = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));
    double *NodePowerCentralSumOrder2Random = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));
    double *NodePowerCentralSumOrder2Fixed = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));
    double *IncrementalNnodeTvla = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));

    for (i = 0; i <= array_n(topoArr); i++)
    {
        NodePowerMeanRandom[i] = 0.0;
        NodePowerMeanFixed[i] = 0.0;
        NodePowerCentralSumOrder2Random[i] = 0.0;
        NodePowerCentralSumOrder2Fixed[i] = 0.0;
        IncrementalNnodeTvla[i] = 0.0;
    }
	printf("\nfile name : %s",vectorFile);
    // End of TVLA calculation related variable declaration //
    
    fvec = fopen(vectorFile, "r");
    

    array_t *orderedSeqGates = orderSequential(G);
    printf("\nAnalyzing internal node activities for random vectors from %s ...\n", vectorFile);

    vecsize = array_n(G->primInEdges) + array_n(G->seqGates);


    printf("\nNew file nodes.txt is being created with the name of the rare edges ...\n");

    vec1 = (char *)malloc(sizeof(char) * (vecsize + 1));
    for (i = 0; i < vecsize + 1; i++)
        vec1[i] = '0';

    for (i = 0; i < array_n(G->edgArr); i++)
    {
        q = array_fetch(edge *, G->edgArr, i);
        q->oldVal = q->newVal = 0;
    }
   
    printf("\nChecking topoArr ...\n");
   
    printf("\nChecking seqGates ...\n");
    for (i = 0; i < array_n(G->seqGates); i++)
    {
        s = array_fetch(vertex *, G->seqGates, i);
        //initial assignment of sequential gates
        for (j = 0; j < array_n(s->outEdges); j++)
        {
            edg2 = array_fetch(edge *, s->outEdges, j);
            port = array_fetch(char *, s->outPorts, j);
            if (!strcmp(port, ".Q"))
            {
                edg2->newVal = 0;
                edg2->oldVal = 0;
            }
            else if (!strcmp(port, ".QN"))
            {
                edg2->newVal = 1;
                edg2->oldVal = 1;
            }
        }
    }
    
    printf("\nAttempting to apply test patterns ...\n");
    
    while (!feof(fvec))
    {
        fscanf(fvec, "%s", vec1);
        //printf("\nStimulus Used : %s",vec1);
        if (feof(fvec))
            break;
        resetflag = 0;
        total_loading_cap = 0;
        for (i = 0; i < array_n(primInEdges); i++)
        {
            q = array_fetch(edge *, primInEdges, i);
            q->oldVal = q->newVal;
            q->newVal = (vec1[i] == '0' ? 0 : 1);
            array_insert_last(uint8_t, q->valueArr, (uint8_t)q->newVal);

            // This part of the code calculates loading capacitances of every primary inputs for each vector
            loading_cap = 0.0;
            if (array_n(q->toNodes))
            {
                for (jj1 = 0; jj1 < array_n(q->toNodes); jj1++)
                {
                    vi = array_fetch(vertex *, q->toNodes, jj1);

                    for (kk1 = 0; kk1 < array_n(vi->inEdges); ++kk1)
                    {
                        ei = array_fetch(edge *, vi->inEdges, kk1);
                        if (!(strcmp(ei->name, q->name)))
                        {
                            tp = array_fetch(pins *, (vi->type)->pinvals, kk1);
                            if (q->oldVal == 0 && q->newVal == 1)
                                loading_cap += tp->rise_capacitance;
                            else if (q->oldVal == 1 && q->newVal == 0)
                                loading_cap += tp->fall_capacitance;
                        }
                    }
                }
                total_loading_cap += loading_cap;
            }

            // End of loading capacitance calculation for every primary input
        }

        if (fullScan)
        {
            for (i = 0; i < array_n(G->seqGates); i++)
            {
                s = array_fetch(vertex *, G->seqGates, i);
                for (j = 0; j < array_n(s->outEdges); j++)
                {
                    edg2 = array_fetch(edge *, s->outEdges, j);
                    port = array_fetch(char *, s->outPorts, j);
                    for (m = 0; m < array_n(s->inEdges); m++)
                    {
                        edg3 = array_fetch(edge *, s->inEdges, m);

                        if (edg3->idx == rstEdgeId && vec1[edg3->idx] == 1)
                        {
                            resetflag = 1;
                        }
                    }
                    if (resetflag != 1)
                    {
                        if (!strcmp(port, ".Q"))
                            edg2->newVal = (vec1[array_n(primInEdges) + i] == '0' ? 0 : 1);
                        else if (!strcmp(port, ".QN"))
                            edg2->newVal = (vec1[array_n(primInEdges) + i] == '0' ? 1 : 0);
                    }
                    else if (resetflag == 1)
                    {
                        if (!strcmp(port, ".Q"))
                            edg2->newVal = 0; // (vec1[array_n(primInEdges) + i] == '0' ? 0 : 1);
                        else if (!strcmp(port, ".QN"))
                            edg2->newVal = 1; //(vec1[array_n(primInEdges) + i] == '0' ? 1 : 0);
                    }
                }
            }
        }

        for (i = 0; i < array_n(topoArr); i++)
        {
            s = array_fetch(vertex *, topoArr, i);
            outTrans = calculateOutTransition(s);
        }

        for (i = 0; i < array_n(topoArr); i++)
        {
            s = array_fetch(vertex *, topoArr, i);
            found = 0;
            if (array_n(s->outEdges))
            {
                q = array_fetch(edge *, s->outEdges, 0);
                //printf("Edge Name : %s : Old : %d New : %d \n", q->name, q->oldVal,q->newVal);	
		        
                for (z = 0; z < array_n(primOutEdges); z++)
                {
                    r = array_fetch(edge *, primOutEdges, z);
                    if(strcmp(q->name,r->name)==0)
                    {
                        found = 1;
                        break;
                    }
                }
                if(found == 0)
                    fprintf(dump_values, "%s:%d ", q->name, q->newVal);

                // This part of the code calculates loading capacitances of every internal nodes for each vector
                loading_cap = 0.0;
                node_level[i] = s->level;
                if (array_n(q->toNodes))
                {
                    loading_cap = 0.0;
                    for (jj1 = 0; jj1 < array_n(q->toNodes); jj1++)
                    {
                        vi = array_fetch(vertex *, q->toNodes, jj1);
                        for (kk1 = 0; kk1 < array_n(vi->inEdges); ++kk1)
                        {
                            ei = array_fetch(edge *, vi->inEdges, kk1);
                            if (!(strcmp(ei->name, q->name)))
                            {

                                tp = array_fetch(pins *, (vi->type)->pinvals, kk1);
                                if (q->oldVal == 0 && q->newVal == 1)
                                    loading_cap += tp->rise_capacitance;
                                else if (q->oldVal == 1 && q->newVal == 0)
                                    loading_cap += tp->fall_capacitance;

                            }
                        }
                    }
                    total_loading_cap += loading_cap;
                }

                // This part of the code updates trace array (random and fixed) for every vector and nodes
                if (flagR)
                {
                    
		            if ((q->oldVal == 0 && q->newVal == 1) || (q->oldVal == 1 && q->newVal == 0))
                    {
                        delta = loading_cap - NodePowerMeanRandom[i];
                        NodePowerMeanRandom[i] = NodePowerMeanRandom[i] + ((double)(delta) / (double)(col + 1));
                        NodePowerCentralSumOrder2Random[i] = NodePowerCentralSumOrder2Random[i] + (double)(delta * delta) * ((double)col / (double)(col + 1));
                        if (NodePowerCentralSumOrder2Random[i] == 0.0 && NodePowerCentralSumOrder2Fixed[i] == 0.0) 
                            IncrementalNnodeTvla[i] = 0.0;
                        else
                        {
                            IncrementalNnodeTvla[i] = ((NodePowerMeanRandom[i] - NodePowerMeanFixed[i])) / (sqrt((NodePowerCentralSumOrder2Random[i] / ((col + 1) * (col + 1))) + (NodePowerCentralSumOrder2Fixed[i] / ((col + 1) * (col + 1)))));
                            IncrementalNnodeTvla[i] = fabs(IncrementalNnodeTvla[i]); 
                        }
                    }
                    else
                    {
                        delta = 0.0 - NodePowerMeanRandom[i];
                        NodePowerMeanRandom[i] = NodePowerMeanRandom[i] + ((double)(delta) / (double)(col + 1));
                        NodePowerCentralSumOrder2Random[i] = NodePowerCentralSumOrder2Random[i] + (double)(delta * delta) * ((double)col / (double)(col + 1));
                        if (NodePowerCentralSumOrder2Random[i] == 0.0 && NodePowerCentralSumOrder2Fixed[i] == 0.0) 
                            IncrementalNnodeTvla[i] = 0.0;
                        else
                        {
                            IncrementalNnodeTvla[i] = ((NodePowerMeanRandom[i] - NodePowerMeanFixed[i])) / (sqrt((NodePowerCentralSumOrder2Random[i] / ((col + 1) * (col + 1))) + (NodePowerCentralSumOrder2Fixed[i] / ((col + 1) * (col + 1)))));
                            IncrementalNnodeTvla[i] = fabs(IncrementalNnodeTvla[i]); 
                        }
                    }
                }
                else
                {
                    if ((q->oldVal == 0 && q->newVal == 1) || (q->oldVal == 1 && q->newVal == 0))
                    {
                        delta = loading_cap - NodePowerMeanFixed[i];
                        NodePowerMeanFixed[i] = NodePowerMeanFixed[i] + ((double)(delta) / (double)(col + 1));
			            NodePowerCentralSumOrder2Fixed[i] = NodePowerCentralSumOrder2Fixed[i] + (double)(delta * delta) * ((double)col / (double)(col + 1));
	                }
                    else
                    {
                        delta = 0.0 - NodePowerMeanFixed[i];
			            NodePowerMeanFixed[i] = NodePowerMeanFixed[i] + ((double)(delta) / (double)(col + 1));
			            NodePowerCentralSumOrder2Fixed[i] = NodePowerCentralSumOrder2Fixed[i] + (double)(delta * delta) * ((double)col / (double)(col + 1));
			        }
                }

                // End of node-wise trace array updates
            }
        }
	    fprintf(dump_values, "\n");

        for (i = 0; i < array_n(orderedSeqGates); i++)
        {
            s = array_fetch(vertex *, orderedSeqGates, i);
            edg1 = findDataEdge(s);
            if(edg1->fromNode == NULL)
                continue;
            for (j = 0; j < array_n(s->outEdges); j++)
            {

                edg2 = array_fetch(edge *, s->outEdges, j);
                edg2->oldVal = edg2->newVal;
                array_insert_last(uint8_t, edg2->valueArr, (uint8_t)edg2->oldVal); //store current state (oldVal)
                //if (DUMPALL && edg2->assnFlag)
                //    fprintf(fout, "%d   ", edg2->newVal);
                port = array_fetch(char *, s->outPorts, j);
                //printf(".%s(%s):%d->",port,edg2->name, edg2->newVal);
                if (!strcmp(port, ".Q"))
                {
                    if (!edg1->fromNode->isSeq) //if predecessor is sequential, take old value, else take newly calculated comb value
                        edg2->newVal = edg1->newVal;
                    else
                        edg2->newVal = edg1->oldVal;
                }
                else if (!strcmp(port, ".QN"))
                {
                    if (!edg1->fromNode->isSeq) //if predecessor is sequential, take old value, else take newly calculated comb value
                        edg2->newVal = (edg1->newVal ? 0 : 1);
                    else
                        edg2->newVal = (edg1->oldVal ? 0 : 1);
                }
                //printf("%d\n",edg2->newVal); 
		//printf("\n--- Seq Name : %s Old : %d New : %d\n",edg2->name, edg2->oldVal, edg2->newVal);

	    }

        }
        
	/*
        if (flagR)
        {
            delta = total_loading_cap - NodePowerMeanRandom[array_n(topoArr)];
	    //printf("\nRand Delta : %lf === %lf\n",delta,total_loading_cap);
            NodePowerMeanRandom[array_n(topoArr)] = NodePowerMeanRandom[array_n(topoArr)] + ((double)(delta) / (double)(col + 1));
	    printf("\nOverall - NodePowerMeanRandom : %lf",NodePowerMeanRandom[array_n(topoArr)]);
            NodePowerCentralSumOrder2Random[array_n(topoArr)] = NodePowerCentralSumOrder2Random[array_n(topoArr)] + (delta * delta) * ((double)col / (double)(col + 1));
	    printf("\nOverall - NodePowerCentralSumOrder2Random : %lf",NodePowerCentralSumOrder2Random[array_n(topoArr)]);
            IncrementalNnodeTvla[array_n(topoArr)] = (NodePowerMeanRandom[array_n(topoArr)] - NodePowerMeanFixed[array_n(topoArr)]) / sqrt((NodePowerCentralSumOrder2Random[array_n(topoArr)] / ((col + 1) * (col + 1))) + (NodePowerCentralSumOrder2Fixed[array_n(topoArr)] / ((col + 1) * (col + 1))));
            IncrementalNnodeTvla[array_n(topoArr)] = fabs(IncrementalNnodeTvla[array_n(topoArr)]);
	    printf("\nOverall - IncrementalNnodeTvla : %lf",IncrementalNnodeTvla[array_n(topoArr)]);
        }
        else
        {
            delta = total_loading_cap - NodePowerMeanFixed[array_n(topoArr)];
	    //printf("\nFix Delta : %lf === %lf\n",delta,total_loading_cap);
            NodePowerMeanFixed[array_n(topoArr)] = NodePowerMeanFixed[array_n(topoArr)] + ((double)(delta) / (double)(col + 1));
	    printf("\nOverall - NodePowerMeanFixed : %lf",NodePowerMeanFixed[array_n(topoArr)]);
            NodePowerCentralSumOrder2Fixed[array_n(topoArr)] = NodePowerCentralSumOrder2Fixed[array_n(topoArr)] + (delta * delta) * ((double)col / (double)(col + 1));
	    printf("\nOverall - NodePowerCentralSumOrder2Fixed : %lf",NodePowerCentralSumOrder2Fixed[array_n(topoArr)]);
        }*/

	

        flagR = !flagR;
        vec_count += 1;
        if (vec_count > 0 && (vec_count % 2) == 0)
            col = col + 1;

        nPatterns++;
    }
    
    for(i=0;i<array_n(topoArr);i++)
    {
	    IncrementalNnodeTvla[array_n(topoArr)] += IncrementalNnodeTvla[i];
    }

    // This part of the code evaluates mean, sd and TVLA values of evry internal node and overall design

    int max_tvla = 0, max_leak = 0;
    tvla_values_file = (char*) malloc(sizeof(char)*120);
    sprintf(tvla_values_file, "../results_updated/tvla_%s.csv", G->header);

    final_file_tvla = fopen(tvla_values_file, "w");

    int cc = 0;

    vertex *vv;
    array_t *vvarr;
    edge *ee;

    // printf("\n\n%-10s%-10s%-80s\n", "Nodename", "TVLA", "Level");
    fprintf(final_file_tvla, "NetName,DrivingNode,TVLA\n");
    for (ii = 0; ii < array_n(topoArr); ii++)
    {
        ss = array_fetch(vertex *, topoArr, ii);
        // printf("%-10s%-10.6lf%-80d\n", array_fetch(edge *, ss->outEdges, 0)->name, IncrementalNnodeTvla[ii], node_level[ii]);

        char result_str[500];
        ee = array_fetch(edge *, ss->outEdges, 0);
        sprintf(result_str, "%s,%s", ee->name, ss->name);
        vvarr = ee->toNodes;
        fprintf(final_file_tvla, "%s,%lf\n", result_str, IncrementalNnodeTvla[ii]);
        
    }

    // printf("OVERALL=%lf\n", IncrementalNnodeTvla[array_n(topoArr)]);
    fprintf(final_file_tvla, "\nOverall = %lf", IncrementalNnodeTvla[array_n(topoArr)]);

    fclose(final_file_tvla);

    free(node_level);
    free(NodePowerMeanRandom);
    free(NodePowerMeanFixed);
    free(NodePowerCentralSumOrder2Random);
    free(NodePowerCentralSumOrder2Fixed);
    free(IncrementalNnodeTvla);
	fclose(dump_values);

    // End of all TVLA related calculation

    
}


void generateStat(graph *G, array_t *topoArr, char *inputFile, char *vectorFile, int rstEdgeId, int extStimulus, char *extStimulusFileName, int traceCount)
{
    int i, j, k, l, m, z, outTrans, flag = 0, ncnt = 0, onecnt = 0, zerocnt = 0, resetflag = 0;
    int seed = 15, b1, b2, sens, gcnt = 0, nodeFlag = 0;
    long nPatterns = 0, activityCnt = 0, vectorActivityCnt = 0, vecsize;
    vertex *s, *v;
    char *str1, *port;
    edge *q, *edg1, *edg2, *edg, *clkedg, *edg3;
    char str[1200], *vec1;
    FILE *fvec, *fout, *yptr, *fnodes;
    float activity_store = 0.0, prob;
    array_t *primInEdges = G->primInEdges;
    char *type;

    // These Variable delarations are used to calculate Node wise TVLA
    FILE *tvlafile, *tvlafile1, *tvlafile2;
    FILE *final_file_tvla;
    char* tvla_values_file;
    int row = 0, vec_count = 0;
    int ii, jj, kk, kk1, jj1;
    vertex *ss;
    double col = 0.0;
    int flagR = 0;
    float loading_cap = 0.0, total_loading_cap = 0.0;
    int node_level[array_n(topoArr)];
    pins *tp;
    vertex *vi;
    edge *ei;
    //double NodePowerMeanRandom [array_n(topoArr)+1];
    //double NodePowerMeanFixed [array_n(topoArr)+1];
    double delta=0.0;
    //double NodePowerCentralSumOrder2Random [array_n(topoArr)+1];
    //double NodePowerCentralSumOrder2Fixed [array_n(topoArr)+1];
    //double IncrementalNnodeTvla[array_n(topoArr)+1];

    double *NodePowerMeanRandom = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));
    double *NodePowerMeanFixed = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));
    double *NodePowerCentralSumOrder2Random = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));
    double *NodePowerCentralSumOrder2Fixed = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));
    double *IncrementalNnodeTvla = (double *)malloc(sizeof(double) * (array_n(topoArr) + 1));

    for (i = 0; i <= array_n(topoArr); i++)
    {
        NodePowerMeanRandom[i] = 0.0;
        NodePowerMeanFixed[i] = 0.0;
        NodePowerCentralSumOrder2Random[i] = 0.0;
        NodePowerCentralSumOrder2Fixed[i] = 0.0;
        IncrementalNnodeTvla[i] = 0.0;
    }

    // End of TVLA calculation related variable declaration //
    if (extStimulus)
    {
        fvec = fopen(extStimulusFileName, "r");
    }
    else
    {
        fvec = fopen(vectorFile, "r");
    }

    array_t *orderedSeqGates = orderSequential(G);
    printf("\nAnalyzing internal node activities for random vectors from %s ...\n", vectorFile);

    vecsize = array_n(G->primInEdges) + array_n(G->seqGates);
	
    //fout = fopen("report.txt", "w");
    //fnodes = fopen("nodes.txt", "w");

    printf("\nNew file nodes.txt is being created with the name of the rare edges ...\n");

    vec1 = (char *)malloc(sizeof(char) * (vecsize + 1));
    for (i = 0; i < vecsize + 1; i++)
        vec1[i] = '0';

    for (i = 0; i < array_n(G->edgArr); i++)
    {
        q = array_fetch(edge *, G->edgArr, i);
        q->oldVal = q->newVal = q->signal0Prob = q->signal1Prob = 0;
        q->activity01 = q->activity10 = q->lowProbVal = 0;
        q->assnFlag = !nodeFlag ? 1 : 0;
    }
    printf("\nIntialized graph list ...\n");
    /*
    if (nodeFlag)
    {
        while (!feof(fnodes))
        {
            fscanf(fnodes, "%s", str);
            edg = findEdge(str, G->edgArr);
            if (edg)
                edg->assnFlag = 1;
            else
                printf("WARNING: Edge %s is not found in file nodes.txt ... Continuing ...\n", str);
            if (feof(fnodes))
                break;
        }
    }
    */
    printf("\nChecking primInEdges ...\n");
    for (i = 0; i < array_n(primInEdges); i++)
    {
        q = array_fetch(edge *, primInEdges, i);
        //if (DUMPALL && q->assnFlag)
        //    fprintf(fout, "%s ", q->name);
    }
    printf("\nChecking topoArr ...\n");
    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        s->activity = s->nInput00 = s->nInput01 = s->nInput10 = s->nInput11 = 0;
        if (array_n(s->outEdges))
        {
            q = array_fetch(edge *, s->outEdges, 0);
            //if (DUMPALL && q->assnFlag)
            //    fprintf(fout, "%s ", q->name);
        }
    }
    printf("\nChecking seqGates ...\n");
    for (i = 0; i < array_n(G->seqGates); i++)
    {
        s = array_fetch(vertex *, G->seqGates, i);
        //initial assignment of sequential gates
        for (j = 0; j < array_n(s->outEdges); j++)
        {
            edg2 = array_fetch(edge *, s->outEdges, j);
            port = array_fetch(char *, s->outPorts, j);
            if (!strcmp(port, ".Q"))
            {
                edg2->newVal = 0;
                edg2->oldVal = 0;
            }
            else if (!strcmp(port, ".QN"))
            {
                edg2->newVal = 1;
                edg2->oldVal = 1;
            }
        }
    }
    //if (DUMPALL)
    //    fprintf(fout, "\n\n");

    /* test:
	for(i=0; i<array_n(topoArr); i++)
	{
		printf("%s ", array_fetch(vertex*,topoArr,i)->name);
	} printf("\n");
	*/
    printf("\nAttempting to apply test patterns ...\n");
    while (!feof(fvec))
    {
        fscanf(fvec, "%s", vec1);
        //printf("Len : %d",strlen(vec1));
        //printf("\nStimulus Used : %s",vec1);
        if (feof(fvec))
            break;
        //printf("vector (%d): %s \n", nPatterns+1, vec1);
        if (extStimulus && nPatterns == 2 * traceCount)
            break;
        // apply the test pattern to PI's
        vectorActivityCnt = 0;
        resetflag = 0;
        total_loading_cap = 0;
        for (i = 0; i < array_n(primInEdges); i++)
        {
            q = array_fetch(edge *, primInEdges, i);
            q->oldVal = q->newVal;
            //q->oldVal = 0;
            q->newVal = (vec1[i] == '0' ? 0 : 1);
            array_insert_last(uint8_t, q->valueArr, (uint8_t)q->newVal);
	    //printf("\nPrimary IN : %s Old : %d New : %d\n",q->name, q->oldVal, q->newVal);
            //if (DUMPALL && q->assnFlag)
            //fprintf(fout, "%s-->>%d   ",q->name, q->newVal);
            //printf( "%s-->>%d   ",q->name, q->newVal);
            q->signal0Prob = (q->newVal) ? q->signal0Prob : q->signal0Prob + 1;
            q->signal1Prob = (q->newVal) ? q->signal1Prob + 1 : q->signal1Prob;
            q->activity01 = (q->oldVal == 0 && q->newVal == 1) ? q->activity01 + 1 : q->activity01;
            q->activity10 = (q->oldVal == 1 && q->newVal == 0) ? q->activity10 + 1 : q->activity10;

            // This part of the code calculates loading capacitances of every primary inputs for each vector - Added By Rajat
            loading_cap = 0.0;
            if (array_n(q->toNodes))
            {

                //printf("NodeName-%s, No.of tonodes-%d  Nodes are:\n", q->name, array_n(q->toNodes));
                for (jj1 = 0; jj1 < array_n(q->toNodes); jj1++)
                {
                    //printf("%s--", array_fetch(vertex*, q->toNodes, jj1)->name);
                    vi = array_fetch(vertex *, q->toNodes, jj1);

                    for (kk1 = 0; kk1 < array_n(vi->inEdges); ++kk1)
                    {
                        ei = array_fetch(edge *, vi->inEdges, kk1);
                        //printf ("\n(%s) && (%s) && %d\n",ei->name, te->name, strcmp(ei->name,te->name));
                        if (!(strcmp(ei->name, q->name)))
                        {
                            // printf ("Gate Area %f and type= %s ", (vi->type)->area, (vi->type)->name);
                            tp = array_fetch(pins *, (vi->type)->pinvals, kk1);
                            //printf ("<<pinName=%s riseCap=%f>>", tp->pinname ,tp->rise_capacitance) ;
                           // loading_cap += (q->oldVal == 0 && q->newVal == 1) ? loading_cap + tp->rise_capacitance : loading_cap + tp->fall_capacitance;
                            //printf ("Inport-%s ",array_fetch(edge*, vi->inPorts, kk1));

				if (q->oldVal == 0 && q->newVal == 1)
					loading_cap += tp->rise_capacitance;
				else if (q->oldVal == 1 && q->newVal == 0)
					loading_cap += tp->fall_capacitance;
                        }
                        // printf("\n");
                    }

                    //printf("\n");
                }
                total_loading_cap += loading_cap;
                //printf ("loading Cap= %f \n",loading_cap );
            }

            // End of loading capacitance calculation for every primary input
        }

        if (fullScan)
        {
            for (i = 0; i < array_n(G->seqGates); i++)
            {
                s = array_fetch(vertex *, G->seqGates, i);
                for (j = 0; j < array_n(s->outEdges); j++)
                {
                    edg2 = array_fetch(edge *, s->outEdges, j);
                    port = array_fetch(char *, s->outPorts, j);
                    for (m = 0; m < array_n(s->inEdges); m++)
                    {
                        edg3 = array_fetch(edge *, s->inEdges, m);

                        if (edg3->idx == rstEdgeId && vec1[edg3->idx] == 1)
                        {
                            resetflag = 1;
                        }
                    }
                    if (resetflag != 1)
                    {
                        if (!strcmp(port, ".Q"))
                            edg2->newVal = (vec1[array_n(primInEdges) + i] == '0' ? 0 : 1);
                        else if (!strcmp(port, ".QN"))
                            edg2->newVal = (vec1[array_n(primInEdges) + i] == '0' ? 1 : 0);
                    }
                    else if (resetflag == 1)
                    {
                        if (!strcmp(port, ".Q"))
                            edg2->newVal = 0; // (vec1[array_n(primInEdges) + i] == '0' ? 0 : 1);
                        else if (!strcmp(port, ".QN"))
                            edg2->newVal = 1; //(vec1[array_n(primInEdges) + i] == '0' ? 1 : 0);
                    }
                }
		//printf("\n--- Seq Name : %s Old : %d New : %d\n",edg2->name, edg2->oldVal, edg2->newVal);
            }
        }

        for (i = 0; i < array_n(topoArr); i++)
        {
            s = array_fetch(vertex *, topoArr, i);

            // compute and set output transition
            outTrans = calculateOutTransition(s);
            s->activity = (outTrans) ? s->activity + 1 : s->activity;
            activityCnt += (outTrans) ? 1 : 0;
            vectorActivityCnt += (outTrans) ? 1 : 0;
            edg1 = array_fetch(edge *, s->inEdges, 0);
            edg2 = array_n(s->inEdges) > 1 ? array_fetch(edge *, s->inEdges, 1) : NULL;
            s->nInput00 = (!edg1->oldVal && ((edg2 && !edg2->oldVal) || !edg2)) ? s->nInput00 + 1 : s->nInput00;
            s->nInput01 = (!edg1->oldVal && edg2 && edg2->oldVal) ? s->nInput01 + 1 : s->nInput01;
            s->nInput10 = (edg1->oldVal && edg2 && !edg2->oldVal) ? s->nInput10 + 1 : s->nInput10;
            s->nInput11 = (edg1->oldVal && ((edg2 && edg2->oldVal) || !edg2)) ? s->nInput11 + 1 : s->nInput11;

            s->nInput00 = (!edg1->newVal && ((edg2 && !edg2->newVal) || !edg2)) ? s->nInput00 + 1 : s->nInput00;
            s->nInput01 = (!edg1->newVal && edg2 && (edg2 && edg2->newVal)) ? s->nInput01 + 1 : s->nInput01;
            s->nInput10 = (edg1->newVal && edg2 && !edg2->newVal) ? s->nInput10 + 1 : s->nInput10;
            s->nInput11 = (edg1->newVal && ((edg2 && edg2->newVal) || !edg2)) ? s->nInput11 + 1 : s->nInput11;
        }

        for (i = 0; i < array_n(topoArr); i++)
        {
            s = array_fetch(vertex *, topoArr, i); //printf("\n%s----%d   \n",s->name, s->level);
            if (array_n(s->outEdges))
            {
                q = array_fetch(edge *, s->outEdges, 0);
		//printf("\nName : %s Old : %d New : %d\n",q->name, q->oldVal, q->newVal);
                q->signal0Prob = (q->newVal) ? q->signal0Prob : q->signal0Prob + 1;
                q->signal1Prob = (q->newVal) ? q->signal1Prob + 1 : q->signal1Prob;
                q->activity01 = (q->oldVal == 0 && q->newVal == 1) ? q->activity01 + 1 : q->activity01;
                q->activity10 = (q->oldVal == 1 && q->newVal == 0) ? q->activity10 + 1 : q->activity10;

                // This part of the code calculates loading capacitances of every internal nodes for each vector - Added By Rajat
                loading_cap = 0.0;
                node_level[i] = s->level;
                if (array_n(q->toNodes))
                {
                    loading_cap = 0.0;
                    for (jj1 = 0; jj1 < array_n(q->toNodes); jj1++)
                    {
                        vi = array_fetch(vertex *, q->toNodes, jj1);
                        for (kk1 = 0; kk1 < array_n(vi->inEdges); ++kk1)
                        {
                            ei = array_fetch(edge *, vi->inEdges, kk1);
                            if (!(strcmp(ei->name, q->name)))
                            {

                                tp = array_fetch(pins *, (vi->type)->pinvals, kk1);

                                //loading_cap += (q->oldVal == 0 && q->newVal == 1) ? loading_cap + tp->rise_capacitance : loading_cap + tp->fall_capacitance;
				if (q->oldVal == 0 && q->newVal == 1)
					loading_cap += tp->rise_capacitance;
				else if (q->oldVal == 1 && q->newVal == 0)
					loading_cap += tp->fall_capacitance;

                            }
                        }
                    }
                    //printf ("loading Cap= %f ",loading_cap );
                    total_loading_cap += loading_cap;
                    //loading_cap *= 1000.0; //total_loading_cap+=loading_cap;
                    //printf ("\nloading Cap= %f\n",total_loading_cap );
                }

                // This part of the code updates trace array (random and fixed) for every vector and nodes
		//printf("\n--Node--:%s\n",q->name);
                if (flagR)
                {
                    
		    if ((q->oldVal == 0 && q->newVal == 1) || (q->oldVal == 1 && q->newVal == 0))
                    {
                        delta = loading_cap - NodePowerMeanRandom[i];
			//printf("\nDelta : %lf === %f == %lf",delta,loading_cap,NodePowerMeanRandom[i]);
                        NodePowerMeanRandom[i] = NodePowerMeanRandom[i] + ((double)(delta) / (double)(col + 1));
			//if(i==2) printf("\nNodePowerMeanRandom :%d -- %lf",i,NodePowerMeanRandom[i]);
			//if(i==2)		printf("\ncalc: %lf -- %lf",delta,(double)(delta * delta) * ((double)col / (double)(col + 1)));
                        NodePowerCentralSumOrder2Random[i] = NodePowerCentralSumOrder2Random[i] + (double)(delta * delta) * ((double)col / (double)(col + 1));
			//if(i==2) printf("\nNodePowerCentralSumOrder2Random :%d -- %lf",i,NodePowerCentralSumOrder2Random[i]);
			if (NodePowerCentralSumOrder2Random[i] == 0.0 && NodePowerCentralSumOrder2Fixed[i] == 0.0) 
				IncrementalNnodeTvla[i] = 0.0;
			else
			{
                        IncrementalNnodeTvla[i] = ((NodePowerMeanRandom[i] - NodePowerMeanFixed[i])) / (sqrt((NodePowerCentralSumOrder2Random[i] / ((col + 1) * (col + 1))) + (NodePowerCentralSumOrder2Fixed[i] / ((col + 1) * (col + 1)))));
			//printf("\ntvla : %lf\n",IncrementalNnodeTvla[i]);
                        IncrementalNnodeTvla[i] = fabs(IncrementalNnodeTvla[i]); 
			}
                    }
                    else
                    {
                        delta = 0.0 - NodePowerMeanRandom[i];
			//printf("\nDelta : %lf === %f == %lf",delta,loading_cap,NodePowerMeanRandom[i]);
                        NodePowerMeanRandom[i] = NodePowerMeanRandom[i] + ((double)(delta) / (double)(col + 1));
			//printf("\nNodePowerMeanRandom :%d -- %lf",i,NodePowerMeanRandom[i]);
                        NodePowerCentralSumOrder2Random[i] = NodePowerCentralSumOrder2Random[i] + (double)(delta * delta) * ((double)col / (double)(col + 1));
			//printf("\nNodePowerCentralSumOrder2Random :%d -- %lf",i,NodePowerCentralSumOrder2Random[i]);
			if (NodePowerCentralSumOrder2Random[i] == 0.0 && NodePowerCentralSumOrder2Fixed[i] == 0.0) 
				IncrementalNnodeTvla[i] = 0.0;
			else
			{
                        IncrementalNnodeTvla[i] = ((NodePowerMeanRandom[i] - NodePowerMeanFixed[i])) / (sqrt((NodePowerCentralSumOrder2Random[i] / ((col + 1) * (col + 1))) + (NodePowerCentralSumOrder2Fixed[i] / ((col + 1) * (col + 1)))));
			//printf("\ntvla : %lf\n",IncrementalNnodeTvla[i]);
                        IncrementalNnodeTvla[i] = fabs(IncrementalNnodeTvla[i]); 
			}
                    }
                }
                else
                {
                    if ((q->oldVal == 0 && q->newVal == 1) || (q->oldVal == 1 && q->newVal == 0))
                    {
                        delta = loading_cap - NodePowerMeanFixed[i];
			//printf("\nDelta : %lf === %f == %lf",delta,loading_cap,NodePowerMeanFixed[i]);
                        NodePowerMeanFixed[i] = NodePowerMeanFixed[i] + ((double)(delta) / (double)(col + 1));
			//if(i==2) printf("\nNodePowerMeanFixed :%d -- %lf",i,NodePowerMeanFixed[i]);
			//if(i==2)		printf("\ncalc: %lf -- %lf",delta, (double)(delta * delta) * ((double)col / (double)(col + 1)));
                        NodePowerCentralSumOrder2Fixed[i] = NodePowerCentralSumOrder2Fixed[i] + (double)(delta * delta) * ((double)col / (double)(col + 1));
			//if(i==2) printf("\nNodePowerCentralSumOrder2Fixed :%d -- %lf",i,NodePowerCentralSumOrder2Fixed[i]);
                    }
                    else
                    {
                        delta = 0.0 - NodePowerMeanFixed[i];
			//printf("\nDelta : %lf === %f == %lf",delta,loading_cap,NodePowerMeanFixed[i]);
                        NodePowerMeanFixed[i] = NodePowerMeanFixed[i] + ((double)(delta) / (double)(col + 1));
			//printf("\nNodePowerMeanFixed :%d -- %lf",i,NodePowerMeanFixed[i]);
                        NodePowerCentralSumOrder2Fixed[i] = NodePowerCentralSumOrder2Fixed[i] + (double)(delta * delta) * ((double)col / (double)(col + 1));
			//printf("\nNodePowerCentralSumOrder2Fixed :%d -- %lf",i,NodePowerCentralSumOrder2Fixed[i]);
                    }
                }

                // End of node-wise trace array updates
            }
        }
        // printf("\n\n");

        // This part of the code updates trace array (random and fixed) for overall design


        // End of overall design trace array updates

        for (i = 0; i < array_n(orderedSeqGates); i++)
        {
            s = array_fetch(vertex *, orderedSeqGates, i);
		//printf("\nSeq Ver : %s\n",s->name);
            edg1 = findDataEdge(s);
            if(edg1->fromNode == NULL)
                continue;
            for (j = 0; j < array_n(s->outEdges); j++)
            {

                edg2 = array_fetch(edge *, s->outEdges, j);
                edg2->oldVal = edg2->newVal;
                array_insert_last(uint8_t, edg2->valueArr, (uint8_t)edg2->oldVal); //store current state (oldVal)
                //if (DUMPALL && edg2->assnFlag)
                //    fprintf(fout, "%d   ", edg2->newVal);
                port = array_fetch(char *, s->outPorts, j);
                //printf(".%s(%s):%d->",port,edg2->name, edg2->newVal);
                if (!strcmp(port, ".Q"))
                {
                    if (!edg1->fromNode->isSeq) //if predecessor is sequential, take old value, else take newly calculated comb value
                        edg2->newVal = edg1->newVal;
                    else
                        edg2->newVal = edg1->oldVal;
                }
                else if (!strcmp(port, ".QN"))
                {
                    if (!edg1->fromNode->isSeq) //if predecessor is sequential, take old value, else take newly calculated comb value
                        edg2->newVal = (edg1->newVal ? 0 : 1);
                    else
                        edg2->newVal = (edg1->oldVal ? 0 : 1);
                }
                //printf("%d\n",edg2->newVal); 
		//printf("\n--- Seq Name : %s Old : %d New : %d\n",edg2->name, edg2->oldVal, edg2->newVal);

	    }

        }
        //if (DUMPALL)
        //   fprintf(fout, " Total switching (vector: %d): %ld\n", nPatterns, vectorActivityCnt);

	/*
        if (flagR)
        {
            delta = total_loading_cap - NodePowerMeanRandom[array_n(topoArr)];
	    //printf("\nRand Delta : %lf === %lf\n",delta,total_loading_cap);
            NodePowerMeanRandom[array_n(topoArr)] = NodePowerMeanRandom[array_n(topoArr)] + ((double)(delta) / (double)(col + 1));
	    printf("\nOverall - NodePowerMeanRandom : %lf",NodePowerMeanRandom[array_n(topoArr)]);
            NodePowerCentralSumOrder2Random[array_n(topoArr)] = NodePowerCentralSumOrder2Random[array_n(topoArr)] + (delta * delta) * ((double)col / (double)(col + 1));
	    printf("\nOverall - NodePowerCentralSumOrder2Random : %lf",NodePowerCentralSumOrder2Random[array_n(topoArr)]);
            IncrementalNnodeTvla[array_n(topoArr)] = (NodePowerMeanRandom[array_n(topoArr)] - NodePowerMeanFixed[array_n(topoArr)]) / sqrt((NodePowerCentralSumOrder2Random[array_n(topoArr)] / ((col + 1) * (col + 1))) + (NodePowerCentralSumOrder2Fixed[array_n(topoArr)] / ((col + 1) * (col + 1))));
            IncrementalNnodeTvla[array_n(topoArr)] = fabs(IncrementalNnodeTvla[array_n(topoArr)]);
	    printf("\nOverall - IncrementalNnodeTvla : %lf",IncrementalNnodeTvla[array_n(topoArr)]);
        }
        else
        {
            delta = total_loading_cap - NodePowerMeanFixed[array_n(topoArr)];
	    //printf("\nFix Delta : %lf === %lf\n",delta,total_loading_cap);
            NodePowerMeanFixed[array_n(topoArr)] = NodePowerMeanFixed[array_n(topoArr)] + ((double)(delta) / (double)(col + 1));
	    printf("\nOverall - NodePowerMeanFixed : %lf",NodePowerMeanFixed[array_n(topoArr)]);
            NodePowerCentralSumOrder2Fixed[array_n(topoArr)] = NodePowerCentralSumOrder2Fixed[array_n(topoArr)] + (delta * delta) * ((double)col / (double)(col + 1));
	    printf("\nOverall - NodePowerCentralSumOrder2Fixed : %lf",NodePowerCentralSumOrder2Fixed[array_n(topoArr)]);
        }*/

	

        flagR = !flagR;
        vec_count += 1;
        if (vec_count > 0 && (vec_count % 2) == 0)
            col = col + 1;

        nPatterns++;
    }
    
    for(i=0;i<array_n(topoArr);i++)
    {
	IncrementalNnodeTvla[array_n(topoArr)] += IncrementalNnodeTvla[i];
    }

    // This part of the code evaluates mean, sd and TVLA values of evry internal node and overall design

    /* NodeWiseTVLA.txt --> Contains information on node-names, TVLA value, and its level. Additionally also contains the overall TVLA of Design
*/
    if (!extStimulus)
    {
        tvlafile = fopen("NodeWiseTVLA.txt", "w");
        int max_tvla = 0, max_leak = 0;
	tvla_values_file = (char*) malloc(sizeof(char)*120);
	sprintf(tvla_values_file, "../results/TVLA_Values_%s.txt", G->header);

        final_file_tvla = fopen(tvla_values_file, "w");

        int cc = 0;

        vertex *vv;
        array_t *vvarr;
        edge *ee;

        printf("\n\n%-10s%-10s%-80s\n", "Nodename", "TVLA", "Level");
        for (ii = 0; ii < array_n(topoArr); ii++)
        {
            ss = array_fetch(vertex *, topoArr, ii);
            printf("%-10s%-10.6lf%-80d\n", array_fetch(edge *, ss->outEdges, 0)->name, IncrementalNnodeTvla[ii], node_level[ii]);

            char result_str[500];
            ee = array_fetch(edge *, ss->outEdges, 0);
            sprintf(result_str, "%s - %s $ %s : ", ee->name, ss->name, ss->type->name);
            vvarr = ee->toNodes;
            if (array_n(vvarr) == 0)
            {
                fprintf(final_file_tvla, "%s # %lf,%d\n", result_str, IncrementalNnodeTvla[ii], node_level[ii]);
            }
            else
            {
                for (k = 0; k < array_n(vvarr); k++)
                {
                    vv = array_fetch(vertex *, vvarr, k);
                    strcat(result_str, " ");
                    strcat(result_str, vv->name);
                }
                fprintf(final_file_tvla, "%s # %lf,%d\n", result_str, IncrementalNnodeTvla[ii], node_level[ii]);
            }

            //printf("\n");

            if (IncrementalNnodeTvla[ii] >= 4.5)
            {
                max_leak++;
            }

            if (ceil(IncrementalNnodeTvla[ii]) > max_tvla)
                max_tvla = ceil(IncrementalNnodeTvla[ii]);
            fprintf(tvlafile, "%s,%lf,%d\n", array_fetch(edge *, ss->outEdges, 0)->name, IncrementalNnodeTvla[ii], node_level[ii]);
        }

        printf("OVERALL=%lf", IncrementalNnodeTvla[array_n(topoArr)]);
        fprintf(final_file_tvla, "\nOverall = %lf", IncrementalNnodeTvla[array_n(topoArr)]);
        fclose(tvlafile);
        fclose(final_file_tvla);

        free(NodePowerMeanRandom);
        free(NodePowerMeanFixed);
        free(NodePowerCentralSumOrder2Random);
        free(NodePowerCentralSumOrder2Fixed);
        free(IncrementalNnodeTvla);

        /* printf("\nNodeName   ToggleCount\n");
        for(i = 0; i < array_n(topoArr); i++)
        {
            s = array_fetch(vertex*, topoArr, i);
            if (array_n(s->outEdges))
            {
                q = array_fetch(edge*, s->outEdges, 0);
                printf("%s  %f\n", q->name, q->activity01+q->activity10);
                //printf("%s 1-0 %f\n  ", q->name, q->activity10);
            }
        }*/
    }

    // End of all TVLA related calculation

    //if(DUMPALL)
    //    fprintf(fout, "\n\n---------------------------------- Signal(0) Probabilities ------------------------------- \n");
    for (i = 0; i < array_n(primInEdges); i++)
    {
        q = array_fetch(edge *, primInEdges, i);
        q->signal0Prob = (float)q->signal0Prob / nPatterns;
        //if (DUMPALL && q->assnFlag)
        //    fprintf(fout, "%1.2f ", q->signal0Prob);
    }
    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        if (array_n(s->outEdges))
        {
            q = array_fetch(edge *, s->outEdges, 0);
            q->signal0Prob = (float)q->signal0Prob / nPatterns;
            //if (DUMPALL && q->assnFlag)
            //    fprintf(fout, "%1.2f ", q->signal0Prob);
        }
    }
    // if (DUMPALL)
    //     fprintf(fout, "\n\n---------------------------------- Signal(1) Probabilities ------------------------------- \n");
    for (i = 0; i < array_n(primInEdges); i++)
    {
        q = array_fetch(edge *, primInEdges, i);
        q->signal1Prob = (float)q->signal1Prob / nPatterns;
        //if (DUMPALL && q->assnFlag)
        //    fprintf(fout, "%1.2f ", q->signal1Prob);
    }
    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        if (array_n(s->outEdges))
        {
            q = array_fetch(edge *, s->outEdges, 0);
            q->signal1Prob = (float)q->signal1Prob / nPatterns;
            //if (DUMPALL && q->assnFlag)
            //    fprintf(fout, "%1.2f ", q->signal1Prob);
        }
    }
    //if (DUMPALL)
    //    fprintf(fout, "\n\n---------------------------------- Transition (0->1) Probabilities ------------------------------- \n");
    for (i = 0; i < array_n(primInEdges); i++)
    {
        q = array_fetch(edge *, primInEdges, i);
        q->activity01 = (float)q->activity01 / nPatterns;
        //if (DUMPALL && q->assnFlag)
        //    fprintf(fout, "%1.2f ", q->activity01);
    }
    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        if (array_n(s->outEdges))
        {
            q = array_fetch(edge *, s->outEdges, 0);
            q->activity01 = (float)q->activity01 / nPatterns;
            //if (DUMPALL && q->assnFlag)
            //    fprintf(fout, "%1.2f ", q->activity01);
        }
    }
    //if (DUMPALL)
    //    fprintf(fout, "\n\n---------------------------------- Transition (1->0) Probabilities ------------------------------- \n");
    for (i = 0; i < array_n(primInEdges); i++)
    {
        q = array_fetch(edge *, primInEdges, i);
        q->activity10 = (float)q->activity10 / nPatterns;
        //if (DUMPALL && q->assnFlag)
        //fprintf(fout, "%1.2f ", q->activity10);
    }
    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        if (array_n(s->outEdges))
        {
            q = array_fetch(edge *, s->outEdges, 0);
            q->activity10 = (float)q->activity10 / nPatterns;
            //if (DUMPALL && q->assnFlag)
            //    fprintf(fout, "%1.2f ", q->activity10);
        }
    }
    //if (DUMPALL)
    //fprintf(fout, "\n\n---------------------------------- Nodes with Low Signal(0) Probabilities (P <= %1.3f) --------------------- \n", TRIGTHRESH);
    for (i = 0; i < array_n(primInEdges); i++)
    {
        q = array_fetch(edge *, primInEdges, i);
        if (q->assnFlag && q->signal0Prob <= TRIGTHRESH)
        {
            //if (DUMPALL)
            //    fprintf(fout, "%s(%1.3f) ", q->name, q->signal0Prob);
            //if (q->signal0Prob > 0) {
            array_insert_last(edge *, G->lowprobEdges, q);
            //}
        }
        //if (DUMPALL && q->signal0Prob <= TRIGTHRESH && !nodeFlag)
        //    fprintf(fnodes, "%s\n", q->name);
    }
    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        if (array_n(s->outEdges))
        {
            q = array_fetch(edge *, s->outEdges, 0);
            if (q->assnFlag && q->signal0Prob <= TRIGTHRESH)
            {
                //if (DUMPALL)
                //    fprintf(fout, "%s(%1.3f) ", q->name, q->signal0Prob);
                zerocnt++;
                array_insert_last(edge *, G->lowprobEdges, q);
            }
            //if (DUMPALL && q->signal0Prob <= TRIGTHRESH && !nodeFlag)
            //    fprintf(fnodes, "%s\n", q->name);
        }
    }
    //if (DUMPALL)
    //    fprintf(fout, "\n\n---------------------------------- Nodes with Low Signal(1) Probabilities (P <= %1.3f) --------------------- \n", TRIGTHRESH);
    for (i = 0; i < array_n(primInEdges); i++)
    {
        q = array_fetch(edge *, primInEdges, i);
        if (q->assnFlag && q->signal1Prob <= TRIGTHRESH)
        {
            //if (DUMPALL)
            //    fprintf(fout, "%s(%1.3f) ", q->name, q->signal1Prob);
            //if (q->signal1Prob > 0) {
            array_insert_last(edge *, G->lowprobEdges, q);
            q->lowProbVal = 1;
            //}
        }
        //if (DUMPALL && q->signal1Prob <= TRIGTHRESH && !nodeFlag)
        //    fprintf(fnodes, "%s\n", q->name);
    }
    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        if (array_n(s->outEdges))
        {
            q = array_fetch(edge *, s->outEdges, 0);
            if (q->assnFlag && q->signal1Prob <= TRIGTHRESH)
            {
                //if (DUMPALL)
                //    fprintf(fout, "%s(%1.3f) ", q->name, q->signal1Prob);
                //if (q->signal1Prob > 0) {
                array_insert_last(edge *, G->lowprobEdges, q);
                q->lowProbVal = 1;
                //}
                onecnt++;
            }
            //if (DUMPALL && q->signal1Prob <= TRIGTHRESH && !nodeFlag)
            //    fprintf(fnodes, "%s\n", q->name);
        }
    }

    //for(i = 0; i < array_n(G->lvlVertArr); i++)
    //   {
    // vertex* s1;
    //s1 = array_fetch(vertex*, G->lvlVertArr, i);double *temparray=array_fetch(pins*, (s1->type)->pinvals , 0)->risedelays;
    //printf ("PATH Delay %s-%d-%lf\n",s1->name,s1->level,*temparray);

    //}

    printf("\n\nNumber of patterns: %d\n", nPatterns);
    printf("\n\nGraph Statistics ...\n");
    printf("Graph vertices: %d (%d transistors)\n", array_n(G->vertexArr), countTransistors(G));
    printf("Number of flip-flops: %d\n", array_n(G->seqGates));
    printf("Number of Edges with Signal Probability below Threshold (%1.4f): %d\n", TRIGTHRESH, onecnt + zerocnt);
    printf("Graph Edges: %d\n", array_n(G->edgArr));
    printf("Circuit depth: %d\n", array_fetch_last(vertex *, G->lvlVertArr)->level);
    printf("Average activity per node: %.2f. ActivityCnt per test: %.0f\n", (float)activityCnt / (float)(nPatterns * array_n(topoArr)), (float)activityCnt / (float)nPatterns);
    if (isCombLoop)
        printf("COMBINATIONAL LOOP DETECTED in the graph ...\n\n");

    printf("\nINFO: Number of Edges with Signal Probability below Threshold (%1.3f): %d\n", TRIGTHRESH, onecnt + zerocnt);
    //fclose(fout);
    fclose(fvec);
    //fclose(fnodes);
    /* uint8_t val;
    for(i=0;i<array_n(G->edgArr);i++)
    {
        q = array_fetch(edge*, G->edgArr, i);
        printf("edge %s: ",q->name);
        for(j=0;j<array_n(q->valueArr);j++)
        {
            val = array_fetch(uint8_t, q->valueArr, j);
            printf("%u ",val);
        }
        printf("\n");
    }*/
}


void generatePO(graph *G, array_t *topoArr, char *vectorFile)
{
    int i, j, k, l, z, outTrans, flag = 0, ncnt = 0, onecnt = 0, zerocnt = 0;
    int seed = 15, b1, b2, sens, gcnt = 0, nodeFlag = 0;
    long nPatterns = 0, activityCnt = 0, vectorActivityCnt = 0, vecsize;
    vertex *s, *v;
    char *str1, *port;
    edge *q, *edg1, *edg2, *edg, *clkedg;
    char str[1200], *vec1;
    FILE *fvec, *fout, *yptr, *fnodes, *fvec_out;
    float activity_store = 0.0, prob;
    array_t *primInEdges = G->primInEdges;
    char *type;

    printf("\nAnalyzing internal node activities for random vectors from %s ...\n", vectorFile);

    vecsize = array_n(G->primInEdges) + array_n(G->seqGates);
    fvec = fopen(vectorFile, "r"); // vectorFile="***_PI.txt"
    fout = fopen("report.txt", "w");
    fnodes = fopen("nodes.txt", "w");
    //
    char *vec_out_name = (char *)malloc(200);
    strcpy(vec_out_name, vectorFile); // vec_out_name="***_PO.txt"
    vec_out_name[strlen(vec_out_name) - 5] = 'O';
    fvec_out = fopen(vec_out_name, "w");
    printf("\nNew file nodes.txt is being created with the name of the rare edges ...\n");

    vec1 = (char *)malloc(sizeof(char) * (vecsize + 1));
    for (i = 0; i < vecsize + 1; i++)
        vec1[i] = '0';

    for (i = 0; i < array_n(G->edgArr); i++)
    {
        q = array_fetch(edge *, G->edgArr, i);
        q->oldVal = q->newVal = q->signal0Prob = q->signal1Prob = 0;
        q->activity01 = q->activity10 = q->lowProbVal = 0;
        q->assnFlag = !nodeFlag ? 1 : 0;
    }
    if (nodeFlag)
    {
        while (!feof(fnodes))
        {
            fscanf(fnodes, "%s", str);
            edg = findEdge(str, G->edgArr);
            if (edg)
                edg->assnFlag = 1;
            else
                printf("WARNING: Edge %s is not found in file nodes.txt ... Continuing ...\n", str);
            if (feof(fnodes))
                break;
        }
    }
    for (i = 0; i < array_n(primInEdges); i++)
    {
        q = array_fetch(edge *, primInEdges, i);
        if (DUMPALL && q->assnFlag)
            fprintf(fout, "%s ", q->name);
    }
    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        s->activity = s->nInput00 = s->nInput01 = s->nInput10 = s->nInput11 = 0;
        if (array_n(s->outEdges))
        {
            q = array_fetch(edge *, s->outEdges, 0);
            if (DUMPALL && q->assnFlag)
                fprintf(fout, "%s ", q->name);
        }
    }
    for (i = 0; i < array_n(G->seqGates); i++)
    {
        s = array_fetch(vertex *, G->seqGates, i);
        q = array_fetch(edge *, s->outEdges, 0);
        if (DUMPALL && q->assnFlag)
            fprintf(fout, "%s ", q->name);
    }
    if (DUMPALL)
        fprintf(fout, "\n\n");

    while (!feof(fvec))
    {
        fscanf(fvec, "%s", vec1);
        if (feof(fvec))
            break;
        //printf("vector (%d): %s \n", nPatterns+1, vec1);

        // apply the test pattern to PI's
        vectorActivityCnt = 0;
        for (i = 0; i < array_n(primInEdges); i++)
        {
            q = array_fetch(edge *, primInEdges, i);
            q->oldVal = q->newVal;
            q->newVal = (vec1[i] == '0' ? 0 : 1);
            if (DUMPALL && q->assnFlag)
                fprintf(fout, "%d   ", q->newVal);
            q->signal0Prob = (q->newVal) ? q->signal0Prob : q->signal0Prob + 1;
            q->signal1Prob = (q->newVal) ? q->signal1Prob + 1 : q->signal1Prob;
            q->activity01 = (q->oldVal == 0 && q->newVal == 1) ? q->activity01 + 1 : q->activity01;
            q->activity10 = (q->oldVal == 1 && q->newVal == 0) ? q->activity10 + 1 : q->activity10;
        }
        if (fullScan)
        {
            for (i = 0; i < array_n(G->seqGates); i++)
            {
                s = array_fetch(vertex *, G->seqGates, i);
                for (j = 0; j < array_n(s->outEdges); j++)
                {
                    edg2 = array_fetch(edge *, s->outEdges, j);
                    port = array_fetch(char *, s->outPorts, j);
                    if (!strcmp(port, ".Q"))
                        edg2->newVal = (vec1[array_n(primInEdges) + i] == '0' ? 0 : 1);
                    else if (!strcmp(port, ".QN"))
                        edg2->newVal = (vec1[array_n(primInEdges) + i] == '0' ? 1 : 0);
                }
            }
        }

        for (i = 0; i < array_n(topoArr); i++)
        {
            s = array_fetch(vertex *, topoArr, i);

            // compute and set output transition
            outTrans = calculateOutTransition(s);
            s->activity = (outTrans) ? s->activity + 1 : s->activity;
            activityCnt += (outTrans) ? 1 : 0;
            vectorActivityCnt += (outTrans) ? 1 : 0;
            edg1 = array_fetch(edge *, s->inEdges, 0);
            edg2 = array_n(s->inEdges) > 1 ? array_fetch(edge *, s->inEdges, 1) : NULL;
            s->nInput00 = (!edg1->oldVal && ((edg2 && !edg2->oldVal) || !edg2)) ? s->nInput00 + 1 : s->nInput00;
            s->nInput01 = (!edg1->oldVal && edg2 && edg2->oldVal) ? s->nInput01 + 1 : s->nInput01;
            s->nInput10 = (edg1->oldVal && edg2 && !edg2->oldVal) ? s->nInput10 + 1 : s->nInput10;
            s->nInput11 = (edg1->oldVal && ((edg2 && edg2->oldVal) || !edg2)) ? s->nInput11 + 1 : s->nInput11;

            s->nInput00 = (!edg1->newVal && ((edg2 && !edg2->newVal) || !edg2)) ? s->nInput00 + 1 : s->nInput00;
            s->nInput01 = (!edg1->newVal && edg2 && (edg2 && edg2->newVal)) ? s->nInput01 + 1 : s->nInput01;
            s->nInput10 = (edg1->newVal && edg2 && !edg2->newVal) ? s->nInput10 + 1 : s->nInput10;
            s->nInput11 = (edg1->newVal && ((edg2 && edg2->newVal) || !edg2)) ? s->nInput11 + 1 : s->nInput11;
        }

        for (i = 0; i < array_n(topoArr); i++)
        {
            s = array_fetch(vertex *, topoArr, i);
            if (array_n(s->outEdges))
            {
                q = array_fetch(edge *, s->outEdges, 0);
                q->signal0Prob = (q->newVal) ? q->signal0Prob : q->signal0Prob + 1;
                q->signal1Prob = (q->newVal) ? q->signal1Prob + 1 : q->signal1Prob;
                q->activity01 = (q->oldVal == 0 && q->newVal == 1) ? q->activity01 + 1 : q->activity01;
                q->activity10 = (q->oldVal == 1 && q->newVal == 0) ? q->activity10 + 1 : q->activity10;
                if (DUMPALL && q->assnFlag)
                    fprintf(fout, "%d   ", q->newVal);
            }
        }

        for (i = 0; i < array_n(G->seqGates); i++)
        {
            s = array_fetch(vertex *, G->seqGates, i);
            edg1 = findDataEdge(s);
            for (j = 0; j < array_n(s->outEdges); j++)
            {
                edg2 = array_fetch(edge *, s->outEdges, j);
                edg2->oldVal = edg2->newVal;
                if (DUMPALL && edg2->assnFlag)
                    fprintf(fout, "%d   ", edg2->newVal);
                port = array_fetch(char *, s->outPorts, j);
                if (!strcmp(port, ".Q"))
                    edg2->newVal = edg1->newVal;
                else if (!strcmp(port, ".QN"))
                    edg2->newVal = (edg1->newVal ? 0 : 1);
            }
        }

        for (i = 0; i < array_n(G->primOutEdges); i++)
            fprintf(fvec_out, "%d", array_fetch(edge *, G->primOutEdges, i)->newVal);
        fprintf(fvec_out, "\n");
        //fprintf(fvec_out, "G switches=%d\n", vectorActivityCnt);
        nPatterns++;
    }

    fclose(fout);
    fclose(fvec);
    fclose(fnodes);
    fclose(fvec_out);
}

// compute delay as pipeline stage delay
// input parameter: device = 0 => standard 1 => optimal
double computeStageDelay(graph *G, array_t *topoArr, float vdd, int numStages, int device)
{
    int i, j, input, nStage = 0;
    double dynPower = 0, leakPower = 0, tPower;
    vertex *vert, *s;
    char cellName[400], tran[400], *tstr;
    double loadCap, absLoadCap;
    int maxLogicDepth = 0, outTran;
    int cnt = 0, stageLogicDepth;
    FILE *fptr = fopen("delayPaths.txt", "r");
    float cellDelay, tDelay;
    float stageDelay, maxStageDelay;
    array_t *stageDelayArr = array_alloc(float, 1);
    array_t *critVertNameArr = array_alloc(char *, 1);
    array_t *critVertInputArr = array_alloc(int, 1);
    array_t *critVertOutTranArr = array_alloc(char *, 1);
    array_t *critVertArr = array_alloc(vertex *, 1);

    if (!fptr)
    {
        printf("Delay file not present ...\n");
        exit(0);
    }

    while (!feof(fptr))
    {
        nStage++;
        fscanf(fptr, "%s %s %d\n", cellName, tran, &input);
        array_insert_last(char *, critVertOutTranArr, (char *)strdup(tran));
        array_insert_last(int, critVertInputArr, input);

        for (i = 0; i < array_n(topoArr); i++)
        {
            s = array_fetch(vertex *, topoArr, i);
            if (!strcmp(cellName, s->name))
            {
                array_insert_last(vertex *, critVertArr, s);
                break;
            }
        }
    }
    fclose(fptr);

    maxLogicDepth = nStage;

    printf("Maximum logic depth of critical path: %d \n", maxLogicDepth);

    stageLogicDepth = ceil((float)maxLogicDepth / (float)numStages); // touch it for exact value

    while (cnt < maxLogicDepth)
    {
        stageDelay = 0;
        i = 0;
        while (i < stageLogicDepth && cnt < maxLogicDepth)
        {
            tstr = array_fetch(char *, critVertOutTranArr, cnt);
            input = array_fetch(int, critVertInputArr, cnt);
            if (!strcmp(tstr, "r"))
                outTran = 01;
            else
                outTran = 10;
            vert = array_fetch(vertex *, critVertArr, cnt);
            if (device)
                cellDelay = getDelayOpt(G, vert, vdd, input, outTran);
            else
                cellDelay = getDelayStd(G, vert, vdd, input, outTran);
            stageDelay += cellDelay;
            i++;
            cnt++;
        }
        array_insert_last(float, stageDelayArr, stageDelay);
    }

    maxStageDelay = 0;
    for (i = 0; i < array_n(stageDelayArr); i++)
    {
        tDelay = array_fetch(float, stageDelayArr, i);
        if (tDelay > maxStageDelay)
            maxStageDelay = tDelay;
    }

    maxStageDelay += setupDelay + clk2QDelay;

    return maxStageDelay;
}

int calculateOutValues(vertex *s)
{
    int *oVal1 = (int *)malloc(sizeof(int));
    int *oVal2 = (int *)malloc(sizeof(int));
    int *nVal1 = (int *)malloc(sizeof(int));
    int *nVal2 = (int *)malloc(sizeof(int));
    array_t *inEdges = s->inEdges;
    array_t *outEdges = s->outEdges;
    edge *edg;
    int i, retval;

    array_t *newInVal = array_alloc(int, 1);

    /* let's assume that there will be max 2 outputs per cell */
    /* if more, we can pass more oVal to the getValue function */

    for (i = 0; i < array_n(inEdges); i++)
    {
        edg = array_fetch(edge *, inEdges, i);
        array_insert_last(int, newInVal, edg->newVal);
    }

    /* compute the value based on gate type */
    getValue(s->type->name, newInVal, nVal1, nVal2);

    /* set the output values on output edges */
    if (array_n(outEdges) == 1)
    {
        edg = array_fetch(edge *, outEdges, 0);
        edg->oldVal = edg->newVal;
        edg->newVal = *nVal1;
    }
    else if (array_n(outEdges) == 2)
    {
        edg = array_fetch(edge *, outEdges, 0);
        edg->oldVal = edg->newVal;
        edg->newVal = *nVal1;
        edg = array_fetch_last(edge *, outEdges);
        edg->oldVal = edg->newVal;
        edg->newVal = *nVal2;
    }
    else
        printf("ERROR: out edges > 2 for %s in CalculateOutTransition.\n", s->type->name);

    if (*oVal1 != *nVal1 || *oVal2 != *nVal2)
        retval = 1;
    else
        retval = 0;

    array_free(newInVal);
    free(oVal1);
    free(oVal2);
    free(nVal1);
    free(nVal2);

    return retval;
}

int countTransistors(graph *G)
{
    int i, count = 0;
    vertex *v;

    for (i = 0; i < array_n(G->vertexArr); i++)
    {
        v = array_fetch(vertex *, G->vertexArr, i);
        if (strstr(v->type->name, "DFF") || !strncasecmp(v->type->name, "latch", 5) || !(strncasecmp(v->type->name, "DFFPOSX1", 8)))
            count += 20;                                                                                                                            // with QN
        else if (strstr(v->type->name, "NAND2") || strstr(v->type->name, "NOR2") || strstr(v->type->name, "nnd2") || strstr(v->type->name, "nor2")) // Patanjali changed this to match gate types in std lib
            count += 4;
        else if (strstr(v->type->name, "hi1s") || strstr(v->type->name, "i1s") || strstr(v->type->name, "ib1s") || (strstr(v->type->name, "INV"))) //jonathan: added more gates
            count += 2;
        else if (!strncasecmp(v->type->name, "AND2", 4) || !strncasecmp(v->type->name, "OR2", 3))
            count += 6;
        else if (!strncasecmp(v->type->name, "AND3", 4) || !strncasecmp(v->type->name, "OR3", 3))
            count += 8;
        else if (!strncasecmp(v->type->name, "AND4", 4) || !strncasecmp(v->type->name, "OR4", 3))
            count += 10;
        else if (!strncasecmp(v->type->name, "NAND3", 4) || !strncasecmp(v->type->name, "NOR3", 4))
            count += 6;
        else if (!strncasecmp(v->type->name, "NAND4", 4) || !strncasecmp(v->type->name, "NOR4", 4))
            count += 8;
        else if (!strncasecmp(v->type->name, "AND5", 4) || !strncasecmp(v->type->name, "OR5", 3))
            count += 12;
        else if (!strncasecmp(v->type->name, "AND6", 4) || !strncasecmp(v->type->name, "OR6", 3))
            count += 14;
        else if (!strncasecmp(v->type->name, "AND7", 4) || !strncasecmp(v->type->name, "OR6", 3))
            count += 16;
        else if (!strncasecmp(v->type->name, "AND8", 4) || !strncasecmp(v->type->name, "OR6", 3))
            count += 18;
        else if (!strncasecmp(v->type->name, "AND9", 4) || !strncasecmp(v->type->name, "OR6", 3))
            count += 20;
        else if (!strncasecmp(v->type->name, "NAND5", 4) || !strncasecmp(v->type->name, "NOR6", 4))
            count += 10;
        else if (!strncasecmp(v->type->name, "NAND6", 4) || !strncasecmp(v->type->name, "NOR6", 4))
            count += 12;
        else if (!strncasecmp(v->type->name, "XOR2", 4))
            count += 10;
        else if (!strncasecmp(v->type->name, "XNOR2", 4))
            count += 10;
        else if (!strncasecmp(v->type->name, "XOR3", 4))
            count += 20;
        else if (!strncasecmp(v->type->name, "XNOR3", 4))
            count += 20;
        else if (!strncasecmp(v->type->name, "nb1", 3))
            count += 4;
        else if (!strncasecmp(v->type->name, "OAI", 3)) // added support for OAI gates
            count += 6;
        else if (!strncasecmp(v->type->name, "BUFX2", 3))
            count += 4;
        else if (!strncasecmp(v->type->name, "FAX1", 5))
            count += 28;
        else if (!strncasecmp(v->type->name, "AOI", 3))
            count += 6;
        else
        {
            count += 4;
            printf("WARNING: Unknown Gate Type in CountTransistor: %s\n", v->type->name);
        }
    }

    return count;
}

int propagateToPO(graph *G, array_t *topoArr, edge *tedg)
{
    int i, startIdx, match = 0, nedges;
    int *prevOutVal, *prevSeqVal;
    int *oldValArr, *newValArr;
    vertex *s;
    edge *edg1;

    if (tedg->bPrimOut)
        return true;

    nedges = array_n(G->edgArr);
    oldValArr = (int *)malloc(sizeof(int) * nedges);
    newValArr = (int *)malloc(sizeof(int) * nedges);
    prevOutVal = (int *)malloc(sizeof(int) * array_n(G->primOutEdges));
    prevSeqVal = (int *)malloc(sizeof(int) * (array_n(G->seqGates) + 1));

    for (i = 0; i < nedges; i++)
    {
        edg1 = array_fetch(edge *, G->edgArr, i);
        oldValArr[i] = edg1->oldVal;
        newValArr[i] = edg1->newVal;
    }

    for (i = 0; i < array_n(G->primOutEdges); i++)
        prevOutVal[i] = array_fetch(edge *, G->primOutEdges, i)->newVal;
    for (i = 0; i < array_n(G->seqGates); i++)
    {
        s = array_fetch(vertex *, G->seqGates, i);
        edg1 = findDataEdge(s);
        prevSeqVal[i] = edg1->newVal;
    }

    //startIdx = (tedg->fromNode) ? tedg->fromNode->topoIdx + 1 : 0;
    startIdx = (tedg->fromNode) ? tedg->fromNode->topoIdx : 0;
    tedg->newVal = (tedg->newVal) ? 0 : 1;        // inject a fault in the tedg
    for (i = startIdx; i < array_n(topoArr); i++) // propagate fault effect
        calculateOutTransition(array_fetch(vertex *, topoArr, i));

    match = true;
    for (i = 0; i < array_n(G->primOutEdges); i++)
    {
        edg1 = array_fetch(edge *, G->primOutEdges, i);
        if (prevOutVal[i] != edg1->newVal)
        {
            match = false;
            break;
        }
    }

    //_sb check the inputs of FFs
    if (fullScan && match) // check only if all match in POs
    {
        for (i = 0; i < array_n(G->seqGates); i++)
        {
            s = array_fetch(vertex *, G->seqGates, i);
            edg1 = findDataEdge(s);
            if (edg1->newVal != prevSeqVal[i])
            {
                match = false;
                break;
            }
        }
    }

    for (i = 0; i < nedges; i++)
    {
        edg1 = array_fetch(edge *, G->edgArr, i);
        edg1->oldVal = oldValArr[i];
        edg1->newVal = newValArr[i];
    }

    free(oldValArr);
    free(newValArr);
    free(prevOutVal);
    free(prevSeqVal);
    return !match;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void propagateValues(graph *G, array_t *topoArr, char *vec)
{
    int i, j, k;
    edge *q, *edg1, *edg2;
    vertex *s;
    char *port;

    // apply the test pattern to PI's
    for (i = 0; i < array_n(G->primInEdges); i++)
    {
        q = array_fetch(edge *, G->primInEdges, i);
        q->oldVal = q->newVal;
        q->newVal = (vec[i] == '0' ? 0 : 1);
    }

    if (fullScan)
    {
        for (i = 0; i < array_n(G->seqGates); i++)
        {
            s = array_fetch(vertex *, G->seqGates, i);
            for (j = 0; j < array_n(s->outEdges); j++)
            {
                edg2 = array_fetch(edge *, s->outEdges, j);
                port = array_fetch(char *, s->outPorts, j);
                if (!strcmp(port, ".Q"))
                    edg2->newVal = (vec[array_n(G->primInEdges) + i] == '0' ? 0 : 1);
                else if (!strcmp(port, ".QN"))
                    edg2->newVal = (vec[array_n(G->primInEdges) + i] == '0' ? 1 : 0);
            }
        }
    }

    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        calculateOutTransition(s);
    }

    if (fullScan)
    {
        for (i = 0; i < array_n(G->seqGates); i++) //--- this will influence the next vector(cycle)
        {
            s = array_fetch(vertex *, G->seqGates, i);
            edg1 = findDataEdge(s);
            for (j = 0; j < array_n(s->outEdges); j++)
            {
                edg2 = array_fetch(edge *, s->outEdges, j);
                edg2->oldVal = edg2->newVal;
                port = array_fetch(char *, s->outPorts, j);
                if (!strcmp(port, ".Q")) // in fact, it is not needed to update newVal under fullscan
                    edg2->newVal = edg1->newVal;
                else if (!strcmp(port, ".QN"))
                    edg2->newVal = (edg1->newVal ? 0 : 1);
            }
        }
    }
}

// slightly different from the above, "just simulate the newVal, keep the oldVal unchanged"
void propagateOnlyNewValues(graph *G, array_t *topoArr, char *vec)
{
    int i, j, k;
    edge *q, *edg2;
    vertex *s;
    char *port;

    // apply the test pattern to PI's
    for (i = 0; i < array_n(G->primInEdges); i++)
    {
        q = array_fetch(edge *, G->primInEdges, i);
        //q->oldVal = q->newVal;//
        q->newVal = (vec[i] == '0' ? 0 : 1);
    }

    if (fullScan)
    {
        for (i = 0; i < array_n(G->seqGates); i++)
        {
            s = array_fetch(vertex *, G->seqGates, i);
            for (j = 0; j < array_n(s->outEdges); j++)
            {
                edg2 = array_fetch(edge *, s->outEdges, j);
                port = array_fetch(char *, s->outPorts, j);
                if (!strcmp(port, ".Q"))
                    edg2->newVal = (vec[array_n(G->primInEdges) + i] == '0' ? 0 : 1);
                else if (!strcmp(port, ".QN"))
                    edg2->newVal = (vec[array_n(G->primInEdges) + i] == '0' ? 1 : 0);
            }
        }
    }

    for (i = 0; i < array_n(topoArr); i++)
    {
        s = array_fetch(vertex *, topoArr, i);
        calculateOutTransitionOnlyNewValues(s);
    }
}

edge *findDataEdge(vertex *s)
{
    int j;
    edge *edg;
    char *port;

    for (j = 0; j < array_n(s->inEdges); j++)
    {
        edg = array_fetch(edge *, s->inEdges, j);
        port = array_fetch(char *, s->inPorts, j);
        if (!strncasecmp(port, ".DIN", 4) || !strncasecmp(port, ".D", 2))
            return edg;
    }
    return NULL;
}

void generateRandomInputs(graph *G, int randVectSize, array_t *prev_vects)
{
    int i, b, x;
    int vectBits = array_n(G->primInEdges) + array_n(G->seqGates);
    char *pattern = (char *)malloc((vectBits + 1) * sizeof(char));
    for (i = 0; i < randVectSize; i++)
    {
        for (b = 0; b < vectBits; b++)
        {
            x = rand() % 1000; // 0~999
            if (x < 500)
                pattern[b] = '0';
            else
                pattern[b] = '1';
        }
        pattern[vectBits] = '\0';
        array_insert(char *, prev_vects, i, strdup(pattern));
    }
}
