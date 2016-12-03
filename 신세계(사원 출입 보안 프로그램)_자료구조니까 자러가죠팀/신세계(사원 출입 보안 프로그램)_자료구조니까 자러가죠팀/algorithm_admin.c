#include "header.h"
#pragma once

int y;

#define BLANK " "
#define DELAY delay(2000);
#define NEXT gotoxy(10,++y);
#define SET gotoxy(5,6);
#define NL gotoxy(5,++y);
#define LINE menu_outline(15);
#define CLS system("cls");

//***************************************************************************************//
// basic function
//***************************************************************************************//

// error function
void error(char *message)
{
	fprintf(stderr,"%s\n", message);
	exit(1);
}

// warning function
void warning(char *message)
{
	printf(stderr, "%s\n", message);
}

// create new user node
UserList* create_new_user_node(DefaultUserData userData)
{
	UserList *new_node = (UserList*)malloc(sizeof(UserList));

	if(new_node == NULL)
		error("메모리 할당 에러.");

	new_node->divLog = NULL;
	new_node->next = NULL;
	new_node->userData = userData;

	return new_node;
}

// create new division node
DivList* create_new_div_node(DefaultDivData divData)
{
	DivList *new_node = (DivList*)malloc(sizeof(DivList));

	if(new_node == NULL)
		error("메모리 할당 에러.");

	new_node->userLog = NULL;
	new_node->next = NULL;
	new_node->divData = divData;

	return new_node;
}

// return last user node
UserList* last_user_node(UserList *head_user)
{
	UserList *tmp = head_user;

	while(tmp->next != NULL)
		tmp = tmp->next;

	return tmp;
}

// return last division node
DivList* last_div_node(DivList *head_div)
{
	DivList *tmp = head_div;

	while(tmp->next != NULL)
		tmp = tmp->next;

	return tmp;
}

