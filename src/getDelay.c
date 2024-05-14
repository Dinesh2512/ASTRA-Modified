// ©Copyright 2020 University of Florida Research Foundation, Inc. All Rights Reserved.

/**********************************************************/
/*                                                        */
/*     Calculating Pin-to-Pin Delay of a Gate             */
/*                                                        */
/*                                                        */
/*             Last update on 11/20/2004                  */
/*                                                        */
/*    Note: Assumes no layout information is available    */
/*                                                        */
/**********************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "array.h"
#include "graph.h"

/* **** Please pass the output transition type "outTran":
        h2l: 10, l2h: 01  */


double getDelayStd(graph* G, vertex* vert, float Vdd, int input, int outTran)
{
    array_t* inEdgArr;
    array_t* outEdgArr;
    vertex* frmNode;
    Type* gate;
    edge* inEdg;
    edge* outEdg;
    int i, n, K;
    double CL, td;
    double a0, a1, a2, a3, absCL;
    char* gateName;


    outEdgArr = vert->outEdges;
    inEdgArr = vert->inEdges;
    outEdg = array_fetch_last(edge*, outEdgArr);


    /* Finding Delay */
    gate = vert->type;


        // Calculate load capacitance, CL //
        CL = findCLStd(G, outEdg, gate, input, &absCL, Vdd);
         if(strncasecmp(gate->name, "i1", 2) == 0){
            if(outTran == 01){
                a3 = -0.7237e-5;
                a2 = 0.7116e-5;
                a1 = -0.2323e-5;
                a0 = 0.0256e-5;
            }
            else if(outTran == 10){
                                a3 =  -0.5353e-5;
                                a2 = 0.5312e-5;
                                a1 = -0.1757e-5;
                                a0 = 0.0198e-5;
                        }

        }
        else if(strncasecmp(gate->name, "nnd", 3) == 0){
            if(input == 1){
                if(outTran == 01){
                                a3 =  -0.6372e-5;
                                a2 = 0.6469e-5;
                                a1 = -0.2194e-5;
                                a0 = 0.0253e-5;
                }
                            else if(outTran == 10){
                            a3 = -0.9677e-5;
                                a2 =  0.9550e-5;
                                a1 = -0.3127e-5;
                                a0 = 0.0345e-5;
                }
            }
            else if(input == 2) {
                if(outTran == 01){
                                a3 = -0.5622e-5;
                                a2 =  0.5800e-5;
                                a1 = -0.2005e-5;
                                a0 = 0.0237e-5;
                }
                                else if(outTran == 10){
                                a3 =  -0.9226e-5;
                                a2 = 0.9189e-5;
                                a1 = -0.3043e-5;
                                a0 = 0.0340e-5;
                }
            }
                }
        else if(strncasecmp(gate->name, "nor", 3) == 0){
                        if(input == 1){
                                if(outTran == 01){
                                a3 = -0.2065e-4;
                                a2 = 0.2019e-4;
                                a1 = -0.0651e-4;
                                a0 = 0.0070e-4;
                }
                                else if(outTran == 10){
                                a3 =-0.8625e-5;
                                a2 =0.8444e-5;
                                a1 =-0.2744e-5;
                                a0 =0.0301e-5;
                }

            }
                        else if(input == 2) {
                                if(outTran == 01){
                                a3 =-0.1547e-5;
                                a2 =0.1559e-5;
                                a1 =-0.0523e-5;
                                a0 =0.0059e-5;
                }
                                else if(outTran == 10){
                                a3 =-0.6429e-6;
                                a2 =0.7728e-6;
                                a1 =-0.3355e-6;
                                a0 =0.0556e-6;
                }
            }
                }

       td = (a3*pow(Vdd,3) + a2*pow(Vdd, 2) + a1*Vdd + a0)*CL;
	//printf("\nSTD  td = %e CL=%e\n",td, CL);
            vert->delay = td;
        return td;
}


