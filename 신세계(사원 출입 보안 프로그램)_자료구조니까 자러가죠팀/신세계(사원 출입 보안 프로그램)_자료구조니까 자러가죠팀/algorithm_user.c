#include "header.h"

int y;

#define NEXT gotoxy(10,++y);

// for time
struct tm t;
time_t now;
char log_time[30] = {NULL};

// record now time to string
char* cal_time()
{
	time(&now);
	t = *localtime(&now);
	strftime(log_time, sizeof(log_time), "%Y년 %m월 %d일 %H:%M:%S", &t);
	return log_time;
}

// return last division log in user node
Div* last_div_log(UserList *tmp_user)
{
	Div *tmp = tmp_user->divLog;

	if(tmp == NULL)
		return NULL;

	while(tmp->next != NULL)
		tmp = tmp->next;

	return tmp;
}

// return last user log in division node
User* last_user_log(DivList *tmp_div)
{
	User *tmp = tmp_div->userLog;

	if(tmp == NULL)
		return NULL;

	while(tmp->next != NULL)
		tmp = tmp->next;

	return tmp;
}

// check rank and time
bool check_rank_time(UserList *tmp_user, DivList *tmp_div)
{
	DefaultDivData divData;
	int openHour = 0, openMin = 0, closeHour = 0, closeMin = 0;
	char open_hour[3] = {NULL}, open_min[3] = {NULL};
	char close_hour[3] = {NULL}, close_min[3] = {NULL};
	
	divData = tmp_div->divData;

	// extract time from enable time 
	strncpy(open_hour, divData.enable_time, 2);
	openHour = atoi(open_hour);
	open_min[0] = divData.enable_time[3];
	open_min[1] = divData.enable_time[4];
	openMin = atoi(open_min);
	close_hour[0] = divData.enable_time[6];
	close_hour[1] = divData.enable_time[7];
	closeHour = atoi(close_hour);
	close_min[0] = divData.enable_time[9];
	close_min[1] = divData.enable_time[10];
	closeMin = atoi(close_min);

	cal_time(); // 현재 시간을 받아옴
	
	if(divData.rank >= tmp_user->userData.rank) // 랭크 비교
	{
		// 부서 개방 시간 < 현재 시간 < 부서 마감 시간
		if((openHour < t.tm_hour) && (t.tm_hour < closeHour)) 
			return true;
		else if((openHour == t.tm_hour) && (t.tm_hour < closeHour))
		{
			if(openMin <= t.tm_min)
				return true;
			else
				return false;
		}
		else if((t.tm_hour > openHour) && (t.tm_hour == closeHour))
		{
			if(t.tm_min < closeMin)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	return false;
}

// create division log in user node
UserList* create_user_div_log(UserList *tmp_user, DivList *tmp_div, bool in_out)
{
	Div *log_div = NULL, *tmp = last_div_log(tmp_user);
	log_div = (Div*)malloc(sizeof(Div));
	if(log_div == NULL)
		error("메모리 할당 에러");

	log_div->next = NULL;
	log_div->divData = tmp_div->divData; // 부서 정보 기록
	cal_time();
	strcpy(log_div->acc_time, log_time); // 출입 시간 기록

	if(in_out)
		log_div->access = true; // 입장 기록
	else
		log_div->access = false; // 퇴장 기록

	// 출입 기록 노드 연결
	if(tmp == NULL)
		tmp_user->divLog = log_div;
	else
		tmp->next = log_div;

	return tmp_user;
}	

// create user log in division node
DivList* create_div_user_log(DivList *tmp_div, UserList *tmp_user, bool in_out)
{
	User *log_user = NULL, *tmp = last_user_log(tmp_div);
	log_user = (User*)malloc(sizeof(User));
	if(log_user == NULL)
		error("메모리 할당 에러");

	log_user->next = NULL;
	log_user->userData = tmp_user->userData; // 사용자 정보 기록
	cal_time();
	strcpy(log_user->acc_time, log_time); // 출입 시간 기록

	if(in_out)
		log_user->access = true; // 입장 기록
	else
		log_user->access = false; // 또는 퇴장 기록

	// 출입 기록 노드 연결
	if(tmp == NULL)
		tmp_div->userLog = log_user;
	else
		tmp->next = log_user;

	return tmp_div;
}

// user come in company
UserList* in_company(UserList *tmp_user, DivList *company_node)
{
	Div *tmp = last_div_log(tmp_user);
	
	reset();

	if(tmp != NULL)
	{
		if(!(strcmp(tmp->divData.name, company_node->divData.name))
			&& (tmp->access))
		{
			
			printf("이미 출근을 했습니다.\n");
			delay_2sec();
			return tmp_user;
		}
		else if(strcmp(tmp->divData.name, company_node->divData.name)
			&& (tmp->access))
		{
			printf("이미 출근을 했습니다.\n");
			delay_2sec();
			return tmp_user;
		}
	}
	
	// record visiting division in tmp_user
	tmp_user = create_user_div_log(tmp_user, company_node, true);
	// record visiting user in tmp_div
	company_node = create_div_user_log(company_node, tmp_user, true);

	printf("%s로 입장하였습니다.\n", company_node->divData.name);
	delay_2sec();
	NEXT;
	
	return tmp_user;
}

// user come out from company
UserList* out_company(UserList *tmp_user, DivList *company_node)
{
	Div *tmp = last_div_log(tmp_user);

	reset();

	if(tmp == NULL)
	{
		printf("출근을 먼저 하십시오.\n");
		delay_2sec();
		return tmp_user;
	}
	else
	{
		if(!(strcmp(tmp->divData.name, company_node->divData.name))
			&& !(tmp->access))
		{
			printf("이미 퇴근을 하였습니다.\n");
			delay_2sec();
			return tmp_user;
		}
		else if(strcmp(tmp->divData.name, company_node->divData.name)
			&& (tmp->acc_time))
		{
			printf("현재 상태에서 퇴근할 수 없습니다.\n");
			delay_2sec();
			return tmp_user;
		}
	}

	// record visiting division in tmp_user
	tmp_user = create_user_div_log(tmp_user, company_node, false);
	// record visiting user in tmp_div
	company_node = create_div_user_log(company_node, tmp_user, false);

	printf("%s에서 퇴장하였습니다.\n", company_node->divData.name);
	delay_2sec();
	NEXT;

	return tmp_user;
}

// user come in division
UserList* in_div(DivList *head_div, UserList *tmp_user, DivList *company_node)
{
	char divNameTmp[20] = {NULL};
	Div *tmp = last_div_log(tmp_user);
	DivList *tmp_div = NULL;

	reset();

	if(tmp == NULL)
	{
		printf("출근을 먼저 하십시오.\n");
		delay_2sec();
		return tmp_user;
	}
	// check user's access whether in or out
	else if(!((tmp->access) && (!strcmp(tmp->divData.name, company_node->divData.name))))
	{
		printf("현재 상태로 입장할 수 없습니다.\n");
		delay_2sec();
		return tmp_user;
	}
	
	while(1)
	{
		reset();
		printf("부서명 : ");
		gets(divNameTmp);
		NEXT;
		if(tmp_div = search_div_node(head_div, divNameTmp))
		{
			if(!strcmp(tmp_div->divData.name, company_node->divData.name))
			{
				printf("현재 %s에 있습니다.\n", tmp_div->divData.name);
				delay_2sec();
				return tmp_user;
			}
			else if(check_rank_time(tmp_user, tmp_div)) // check rank and enable time
			{
				printf("입장이 완료되었습니다.\n");
				delay_2sec();
				NEXT;
				break;
			}
			printf("입장이 거절되었습니다.\n");
			NEXT;
			printf("등급 또는 출입 가능 시간을 확인하여 주십시오.\n");
			delay_2sec();
			return tmp_user;
		}
		else
		{
			printf("일치하는 부서명이 없습니다.\n");
			delay_2sec();
			NEXT;
			printf("나가기 : ESC\n");
			if(getch()==27)
				return tmp_user;
			continue;
		}
	}
	
	// user out company
	tmp_user = out_company(tmp_user, company_node);
	// record visiting division in tmp_user
	tmp_user = create_user_div_log(tmp_user, tmp_div, true);
	// record visiting user in tmp_div
	tmp_div = create_div_user_log(tmp_div, tmp_user, true);

	printf("%s로 입장하였습니다.\n", tmp_div->divData.name);
	delay_2sec();
	NEXT;
	
	return tmp_user;
}

// user come out from division
UserList* out_div(DivList *head_div, UserList *tmp_user, DivList *company_node)
{
	char divNameTmp[20] = {NULL};
	Div *tmp = last_div_log(tmp_user);
	DivList *tmp_div = NULL;

	reset();
	
	if(tmp == NULL)
	{
		printf("출근을 먼저 하십시오.\n");
		delay_2sec();
		return tmp_user;
	}
	// check user's access whether in or out
	else if(!((tmp->access) && (strcmp(tmp->divData.name, company_node->divData.name))))
	{
		printf("현재 상태에서 퇴장할 수 없습니다.\n");
		delay_2sec();
		return tmp_user;
	}

	strcpy(divNameTmp, tmp->divData.name);
	tmp_div = search_div_node(head_div, divNameTmp);

	printf("%s에서 퇴장하였습니다.\n", tmp_div->divData.name);
	delay_2sec();
	NEXT;
	
	// record visiting division in tmp_user
	tmp_user = create_user_div_log(tmp_user, tmp_div, false);
	// record visiting user in tmp_div
	tmp_div = create_div_user_log(tmp_div, tmp_user, false);
	// user in company
	tmp_user = in_company(tmp_user, company_node);

	return tmp_user;
}

