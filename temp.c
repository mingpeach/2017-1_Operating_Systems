#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CPUtime 50
#define SCHEDULE 0
#define IDLE -1

typedef struct process PROCESS;
typedef struct process {
	int ProcessID;
	int CPUburstTime;
	int IOburstTime;	
	int ArrivalTime;
	int Priority;
	int RemainTime;
	int WaitingTime;
	int ExecutingTime; /* to compute Turnaround time */
	int Turnaround;
	PROCESS *Next;
} PROCESS;

PROCESS *head = NULL, *tail = NULL;
PROCESS *readyHead = NULL, *readyTail = NULL;
PROCESS *endHead = NULL, *endTail = NULL;
int processNum, CPUstate;

/*--- function declaration ---*/
void CreateProcess();
void Enqueue(PROCESS *node, PROCESS **head, PROCESS **tail);

void Menu();
void Select(int select);

void PrintProcess(PROCESS **head);
void PrintEvaluation();

void SortByArrival(PROCESS **head, PROCESS **tail);
void SortByBurst(PROCESS **head, PROCESS **tail);
void SortByPriority(PROCESS **head, PROCESS **tail);
void SortByRemain(PROCESS **head, PROCESS **tail);

void CopyNode(PROCESS *node, PROCESS *new);
void CopyQueue(PROCESS **queue, PROCESS **head, PROCESS **tail);

void FCFS();

/*--- function definition ---*/
void Menu() {

	int select;
	printf("+----------------------------------------------------------------+\n");
	printf("|                    CPU Scheduling Simulator                    |\n");
	printf("+----------------------------------------------------------------+\n");
	printf("|                                                                |\n");
	printf("|   1. FCFS(First Come First Served)                             |\n");
	printf("|   2. Nonpreemptive SJF(Shortest Job First)                     |\n");
	printf("|   3. Preemptive SJF(Shortest Job First)                        |\n");
	printf("|   4. Nonpreemptive Priority                                    |\n");
	printf("|   5. Preemptive Priority                                       |\n");	
	printf("|   6. RR(Round Robin)                                           |\n");
	printf("|   7. Evaluation                                                |\n");
	printf("|   8. Exit                                                      |\n");
	printf("|                                                                |\n");
	printf("+----------------------------------------------------------------+\n\n");	
	printf("                 Select CPU Scheduling Algorithm : ");
	scanf("%d", &select);
	Select(select);

}

void Select(int select) {

	switch(select) {
		case 1: 
			//FCFS
			break;
		case 2:
			//NPSJF
			break;
		case 3:
			//PSJF
			break;
		case 4:
			//NPPriority
			break;
		case 5:
			//PPriority
			break;
		case 6:
			//RR
			break;
		case 7:
			//Evaluation
			break;
		case 8:
			//Exit
			break;
		default:
			//Error
			break;
	}

}

void CreateProcess() {

	int i;
	long seed;

	head = NULL, tail = NULL;
	PROCESS *node;

	seed = time(NULL);
    srand(seed);

	//srand(time(NULL));

	printf("+----------------------------------------------------------------+\n");
	printf("|                         ProcessCreation                        |\n");
	printf("+----------------------------------------------------------------+\n");
	printf("                 Enter the number of process : ");
	scanf("%d", &processNum);

	for(i = 0;i < processNum; i++) {
		node = (PROCESS *)malloc(sizeof(PROCESS));
		node->ProcessID = i+1;
		node->CPUburstTime = rand() % CPUtime + 1;
		node->IOburstTime = 0;
		node->ArrivalTime = rand() % CPUtime + 1;
		node->Priority = rand() % processNum + 1;
		node->RemainTime = node->CPUburstTime;
		node->WaitingTime = 0;
		node->ExecutingTime = 0;
		node->Turnaround = 0;
		node->Next = NULL;
		Enqueue(node, &head, &tail);
	}

}

void Enqueue(PROCESS *node, PROCESS **head, PROCESS **tail) {

	if(*head == NULL) /* if queue is empty */
		*head = node;
	else (*tail)->Next = node;
	*tail = node; 

}

void PrintProcess(PROCESS **head) {

	printf("+-----+-------+--------+--------+-------+------+------+----------+\n");
	printf("| PID |Arrival|Priority|CPUburst|IOburst|Remain| Wait |Turnaround|\n");
	printf("+-----+-------+--------+--------+-------+------+------+----------+\n");	

	for(; *head != NULL; *head = (*head)->Next) {
		printf("| %3d |  %3d  |  %3d   |  %3d   |  %3d  | %3d  | %4d |   %3d    |\n", 
			(*head)->ProcessID, (*head)->ArrivalTime, (*head)->Priority, (*head)->CPUburstTime,
			(*head)->IOburstTime, (*head)->RemainTime, (*head)->WaitingTime, (*head)->Turnaround);
		printf("+-----+-------+--------+--------+-------+------+------+----------+\n");
	}

}

