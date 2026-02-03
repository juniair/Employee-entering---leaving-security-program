#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#pragma once

typedef enum {false, true} bool;

typedef struct DefaultUserData {
	char name[20];
	char id[20];
	char division[20];
	char rank;
} DefaultUserData;

typedef struct DefaultDivData {
	char name[20];
	char rank;
	char enable_time[20];
} DefaultDivData;

typedef struct User {
	DefaultUserData userData;
	struct User *next;
	bool access;
	char acc_time[30];
} User;

typedef struct Div {
	DefaultDivData divData;
	struct Div *next;
	bool access;
	char acc_time[30];
} Div;

typedef struct UserList {
	DefaultUserData userData;
	struct UserList *next;
	Div *divLog;
} UserList;

typedef struct DivList {
	DefaultDivData divData;
	struct DivList *next;
	User *userLog;
} DivList;


typedef struct DivList2 {
	DefaultDivData divData;
	struct DivList2 *next;
	User *userLog;
} DivList2;