// search division node
DivList* search_div_node(DivList *head_div, char *divNameTmp)
{
	DivList *tmp = head_div;

	while(tmp != NULL)
	{
		if(!strcmp(tmp->divData.name, divNameTmp))
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}

//***************************************************************************************//
// function for User
//***************************************************************************************//

// display user node
void display_user_node(UserList *tmp_user)
{
	printf("│%8s %6s %3s %-20s %c %3s│\n", tmp_user->userData.name, tmp_user->userData.id, 
		BLANK, tmp_user->userData.division, tmp_user->userData.rank, BLANK);
}

// display user list
void display_user_list(UserList *head_user)
{
	UserList *tmp;
	tmp = head_user;

	reset();
	printf("┌───────────────────────┐\n");
	NEXT;
	printf("│%3sName%6sID%7sDivision%9sRank%3s│\n", BLANK, BLANK, BLANK, BLANK, BLANK);
	NEXT;
	printf("│───────────────────────│\n");
	NEXT;
	while(tmp != NULL)
	{
		display_user_node(tmp);
		NEXT;
		tmp = tmp->next;
	}

	printf("└───────────────────────┘\n");
	NEXT;
	printf("  나가기 : ESC\n");
	while(getch()!=27);
}

// search user node
UserList* search_user_node(UserList *head_user, char *userIDTmp)
{
	UserList *tmp = head_user;

	while(tmp != NULL)
	{
		if(!strcmp(tmp->userData.id, userIDTmp))
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}

// create user node
UserList* create_user_node(UserList *head_user, DivList *head_div)
{
	UserList *tmp_user = NULL, *new_node = NULL;
	DivList *tmp_div = NULL;
	DefaultUserData userData;

	while(1) {
		reset();

		printf("사용자 이름 : ");
		gets(userData.name);
		NEXT;
	
		printf("사용자 ID : ");
		gets(userData.id);
		NEXT;
		if(search_user_node(head_user, userData.id)) // check user ID overlapped
		{
			printf("중복된 ID입니다.\n");
			delay_2sec();
			continue;
		}

		printf("사용자 부서 : ");
		gets(userData.division);
		NEXT;
		if(!search_div_node(head_div, userData.division)) // check division name
		{
			printf("일치하는 부서명이 없습니다.\n");
			delay_2sec();
			NEXT;
			printf("나가기 : ESC");
			if(getch()==27)
				return head_user;
			continue;
		}

		printf("사용자 등급 : ");
		userData.rank = getchar();
		fflush(stdin);
		NEXT;
		tmp_div = search_div_node(head_div, userData.division);
		if(userData.rank > tmp_div->divData.rank)
		{
			if((userData.rank < 'A') || (userData.rank > 'D'))
				printf("등급이 범위에 맞지 않습니다.(A~D)\n"); // 65~68 아스키 코드 값
			else
				printf("부서의 등급보다 높아야 합니다.\n");
			delay_2sec();
			continue;
		}
		else
			break;
	}

	new_node = create_new_user_node(userData);

	if(head_user == NULL)
		head_user = new_node;
	else
	{
		tmp_user = last_user_node(head_user);
		tmp_user->next = new_node;
	}

	printf("새로운 사용자가 추가되었습니다.\n");
	delay_2sec();

	return head_user;
}

// check user's location
void check_user_location(UserList *tmp_user, DivList *company_node)
{
	Div *tmp = last_div_log(tmp_user);

	reset();
	// 사용자가 방문한 부서 기록이 없을 때
	if(tmp == NULL)
	{
		printf("해당 사용자는 현재 사내에 없습니다.\n");
		delay_2sec();
		return;
	}
	// 사용자가 완전히 사내에서 퇴근하였을 때
	else if(!strcmp(tmp->divData.name, company_node->divData.name)
		&& !(tmp->access))
	{
		printf("해당 사용자는 현재 퇴근했습니다.\n");
		delay_2sec();
		return;
	}
	// 
	printf("해당 사용자는 %s에 있습니다.\n", tmp->divData.name);
	delay_2sec();
	return;
}

// check user's visiting record
void check_user_log(UserList *tmp_user)
{
	Div *tmp = tmp_user->divLog;
	reset();

	if(tmp == NULL)
	{
		printf("해당 사용자는 현재 사내에 없습니다.\n");
		delay_2sec();
		return;
	}
	while(tmp != NULL)
	{
		printf("시간 : %s 부서 : %s 출입 여부 : %s\n", tmp->acc_time, 
			tmp->divData.name, (tmp->access == true) ? "in" : "out");
		NEXT;
		tmp = tmp->next;
	}
	printf("  나가기 : ESC\n");
	while(getch()!=27);
}

// change user node
UserList* change_user_node(UserList *head_user, DivList *head_div, UserList *tmp_user)
{
	DivList *tmp_div = NULL;
	DefaultUserData userData;

	// print 기존 사용자 정보
	while(1) {
		reset();
		printf("사용자 이름 : ");
		gets(userData.name);
		NEXT;
		printf("사용자 ID : ");
		gets(userData.id);
		NEXT;
		if((search_user_node(head_user, userData.id)) && 
			(strcmp(tmp_user->userData.id, userData.id))) // check user ID overlapped
		{
			printf("중복되는 ID입니다.\n");
			delay_2sec();
			continue;
		}

		printf("사용자 부서 : ");
		gets(userData.division);
		NEXT;
		if(!search_div_node(head_div, userData.division)) // check division name
		{
			printf("일치하는 부서명이 없습니다.\n");
			delay_2sec();
			NEXT;
			printf("나가기 : ESC");
			if(getch()==27)
				return head_user;
			continue;
		}

		printf("사용자 등급 : ");
		userData.rank = getchar();
		fflush(stdin);
		NEXT;
		tmp_div = search_div_node(head_div, userData.division);
		if(userData.rank > tmp_div->divData.rank)
		{
			if((userData.rank < 'A') || (userData.rank > 'D'))
				printf("등급이 범위에 맞지 않습니다.(A~D)\n"); // 65~68 아스키 코드 값
			else
				printf("부서의 등급보다 높아야 합니다.\n");
			delay_2sec();
			continue;
		}
		else
			break;
	}

	tmp_user->userData = userData;

	printf("사용자 정보가 변경되었습니다.\n");
	delay_2sec();

	return head_user;
}

// delete user node
UserList* delete_user_node(UserList *head_user, UserList *tmp_user)
{
	UserList *before_user = head_user, *after_user = NULL;
	
	// total node number is 1
	if(before_user->next == NULL)
	{
		free(head_user);
		return NULL;
	}
	else if(before_user == tmp_user) // tmp_user is first node in user list
	{
		head_user = before_user->next;
		free(before_user);
		return head_user;
	}
	
	// search node before tmp_user
	while(before_user->next != tmp_user)
		before_user = before_user->next;
	
	// tmp_user is last node in user list
	if(tmp_user->next == NULL) 
	{
		before_user->next = NULL;
		free(tmp_user);
		return head_user;
	}
	else
	{
		after_user = tmp_user->next;
		before_user->next = after_user;
		free(tmp_user);
		return head_user;
	}
}


//***************************************************************************************//
// function for Div
//***************************************************************************************//

// display division node
void display_div_node(DivList *tmp_div)
{
	printf("│%s%-20s %c %18s%2s│\n", BLANK, tmp_div->divData.name, 
		tmp_div->divData.rank, tmp_div->divData.enable_time, BLANK);
}

// display division list
void display_div_list(DivList *head_div)
{
	DivList *tmp;

	reset();
	tmp = head_div;
	printf("┌──────────────────────┐\n");
	NEXT;
	printf("│%3sDivision%9sRank%7sEnable time%2s│\n", BLANK, BLANK, BLANK, BLANK);
	NEXT;
	printf("│──────────────────────│\n");
	NEXT;

	while(tmp != NULL)
	{
		display_div_node(tmp);
		NEXT;
		tmp = tmp->next;
	}

	printf("└──────────────────────┘\n");
	NEXT;
	printf("  나가기 : ESC\n");
	while(getch()!=27);
}

// create division node
DivList* create_div_node(DivList *head_div)
{
	DivList *tmp = NULL, *new_node = NULL;
	DefaultDivData divData;

	while(1) {
		reset();
		printf("부서 이름 : ");
		gets(divData.name);
		NEXT;
		if(search_div_node(head_div, divData.name))  // check division name overlapped
		{
			printf("중복된 이름입니다.\n");
			delay_2sec();
			continue;
		}

		printf("부서 출입 가능 시간(HH:MM~HH:MM) : ");
		gets(divData.enable_time);
		NEXT;

		printf("부서 등급 : ");
		divData.rank = getchar();
		fflush(stdin);
		NEXT;
		if((divData.rank < 65) || (divData.rank > 68))
		{
			printf("등급이 범위에 맞지 않습니다.(A~D)\n");
			delay_2sec();
			NEXT;
			printf("나가기 : ESC");
			if(getch()==27)
				return head_div;
			continue;
		}
		else
			break;
	}
	
	new_node = create_new_div_node(divData);

	if(head_div == NULL)
		head_div = new_node;
	else
	{
		tmp = last_div_node(head_div);
		tmp->next = new_node;
	}

	printf("새로운 부서가 추가되었습니다.\n");
	delay_2sec();

	return head_div;
}

// change div node
DivList* change_div_node(DivList *head_div, DivList *tmp_div)
{
	DefaultDivData divData;
	
	while(1) {
		reset();
		printf("부서 이름 : ");
		gets(divData.name);
		NEXT;
		if((search_div_node(head_div, divData.name)) &&
			(strcmp(divData.name, tmp_div->divData.name))) // check div name overlapped
		{
			printf("중복되는 이름입니다.\n");
			delay_2sec();
			continue;
		}

		printf("부서 출입 가능 시간 : ");
		gets(divData.enable_time);
		NEXT;

		printf("부서 등급 : ");
		divData.rank = getchar();
		fflush(stdin);
		NEXT;
		if((divData.rank < 65) || (divData.rank > 68))
		{
			printf("등급이 범위에 맞지 않습니다.(A~D)\n");
			delay_2sec();
			NEXT;
			printf("나가기 : ESC");
			if(getch()==27)
				return head_div;
			continue;
		}
		else
			break;
	}

	tmp_div->divData = divData;

	printf("부서 정보가 변경되었습니다.\n");
	delay_2sec();

	return head_div;
}

// delete division node
DivList* delete_div_node(DivList *head_div, DivList *tmp_div)
{
	DivList *before_div = head_div, *after_div = NULL;
	
	// total node number is 1
	if(before_div->next == NULL)
	{
		free(head_div);
		return NULL;
	}
	else if(before_div == tmp_div) // tmp_div is first node in division list
	{
		head_div = before_div->next;
		free(before_div);
		return head_div;
	}
	
	// search node before tmp_div
	while(before_div->next != tmp_div)
		before_div = before_div->next;
	
	// tmp_div is last node in division list
	if(tmp_div->next == NULL) 
	{
		before_div->next = NULL;
		free(tmp_div);
		return head_div;
	}
	else
	{
		after_div = tmp_div->next;
		before_div->next = after_div;
		free(tmp_div);
		return head_div;
	}
}

// check visiting user's record in division
void check_div_log(DivList *tmp_div)
{
	User *users = NULL, *users_tmp = NULL, *tmp = NULL;
	User *tmp1 = tmp_div->userLog;
	User *tmp2 = NULL;

	reset();

	if(tmp1 == NULL)
	{
		printf("방문 기록이 없습니다.\n");
		delay_2sec();
		return;
	}
	else
	{
		while(tmp1->next != NULL)
		{
			if(tmp1->access)
			{
				tmp2 = tmp1->next;
				while(tmp2 != NULL)
				{
					if(!(strcmp(tmp2->userData.id, tmp1->userData.id)) 
						&& !(tmp2->access))
					{
						tmp2 = NULL;
						continue;
					}
					else if(tmp2->next == NULL)
					{
						users_tmp = (User*)malloc(sizeof(User));
						if(users_tmp == NULL)
							error("메모리 할당 에러");
						users_tmp->userData = tmp1->userData;
						users_tmp->access = tmp1->access;
						strcpy(users_tmp->acc_time, tmp1->acc_time);
						users_tmp->next = NULL;
						if(users == NULL)
							users = users_tmp;
						else
						{
							tmp = users;
							while(tmp->next != NULL)
								tmp = tmp->next;
							tmp->next = users_tmp;
						}
					}
					tmp2 = tmp2->next;
				}
			}
			tmp1 = tmp1->next;
		}
		if(tmp1->access)
		{
			users_tmp = (User*)malloc(sizeof(User));
			if(users_tmp == NULL)
				error("메모리 할당 에러");
			users_tmp->userData = tmp1->userData;
			users_tmp->access = tmp1->access;
			strcpy(users_tmp->acc_time, tmp1->acc_time);
			users_tmp->next = NULL;
			if(users == NULL)
				users = users_tmp;
			else
			{
				tmp = users;
				while(tmp->next != NULL)
					tmp = tmp->next;
				tmp->next = users_tmp;
			}
		}
	}

	while(users != NULL)
	{
		printf("이름 : %s / 시간 : %s / 출입 : %s\n", users->userData.name, 
			users->acc_time, (users->access == true) ? "in" : "out");
		NEXT;
		users = users->next;
	}
	printf("  나가기 : ESC\n");
	while(getch()!=27);
}

// print houly visiting users
void print_hourly_visiting_users(DivList *tmp_div)
{
	User *tmp1 = tmp_div->userLog;
	User *tmp2 = NULL;
	char tok1[2] = {NULL}, tok2[2] = {NULL}, now_time[30] = {NULL};
	int in_hour = 0, out_hour = 0, i = 0, j = 0;
	int cnt[24] = {0};

	reset();
	
	if(tmp1 == NULL)
	{
		printf("방문 기록이 없습니다.\n");
		delay_2sec();
		return;
	}
	else
	{
		while(tmp1->next != NULL)
		{
			if(tmp1->access)
			{
				tmp2 = tmp1->next;
				while(tmp2 != NULL)
				{
					if(!(strcmp(tmp2->userData.id, tmp1->userData.id)) 
						&& !(tmp2->access))
					{
						tok1[0] = tmp1->acc_time[17];
						tok1[1] = tmp1->acc_time[18];
						tok2[0] = tmp2->acc_time[17];
						tok2[1] = tmp2->acc_time[18];
						in_hour = atoi(tok1);
						out_hour = atoi(tok2);
						if(in_hour == out_hour)
							cnt[in_hour]++;
						else
							for(i=in_hour; i<=out_hour; i++)
								cnt[i]++;
						tmp2 = NULL;
						continue;
					}
					else if(tmp2->next == NULL) // last node
					{
						strcpy(now_time,cal_time());
						tok1[0] = tmp1->acc_time[17];
						tok1[1] = tmp1->acc_time[18];
						tok2[0] = now_time[17];
						tok2[1] = now_time[18];
						in_hour = atoi(tok1);
						out_hour = atoi(tok2);
						if(in_hour == out_hour)
							cnt[in_hour]++;
						else
							for(i=in_hour; i<=out_hour; i++)
								cnt[i]++;
					}
					tmp2 = tmp2->next;
				}
			}
			tmp1 = tmp1->next;
		}
		if(tmp1->access)
		{
			strcpy(now_time,cal_time());
			tok1[0] = tmp1->acc_time[17];
			tok1[1] = tmp1->acc_time[18];
			tok2[0] = now_time[17];
			tok2[1] = now_time[18];
			in_hour = atoi(tok1);
			out_hour = atoi(tok2);
			if(in_hour == out_hour)
				cnt[in_hour]++;
			else
				for(i=in_hour; i<=out_hour; i++)
					cnt[i]++;
		}
	}

	SET;
	printf(" │   00   01   02   03   04   05   06   07   08   09   10   11    │\n");
	NL;
	printf("───────────────────────────────────\n");
	NL;
	for(i=0;i<9;i++)
	{
		printf("%d│%64s│\n", i+1, BLANK);
		gotoxy(10,y);
		for(j=0;j<12;j++)
		{
			if(cnt[j]>0)
			{
				printf(" ■  ");
				cnt[j]--;
			}
			else
				printf("     ");
		}
		NL;
	}

	printf("  다음 : ENTER\n");
	while(getch()!=13);
	
	CLS;
	LINE;
	y = 6;
	SET;
	printf(" │ 12   13   14   15   16   17   18   19   20   21   22   23   24 │\n");
	NL;
	printf("───────────────────────────────────\n");
	NL;
	for(i=0;i<9;i++)
	{
		printf("%d│%64s│\n", i+1, BLANK);
		gotoxy(8,y);
		for(j=12;j<24;j++)
		{
			if(cnt[j] > 0)
			{
				printf(" ■  ");
				cnt[j]--;
			}
			else
				printf("     ");
		}
		NL;
	}

	printf("  나가기 : ESC\n");
	while(getch()!=27);
}

//***************************************************************************************//
// initailizing function
//***************************************************************************************//

// resturn DefaultUserData 
DefaultUserData init_userData(char *name, char *ID, char *division, char rank)
{
	DefaultUserData userData;

	strcpy(userData.name, name);
	strcpy(userData.id, ID);
	strcpy(userData.division, division);
	userData.rank = rank;
	
	return userData;
}

// initailize user list
UserList* init_user_list(UserList *head_user)
{
	DefaultUserData userData;
	UserList *new_node = NULL, *tmp = NULL;
	
	userData = init_userData("양우민", "YWM", "Information", 'C');
	new_node = create_new_user_node(userData);
	head_user = new_node;

	// **** 여기서부터 초기화할 부서 추가 ****//

	tmp = last_user_node(head_user);
	userData = init_userData("신동렬", "SDR", "Development", 'A');
	new_node = create_new_user_node(userData);
	tmp->next = new_node;

	tmp = last_user_node(head_user);
	userData = init_userData("김경준", "KKJ", "General Affairs", 'C');
	new_node = create_new_user_node(userData);
	tmp->next = new_node;

	tmp = last_user_node(head_user);
	userData = init_userData("이현수", "LHS", "Security", 'B');
	new_node = create_new_user_node(userData);
	tmp->next = new_node;

	// *** 여기까지 초기화할 부서 추가 완료 ***//

	return head_user;
}

// return DefaultUserData
DefaultDivData init_divData(char *name, char rank, char *enable_time)
{
	DefaultDivData divData;

	strcpy(divData.name, name);
	divData.rank = rank;
	strcpy(divData.enable_time, enable_time);

	return divData;
}

// initailize user list
DivList* init_div_list(DivList *head_div)
{
	DefaultDivData divData;
	DivList *new_node = NULL, *tmp = NULL;
	
	divData = init_divData("Company", 'D', "00:00~23:59");
	new_node = create_new_div_node(divData);
	head_div = new_node;

	// **** 여기서부터 초기화할 부서 추가 ****//

	tmp = last_div_node(head_div);
	divData = init_divData("Human Resource", 'A', "09:00~17:00");
	new_node = create_new_div_node(divData);
	tmp->next = new_node;

	tmp = last_div_node(head_div);
	divData = init_divData("General affairs", 'C', "08:00~23:50");
	new_node = create_new_div_node(divData);
	tmp->next = new_node;

	tmp = last_div_node(head_div);
	divData = init_divData("Accounting", 'B', "10:00~18:00");
	new_node = create_new_div_node(divData);
	tmp->next = new_node;

	tmp = last_div_node(head_div);
	divData = init_divData("Development", 'A', "13:00~17:00");
	new_node = create_new_div_node(divData);
	tmp->next = new_node;

	tmp = last_div_node(head_div);
	divData = init_divData("Network", 'B', "01:05~23:30");
	new_node = create_new_div_node(divData);
	tmp->next = new_node;

	tmp = last_div_node(head_div);
	divData = init_divData("Security", 'C', "00:00~21:00");
	new_node = create_new_div_node(divData);
	tmp->next = new_node;

	tmp = last_div_node(head_div);
	divData = init_divData("Information", 'D', "09:00~16:00");
	new_node = create_new_div_node(divData);
	tmp->next = new_node;

	// **** 여기서부터 초기화할 부서 추가 ****//

	return head_div;
}