void SortByArrival(PROCESS **head, PROCESS **tail) {

	PROCESS *FRONT, *MID, *END; /* for swap */
	PROCESS *shead, *stail, *temp; 
	int i;

	/* shead -> head -> ... -> tail -> stail */
	shead = (PROCESS *)malloc(sizeof(PROCESS));
	stail = (PROCESS *)malloc(sizeof(PROCESS));
	(*tail)->Next = (PROCESS *)malloc(sizeof(PROCESS));

	shead->Next = *head;
	stail = (*tail)->Next;
	stail->Next = NULL;

	/* bubble sort */
	for (i=0; i<processNum; i++)
	{
		FRONT = shead;
		MID = FRONT->Next;
		END = MID->Next;

		while(END!=stail) {

			if(MID->ArrivalTime > END->ArrivalTime) {
				FRONT->Next = END;
				MID->Next = END->Next;
				END->Next = MID;
			}
			FRONT = FRONT->Next;
			MID = FRONT->Next;
			END = MID->Next;
		}
	}

	/* set head and tail */
	*head = shead->Next;
	MID->Next = NULL; /* to cut off stail */
	*tail = FRONT->Next;

	free(shead);
	free(stail);

}

void SortByBurst(PROCESS **head, PROCESS **tail) {

	PROCESS *FRONT, *MID, *END; /* for swap */
	PROCESS *shead, *stail, *temp; 
	int i;

	/* shead -> head -> ... -> tail -> stail */
	shead = (PROCESS *)malloc(sizeof(PROCESS));
	stail = (PROCESS *)malloc(sizeof(PROCESS));
	(*tail)->Next = (PROCESS *)malloc(sizeof(PROCESS));

	shead->Next = *head;
	stail = (*tail)->Next;
	stail->Next = NULL;

	/* bubble sort */
	for (i=0; i<processNum; i++)
	{
		FRONT = shead;
		MID = FRONT->Next;
		END = MID->Next;

		while(END!=stail) {

			if(MID->CPUburstTime > END->CPUburstTime) {
				FRONT->Next = END;
				MID->Next = END->Next;
				END->Next = MID;
			}
			FRONT = FRONT->Next;
			MID = FRONT->Next;
			END = MID->Next;
		}
	}

	/* set head and tail */
	*head = shead->Next;
	MID->Next = NULL; /* to cut off stail */
	*tail = FRONT->Next;

	free(shead);
	free(stail);

}

void SortByPriority(PROCESS **head, PROCESS **tail) {

	PROCESS *FRONT, *MID, *END; /* for swap */
	PROCESS *shead, *stail, *temp; 
	int i;

	/* shead -> head -> ... -> tail -> stail */
	shead = (PROCESS *)malloc(sizeof(PROCESS));
	stail = (PROCESS *)malloc(sizeof(PROCESS));
	(*tail)->Next = (PROCESS *)malloc(sizeof(PROCESS));

	shead->Next = *head;
	stail = (*tail)->Next;
	stail->Next = NULL;

	/* bubble sort */
	for (i=0; i<processNum; i++)
	{
		FRONT = shead;
		MID = FRONT->Next;
		END = MID->Next;

		while(END!=stail) {

			if(MID->Priority > END->Priority) {
				FRONT->Next = END;
				MID->Next = END->Next;
				END->Next = MID;
			}
			FRONT = FRONT->Next;
			MID = FRONT->Next;
			END = MID->Next;
		}
	}

	/* set head and tail */
	*head = shead->Next;
	MID->Next = NULL; /* to cut off stail */
	*tail = FRONT->Next;

	free(shead);
	free(stail);

}

void SortByRemain(PROCESS **head, PROCESS **tail) {

	PROCESS *FRONT, *MID, *END; /* for swap */
	PROCESS *shead, *stail, *temp; 
	int i;

	/* shead -> head -> ... -> tail -> stail */
	shead = (PROCESS *)malloc(sizeof(PROCESS));
	stail = (PROCESS *)malloc(sizeof(PROCESS));
	(*tail)->Next = (PROCESS *)malloc(sizeof(PROCESS));

	shead->Next = *head;
	stail = (*tail)->Next;
	stail->Next = NULL;

	/* bubble sort */
	for (i=0; i<processNum; i++)
	{
		FRONT = shead;
		MID = FRONT->Next;
		END = MID->Next;

		while(END!=stail) {

			if(MID->RemainTime > END->RemainTime) {
				FRONT->Next = END;
				MID->Next = END->Next;
				END->Next = MID;
			}
			FRONT = FRONT->Next;
			MID = FRONT->Next;
			END = MID->Next;
		}
	}

	/* set head and tail */
	*head = shead->Next;
	MID->Next = NULL; /* to cut off stail */
	*tail = FRONT->Next;

	free(shead);
	free(stail);

}

