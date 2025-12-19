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


#define GRADE_Ap        0
#define GRADE_A0        1
#define GRADE_Am        2
#define GRADE_Bp        3
#define GRADE_B0        4
#define GRADE_Bm        5
#define GRADE_Cp        6
#define GRADE_C0        7
#define GRADE_Cm        8
#define GRADE_Dp        9
#define GRADE_D0        10
#define GRADE_Dm        11
#define GRADE_F         12


static char smmObj_NodeName[SMMNODE_MAX_GRADE][MAX_CHARNAME] = {
       "A+",
       "A0",
       "A-",
       "B+",
       "B0",
       "B-",
       "C+",
       "C0",
       "C-",
       "D+",
       "D0",
       "D-",
       "F"
};

static char smmObj_GradeName[MAX_NODETYPE][MAX_CHARNAME] = {
       "lecture",
       "restaurant",
       "laboratory",
       "home",
       "gotoLab",
       "foodChance",
       "festival"
};


//structure type definition 
typedef struct{
        char name[MAX_CHARNAME];
        int objType;
        int type;
        int credit;
        int energy;
        int grade;
} smmObj_object_t; 

//object generation
void* smmObj_genObject(char* name, int objType, int type, int credit, int energy, int grade)
{
    smmObj_object_t* ptr;
    ptr = (smmObj_object_t*)malloc(sizeof(smmObj_object_t));
    
    strcpy(ptr -> name, name);
    ptr -> type = type;
    ptr -> objType = objType;
    ptr -> credit = credit;
    ptr -> energy = energy;
    ptr -> grade = grade;
    
    return ((void*)ptr);
}


//member retrieving
char* smmObj_getObjectName(void *ptr)
{
      smmObj_object_t* objPtr = (smmObj_object_t*)ptr; 
      return(objPtr -> name);      
}

int smmObj_getObjectType(void *ptr)
{
     smmObj_object_t* objPtr = (smmObj_object_t*)ptr; 
     return (objPtr -> objType);
}

int smmObj_getObjectCredit(void *ptr)
{
     smmObj_object_t* objPtr = (smmObj_object_t*)ptr; 
     return (objPtr -> credit);
}

int smmObj_getObjectEnergy(void *ptr)
{
     smmObj_object_t* objPtr = (smmObj_object_t*)ptr; 
     return (objPtr -> energy);
}

int smmObj_getObjectgrade(void *ptr)
{
      smmObj_object_t* objPtr = (smmObj_object_t*)ptr; 
      return (objPtr -> grade);
}

#if 0
char* smmObj_getTypeName(int node_type)
{
      smmObj_object_t* objPtr = (smmObj_object_t*)ptr; 
      return (objPtr -> type);     
}
#endif



#if 0
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
#endif

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
