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
int processNum, CPUstate, quantum;

double waiting[6] = {0,0,0,0,0,0};
double turnaround[6] = {0,0,0,0,0,0};

/*--- function declaration ---*/
void CreateProcess();
void Enqueue(PROCESS *node, PROCESS **head, PROCESS **tail);

void Menu();
void Select(int select);

void PrintProcess(PROCESS **head);
void PrintEval();
void PrintMin(int result);
void InitEval();

void SortByArrival(PROCESS **head, PROCESS **tail);
void SortByBurst(PROCESS **head, PROCESS **tail);
void SortByPriority(PROCESS **head, PROCESS **tail);
void SortByRemain(PROCESS **head, PROCESS **tail);

void CopyNode(PROCESS *node, PROCESS *new);
void CopyQueue(PROCESS **queue, PROCESS **head, PROCESS **tail);

void FCFS();
void NPSJF();
void NPPriority();

void PSJF();
void PPriority();
void RR();

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
			InitEval();
			FCFS();		
			break;
		case 2:
			InitEval();
			NPSJF();
			break;
		case 3:
			InitEval();
			PSJF();
			break;
		case 4:
			InitEval();
			NPPriority();
			break;
		case 5:
			InitEval();
			PPriority();
			break;
		case 6:
			InitEval();
			RR();
			break;
		case 7:
			InitEval();		
			FCFS();
			NPSJF();
			PSJF();
			NPPriority();
			PPriority();
			RR();
			PrintEval();
			break;
		case 8:
			//Exit
			exit(0);
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

	PROCESS *temp;
	temp = head;
	PrintProcess(&temp);
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

void PrintEval() {

	int i, result = 0;
	double minWaiting;
	double minTurnaround;

	printf("+----------------------------------------------------------------+\n");
	printf("|                          Final Report                          |\n");
	printf("+----------------------------------------------------------------+\n");
	printf("| FCFS Average Waiting Time = %.2lf                             \n", waiting[0]/(double)processNum);
	printf("| FCFS Average Turnaround Time = %.2lf                          \n", turnaround[0]/(double)processNum);
	printf("+----------------------------------------------------------------+\n");
	printf("| NonPreemptive SJF Average Waiting Time = %.2lf                \n", waiting[1]/(double)processNum);
	printf("| NonPreemptive SJF Average Turnaround Time = %.2lf             \n", turnaround[1]/(double)processNum);
	printf("+----------------------------------------------------------------+\n");
	printf("| NonPreemptive Priority Average Waiting Time = %.2lf           \n", waiting[2]/(double)processNum);
	printf("| NonPreemptive Priority Average Turnaround Time = %.2lf        \n", turnaround[2]/(double)processNum);
	printf("+----------------------------------------------------------------+\n");
	printf("| Preemptive SJF Average Waiting Time = %.2lf                   \n", waiting[3]/(double)processNum);
	printf("| Preemptive SJF Average Turnaround Time = %.2lf                \n", turnaround[3]/(double)processNum);
	printf("+----------------------------------------------------------------+\n");
	printf("| Preemptive Priority Average Waiting Time = %.2lf              \n", waiting[4]/(double)processNum);
	printf("| Preemptive Priority Average Turnaround Time = %.2lf           \n", turnaround[4]/(double)processNum);
	printf("+----------------------------------------------------------------+\n");
	printf("| Round Robin Average Waiting Time = %.2lf                      \n", waiting[5]/(double)processNum);
	printf("| Round Robin Average Turnaround Time = %.2lf                   \n", turnaround[5]/(double)processNum);
	printf("+----------------------------------------------------------------+\n");	

	minWaiting = waiting[0];
	minTurnaround = turnaround[0];

	for(i = 1; i < 6; i++) {
		if(minWaiting > waiting[i]) {
			minWaiting = waiting[i];
			result = i;
		}
	}
	printf("| Minimum Waiting Time Algorithm : ");
	PrintMin(result);

	result = 0;
	for(i = 1; i < 6; i++) {
		if(minTurnaround > turnaround[i]) {
			minTurnaround = turnaround[i];
			result = i;
		}
	}
	printf("| Minimum Turnaround Time Algorithm : ");
	PrintMin(result);
	printf("+----------------------------------------------------------------+\n");	

}

void PrintMin(int result) {
	switch(result) {
		case 0:
			printf("FCFS\n");
			break;
		case 1:
			printf("NonPreemptive SJF\n");
			break;
		case 2:
			printf("NonPreemptive Priority\n");
			break;
		case 3:
			printf("Preemptive SJF\n");
			break;
		case 4:
			printf("Preemptive Priority\n");
			break;
		case 5:
			printf("Round Robin\n");
			break;										
	}
}