void CopyNode(PROCESS *node, PROCESS *new) {

	new->ProcessID = node->ProcessID;
	new->CPUburstTime = node->CPUburstTime;
	new->IOburstTime = node->IOburstTime;	
	new->ArrivalTime = node->ArrivalTime;
	new->Priority = node->Priority;
	new->RemainTime = node->RemainTime;
	new->WaitingTime = node->WaitingTime;
	new->ExecutingTime = node->ExecutingTime;
	new->Turnaround = node->Turnaround;
	new->Next = NULL;	

}

void CopyQueue(PROCESS **queue, PROCESS **head, PROCESS **tail) {

	PROCESS *node;
	for(; *queue != NULL; *queue = (*queue)->Next) {
		node = (PROCESS *)malloc(sizeof(PROCESS));
		CopyNode(*queue, node);
		Enqueue(node, head, tail);
	}

}

/*
void Init(PROCESS **head, PROCESS **tail) {

	int i;

	for(i = 0;i < processNum; i++) {
		head->RemainTime = head->CPUburstTime;
		head->WaitingTime = 0;
		head->ExecutingTime = 0;
		head->Turnaround = 0;
		head = head->Next;
	}

}
*/

void FCFS() {

	SortByArrival(&head, &tail);

	/* Process Queue */
	PROCESS *processHead = NULL, *processTail = NULL;
	PROCESS *processQueue = head, *node, *readyNode;

	int time = 0, endNum = 0;
	CPUstate = IDLE;

	/* Initialize ReadyQueue */
	readyHead = NULL;
	readyTail = NULL;

	/* Copy ProcessQueue */
	CopyQueue(&processQueue, &processHead, &processTail);

	printf("+----------------------------------------------------------------+\n");
	printf("|                     FCFS Scheduling Report                     |\n");
	printf("+----------------------------------------------------------------+\n");
	printf("************************** Gantt Chart ***************************\n\n");

	while(endNum < processNum) {

		processQueue = processHead;

		/*  Insert ReadyQueue - Job Scheduling */
		while(processQueue != NULL && processQueue->ArrivalTime == time) {
			node = (PROCESS *)malloc(sizeof(PROCESS));
			CopyNode(processQueue, node);
			Enqueue(node, &readyHead, &readyTail);

			/* remove job scheduled process from process list */
			processQueue = processQueue->Next; 

			/* next process */
			processHead = processHead->Next;
		}

		/* remove end process from readyqueue and make CPU idle */
		if(CPUstate == SCHEDULE) {
			CPUstate = IDLE;
			node = (PROCESS *)malloc(sizeof(PROCESS));
			CopyNode(readyHead, node);
			Enqueue(node, &endHead, &endTail);
			/* remove end process from ready queue */
			readyHead = readyHead->Next; 
		}

		/* CPU Scheduling */
		readyNode = readyHead;
		while(readyNode != NULL) {

			/* current executing process */
			if(CPUstate == readyNode->ProcessID) {
				printf("-");
				/* end of execute */
				if(readyNode->RemainTime == 1) {
					readyNode->Turnaround = readyNode->WaitingTime + readyNode->ExecutingTime + 1;
					endNum++;
					CPUstate = SCHEDULE;
				}				
				readyNode->ExecutingTime++;
				readyNode->RemainTime--;
			}

			/* cpu state is idle */
			else if(CPUstate == IDLE) {
				printf("%d",readyNode->ProcessID);
				CPUstate = readyNode->ProcessID;
				/* end of execute */
				if(readyNode->RemainTime == 1) {
					readyNode->Turnaround = readyNode->WaitingTime + readyNode->ExecutingTime + 1;
					endNum++;
					CPUstate = SCHEDULE;
				}				
				readyNode->ExecutingTime++;
				readyNode->RemainTime--;
			}

			/* waiting process */
			else {
				readyNode->WaitingTime++;
			}

			readyNode = readyNode->Next;
		}

		if(readyHead==NULL && CPUstate==IDLE) {
			printf(" ");
		}

		time++;
	}

	node = (PROCESS *)malloc(sizeof(PROCESS));
	CopyNode(readyHead, node);
	Enqueue(node, &endHead, &endTail);
	printf("\n\n");

	printf("**************************** Result ******************************\n\n");
	PrintProcess(&endHead);

}










int main(void) {

	PROCESS *temp;
	CreateProcess();
	
	temp = head;
	PrintProcess(&temp);
	FCFS();
	
	return 0;

}