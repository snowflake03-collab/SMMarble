//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#if 0
각 노드에서 뭘 하는지 printf를 통해 표시해주자
뭘 하고 돌아다니는지를 모르겠단 말이지...

, 턴 시작 시 모든 플레이어의 상태 (위치, 실험
중 상태, 누적 학점, 현재 에너지)를 출력해야 함
-> 어랏? 턴 종료시가 아니었어??
아!! g를 눌렀을 경우,, 아닌가? 그거는

구현해야 할 부분
- 게임 종료 조건과 종료 시 동작은 다음과 같음
? 플레이어 중 한명이 GRADUATE_CREDIT 이상의 학점을 이수하고 집으로 이동하면 게임이 즉시 종료됨
? 게임 종료와 함께 졸업한 플레이어가 수강한 강의의 이름, 학점, 성적을 각각 출력


#endif


#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

//board configuration parameters 
static int smm_board_nr;
static int smm_food_nr;
static int smm_festival_nr;
static int smm_player_nr;

//각 player들의 정보(이름,위치,학점,졸업여부,에너지)를 담은 구조체 
typedef struct{
        char name[MAX_CHARNAME];
        int pos;
        int credit;
        int flag_graduated;
        int energy;
        int is_experimenting; //실험 중인지 아닌지를 저장할 변수 
} smm_player_t;

//동적 메모리 할당 
smm_player_t *smm_players;

//prototyping
void generatePlayers(int n, int initEnergy); //generate a new player & 초기 에너지 설정 
void printPlayerStatus(void); //print all player status at the beginning of each turn
void printGrades(int player); //print all the grade history of the player

//function prototypes
#if 0
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
#endif


void printGrades(int player) //print grade history of the player
{
     int i; 
     int size = smmdb_len(LISTNO_OFFSET_GRADE + player);
     
     //그 player가 수강한 모든 과목과 점수를 출력
     printf("%s의 수강한 과목 목록\n", smm_players[player].name); 
     for(i = 0; i < size; i++)
     {
           void* gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
     
           printf("과목명: %s - 성적 : %s \n", smmObj_getObjectName(gradePtr), smmObj_getObjectgrade(gradePtr)); 
     } 
     printf("현재 %s의 총 수강 학점: %d / 졸업학점(%d)\n",  smm_players[player].name , smm_players[player].credit, GRADUATE_CREDIT);
}

void* findGrade(int player, char *lectureName) //find the grade from the player's grade history
{
      int size = smmdb_len(LISTNO_OFFSET_GRADE+player);
      int i;
      
      for(i = 0; i < size; i++)
      {
          void *ptr = smmdb_getData(LISTNO_OFFSET_GRADE+player, i);
          if (strcmp((char*)smmObj_getObjectName(ptr), lectureName) == 0) //strcmp를 통해 그 과목 들었었는지 검사 
          {
              return ptr;
          }
      }
      //안 들었던 과목이면 NULL 
      return NULL;
}


int isGraduated(void) //check if any player is graduated
{
    int i;
    for(i = 0; i < smm_player_nr; i++)
    {
          //if found graduated player, end the process 
          if(smm_players[i].flag_graduated == 1)
              return 1; 
    } 
    
    return 0;
}


void goForward(int player, int step)
{
    //make player go "step" steps on the board (check if player is graduated)
    int i;
    void *ptr;
    void *current_node_ptr;
    
    //current 위치 확인 후 출력하기 
    ptr = smmdb_getData(LISTNO_NODE, smm_players[player].pos);
    printf("start from %i(%s)\n", smm_players[player].pos, smmObj_getObjectName(ptr));

    for (i = 0; i < step; i++)
    {   
        //1칸씩 나온 횟수 채울 때까지 이동하면서 어디 있는지 출력 
        smm_players[player].pos = (smm_players[player].pos + 1) % smm_board_nr;
        
        //우리집의 경우 지나치기만 해도 energy 충전 
        if(smm_players[player].pos == 0)
        {
             void* home_ptr = smmdb_getData(LISTNO_NODE, 0);
             int home_energy = smmObj_getObjectEnergy(home_ptr);
             smm_players[player].energy += home_energy;
              printf("%s가 집을 지나치면서 에너지를 %d만큼 충전\n", smm_players[player].name, home_energy); 
        }
        
    
        current_node_ptr = smmdb_getData(LISTNO_NODE, smm_players[player].pos);
        printf("    => moved to %i(%s)\n", smm_players[player].pos, smmObj_getObjectName(current_node_ptr));
    }
}