double getDelayOpt(graph* G, vertex* vert, float Vdd, int input, int outTran)
{
    array_t* inEdgArr;
    array_t* outEdgArr;
    vertex* frmNode;
    Type* gate;
    edge* inEdg;
    edge* outEdg;
    int i, n, K;
    double CL, td;
    double a0, a1, a2, a3, absCL;
    char* gateName;


    outEdgArr = vert->outEdges;
    inEdgArr = vert->inEdges;
    outEdg = array_fetch_last(edge*, outEdgArr);


    /* Finding Delay */
    gate = vert->type;


        // Calculate load capacitance, CL //
        CL = findCLStd(G, outEdg, gate, input, &absCL, Vdd);
          if(strncasecmp(gate->name, "i1", 2) == 0){
            if(outTran == 01){
                a3 = -0.2152e-5;
                a2 = 0.2099e-5;
                a1 = -0.0689e-5;
                a0 = 0.0079e-5;
            }
            else if(outTran == 10){
                    a3 = -0.3823e-5;
                    a2 = 0.3703e-5;
                    a1 = -0.1195e-5;
                    a0 = 0.0132e-5;
                        }

        }
        else if(strncasecmp(gate->name, "nnd", 3) == 0){
            if(input == 1){
                if(outTran == 01){
                               a3 =  -0.3068e-5;
                                a2 = 0.3002e-5;
                                a1 = -0.0983e-5;
                                a0 = 0.0111e-5;
                }
                            else if(outTran == 10){
                                a3 = -0.6518e-5;
                                a2 = 0.6313e-5;
                                a1 = -0.2026e-5;
                                a0 = 0.0219e-5;
                }
            }
            else if(input == 2) {
                if(outTran == 01){
                                a3 = -0.2814e-5;
                                a2 = 0.2805e-5;
                                a1 = -0.0939e-5;
                                a0 = 0.0109e-5;
                }
                                else if(outTran == 10){
                                a3 = -0.5597e-5;
                                a2 = 0.5499e-5;
                                a1 = -0.1799e-5;
                                a0 = 0.0200e-5;
                }
            }
                }
        else if(strncasecmp(gate->name, "nor", 3) == 0){
                        if(input == 1){
                                if(outTran == 01){
                                a3 = -0.7836e-5;
                                a2 = 0.7479e-5;
                                a1 = -0.2354e-5;
                                a0 = 0.0248e-5;
                }
                                else if(outTran == 10){
                                a3 =-0.4079e-5;
                                a2 =0.4014e-5;
                                a1 =-0.1319e-5;
                                a0 =0.0149e-5;
                }

            }
                        else if(input == 2) {
                                if(outTran == 01){
                                a3 =-0.7555e-5;
                                a2 =0.7397e-5;
                                a1 =-0.2404e-5;
                                a0 =0.0264e-5;
                }
                                else if(outTran == 10){
                                a3 = 0.6161e-6;
                                a2 =-0.2777e-6;
                                a1 =-0.0539e-6;
                                a0 =0.0301e-6;
                }
            }
                }
      td = (a3*pow(Vdd, 3) + a2*pow(Vdd, 2) + a1*Vdd + a0)*CL;
          	//printf("\nOPT  td = %e CL=%e\n",td, CL);
	  vert->delay = td;
        return td;
}

/*  Calculate the equivalent load capacitance, CL */
double findCLStd(graph* G, edge* outEdg, Type* gate, int input, double* absCL, float vdd)
{
    array_t*  outNodeArr;
    array_t*  inEdgArr;
    edge*     inEdg;
    vertex*   outNode;
    vertex*   inNode;
    Type*     outGate;
    Type*     inGate;
    char*     outGateName;
    double    capOut;     // Fanout Gate capacitance //
    double    delCapOut;  // Gate diffusion cap. based on input location //
    double    CL;         // Total Load Capacitance  //
    int       i;

    double  CL1, CLnorm;

    inNode = outEdg->fromNode;

    /* Calculation of Output Gate capacitance */
    outNodeArr = outEdg->toNodes;
    capOut = 0;
    if(array_n(outNodeArr) == 0)     // Primary Output //
        capOut = 1e-15;
    else
    {
        for(i = 0; i < array_n(outNodeArr); i++)
        {
            outNode = array_fetch(vertex*, outNodeArr, i);
            outGate = outNode->type;

            outGateName = outGate->name;
            if(strncasecmp(outGateName, "i1", 2) == 0){
                capOut += 2.5*(0.0286*vdd*vdd - 0.0974* vdd +2.7484);
            }
            else if(strncasecmp(outGateName, "nnd", 3) == 0){
                capOut += 3*(0.0286*vdd*vdd - 0.0974* vdd +2.7484);
            }
            else if(strncasecmp(outGateName, "nor", 3) == 0){
                capOut += 4.5*(0.0286*vdd*vdd - 0.0974* vdd +2.7484);
            }
            else
                printf("GATE NOT FOUND \n");

        }
    }

    /* Calculation of equivalent load capacitance based on input location */
    // inNode = outEdg->fromNode;
    inGate = inNode->type;
        inEdgArr = inNode->inEdges;


    delCapOut = 0;        // Initialize //


    if(strncasecmp(inGate->name, "i1", 2) == 0)
        {
                        delCapOut = 2.5*(0.0394 *vdd*vdd - 0.3223*vdd + 1.1967);

        }
    else if(strncasecmp(inGate->name, "nnd", 3) == 0)
    {
        if(input == 1)
            delCapOut = 5*(0.0394 *vdd*vdd - 0.3223*vdd + 1.1967);
        else if(input == 2)
            delCapOut = 7*(0.0394 *vdd*vdd - 0.3223*vdd + 1.1967);

    }
    else if(strncasecmp(inGate->name, "nor", 3) == 0)
    {
        if(input == 1)
                        delCapOut = 6*(0.0394 *vdd*vdd - 0.3223*vdd + 1.1967);
                else if(input == 2)
                        delCapOut = 14*(0.0394 *vdd*vdd - 0.3223*vdd + 1.1967);
    }
    else
                                printf("GATE NOT FOUND \n");

    /* Calculation of Diffusion Capacitance */

    CL = capOut +delCapOut;

    CL1 = 2.5*(0.0286*vdd*vdd - 0.0974* vdd +1.7484)+delCapOut;

    *absCL = CL*1e-15;

    CLnorm = CL/CL1;
    return CLnorm;
}


