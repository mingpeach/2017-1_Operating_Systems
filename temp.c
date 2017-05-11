#include <stdio.h>
#include <stdlib.h>

#define CPUtime 50

typedef struct process PROCESS;
typedef struct process {
	int ProcessID;
	int CPUburstTime;
	int IOburstTime;	
	int ArrivalTime;
	int Priority;
	int RemainTime;
	int WaitingTime;
	int Turnaround;
	PROCESS *Next;
} PROCESS;

PROCESS *head = NULL, *tail = NULL;
int processNum;

/*--- function declaration ---*/
void Menu();
void Select(int select);
void CreateProcess();
void Enqueue(PROCESS *node, PROCESS **head, PROCESS **tail);
void PrintProcess(PROCESS **node);
void SWAP(PROCESS *node1, PROCESS *node2);
void SortByArrival(PROCESS **head, PROCESS **tail);
void SortByBurst(PROCESS **head, PROCESS **tail);
void SortByPriority(PROCESS **head, PROCESS **tail)


/*--- function definition ---*/
void Menu() {

	int select;
	printf("      +----------------------------------------------------------------+\n");
	printf("      |                    CPU Scheduling Simulator                    |\n");
	printf("      +----------------------------------------------------------------+\n");
	printf("      |                                                                |\n");
	printf("      |   1. FCFS(First Come First Served)                             |\n");
	printf("      |   2. Nonpreemptive SJF(Shortest Job First)                     |\n");
	printf("      |   3. Preemptive SJF(Shortest Job First)                        |\n");
	printf("      |   4. Nonpreemptive Priority                                    |\n");
	printf("      |   5. Preemptive Priority                                       |\n");	
	printf("      |   6. RR(Round Robin)                                           |\n");
	printf("      |   7. Evaluation                                                |\n");
	printf("      |   8. Exit                                                      |\n");
	printf("      |                                                                |\n");
	printf("      +----------------------------------------------------------------+\n\n");	
	printf("          Select CPU Scheduling Algorithm : ");
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

	head = NULL, tail = NULL;
	PROCESS *node;

	srand(time(NULL));

	printf("Enter the number of process : ");
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

void PrintProcess(PROCESS **queue) {

	printf("+-----+-------+--------+--------+-------+------+------+----------+\n");
	printf("| PID |Arrival|Priority|CPUburst|IOburst|Remain| Wait |Turnaround|\n");
	printf("+-----+-------+--------+--------+-------+------+------+----------+\n");	

	for(; *queue != NULL; *queue = (*queue)->Next) {
		printf("| %d |  %d  |  %d   |  %d   |  %d  | %d  | %d |   %d    |\n", 
			(*queue)->ProcessID, (*queue)->ArrivalTime, (*queue)->Priority, (*queue)->CPUburstTime,
			(*queue)->IOburstTime, (*queue)->RemainTime, (*queue)->WaitingTime, (*queue)->Turnaround);
		printf("+-----+-------+--------+--------+-------+------+------+----------+\n");
	}

}

void SWAP(PROCESS *node1, PROCESS *node2) {

	PROCESS swap;

	swap = *node1;
	*node1 = *node2;
	*node2 = swap;
	
}

int main(void) {

	return 0;
}