void printPlayerStatus(void) //print all player status at the beginning of each turn
{
     int i;
     
     for(i = 0; i < smm_player_nr ; i++)
     {
         void* ptr = smmdb_getData(LISTNO_NODE, smm_players[i].pos);
 
         printf("%s - position:%i(%s), credit:%i, energy:%i\n", 
         smm_players[i].name, 
         smm_players[i].pos, 
         smmObj_getObjectName(ptr), 
         smm_players[i].credit, 
         smm_players[i].energy);
     }
}


void generatePlayers(int n, int initEnergy) //generate a new player
{
     int i;
     //새 플레이어 만들기 위해 동적 메모리 할당 
     smm_players = malloc(n*sizeof(smm_player_t));
     
     //반복문 이용해서 초기화 
     for(i = 0; i < n; i++)
     {
         smm_players[i].pos = 0;
         smm_players[i].credit = 0;
         smm_players[i].energy = initEnergy;
         smm_players[i].flag_graduated = 0;
         
         printf("Input %i-th player name:",i+1);
         scanf("%s", &smm_players[i].name[0]);
         fflush(stdin);
     }      
}


int rolldie(int player)
{
    char c;
    while(1)
    {
         printf(" Press any key to roll a die (press g to see grade): ");
         c = getchar();
         while( getchar() != '\n');
         printPlayerStatus();
    
         if (c == 'g')
         {
               int i;
               for(i = 0; i < smm_player_nr; i++)
               {
                    printGrades(i);
               }
               continue;
         }
         else
         {
              break;
         }
    }
    return (rand()%MAX_DIE + 1);
}

//action code when a player stays at a node
void actionNode(int player)
{
    void *ptr = smmdb_getData(LISTNO_NODE,smm_players[player].pos);
    int type = smmObj_getObjectType(ptr); 
    int credit = smmObj_getObjectCredit(ptr);
    int energy = smmObj_getObjectEnergy(ptr);
    int grade; 
    void *gradePtr;
    
    switch(type)
    {
        
        case SMMNODE_TYPE_LECTURE:
             
        
        if(findGrade(player, smmObj_getObjectName(ptr)) == NULL) //전에 들었던 강의는 듣지 않음 
        {
             if(credit > 0)
             { 
                  int lecture_registor = rand() % 2;
                  
                  if(lecture_registor == 1)
                  { 
                       printf("해당 강의를 수강합니다.\n"); 
                       smm_players[player].credit += credit; 
                       smm_players[player].energy -= energy;
             
                       grade = rand() % SMMNODE_MAX_GRADE;
             
                       gradePtr = smmObj_genObject(smmObj_getObjectName(ptr), SMMNODE_OBJTYPE_GRADE, 
                       type , credit, energy, grade);
                  
                       smmdb_addTail(LISTNO_OFFSET_GRADE+player, gradePtr);
                  }
                  else
                  {
                       printf("강의를 드랍합니다.\n"); 
                  }
             }
        }
             break;
             
        case SMMNODE_TYPE_RESTUARANT:     
             smm_players[player].energy += energy;
             break;
             
        case SMMNODE_TYPE_LABORATORY: 
             
             printf("실험실 입장"); 
             
             smm_players[player].is_experimenting = 1; 
             smm_players[player].energy -= energy;
             break;
             
        case SMMNODE_TYPE_HOME:       
             smm_players[player].energy += energy;
             if(smm_players[player].credit >= GRADUATE_CREDIT)
             {
                 smm_players[player].flag_graduated = 1; 
                 printf("졸업!");                  
             }
             break;
             
        case SMMNODE_TYPE_GOTOLAB:     
             {
                   int i;
                   smm_players[player].is_experimenting = 1;
                   printf("   %s가 실험실(%d)로 이동함.\n", smm_players[player].name, smm_players[player].pos);
                   for(i = 0; i < smm_board_nr; i++)
                   {
                        void* node_ptr = smmdb_getData(LISTNO_NODE, i);
                        if(smmObj_getObjectType(node_ptr) == SMMNODE_TYPE_LABORATORY)
                        {
                              smm_players[player].pos = i;
                              break;                                   
                        }
                   }     
             
             }
             break;
              
        case SMMNODE_TYPE_FOODCHANGE:
             {
                  //choose random food 
                  int chosen_Food = rand() % smm_food_nr; 
                  //bring that food's address
                  void* food_ptr = smmdb_getData(LISTNO_FOODCARD, chosen_Food);
                  
                  if(food_ptr != NULL)
                  {
                  int food_energy = smmObj_getObjectEnergy(food_ptr);
                  char* food_name = smmObj_getObjectName(food_ptr);
                  
                  smm_players[player].energy += food_energy;
                  printf(" [%s]를 섭취, %s의 에너지가 %d만큼 보충됨\n", 
                           food_name, smm_players[player].name, food_energy);
                  }
             } 
             break;
             
             
        case SMMNODE_TYPE_FESTIVAL:       
             {
                  int chosen_FEST = rand() % smm_festival_nr;
                  
                  void* fest_ptr = smmdb_getData(LISTNO_FESTCARD, chosen_FEST);             
                         
                  if(fest_ptr != NULL)
                  {      
    
                  char* fest_content = smmObj_getObjectName(fest_ptr);
                  printf("%s의 축제 미션: %s \n", smm_players[player].name, fest_content); 
                  } 
             }
             break;
             

        //case lecture: 
        default:
            break;
    }
}



