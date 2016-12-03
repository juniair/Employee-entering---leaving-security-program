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
	strftime(log_time, sizeof(log_time), "%Y�� %m�� %d�� %H:%M:%S", &t);
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

	cal_time(); // ���� �ð��� �޾ƿ�
	
	if(divData.rank >= tmp_user->userData.rank) // ��ũ ��
	{
		// �μ� ���� �ð� < ���� �ð� < �μ� ���� �ð�
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
		error("�޸� �Ҵ� ����");

	log_div->next = NULL;
	log_div->divData = tmp_div->divData; // �μ� ���� ���
	cal_time();
	strcpy(log_div->acc_time, log_time); // ���� �ð� ���

	if(in_out)
		log_div->access = true; // ���� ���
	else
		log_div->access = false; // ���� ���

	// ���� ��� ��� ����
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
		error("�޸� �Ҵ� ����");

	log_user->next = NULL;
	log_user->userData = tmp_user->userData; // ����� ���� ���
	cal_time();
	strcpy(log_user->acc_time, log_time); // ���� �ð� ���

	if(in_out)
		log_user->access = true; // ���� ���
	else
		log_user->access = false; // �Ǵ� ���� ���

	// ���� ��� ��� ����
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
			
			printf("�̹� ����� �߽��ϴ�.\n");
			delay_2sec();
			return tmp_user;
		}
		else if(strcmp(tmp->divData.name, company_node->divData.name)
			&& (tmp->access))
		{
			printf("�̹� ����� �߽��ϴ�.\n");
			delay_2sec();
			return tmp_user;
		}
	}
	
	// record visiting division in tmp_user
	tmp_user = create_user_div_log(tmp_user, company_node, true);
	// record visiting user in tmp_div
	company_node = create_div_user_log(company_node, tmp_user, true);

	printf("%s�� �����Ͽ����ϴ�.\n", company_node->divData.name);
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
		printf("����� ���� �Ͻʽÿ�.\n");
		delay_2sec();
		return tmp_user;
	}
	else
	{
		if(!(strcmp(tmp->divData.name, company_node->divData.name))
			&& !(tmp->access))
		{
			printf("�̹� ����� �Ͽ����ϴ�.\n");
			delay_2sec();
			return tmp_user;
		}
		else if(strcmp(tmp->divData.name, company_node->divData.name)
			&& (tmp->acc_time))
		{
			printf("���� ���¿��� ����� �� �����ϴ�.\n");
			delay_2sec();
			return tmp_user;
		}
	}

	// record visiting division in tmp_user
	tmp_user = create_user_div_log(tmp_user, company_node, false);
	// record visiting user in tmp_div
	company_node = create_div_user_log(company_node, tmp_user, false);

	printf("%s���� �����Ͽ����ϴ�.\n", company_node->divData.name);
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
		printf("����� ���� �Ͻʽÿ�.\n");
		delay_2sec();
		return tmp_user;
	}
	// check user's access whether in or out
	else if(!((tmp->access) && (!strcmp(tmp->divData.name, company_node->divData.name))))
	{
		printf("���� ���·� ������ �� �����ϴ�.\n");
		delay_2sec();
		return tmp_user;
	}
	
	while(1)
	{
		reset();
		printf("�μ��� : ");
		gets(divNameTmp);
		NEXT;
		if(tmp_div = search_div_node(head_div, divNameTmp))
		{
			if(!strcmp(tmp_div->divData.name, company_node->divData.name))
			{
				printf("���� %s�� �ֽ��ϴ�.\n", tmp_div->divData.name);
				delay_2sec();
				return tmp_user;
			}
			else if(check_rank_time(tmp_user, tmp_div)) // check rank and enable time
			{
				printf("������ �Ϸ�Ǿ����ϴ�.\n");
				delay_2sec();
				NEXT;
				break;
			}
			printf("������ �����Ǿ����ϴ�.\n");
			NEXT;
			printf("��� �Ǵ� ���� ���� �ð��� Ȯ���Ͽ� �ֽʽÿ�.\n");
			delay_2sec();
			return tmp_user;
		}
		else
		{
			printf("��ġ�ϴ� �μ����� �����ϴ�.\n");
			delay_2sec();
			NEXT;
			printf("������ : ESC\n");
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

	printf("%s�� �����Ͽ����ϴ�.\n", tmp_div->divData.name);
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
		printf("����� ���� �Ͻʽÿ�.\n");
		delay_2sec();
		return tmp_user;
	}
	// check user's access whether in or out
	else if(!((tmp->access) && (strcmp(tmp->divData.name, company_node->divData.name))))
	{
		printf("���� ���¿��� ������ �� �����ϴ�.\n");
		delay_2sec();
		return tmp_user;
	}

	strcpy(divNameTmp, tmp->divData.name);
	tmp_div = search_div_node(head_div, divNameTmp);

	printf("%s���� �����Ͽ����ϴ�.\n", tmp_div->divData.name);
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

