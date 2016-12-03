#include "header.h"
#include "turboc.h"
#pragma once

int x = 10, y = 5, sel = 1;

#define CLS system("cls");
#define NEXT gotoxy(10,++y);
#define LINE menu_outline(15);
#define DELAY delay(2000);
#define BLANK " "

int progressNum()
{
	int tmp;
	scanf("%d", &tmp);
	fflush(stdin);
	return tmp;
}
#define pN progressNum()

int reset()
{
	CLS;
	LINE;
	sel = 1;
	x = 10;
	y = 6;
	return sel;
}
#define RESET reset()

void delay_2sec()
{
	delay(2000);
}

void main() 
{
	char adminIDTmp[20] = {NULL}, adminPWTmp[20] = {NULL}; // temporary admin ID, PW
	char divNameTmp[20] = {NULL}, userIDTmp[20] = {NULL}; // temporary division name and user ID
	char adminID[20] = "data", adminPW[20] = "data"; // admin account

	char key;
	char *menu1[3] = {{"1. ������ �α���"}, {"2. ����� �α���"}, {"3. ����"}};
	char *menu2[3] = {{"1. ����� ����Ʈ ����"}, {"2. �μ� ����Ʈ ����"}, {"3. ���� �޴���"}};
	char *menu3[4] = {{"1. ����� ����Ʈ ���"}, {"2. ����� ��� �߰�"}, {"3. ����� ��� ����"}, {"4. ���� �޴���"}};
	char *menu4[5] = {{"1. ���� ��ġ ��ȸ"}, {"2. �湮 ��� ��ȸ"}, {"3. ���� ����"}, {"4. ����"}, {"5. ���� �޴���"}};
	char *menu5[4] = {{"1. �μ� ����Ʈ ���"},{"2. �μ� ��� �߰�"}, {"3. �μ� ��� ����"}, {"4. ���� �޴���"}};
	char *menu6[5] = {{"1. �ð��뺰 �湮�� ��ȸ"}, {"2. ���� �湮�� ��ȸ"}, {"3. �μ� ��� ����"}, {"4. ����"}, {"5. ���� �޴���"}};
	char *menu8[4] = {{"1. ���"}, {"2. ���"}, {"3. �μ� ����"}, {"4. ����� �α׾ƿ�"}};
	char *menu9[4] = {{"1. �μ� ����Ʈ ���"}, {"2. �μ� ����"}, {"3. �μ� ����"}, {"4. ���� �޴���"}};

	UserList *head_user = NULL, *tmp_user = NULL; // user list, temporary user node
	DivList *head_div = NULL, *tmp_div, *company_node; // division list, temporary division node, company node
	
	UserList *user_node = NULL; // user login node
	DivList *div_node = NULL; // division login node

	head_user = init_user_list(head_user); // initializing user list
	head_div = init_div_list(head_div); // initializing division list

	setcursortype(NOCURSOR);

	while(1)
	{
beginning:;
		RESET;
		sel = menu_sel(&menu1, sel, sizeof(menu1)/4);
		// 1. ������ �α��� / 2. ����� �α��� / 3. ����

		switch(sel)
		{
		case 1 : // admin login
			while(1)
			{
				RESET;
				printf("������ ID : ");
				gets(adminIDTmp);
				NEXT;
				printf("������ PW : ");
				gets(adminPWTmp);
				NEXT;

				if((!strcmp(adminIDTmp, adminID)) && (!strcmp(adminPWTmp, adminPW))) // succeed account confirming
				{
					printf("�α��ο� �����Ͽ����ϴ�.\n");
					DELAY;
					break;
				}
				else // fail account confriming
				{
					printf("������ ��ġ���� �ʽ��ϴ�.\n");
					DELAY;
					goto beginning; // go to beginning
				}
			}	
			
			while(1)
			{
admin:;
				RESET;
				sel = menu_sel(&menu2, sel, sizeof(menu2)/4);
				// 1. ����� ����Ʈ ���� / 2. �μ� ����Ʈ ���� / 3. ������ �α׾ƿ�
				switch(sel)
				{
				case 1 : // access to user list
					while(1)
					{
admin_user:;
						RESET;
						sel = menu_sel(&menu3, sel, sizeof(menu3)/4);
						// 1. ����� ����Ʈ ��� / 2. ����� ��� �߰� / 3. ����� ��� ���� / 4. ���� �޴���
						switch(sel)
						{
						case 1 : // print user list
							display_user_list(head_user); // display user list
							break;

						case 2 : // insert user node in user list
							head_user = create_user_node(head_user, head_div); // create user node
							break;

						case 3 : // access user node
							RESET;
							printf("����� ID : ");
							gets(userIDTmp);
							NEXT;
							tmp_user = search_user_node(head_user, userIDTmp); // search user node
							if(tmp_user == NULL)
							{
								printf("��ġ�ϴ� ID�� �����ϴ�.\n");
								DELAY;
								break;
							}
				
							while(1)
							{
								RESET;
								sel = menu_sel(&menu4, sel, sizeof(menu4)/4);
								// 1. ���� ��ġ ��ȸ / 2. �湮 ��� ��ȸ / 3. ���� ���� / 4. ���� / 5. ���� �޴���
								switch(sel)
								{
								case 1 : // print user's location
									check_user_location(tmp_user, company_node); // check user's location
									break;

								case 2 : // print user's visiting record
									check_user_log(tmp_user);// check user's visiting record
									break;

								case 3 : // change user's data
									head_user = change_user_node(head_user, head_div, tmp_user); // change user node
									break;

								case 4 : // delete user node
									head_user = delete_user_node(head_user, tmp_user); // delete user node
									tmp_user = NULL;

								case 5 :
									goto admin_user; // go to admin_user
								}
							}							
							break;

						case 4 :
							goto admin; // go to admin
						}
					}					
					break;

				case 2 : // access to division list
					while(1)
					{
admin_div:;
						RESET;
						sel = menu_sel(&menu5, sel, sizeof(menu5)/4);
						// 1. �μ� ����Ʈ ��� / 2. �μ� ��� �߰� / 3. �μ� ��� ���� / 4. ���� �޴���
						switch(sel)
						{
						case 1 : // print division list
							display_div_list(head_div); // display division node
							break;

						case 2 : // insert division node in division list
							head_div = create_div_node(head_div); // create division node
							break;

						case 3 : // access to division node
							RESET;
							printf("�μ� �̸� : ");
							gets(divNameTmp);
							NEXT;
							div_node = search_div_node(head_div, divNameTmp); // search division node
							if(div_node == NULL)
							{
								printf("��ġ�ϴ� �μ����� �����ϴ�.\n");
								DELAY;
								break;
							}

							while(1)
							{
								RESET;
								sel = menu_sel(&menu6, sel, sizeof(menu6)/4);
								// 1. �ð��뺰 �湮�� ��ȸ / 2. ���� �湮�� ��ȸ / 3. �μ� ��� ���� / 4. ���� / 5. ���� �޴���
								switch(sel)
								{
								case 1 : // print hourly visiting users
									print_hourly_visiting_users(div_node);
									break;

								case 2 : // check visiting users now
									check_div_log(div_node);
									break;

								case 3 : // change division node
									head_div = change_div_node(head_div, div_node);
									break;

								case 4 : // delete division node
									head_div = delete_div_node(head_div, div_node);

								case 5 :
									goto admin_div; // go to admin_div
								}
							}
							break;

						case 4 :
							goto admin; // go to admin
						}
					}			
					break;

				case 3 :
					goto beginning; // go to beginning
				}
			}
			break;

		case 2 : // user login
			while(1)
			{
				RESET;
				printf("����� ID : ");
				gets(userIDTmp);
				NEXT;
				if((user_node = search_user_node(head_user, userIDTmp))) // login success
				{
					printf("�α��ο� �����Ͽ����ϴ�.\n"); 
					DELAY;
					break;
				}
				else // login failure
				{
					printf("�������� �ʴ� ������Դϴ�.\n");
					DELAY;
					goto beginning; // go to beginning
				}
			}

			if(!(company_node = search_div_node(head_div, "Company")))
			{
				RESET;
				printf("Company�� �������� �ʽ��ϴ�.\n");
				DELAY;
				break;
			}

			while(1)
			{
user:;
				RESET;
				sel = menu_sel(&menu8, sel, sizeof(menu8)/4);
				// "1. ��� / 2. ��� / 3. �μ� ���� / 4. ����� �α׾ƿ�
				switch(sel)
				{
				case 1 :
					user_node = in_company(user_node, company_node);
					break;

				case 2 :
					user_node = out_company(user_node, company_node);
					break;

				case 3 :
					while(1)
					{
						RESET;
						sel = menu_sel(&menu9, sel, sizeof(menu9)/4);
						// 1. �μ� ����Ʈ ��� / 2. �μ� ���� / 3. �μ� ���� / 4. ���� �޴���
						switch(sel)
						{
						case 1 : // print division list
							display_div_list(head_div);
							break;

						case 2 : // entry in division
							user_node = in_div(head_div, user_node, company_node);
							break;

						case 3 : // exit from division
							user_node = out_div(head_div, user_node, company_node);
							break;

						case 4 : // go to user
							goto user;
						}
					}
					break;

				case 4 :
					goto beginning;
				}
			}
			break;

		case 3 :
			goto end;
		}
	}
end:;
	CLS;
	LINE;
}