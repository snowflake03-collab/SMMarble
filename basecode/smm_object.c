//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODENR      100
#define MAX_NODETYPE    7
#define MAX_GRADE       9

#define SMMNODE_TYPE_LECTURE       0
#define SMMNODE_TYPE_RESTUARANT    1
#define SMMNODE_TYPE_LABORATORY    2
#define SMMNODE_TYPE_HOME          3
#define SMMNODE_TYPE_GOTOLAB       4
#define SMMNODE_TYPE_FOODCHANGE    5
#define SMMNODE_TYPE_FESTIVAL      6

static char smmObj_NodeName[MAX_NODETYPE][MAX_CHARNAME] = {
       "lecture",
       "restaurant",
       "laboratory",
       "home",
       "gotoLab",
       "foodChance",
       "festival"
};

static int smmObj_nodeNr = 0;

#if 0
static int smm_nodeNr = 0;
static char smm_name[MAX_NODENR][MAX_CHARNAME];
static int smm_type[MAX_NODENR];
static int smm_credit[MAX_NODENR];
static int smm_energy[MAX_NODENR];
#endif

typedef struct{
        char name[MAX_CHARNAME];
        int type;
        int credit;
        int energy;
} smmObj_board_t;

//structure instance array deifinition
static smmObj_board_t smmObj_board[MAX_CHARNAME];


//object generation
int smmObj_genNode(char* name, int type, int credit, int energy)
{
    strcpy(smmObj_board[smmObj_nodeNr].name, name);
    smmObj_board[smmObj_nodeNr].type = type;
    smmObj_board[smmObj_nodeNr].credit = credit;
    smmObj_board[smmObj_nodeNr].energy = energy;
    
    smmObj_nodeNr++;
    
    return (smmObj_nodeNr);
}



//member retrieving
char* smmObj_getNodeName(int node_nr)
{
      return(smmObj_board[smmObj_nodeNr].name);      
}

int smmObj_getNodeType(int (node_nr))
{
    return (smmObj_board[smmObj_nodeNr].type);
}

int smmObj_getNodeEnergy(int node_nr)
{
    return (smmObj_board[smmObj_nodeNr].energy);
}

char* smmObj_gettypeName(int node_type)
{
      return (smmObj_NodeName[node_type]);
}




#if 0
//element to string
char* smmObj_getNodeName(smmNode_e type)
{
    return smmNodeName[type];
}

char* smmObj_getGradeName(smmGrade_e grade)
{
    return smmGradeName[grade];
}
#endif
