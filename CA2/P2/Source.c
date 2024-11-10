#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************************************************************************************************************************************************/
//prototypes
void sign_up(struct members* head_members);
int login(struct members* head_members, struct members* test);
void storing_users_data(struct members* head_members);
void adding_hall(struct halls* head_halls, int times_hall);
void adding_food(struct foods* head_foods, struct members* head_halls, int times_food);
void adding_coupon(struct coupons* head_coupon, int times_coupon);
void food_report(struct food* head_foods, int* times_food);
void hall_report(struct halls* head_halls, int times_hall);
void admin_command(struct members* head_mebers, struct halls* head_halls, struct foods* head_foods, struct coupons* head_coupons, int* times_hall, int* times_food, int* times_coupon);
void member_command(struct members* login1, struct members* head_members, struct foods* head_foods, struct halls* head_halls, struct coupons* head_coupons);
void exit_program(struct members* head_members, struct halls* head_halls, struct foods* head_foods, struct coupons* head_coupons, int* times_hall, int* times_food, int* times_coupon);

/***************************************************************************************************************************************************************************************************/
//structures
struct reserve {
	char reserved_menu_item[100];
	unsigned reserved_price;
	long long unsigned reserved_food_id;
	char reserved_hall_name[100];
	struct reserve* next_reserved;
};
/***************************************************************************************************************************************************************************************************/
struct members {
	char username[100];
	long long unsigned student_id;
	char password[100];
	int money;
	struct reserve* reserved_food;
	struct members* next_user;
};
/***************************************************************************************************************************************************************************************************/
struct halls {
	char hall_name[100];
	long long unsigned hall_id;
	unsigned capacity;
	int num_of_reserves[7];
	int hall_profit[7];
	struct halls* next_hall;
};
/***************************************************************************************************************************************************************************************************/
struct foods {
	char food_name[100];
	long long unsigned food_id;
	unsigned price;
	unsigned capacity;
	long long unsigned hall_id;
	unsigned day;
	int num_of_reserves;
	int food_profit;
	struct foods* next_food;
};
/***************************************************************************************************************************************************************************************************/
struct coupons {
	long long unsigned coupon_id;
	unsigned percentage;
	unsigned capacity;
	struct coupons* next_coupon;
};
/***************************************************************************************************************************************************************************************************/
void sign_up(struct members* head_members)
{
	//a pointer which will point to the last node
	struct members* cur1_members = head_members;
	//a pointer which saves the entrance data
	struct members* tmp_members = (struct members*)malloc(sizeof(struct members));
	//a pointer which traverse the list to see if the entrance username and id is unique or not
	struct members* cur2_members = head_members;
	struct reserve* head_reserve = (struct reserve*)malloc(sizeof(struct reserve));

	FILE* information;
	//opening users' file for appending
	information = fopen("users.txt", "a");
	int compare1, compare2;
	//moving the pointer to the last node
	while (cur1_members->next_user != NULL) {
		cur1_members = cur1_members->next_user;
	}
	//scanning new data
	scanf("%s", tmp_members->username);
	scanf("%llu", &tmp_members->student_id);
	scanf("%s", tmp_members->password);
	tmp_members->next_user = NULL;
	//checking if the username or the id are unique or not
	if (head_members->next_user != NULL) {
		while (cur2_members != NULL) {
			compare1 = strcmp(tmp_members->username, cur2_members->username);
			if (tmp_members->student_id == cur2_members->student_id)
				compare2 = 0;
			else
				compare2 = 1;
			if (compare1 == 0 || compare2 == 0) {
				printf("User already exists\n");
				break;
			}
			else {
				//adding the given information to users' file after checking all the previous members and connecting tmp pointer to the end of the linked list
				if (cur2_members->next_user == NULL) {
					fprintf(information, "%s %llu %s\n", tmp_members->username, tmp_members->student_id, tmp_members->password);
					cur1_members->next_user = tmp_members;
					tmp_members->money = 0;
					tmp_members->reserved_food = head_reserve;
					head_reserve->next_reserved = NULL;
					printf("Done\n");
					break;
				}
			}
			cur2_members = cur2_members->next_user;
		}
	}
	//no need to check if it's the first member
	else if (head_members->next_user == NULL) {
		fprintf(information, "%s %llu %s\n", tmp_members->username, tmp_members->student_id, tmp_members->password);
		printf("Done\n");
	}
	//closing the file that we have opened
	fclose(information);
}
/***************************************************************************************************************************************************************************************************/
int login(struct members* head_members, struct members* test){
	test = head_members;
	//getting the characters one by one
	char* username = (char*)malloc(sizeof(char));
	int i = 0;
	char c;
	int tmp1;
	while ((c = getchar()) != ' ') {
		username[i] = c;
		i++;
		tmp1 = (char*)realloc(username, (i + 1) * sizeof(char));
		if (tmp1 != NULL)
			username = tmp1;
		username[i] = '\0';
	}


	char* password = (char*)malloc(sizeof(char));
	int j = 0;
	char h;
	int tmp2;
	while ((h = getchar()) != '\n') {
		if (h == ' ') continue;
		else {
			password[j] = h;
			j++;
			tmp2 = (char*)realloc(password, (j + 1) * sizeof(char));
			if (tmp2 != NULL)
				password = tmp2;
			password[j] = '\0';
		}
	}
	//checking if the regular member's username and password are true or not
	if ((strcmp(username, "admin") != 0) || (strcmp(password, "admin") != 0)) {
		while (test != NULL) {
			if ((strcmp(username, test->username) == 0) && (strcmp(password, test->password) == 0)) {
				printf("Welcome %s\n", test->username);
				return test;
			}
			else {
				//after checking the infomation of all members and not finding any same member
				if (test->next_user == NULL) {
					printf("Wrong username or pass\n");
					return 0;
				}
			}
			test = test->next_user;
		}
	}
	//admin's information doesn't exist in users' file
	else if ((strcmp(username, "admin") == 0) && (strcmp(password, "admin") == 0)) {
		printf("welcome admin\n");
		return 1;
	}
	//freeing the allocated memory
	free(username);
	free(password);
}
/***************************************************************************************************************************************************************************************************/
void storing_users_data(struct members* head_members) {
	//a pointer which saves the second user's data
	struct members* tmp_members = (struct members*)malloc(sizeof(struct members));
	//pointers to allocate memory to the first and second users' reserves
	struct reserve* store1_reserve = (struct reserve*)malloc(sizeof(struct reserve));
	struct reserve* store2_reserve = (struct reserve*)malloc(sizeof(struct reserve));
	FILE* users_file;
	users_file = fopen("users.txt", "r");
	if (users_file == NULL) {
		//creating a file if it doesn't exist already
		users_file = fopen("users.txt", "w");
	}
	//scanning the first and second users' data from the file and saving them in 2 nodes and connecting the nodes
	fscanf(users_file, "%s %llu %s", head_members->username, &head_members->student_id, head_members->password);
	fscanf(users_file, "%s %llu %s", tmp_members->username, &tmp_members->student_id, tmp_members->password);
	head_members->next_user = tmp_members;
	tmp_members->next_user = NULL;
		head_members->money = 0;
		head_members->reserved_food = store1_reserve;
		store1_reserve->next_reserved = NULL;
		tmp_members->money = 0;
		tmp_members->reserved_food = store2_reserve;
		store2_reserve->next_reserved = NULL;
	
    //scanning the users' data from the file one by one and adding them to the list
	while (!feof(users_file)) {
		struct members* tmp2_members = (struct members*)malloc(sizeof(struct members));
		struct reserve* store_reserve = (struct reserve*)malloc(sizeof(struct reserve));
			fscanf(users_file, "%s %llu %s", tmp2_members->username, &tmp2_members->student_id, tmp2_members->password);
			tmp2_members->money = 0;
			tmp2_members->reserved_food = store_reserve;
			store_reserve->next_reserved = NULL;
			//connecting the new node to the list
			tmp_members->next_user = tmp2_members;
			tmp_members = tmp_members->next_user;
			tmp_members->next_user = NULL;
			tmp2_members = NULL;
			free(tmp2_members);
	}
	//closing the file that we have opened
	fclose(users_file);
}
/***************************************************************************************************************************************************************************************************/
void adding_hall(struct halls* head_halls, int times_hall) {
	    //a pointer which will point to the last node
		struct halls* cur1_halls = head_halls;
		//a pointer which saves the entrance data
		struct halls* tmp_halls = (struct halls*)malloc(sizeof(struct halls));
		//a pointer which traverse the list to see if the entrance id is unique or not
		struct halls* cur2_halls = head_halls;
		int compare1;
		//scanning the first hall's data
		if (times_hall == 0) {
			scanf("%s", tmp_halls->hall_name);
			scanf("%llu", &tmp_halls->hall_id);
			scanf("%u", &tmp_halls->capacity);
			tmp_halls->next_hall = NULL;
			for (int i = 0; i < 7; i++) {
				//initializing each day's number of reserves and profit for this hall
				tmp_halls->num_of_reserves[i] = 0;
				tmp_halls->hall_profit[i] = 0;
			}
			*head_halls = *tmp_halls;			
			printf("Done\n");
		}
		else {
			    //moving the pointer to the last node
	        	while (cur1_halls->next_hall != NULL) {
				cur1_halls = cur1_halls->next_hall;
			    }
			scanf("%s", tmp_halls->hall_name);
			scanf("%llu", &tmp_halls->hall_id);
			scanf("%u", &tmp_halls->capacity);
			tmp_halls->next_hall = NULL;
		     	//checking if the id is unique or not
				while (cur2_halls != NULL) {
					if (tmp_halls->hall_id == cur2_halls->hall_id)
						compare1 = 0;
					else
						compare1 = 1;
					if (compare1 == 0) {
						printf("Hall id is not unique\n");
						break;
					}
					else {
						//connecting tmp pointer to the end of the linked list after checking all the previous nodes
						if (cur2_halls == cur1_halls) {
							cur1_halls->next_hall = tmp_halls;
							for (int i = 0; i < 7; i++) {
								tmp_halls->num_of_reserves[i] = 0;
								tmp_halls->hall_profit[i] = 0;
							}
							printf("Done\n");
							break;
						}
					}
					cur2_halls = cur2_halls->next_hall;
				}
		
		}
	
}
/***************************************************************************************************************************************************************************************************/
void adding_food(struct foods* head_foods, struct members* head_halls, int* times_food) {
	//a pointer which will point to the last node
	struct foods* cur1_foods = head_foods;
	//a pointer which saves the entrance data
	struct foods* tmp_foods = (struct foods*)malloc(sizeof(struct foods));
	//a pointer which traverse the list to see if the entrance id is unique or not
	struct foods* cur2_foods = head_foods;
	int compare1;
	//scanning the first food's data
	if (*times_food == 0) {
		scanf("%s", tmp_foods->food_name);
		scanf("%llu", &tmp_foods->food_id);
		scanf("%u", &tmp_foods->price);
		scanf("%u", &tmp_foods->capacity);
		scanf("%llu", &tmp_foods->hall_id);
		scanf("%u", &tmp_foods->day);
		if (tmp_foods->day > 0 && tmp_foods->day < 8) {
			struct halls* check_halls = head_halls;
			while (check_halls != NULL) {
				//check if the hall id exists or not
				if (tmp_foods->hall_id == check_halls->hall_id)
					compare1 = 0;
				else
					compare1 = 1;
				if (compare1 == 0) {
					tmp_foods->next_food = NULL;
					tmp_foods->num_of_reserves = 0;
					tmp_foods->food_profit = 0;
					*head_foods = *tmp_foods;
					printf("Done\n");
					*times_food += 1;
					break;
				}
				check_halls = check_halls->next_hall;
				if (check_halls == NULL) {
					printf("Wrong hall id\n");
				}
			}		
		}
	}
	else {
		//moving the pointer to the last node
		while (cur1_foods->next_food != NULL) {
			cur1_foods = cur1_foods->next_food;
		}
		scanf("%s", tmp_foods->food_name);
		scanf("%llu", &tmp_foods->food_id);
		scanf("%u", &tmp_foods->price);
		scanf("%u", &tmp_foods->capacity);
		scanf("%llu", &tmp_foods->hall_id);
		scanf("%u", &tmp_foods->day);
		tmp_foods->next_food = NULL;
		if (tmp_foods->day > 0 && tmp_foods->day < 8) {
			//checking if the id is unique or not
			while (cur2_foods != NULL) {
				if (tmp_foods->food_id == cur2_foods->food_id)
					compare1 = 0;
				else
					compare1 = 1;
				if (compare1 == 0) {
					printf("Food id is not unique\n");
					break;
				}
				else {
					//connecting tmp pointer to the end of the linked list after checking all the previous nodes and all the hall ids
					if (cur2_foods == cur1_foods) {
						struct halls* check_halls = head_halls;
						while (check_halls != NULL) {
							if (tmp_foods->hall_id == check_halls->hall_id)
								compare1 = 0;
							else
								compare1 = 1;
							if (compare1 == 0) {
								tmp_foods->num_of_reserves = 0;
								tmp_foods->food_profit = 0;
								cur1_foods->next_food = tmp_foods;
								printf("Done\n");
								*times_food += 1;
								break;
							}
							check_halls = check_halls->next_hall;
							if (check_halls == NULL) {
								printf("Wrong hall id\n");
							}
						}
						break;
					}
				}
				cur2_foods = cur2_foods->next_food;
			}
		}

	}
}
/***************************************************************************************************************************************************************************************************/
void adding_coupon(struct coupons* head_coupons, int times_coupon) {
	//a pointer which will point to the last node
	struct coupons* cur1_coupons = head_coupons;
	//a pointer which saves the entrance data
	struct coupons* tmp_coupons = (struct coupons*)malloc(sizeof(struct coupons));
	//a pointer which traverse the list to see if the entrance id is unique or not
	struct coupons* cur2_coupons = head_coupons;
	int compare1;
	//scanning the first coupon's data
	if (times_coupon == 0) {
		scanf("%llu", &tmp_coupons->coupon_id);
		scanf("%u", &tmp_coupons->percentage);
		scanf("%u", &tmp_coupons->capacity);
		tmp_coupons->next_coupon = NULL;
		*head_coupons = *tmp_coupons;
		printf("Done\n");
	}
	else {
		//moving the pointer to the last node
		while (cur1_coupons->next_coupon != NULL) {
			cur1_coupons = cur1_coupons->next_coupon;
		}
		scanf("%llu", &tmp_coupons->coupon_id);
		scanf("%u", &tmp_coupons->percentage);
		scanf("%u", &tmp_coupons->capacity);
		tmp_coupons->next_coupon = NULL;
		//checking if the id is unique or not
		while (cur2_coupons != NULL) {
			if (tmp_coupons->coupon_id == cur2_coupons->coupon_id)
				compare1 = 0;
			else
				compare1 = 1;
			if (compare1 == 0) {
				printf("Coupon id is not unique\n");
				break;
			}
			else {
				//connecting tmp pointer to the end of the linked list after checking all the previous nodes
				if (cur2_coupons == cur1_coupons) {
					cur1_coupons->next_coupon = tmp_coupons;
					printf("Done\n");
					break;
				}
			}
			cur2_coupons = cur2_coupons->next_coupon;
		}
	}
}
/***************************************************************************************************************************************************************************************************/
void food_report(struct foods* head_foods, int* times_food) {
	struct foods* food_inf = head_foods;
	FILE* food_report;
	food_report = fopen("food_report.txt", "w");
	//printing existing foods' data in the file
	if (*times_food != 0) {
		while (food_inf != NULL) {
			fprintf(food_report, "%llu %u %d %d\n", food_inf->food_id, food_inf->capacity, food_inf->num_of_reserves, food_inf->food_profit);
			food_inf = food_inf->next_food;
		}
	}
	else {
		fprintf(food_report, "There's no food available");
	}
	printf("Done\n");
	fclose(food_report);
}
/***************************************************************************************************************************************************************************************************/
void hall_report(struct halls* head_halls, int times_hall) {
	struct halls* hall_inf = head_halls;
	FILE* hall_report;
	hall_report = fopen("hall_report.txt", "w");
	//printing existing halls' data for each day in the file
	if (times_hall != 0) {
		while (hall_inf != NULL) {
			fprintf(hall_report, "%llu %s %u\n", hall_inf->hall_id, hall_inf->hall_name, hall_inf->capacity);
			for (int i = 0; i < 7; i++) {
				fprintf(hall_report, "%d ", i+1);
				fprintf(hall_report, "%d %d\n", hall_inf->num_of_reserves[i], hall_inf->hall_profit[i]);
			}
			hall_inf = hall_inf->next_hall;
		}

	}
	else {
		fprintf(hall_report, "There's no hall");
	}
	printf("Done\n");
	fclose(hall_report);
}
/***************************************************************************************************************************************************************************************************/
void admin_command(struct members* head_members, struct halls* head_halls, struct foods* head_foods, struct coupons* head_coupons, int* times_hall, int* times_food, int* times_coupon) {
	while (1) {
		//getting the characters one by one
		char* command = (char*)malloc(sizeof(char));
		int a = 0;
		char d;
		int tmp;
		while ((d = getchar()) != ' ' && (d) != '\n') {
			command[a] = d;
			a++;
			tmp = (char*)realloc(command, (a + 1) * sizeof(char));
			if (tmp != NULL)
				command = tmp;
			command[a] = '\0';
		}
		if (strcmp(command, "add_hall") == 0) {
			adding_hall(head_halls, *times_hall);
			*times_hall += 1;
			free(command);
		}
		else if (strcmp(command, "add_food") == 0) {
			adding_food(head_foods, head_halls, *&times_food);
			free(command);
		}
		else if (strcmp(command, "add_coupon") == 0) {
			adding_coupon(head_coupons, *times_coupon);
			*times_coupon += 1;
			free(command);
		}
		else if (strcmp(command, "food_report") == 0) {
			food_report(head_foods, *&times_food);
			free(command);
		}
		else if (strcmp(command, "hall_report") == 0) {
			hall_report(head_halls, *times_hall);
			free(command);
		}
		else if (strcmp(command, "logout") == 0) {
			printf("Done\n");
			free(command);
			break;
		}
		//when you want to signup before logging out
		else if (strcmp(command, "signup") == 0) {
			printf("You should logout first\n");
			free(command);
		}
		//when you want to login another account before logging out this account
		else if (strcmp(command, "login") == 0) {
			printf("You should logout first\n");
			free(command);
		}
		//when you want to use regular members' commands
		else if (strcmp(command, "reserve") == 0) {
			printf("You should login as a regular member\n");
			free(command);
		}
		else if (strcmp(command, "reserve_discount") == 0) {
			printf("You should login as a regular member\n");
			free(command);
		}
		else if (strcmp(command, "get_money") == 0) {
			printf("You should login as a regular member\n");
			free(command);
		}
		else if (strcmp(command, "show_money") == 0) {
			printf("You should login as a regular member\n");
			free(command);
		}
		else if (strcmp(command, "menu") == 0) {
			printf("You should login as a regular member\n");
			free(command);
		}
		else if (strcmp(command, "show_reserved") == 0) {
			printf("You should login as a regular member\n");
			free(command);
		}
		else if (strcmp(command, "exit") == 0) {
			exit_program(head_members, head_halls, head_foods, head_coupons, *times_hall, *&times_food, *times_coupon);
		}
	
	}
}
/***************************************************************************************************************************************************************************************************/
void member_command(struct members* login1, struct members* head_members, struct foods* head_foods, struct halls* head_halls, struct coupons* head_coupons, int* times_hall, int* times_food, int* times_coupon) {
	while (1) {
		struct foods* check_food = head_foods;
		struct halls* check_hall = head_halls;
		struct coupons* check_coupon = head_coupons;
		struct reserve* check_reserve = login1->reserved_food;
		struct reserve* cur_reserve = (struct reserve*)malloc(sizeof(struct reserve));
		//getting the characters one by one
		char* command = (char*)malloc(sizeof(char));
		int a = 0;
		char d;
		int tmp;
		while ((d = getchar()) != ' ' && (d) != '\n') {
			command[a] = d;
			a++;
			tmp = (char*)realloc(command, (a + 1) * sizeof(char));
			if (tmp != NULL)
				command = tmp;
			command[a] = '\0';
		}

		//commands that a regular member can't use
		if ((strcmp(command, "add_hall") == 0) || (strcmp(command, "add_food") == 0) || (strcmp(command, "add_coupon") == 0)) {
			printf("permission denied\n");
			free(command);
		}
		else if (strcmp(command, "logout") == 0) {
			printf("Done\n");
			break;
			free(command);
		}
		//when you want to signup before logging out
		else if (strcmp(command, "signup") == 0) {
			printf("You should logout first\n");
			free(command);
		}
		//when you want to login another account before logging out this account
		else if (strcmp(command, "login") == 0) {
			printf("You should logout first\n");
			free(command);
		}
		else if (strcmp(command, "get_money") == 0) {
			int get_money;
			scanf("%d", &get_money);
			login1->money += get_money;
			printf("Done\n");
		}
		else if (strcmp(command, "show_money") == 0) {
			printf("%d\n", login1->money);
		}
		else if (strcmp(command, "menu") == 0) {
			for (int i = 1; i < 8; i++) {
				printf("%d:\n", i);
				while (check_food != NULL) {
					//checking each food's day 
					if (check_food->day == i) {
						while (check_hall != NULL) {
							if (check_hall->hall_id == check_food->hall_id) {
								printf("%s %u %llu %s %u\n", check_food->food_name, check_food->price, check_food->food_id, check_hall->hall_name, check_food->capacity);
							}
							check_hall = check_hall->next_hall;
						}
						check_hall = head_halls;
					}
					check_food = check_food->next_food;
				}
				//our check pointers have to point to the heads again for next commands
				check_food = head_foods;
			}
		}
		else if (strcmp(command, "reserve") == 0) {
		    long long unsigned food_id;
		    scanf("%llu", &food_id);
			while (check_food != NULL) {
				//checking if the food exists or not
				if (check_food->food_id == food_id) {
						while (check_hall != NULL) {
							//finding the hall that serves the wanted food
							if (check_hall->hall_id == check_food->hall_id) break;
							check_hall = check_hall->next_hall;
						}
						//needed conditions to reserve food
						if ((check_food->price <= login1->money) && (check_food->capacity > 0) && (check_hall->capacity > 0)) {
							login1->money -= check_food->price;
							//moving the pointer to the last node
							while (check_reserve->next_reserved != NULL) {
								check_reserve = check_reserve->next_reserved;
							}
							//adding the reserved food to the member's reserve linked list
							strcpy(check_reserve->reserved_menu_item,check_food->food_name);
							check_reserve->reserved_price = check_food->price;
							check_reserve->reserved_food_id = check_food->food_id;
							strcpy(check_reserve->reserved_hall_name, check_hall->hall_name);
							check_reserve->next_reserved = cur_reserve;
							cur_reserve->next_reserved = NULL;
							//updating food and hall's information
							check_food->capacity -= 1;
							check_food->num_of_reserves += 1;
							check_food->food_profit += check_food->price;
							for (int i = 0; i < 7; i++) {
								if (check_food->day == i+1) {
									check_hall->num_of_reserves[i] += 1;
									check_hall->hall_profit[i] += check_food->price;
								}
							}
							check_hall->capacity -= 1;
							printf("Done\n");
							break;
						}
						else {
							printf("Not possible\n");
							break;
						}
				}
				else {
					if (check_food->next_food == NULL) {
						printf("Not possible\n");
						break;
					}
				}
				check_food = check_food->next_food;
		    }
			//our check pointers have to point to the heads again for next commands
			check_food = head_foods;
			check_hall = head_halls;
		}
		else if (strcmp(command, "reserve_discount") == 0) {
		int check_if_reserved = 0;
			long long unsigned food_id, coupon_id;
			scanf("%llu %llu", &food_id, &coupon_id);
			while (check_food != NULL) {
				//checking if the food exists or not
				if (check_food->food_id == food_id) {
					while (check_coupon != NULL) {
						//checking if the coupon exists or not
						if (check_coupon->coupon_id == coupon_id) {
							while (check_hall != NULL) {
								//finding the hall that serves the wanted food
								if (check_hall->hall_id == check_food->hall_id) break;
								check_hall = check_hall->next_hall;
							}
							//needed conditions to reserve food
							if ((check_food->price <= login1->money) && (check_food->capacity > 0) && (check_hall->capacity > 0) && (check_coupon->capacity > 0)) {
								//applying the discount to food's price
								float discount = (float)(check_coupon->percentage) / 100;
								int decreased_amount = discount * (check_food->price);
								int new_price = check_food->price - decreased_amount;
								login1->money = login1->money - new_price;
								//moving the pointer to the last node
								while (check_reserve->next_reserved != NULL) {
									check_reserve = check_reserve->next_reserved;
								}
								//adding the reserved food to the member's reserve linked list
								strcpy(check_reserve->reserved_menu_item, check_food->food_name);
								check_reserve->reserved_price = new_price;
								check_reserve->reserved_food_id = check_food->food_id;
								strcpy(check_reserve->reserved_hall_name, check_hall->hall_name);
								check_reserve->next_reserved = cur_reserve;
								cur_reserve->next_reserved = NULL;
								//updating food and hall's information
								check_food->capacity -= 1;
								check_food->num_of_reserves += 1;
								check_food->food_profit += new_price;
								for (int i = 0; i < 7; i++) {
									if (check_food->day == i + 1) {
										check_hall->num_of_reserves[i] += 1;
										check_hall->hall_profit[i] += new_price;
									}
								}
								check_hall->capacity -= 1;
								check_coupon->capacity -= 1;
								printf("Done\n");
								check_if_reserved += 1;
								break;
							}
							else {
								printf("Not possible\n");
								check_if_reserved += 1;
								break;
							}
						}
						else {
							if (check_coupon->next_coupon == NULL) {
								printf("Not possible\n");
								check_if_reserved += 1;
								break;
							}
						}
						check_coupon = check_coupon->next_coupon;
					}
				}
				else if (check_if_reserved == 1) break;

				else {
					if (check_food->next_food == NULL)
						printf("Not possible\n");
				}
				check_food = check_food->next_food;
			}
			//our check pointers have to point to the heads again for next commands
			check_food = head_foods;
			check_hall = head_halls;
		}
		else if (strcmp(command, "show_reserved") == 0) {
		  for (int i = 1; i < 8; i++) {
			  printf("%d:\n", i);
			  if (*times_food != 0) {
				  while (check_reserve != NULL) {
					  while (check_food != NULL) {
						  if (check_food->food_id == check_reserve->reserved_food_id) {
							  if (check_food->day == i) {
								  //printing each day's reserved foods
								  printf("%s %u %llu %s\n", check_reserve->reserved_menu_item, check_reserve->reserved_price, check_reserve->reserved_food_id, check_reserve->reserved_hall_name);
							  }
						  }
						  check_food = check_food->next_food;
					  }
					  check_food = head_foods;
					  check_reserve = check_reserve->next_reserved;
				  }
			  }
			  check_reserve = login1->reserved_food;
		  }		  
	    }
		else if (strcmp(command, "exit") == 0) {
		exit_program(head_members, head_halls, head_foods, head_coupons, *times_hall, *&times_food, *times_coupon);
        }
	}
}
/***************************************************************************************************************************************************************************************************/
void exit_program(struct members* head_members, struct halls* head_halls, struct foods* head_foods, struct coupons* head_coupons, int times_hall, int* times_food, int times_coupon) {
	struct members* delete_member_node = head_members;
	struct members* member_reserve = head_members;	
	struct halls* delete_hall_node = head_halls;
	struct foods* delete_food_node = head_foods;
	struct coupons* delete_coupon_node = head_coupons;
    //deleting each member's reserved foods
	while (member_reserve != NULL) {
		struct reserve* delete_reserve_node = member_reserve->reserved_food;
		while (delete_reserve_node != NULL) {
			delete_reserve_node = delete_reserve_node->next_reserved;
			free(member_reserve->reserved_food);
			member_reserve->reserved_food = delete_reserve_node;
		}
		member_reserve = member_reserve->next_user;
	}
	while (delete_member_node != NULL) {
		delete_member_node = delete_member_node->next_user;
		free(head_members);
		head_members = delete_member_node;
	}
	if (times_hall == 0) {
		delete_hall_node = NULL;
		head_halls = delete_hall_node;
	}
	else if (times_hall != 0) {
		while (delete_hall_node != NULL) {
			delete_hall_node = delete_hall_node->next_hall;
			free(head_halls);
			head_halls = delete_hall_node;
		}
	}
	if (*times_food == 0) {
		delete_food_node = NULL;
		head_foods = delete_food_node;
	}
	else if (*times_food != 0) {
		while (delete_food_node != NULL) {
			delete_food_node = delete_food_node->next_food;
			free(head_foods);
			head_foods = delete_food_node;
		}
	}
	if (times_coupon == 0) {
		delete_coupon_node = NULL;
		head_coupons = delete_coupon_node;
	}
	else if (times_coupon != 0) {
		while (delete_coupon_node != NULL) {
			delete_coupon_node = delete_coupon_node->next_coupon;
			free(head_coupons);
			head_coupons = delete_coupon_node;
		}
	}
	//finishing the program
	exit(0);
}
/***************************************************************************************************************************************************************************************************/
int main() {
	//allocating memory to the heads
	struct members* head_members = (struct members*)malloc(sizeof(struct members));
	struct halls* head_halls = (struct halls*)malloc(sizeof(struct halls));
	struct foods* head_foods = (struct foods*)malloc(sizeof(struct foods));
	struct coupons* head_coupons = (struct coupons*)malloc(sizeof(struct coupons));
	struct members* test = (struct members*)malloc(sizeof(struct members));
	int times_save = 0;
	//number of times that we add hall or food or coupon
	int times_hall = 0;
	int times_food = 0;
	int times_coupon = 0;
	while (1) {
		char* start = (char*)malloc(sizeof(char));
		int i = 0;
		int tmp;
		char c;
		//getting the characters one by one
		while ((c = getchar()) != ' ' && c != '\n') {
			start[i] = c;
			i++;
			tmp = (char*)realloc(start, (i + 1) * sizeof(char));
			if (tmp != NULL)
				start = tmp;
			start[i] = '\0';
		}
		//storing information from the users' file to the linked list only when we start the program
		if (times_save == 0)
		storing_users_data(head_members);
		times_save += 1;
		
		if (strcmp(start, "signup") == 0) {
			sign_up(head_members);
			free(start);
		}
		
		else if (strcmp(start, "login") == 0) {
			struct members* login1 = (struct members*)malloc(sizeof(struct members));
		    login1 = login(head_members, test);
			//when admin logins
			if (login1 == 1) {
				admin_command(head_members, head_halls, head_foods, head_coupons, &times_hall, &times_food, &times_coupon);
			}
			//when we use wrong username or password
			else if (login1 == 0) {
				continue;
			}
			//when a regular member logins
			else if (login1 != 1 && login1 != 0) {
				member_command(login1, head_members, head_foods, head_halls, head_coupons, &times_hall, &times_food, &times_coupon);
			}		
			free(start);
		}
		//using admin's commands before logging in
		else if ((strcmp(start, "add_hall") == 0) || (strcmp(start, "add_food") == 0) || (strcmp(start, "add_coupon") == 0) || (strcmp(start, "food_report") == 0) || (strcmp(start, "hall_report") == 0)) {
			printf("access denied\n");
			free(start);
		}
		//using a regular member's commands before logging in
		else if ((strcmp(start, "get_money") == 0) || (strcmp(start, "show_money") == 0) || (strcmp(start, "menu") == 0) || (strcmp(start, "reserve") == 0) || (strcmp(start, "reserve_discount") == 0) || (strcmp(start, "show_reserved") == 0)) {
			printf("access denied\n");
			free(start);
		}
		//logging out before logging in
		else if ((strcmp(start, "logout") == 0)) {
			printf("You should login first\n");
			free(start);
		}
		else if ((strcmp(start, "exit") == 0)) {
			exit_program(head_members, head_halls, head_foods, head_coupons, times_hall, &times_food, times_coupon);
			free(start);
		}
	}
}