/*  Calculate the equivalent load capacitance, CL */
double findCLOpt(graph* G, edge* outEdg, Type* gate, int input, double* absCL, float Vdd)
{
    array_t*  outNodeArr;
    array_t*  inEdgArr;
    edge*     inEdg;
    vertex*   outNode;
    vertex*   inNode;
    Type*     outGate;
    Type*     inGate;
    char*     outGateName;
    double    capOut;     // Fanout Gate capacitance //
    double    delCapOut;  // Gate diffusion cap. based on input location //
    double    CL;         // Total Load Capacitance  //
    int       i;

    double  CL1, CLnorm;

    inNode = outEdg->fromNode;

    /* Calculation of Output Gate capacitance */
    outNodeArr = outEdg->toNodes;
    capOut = 0;
    if(array_n(outNodeArr) == 0)     // Primary Output //
        capOut = 1e-15;
    else
    {
        for(i = 0; i < array_n(outNodeArr); i++)
        {
            outNode = array_fetch(vertex*, outNodeArr, i);
            outGate = outNode->type;

            outGateName = outGate->name;
            if(strncasecmp(outGateName, "i1", 2) == 0){
                capOut += 0.5*(0.0246*Vdd*Vdd - 0.0857* Vdd +1.783);
            }
            else if(strncasecmp(outGateName, "nnd", 3) == 0){
                capOut += 2*(0.0246*Vdd*Vdd - 0.0857* Vdd +1.7383);
            }
            else if(strncasecmp(outGateName, "nor", 3) == 0){
                capOut += 0.5*(0.0246*Vdd*Vdd - 0.0857* Vdd +1.783);
            }
            else
                printf("GATE NOT FOUND \n");

        }
    }

    /* Calculation of equivalent load capacitance based on input location */
    // inNode = outEdg->fromNode;
    inGate = inNode->type;
        inEdgArr = inNode->inEdges;


    delCapOut = 0;        // Initialize //


    if(strncasecmp(inGate->name, "i1", 2) == 0)
        {
                        delCapOut = 2.5*(0.0202 *Vdd*Vdd - 0.1870*Vdd + 0.7876);

        }
    else if(strncasecmp(inGate->name, "nnd", 3) == 0)
    {
        if(input == 1)
            delCapOut = 5*(0.0202 *Vdd*Vdd - 0.1870*Vdd + 0.7876);
        else if(input == 2)
            delCapOut = 7*(0.0202 *Vdd*Vdd - 0.1870*Vdd + 0.7876);

    }
    else if(strncasecmp(inGate->name, "nor", 3) == 0)
    {
        if(input == 1)
                        delCapOut = 3*(0.0202 *Vdd*Vdd - 0.1870*Vdd + 0.7876);
                else if(input == 2)
                        delCapOut = 7*(0.0202 *Vdd*Vdd - 0.1870*Vdd + 0.7876);
    }
    else
                                printf("GATE NOT FOUND \n");

    /* Calculation of Diffusion Capacitance */
    CL = capOut +delCapOut;   // absolute value of capacitance is CL * 1e-15

    CL1 = 2.5*(0.0246*Vdd*Vdd - 0.0857* Vdd +0.783) +delCapOut;

    CLnorm = CL/CL1;

    *absCL = CL*1e-15;
    return CLnorm;
}