void InitEval() {
	int i;
	for(i = 0; i < 6; i++) {
		waiting[i] = 0;
		turnaround [i] = 0;
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

			if(MID->Priority < END->Priority) {
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
					waiting[0] += readyNode->WaitingTime;
					turnaround[0] += readyNode->Turnaround;
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
					waiting[0] += readyNode->WaitingTime;
					turnaround[0] += readyNode->Turnaround;
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
	printf("\n  average waiting time : %.2lf average turnaround time : %.2lf \n",
		waiting[0]/(double)processNum, turnaround[0]/(double)processNum);

}

void NPSJF() {

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
	printf("|              Non-Preemptive SJF Scheduling Report              |\n");
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
			if(readyHead != NULL)
				SortByBurst(&readyHead, &readyTail);
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
					waiting[1] += readyNode->WaitingTime;
					turnaround[1] += readyNode->Turnaround;
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
					waiting[1] += readyNode->WaitingTime;
					turnaround[1] += readyNode->Turnaround;
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
	printf("\n  average waiting time : %.2lf average turnaround time : %.2lf \n",
		waiting[1]/(double)processNum, turnaround[1]/(double)processNum);
}

void NPPriority() {

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
	printf("|           Non-Preemptive Pritority Scheduling Report           |\n");
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
			if(readyHead != NULL)
				SortByPriority(&readyHead, &readyTail);
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
					waiting[2] += readyNode->WaitingTime;
					turnaround[2] += readyNode->Turnaround;
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
					waiting[2] += readyNode->WaitingTime;
					turnaround[2] += readyNode->Turnaround;
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
	printf("\n  average waiting time : %.2lf average turnaround time : %.2lf \n",
		waiting[2]/(double)processNum, turnaround[2]/(double)processNum);
}

void PSJF() {

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
	printf("|                Preemptive SJF Scheduling Report                |\n");
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

		/* sort by remain time */
		if(readyHead != NULL) {
			SortByRemain(&readyHead, &readyTail);
			CPUstate = readyHead->ProcessID;
			printf("%d", readyHead->ProcessID);
		}

		/* CPU Scheduling */
		readyNode = readyHead;
		while(readyNode != NULL) {

			/* current executing process */
			if(CPUstate == readyNode->ProcessID) {
				/* end of execute */
				if(readyNode->RemainTime == 1) {
					readyNode->Turnaround = readyNode->WaitingTime + readyNode->ExecutingTime + 1;
					waiting[3] += readyNode->WaitingTime;
					turnaround[3] += readyNode->Turnaround;
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
	printf("\n  average waiting time : %.2lf average turnaround time : %.2lf \n",
		waiting[3]/(double)processNum, turnaround[3]/(double)processNum);
}

void PPriority() {

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
	printf("|             Preemptive Priority Scheduling Report              |\n");
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

		/* sort by priority */
		if(readyHead != NULL) {
			SortByPriority(&readyHead, &readyTail);
			CPUstate = readyHead->ProcessID;
			printf("%d", readyHead->ProcessID);
		}

		/* CPU Scheduling */
		readyNode = readyHead;
		while(readyNode != NULL) {

			/* current executing process */
			if(CPUstate == readyNode->ProcessID) {
				/* end of execute */
				if(readyNode->RemainTime == 1) {
					readyNode->Turnaround = readyNode->WaitingTime + readyNode->ExecutingTime + 1;
					waiting[4] += readyNode->WaitingTime;
					turnaround[4] += readyNode->Turnaround;
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
	printf("\n  average waiting time : %.2lf average turnaround time : %.2lf \n",
		waiting[4]/(double)processNum, turnaround[4]/(double)processNum);
}

void RR() {

	SortByArrival(&head, &tail);

	/* Process Queue */
	PROCESS *processHead = NULL, *processTail = NULL;
	PROCESS *processQueue = head, *node, *readyNode;

	int time = 0, endNum = 0, timequantum = 0;
	CPUstate = IDLE;

	/* Initialize ReadyQueue */
	readyHead = NULL;
	readyTail = NULL;

	/* Copy ProcessQueue */
	CopyQueue(&processQueue, &processHead, &processTail);
	printf("+----------------------------------------------------------------+\n");
	printf("|                     Round Robin Scheduling                     |\n");
	printf("+----------------------------------------------------------------+\n");	
	printf("                      Enter quantum: ");
	scanf("%d", &quantum);

	printf("+----------------------------------------------------------------+\n");
	printf("|                 Round Robin Scheduling Report                  |\n");
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

		if(CPUstate <= 0 || timequantum == 0) {

			/* reinitiate quantum */
			timequantum = quantum;

			/* time expired - move to ready queue */
			if(CPUstate > 0) {
				node = (PROCESS *)malloc(sizeof(PROCESS));
				CopyNode(readyHead, node);
				Enqueue(node, &readyHead, &readyTail);
				readyHead = readyHead->Next;
			}
			/* done */
			else if(CPUstate == SCHEDULE) {
				CPUstate = IDLE;
				readyHead = readyHead->Next;
			}
			/* cpu scheduling */
			if(readyHead != NULL) {
				CPUstate = readyHead->ProcessID;
			}
		}

		readyNode = readyHead;
		while(readyNode != NULL) {

			/* current executing process */
			if(CPUstate == readyNode->ProcessID) {
				printf("%d", readyNode->ProcessID);
				/* end of execute */
				if(readyNode->RemainTime == 1) {
					readyNode->Turnaround = readyNode->WaitingTime + readyNode->ExecutingTime + 1;
					waiting[5] += readyNode->WaitingTime;
					turnaround[5] += readyNode->Turnaround;
					endNum++;
					CPUstate = SCHEDULE;

					readyNode->ExecutingTime++;
					readyNode->RemainTime--;

					node = (PROCESS *)malloc(sizeof(PROCESS));
					CopyNode(readyNode, node);
					Enqueue(node, &endHead, &endTail);				
				}	
				else {			
					readyNode->ExecutingTime++;
					readyNode->RemainTime--;
				}
				timequantum--;
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

	printf("\n\n");

	printf("**************************** Result ******************************\n\n");
	PrintProcess(&endHead);
	printf("\n  average waiting time : %.2lf average turnaround time : %.2lf \n",
		waiting[5]/(double)processNum, turnaround[5]/(double)processNum);
}

int main(void) {

	CreateProcess();
	while(1) {
	Menu();
	}
	
	return 0;

}