int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int turn;
    
    smm_board_nr = 0;
    smm_food_nr = 0;
    smm_festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n"); 
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        printf("%s %i %i %i\n", name, type, credit, energy); 
        void* ptr = smmObj_genObject(name, SMMNODE_OBJTYPE_BOARD, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, ptr);
        smm_board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", smm_board_nr);
    
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
 
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        //store the parameter set
        printf("%s %i\n", name, energy); 
        void* ptr = smmObj_genObject(name, SMMNODE_OBJTYPE_FOOD, 0, 0, energy, 0);
        smmdb_addTail(LISTNO_FOODCARD, ptr);  
        smm_food_nr++; //이렇게 세는게... 으음 최선? 
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", smm_food_nr);
    
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name) == 1) //read a festival card string
    {  
        //store the parameter set
        printf("%s\n", name);
        void* ptr = smmObj_genObject(name, SMMNODE_OBJTYPE_FEST, 0, 0, 0, 0);
        smmdb_addTail(LISTNO_FESTCARD, ptr); 
        smm_festival_nr++; //세라는 지시가 있었나?? 설명 다시 보자... 
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", smm_festival_nr);
    
    

    //2. Player configuration ---------------------------------------------------------------------------------

    do
    {
        //input player number to player_nr
        printf("Input player number:");
        scanf("%i", &smm_player_nr);
        fflush(stdin); 
        
        if(smm_player_nr <= 0 || smm_player_nr > MAX_PLAYER)
            printf("Invalid player number!\n");
    }
    while (smm_player_nr <= 0 || smm_player_nr > MAX_PLAYER);
    
    

    generatePlayers(smm_player_nr, smmObj_getObjectEnergy(smmdb_getData(LISTNO_NODE, 0)));

    

    turn = 0;
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (isGraduated() == 0) //is anybody graduated?
    {  
         int die_result;
         
         //4-1. initial printing
         printPlayerStatus();

         //if in experiment
         if(smm_players[turn].is_experimenting == 1)
         {
              printf("실험을 해야해!!!!"); 
              //실험 중일 경우  
              int success_exper = 3;
              int die = (rand()%MAX_DIE + 1);
              die_result = die;
              
              //실험 시되마다 에너지 소모
              smm_players[turn].energy -= 3;
              printf("실험 시도, 에너지 소모"); 
              
              //주사위 굴려서 기준값과 비교, 이상일 경우 실험 종료 
              printf("실험 종료 가능?"); 
              if(die >= success_exper)
              {
                   printf("결과값: %d, 실험 종료", die);
                   smm_players[turn].is_experimenting = 0;
              } 
              else
              {
                  //실험을 종료하지 못할 경우 이 노드는 여전히 실험실 노드에 머무름 
                   printf("결과값: %d, 여전히 실험중", die); 
                   turn = (turn + 1) % smm_player_nr;
                   continue;
              }
          }
          else
          {           
              //4-2. die rolling (if not in experiment)
              die_result = rolldie(turn);    
          }
          
        
        //4-3. go forward
        goForward(turn, die_result);
        

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn + 1)% smm_player_nr;
    }
    


    free(smm_players);

    system("PAUSE");
    return 0;
}
