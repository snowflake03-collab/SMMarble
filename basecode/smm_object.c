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
#define MAX_NAME        40

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
char* smmObj_getNodeName(int (node_nr))
{
      return(smmObj_board[node_nr].name);      
}

int smmObj_getNodeType(int (node_nr))
{
    return (smmObj_board[node_nr].type);
}
    
int smmObj_getNodeCredit(int (node_nr))
{
    return (smmObj_board[node_nr].credit);
}

int smmObj_getNodeEnergy(int (node_nr))
{
    return (smmObj_board[node_nr].energy);
    //어라, energy가 18로 나와야 하는데 credit이 나오는데... 
}

char* smmObj_gettypeName(int node_type)
{
      return (smmObj_NodeName[node_type]);
}


//food 카드 
static int smm_food_nr = 0;

typedef struct
{
    char name[MAX_NAME];
    int energy;
} smm_Food_t;

static smm_Food_t smm_Food[MAX_NAME];

int smmObj_genFood(char* name, int energy)
{
    strcpy(smm_Food[smm_food_nr].name, name);
    smm_Food[smm_food_nr].energy = energy;
    
    smm_food_nr++;
    
    return smm_food_nr;
}

//festival 카드
static int smm_festival_nr = 0;

typedef struct
{
    char name[MAX_NAME]; //그냥 NODENR 매크로 같이 써도 되지 않아?? 
} smm_festival_t;

static smm_festival_t smm_festival[MAX_NAME];

int smmObj_genFestival(char* name)
{
    strcpy(smm_Food[smm_food_nr].name, name);
    
    smm_festival_nr++;
    
    return smm_festival_nr;
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
