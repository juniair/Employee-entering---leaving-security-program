#include "header.h"
#pragma once

#define COL GetStdHandle(STD_OUTPUT_HANDLE)
#define BLOOD SetConsoleTextAttribute(COL, 0x0004)
#define RED SetConsoleTextAttribute(COL, 0x000c)
#define WHITE SetConsoleTextAttribute(COL, 0x000f)

#define BLANK " "
#define CLS system("cls");
#define SET gotoxy(10,6);
#define NEXT gotoxy(10,y++);
#define LINE menu_outline(15);

int menu_outline(int line)
{
	int i=0;
	printf("\n%3s┌───────────────────────────────────┐\n", BLANK);
	printf("%3s│%27s신%5s세%5s계%27s│\n", BLANK, BLANK, BLANK, BLANK, BLANK);
	printf("%3s│%25s(출입 보안 프로그램)%25s│\n", BLANK, BLANK, BLANK);
	printf("%3s│───────────────────────────────────│\n", BLANK);
	for(i=0;i<line;i++)
		printf("%3s│%70s│\n", BLANK, BLANK);
	printf("%3s│───────────────────────────────────│\n", BLANK);
	printf("%3s│%3s(Copyrightⓒ. 2014. 자료구조니깐 자러가죠. All rights reserved.)%3s│\n", BLANK, BLANK, BLANK);
	printf("%3s└───────────────────────────────────┘\n", BLANK);
	gotoxy(10,6);
}

int menu_view(char **menu, int sel, int size)
{
	int x=10, y=6, i=0, j=0;
	SET;

	for(i=0;i<size;i++)
	{
		if((i+1) == sel)
			RED;
		j = 0;
		x = 10;
		gotoxy(x, y++);
		printf("%s\n", menu[i]);
		if((i+1) == sel)
			WHITE;
	}
}


int menu_sel(char **menu, int sel, int size)
{
	char key;
	
	do
	{
		CLS;
		LINE;
		menu_view(menu, sel, size);
		key = getch();
		if(key==72)
		{
			if(sel > 1)
				sel--;
			menu_view(menu, sel, size);
		}
		else if(key==80)
		{
			if(sel < size)
				sel++;
			menu_view(menu, sel, size);
		}
		else if(key==13)
			break;
		else
			continue;

	} while(1);

	return sel;
}
