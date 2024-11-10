#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************************************************************************************************************************************************/
//prototypes
void sign_up(struct users* members, int num_plus_1, int num_of_lines);
int login(struct users* members, int num_plus_1);
int counting_lines(FILE* users_file);
int storing_users_data(struct users* members, int num_plus_1);
void adding_hall(struct halls* hall, int num_of_halls, int hall_plus_1);
int counting_halls(FILE* hall_file);
int storing_halls_data(struct halls* hall, int hall_plus_1);
void adding_food(struct foods* food, struct halls* hall, int num_of_foods, int food_plus_1, int hall_plus_1);
int counting_foods(FILE* food_file);
int storing_foods_data(struct foods* food, int food_plus_1);
void adding_coupon(struct coupons* coupon, int num_of_coupons, int coupon_plus_1);
int counting_coupons(FILE* coupon_file);
int storing_coupons_data(struct coupons* coupon, int coupon_plus_1);

/***************************************************************************************************************************************************************************************************/
struct users {
	char* username;
	long long unsigned student_id;
	char* password;
};
/***************************************************************************************************************************************************************************************************/
struct halls {
	char* hall_name;
	long long unsigned hall_id;
	unsigned capacity;
};
/***************************************************************************************************************************************************************************************************/
struct foods {
	char* food_name;
	long long unsigned food_id;
	unsigned price;
	unsigned capacity;
	long long unsigned hall_id;
	unsigned day;

};
/***************************************************************************************************************************************************************************************************/
struct coupons {
	long long unsigned coupon_id;
    unsigned percentage;
	unsigned capacity;
};
/***************************************************************************************************************************************************************************************************/
void sign_up(struct users* members, int num_plus_1, int num_of_lines)
{
	FILE* information;
	//opening users' file for appending
	information = fopen("users.txt", "a");
		int compare1, compare2;
			//scanning users' information
			for (int x = num_of_lines; x < num_plus_1; x++) {
				members[x].username = (char*)malloc(sizeof(char*));
					scanf("%s", members[x].username);
			}

		for (int x = num_of_lines; x < num_plus_1; x++) {
			scanf("%llu", &members[x].student_id);
		}

		for (int x = num_of_lines; x < num_plus_1; x++) {
			members[x].password = (char*)malloc(sizeof(char*));
			scanf("%s", members[x].password);
		}
		//checking if the username or the id is unique or not
		if (num_of_lines != 0) {
			for (int i = 0; i < num_of_lines; i++) {
				compare1 = strcmp(members[num_of_lines].username, members[i].username);
				if (members[num_of_lines].student_id == members[i].student_id)
					compare2 = 0;
				else
					compare2 = 1;
				if (compare1 == 0 || compare2 == 0) {
					printf("User already exists\n");
					break;
				}
				else {
					//adding the given information to users' file after checking all the previous members
					if (i == num_of_lines - 1) {
						fprintf(information, "%s %llu %s\n", members[num_of_lines].username, members[num_of_lines].student_id, members[num_of_lines].password);
						printf("Done\n");
					}
				}
			}
		}
		//no need to check if it's the first member
		else if (num_of_lines == 0) {
			fprintf(information, "%s %llu %s\n", members[num_of_lines].username, members[num_of_lines].student_id, members[num_of_lines].password);
			printf("Done\n");
		}
		//closing the file that we have opened
		fclose(information);
}
/***************************************************************************************************************************************************************************************************/
int login(struct users* members, int num_plus_1) {
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
		
	
	//checking if the username and password are true or not
	if ((strcmp(username, "admin") != 0) || (strcmp(password, "admin") != 0)) {
		for (int i = 0; i < num_plus_1; i++) {
			if ((strcmp(username, members[i].username) == 0) && (strcmp(password, members[i].password) == 0)) {
				printf("Welcome %s\n", members[i].username);
				return 2;
				break;
			}
			else {
				//after checking the infomation of all members and not finding any same member
				if (i == num_plus_1 - 1)
					printf("Wrong username or pass\n");
				    
			}
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
int counting_lines(FILE* users_file) {
	int num_of_lines = 0;
	char c;
	//adding number of members by checking '\n'
	while ((c = fgetc(users_file)) != EOF) {
		if (c == '\n') {
			num_of_lines += 1;
		}
	}
	return num_of_lines;
}
/***************************************************************************************************************************************************************************************************/
int storing_users_data(struct users* members, int num_plus_1) {
	FILE* users_file1;
	users_file1 = fopen("users.txt", "r");
	//saving the datas in users' file in the structure
	for (int j = 0; j < num_plus_1 - 1; j++) {
		if (!feof(users_file1)) {
			fscanf(users_file1, "%s %llu %s", (members + j)->username, &(members + j)->student_id, (members + j)->password);
		}
	}
	//closing the file that we have opened
	fclose(users_file1);
	return members;
}
/***************************************************************************************************************************************************************************************************/
void adding_hall(struct halls* hall, int num_of_halls, int hall_plus_1) {
	while (1) {
		FILE* hall_information;
		//opening halls' file for appending
		hall_information = fopen("halls.txt", "a");
		int compare;
		//scanning halls' information
		for (int x = num_of_halls; x < hall_plus_1; x++) {
			hall[x].hall_name = (char*)malloc(sizeof(char*));
			scanf("%s", hall[x].hall_name);
		}

		for (int x = num_of_halls; x < hall_plus_1; x++) {
			scanf("%llu", &hall[x].hall_id);
		}

		for (int x = num_of_halls; x < hall_plus_1; x++) {
			scanf("%u", &hall[x].capacity);
		}
		//checking if the hall id is unique or not
		if (num_of_halls != 0) {
			for (int i = 0; i < num_of_halls; i++) {
				if (hall[num_of_halls].hall_id == hall[i].hall_id)
					compare = 0;
				else
					compare = 1;
				if (compare == 0) {
					printf("Hall id is not unique\n");
					break;
				}
				else {
					//adding the given information to halls' file after checking all the previous halls
					if (i == num_of_halls - 1) {
						fprintf(hall_information, "%s %llu %u\n", hall[num_of_halls].hall_name, hall[num_of_halls].hall_id, hall[num_of_halls].capacity);
						printf("Done\n");
					}
				}
			}
		}
		//no need to check if it's the first hall
		else if (num_of_halls == 0) {
			fprintf(hall_information, "%s %llu %u\n", hall[num_of_halls].hall_name, hall[num_of_halls].hall_id, hall[num_of_halls].capacity);
			printf("Done\n");
		}
		//closing the file that we have opened
		fclose(hall_information);
		int c;
		//stop adding hall after pressing ENTER
		if ((c = getchar()) == '\n') break;
	}
}
/***************************************************************************************************************************************************************************************************/
int counting_halls(FILE* hall_file) {
	int num_of_halls = 0;
	char c;
	//adding number of halls by checking '\n'
	while ((c = fgetc(hall_file)) != EOF) {
		if (c == '\n') {
			num_of_halls += 1;
		} 
	}
	return num_of_halls;
}
/***************************************************************************************************************************************************************************************************/
int storing_halls_data(struct halls* hall, int hall_plus_1) {
	FILE* hall_file1;
	hall_file1 = fopen("halls.txt", "r");
	//saving the datas in halls' file in the structure
	for (int j = 0; j < hall_plus_1 - 1; j++) {
		if (!feof(hall_file1)) {
			fscanf(hall_file1, "%s %llu %u", (hall + j)->hall_name, &(hall + j)->hall_id, &(hall + j)->capacity);
		}
	}
	//closing the file that we have opened
	fclose(hall_file1);
	return hall;
}
/***************************************************************************************************************************************************************************************************/
void adding_food(struct foods* food, struct halls* hall, int num_of_foods, int food_plus_1, int hall_plus_1) {
	while (1) {
		FILE* food_information;
		//opening foods; file for appending
		food_information = fopen("foods.txt", "a");
		int compare;
		//scanning foods' information
		for (int x = num_of_foods; x < food_plus_1; x++) {
			food[x].food_name = (char*)malloc(sizeof(char*));
			scanf("%s", food[x].food_name);
		}

		for (int x = num_of_foods; x < food_plus_1; x++) {
			scanf("%llu", &food[x].food_id);
		}

		for (int x = num_of_foods; x < food_plus_1; x++) {
			scanf("%u", &food[x].price);
		}

		for (int x = num_of_foods; x < food_plus_1; x++) {
			scanf("%u", &food[x].capacity);
		}

		for (int x = num_of_foods; x < food_plus_1; x++) {
			scanf("%llu", &food[x].hall_id);
		}

		for (int x = num_of_foods; x < food_plus_1; x++) {
			scanf("%u", &food[x].day);
		}
		//day must be between 1 and 7
		if (food[num_of_foods].day > 0 && food[num_of_foods].day < 8) {
			//checking if the food id is unique ornot
			if (num_of_foods != 0) {
				for (int i = 0; i < num_of_foods; i++) {
					if (food[num_of_foods].food_id == food[i].food_id)
						compare = 0;
					else
						compare = 1;
					if (compare == 0) {
						printf("Food id is not unique\n");
						break;
					}
					else {
						if (i == num_of_foods - 1) {
							//checking if the hall id exists or not
							for (int j = 0; j < hall_plus_1; j++) {
								if (food[num_of_foods].hall_id == hall[j].hall_id) {
									//adding the given information to foods' file after checking all the previous foods and halls
									fprintf(food_information, "%s %llu %u %u %llu %u\n", food[num_of_foods].food_name, food[num_of_foods].food_id, food[num_of_foods].price, food[num_of_foods].capacity, food[num_of_foods].hall_id, food[num_of_foods].day);
									printf("Done\n");
									break;
								}
								else {
									if (j == hall_plus_1 - 1) {
										printf("Wrong hall id\n");
										break;
									}
								}
							}
						}
					}
				}
			}
			//no need to check if it's the first food
			else if (num_of_foods == 0) {
				fprintf(food_information, "%s %llu %u %u %llu %u\n", food[num_of_foods].food_name, food[num_of_foods].food_id, food[num_of_foods].price, food[num_of_foods].capacity, food[num_of_foods].hall_id, food[num_of_foods].day);
				printf("Done\n");
			}
		}
		//closing the file that we have opened
		fclose(food_information);
		int c;
		//stop adding food after pressing ENTER
		if ((c = getchar()) == '\n') break;
	}
}
/***************************************************************************************************************************************************************************************************/
int counting_foods(FILE* food_file) {
	int num_of_foods = 0;
	char c;
	//adding number of foods by checking '\n'
	while ((c = fgetc(food_file)) != EOF) {
		if (c == '\n') {
			num_of_foods += 1;
		}
	}
	return num_of_foods;
}
/***************************************************************************************************************************************************************************************************/
int storing_foods_data(struct foods* food, int food_plus_1) {
	FILE* food_file1;
	food_file1 = fopen("foods.txt", "r");
	//saving the datas in food' file in the structure
	for (int j = 0; j < food_plus_1 - 1; j++) {
		if (!feof(food_file1)) {
			fscanf(food_file1, "%s %llu %u %u %llu %u", (food + j)->food_name, &(food + j)->food_id, &(food + j)->price, &(food + j)->capacity, &(food + j)->hall_id, &(food + j)->day);
		}
	}
	fclose(food_file1);
	return food;
}
/***************************************************************************************************************************************************************************************************/
void adding_coupon(struct coupons* coupon, int num_of_coupons, int coupon_plus_1) {
	while (1) {
		FILE* coupon_information;
		//opening coupons' file for appending
		coupon_information = fopen("coupons.txt", "a");
		int compare;
		//scanning coupons' information
		for (int x = num_of_coupons; x < coupon_plus_1; x++) {
			scanf("%llu", &coupon[x].coupon_id);
		}

		for (int x = num_of_coupons; x < coupon_plus_1; x++) {
			scanf("%u", &coupon[x].percentage);
		}

		for (int x = num_of_coupons; x < coupon_plus_1; x++) {
			scanf("%u", &coupon[x].capacity);
		}
		//the percentage must be between 1 and 100
		if (coupon[num_of_coupons].percentage > 0 && coupon[num_of_coupons].percentage < 101) {
			//checking if the coupon id is unique or not
			if (num_of_coupons != 0) {
				for (int i = 0; i < num_of_coupons; i++) {
					if (coupon[num_of_coupons].coupon_id == coupon[i].coupon_id)
						compare = 0;
					else
						compare = 1;
					if (compare == 0) {
						printf("Coupon id is not unique\n");
						break;
					}
					else {
						if (i == num_of_coupons - 1) {
							//adding the given information to coupons' file after checking all the previous coupons
							fprintf(coupon_information, "%llu %u %u\n", coupon[num_of_coupons].coupon_id, coupon[num_of_coupons].percentage, coupon[num_of_coupons].capacity);
							printf("Done\n");
						}
					}
				}
			}
			//no need to check if it's the first coupon
			else if (num_of_coupons == 0) {
				fprintf(coupon_information, "%llu %u %u\n", coupon[num_of_coupons].coupon_id, coupon[num_of_coupons].percentage, coupon[num_of_coupons].capacity);
				printf("Done\n");
			}
		}
		//closing the file that we have opened
		fclose(coupon_information);
		int c;
		//stop adding coupon after pressing ENTER
		if ((c = getchar()) == '\n') break;
	}
}
/***************************************************************************************************************************************************************************************************/
int counting_coupons(FILE* coupon_file) {
	int num_of_coupons = 0;
	char c;
	//adding number of coupons by checking '\n'
	while ((c = fgetc(coupon_file)) != EOF) {
		if (c == '\n') {
			num_of_coupons += 1;
		}
	}
	return num_of_coupons;
}
/***************************************************************************************************************************************************************************************************/
int storing_coupons_data(struct coupons* coupon, int coupon_plus_1) {
	FILE* coupon_file1;
	coupon_file1 = fopen("coupons.txt", "r");
	//saving the datas in coupons' file in the structure
	for (int j = 0; j < coupon_plus_1 - 1; j++) {
		if (!feof(coupon_file1)) {
			fscanf(coupon_file1, "%llu %u %u", &(coupon + j)->coupon_id, &(coupon + j)->percentage, &(coupon + j)->capacity);
		}
	}
	//closing the file that we have opened
	fclose(coupon_file1);
	return coupon;
}
/***************************************************************************************************************************************************************************************************/
int main() {
	while (1) {
		char start1[] = "signup";
		char start2[] = "login";
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
		
		FILE* users_file;
		users_file = fopen("users.txt", "r");
		//creating the file if it doesn't exist
		if (users_file == NULL) {
			users_file = fopen("users.txt", "w");
		}
		int num_of_lines;
		num_of_lines = counting_lines(users_file);
		fclose(users_file);
		int num_plus_1 = num_of_lines + 1;
		//allocating memory to the structure
		struct users* members = (struct users*)malloc(num_plus_1 * sizeof(struct users));
		//allocating mamory to arrays in the structure
		for (int j = 0; j < num_plus_1; j++) {
			members[j].username = (char*)malloc(sizeof(char*));
			members[j].password = (char*)malloc(sizeof(char*));
		}
		members = storing_users_data(members, num_plus_1);
		//signup
		if (strcmp(start, start1) == 0) {
			sign_up(members, num_plus_1, num_of_lines);
			free(start);
			free(members);
		}
		//login
		else if (strcmp(start, start2) == 0) {
			int login1;
			for (int i = 0; i < 1; i++)
				login1 = login(members, num_plus_1);
			//when admin logins
			if (login1 == 1) {
				while (1) {
					//different commands that admin can use
					char command1[] = "add_hall";
					char command2[] = "add_food";
					char command3[] = "add_coupon";
					char command4[] = "logout";
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
					
      				//adding hall
					if (strcmp(command, command1) == 0) {
						FILE* hall_file;
						hall_file = fopen("halls.txt", "r");
						//creating the file if it doesn't exist
						if (hall_file == NULL) {
							hall_file = fopen("halls.txt", "w");
						}
						int num_of_halls;
						num_of_halls = counting_halls(hall_file);
						fclose(hall_file);
						int hall_plus_1 = num_of_halls + 1;
						//allocating memory to the structure
						struct halls* hall = (struct halls*)malloc(hall_plus_1 * sizeof(struct halls));
						//allocating mamory to arrays in the structure
						for (int j = 0; j < hall_plus_1; j++) {
							hall[j].hall_name = (char*)malloc(sizeof(char*));
						}
						hall = storing_halls_data(hall, hall_plus_1);
						adding_hall(hall, num_of_halls, hall_plus_1);
						free(command);
						free(hall);
					}
					//adding food
					else if (strcmp(command, command2) == 0) {
						FILE* food_file;
						food_file = fopen("foods.txt", "r");
						//creating the file if it doesn't exist
						if (food_file == NULL) {
							food_file = fopen("foods.txt", "w");
						}
						int num_of_foods;
						num_of_foods = counting_foods(food_file);
						fclose(food_file);
						int food_plus_1 = num_of_foods + 1;
						//allocating memory to the structure
						struct foods* food = (struct foods*)malloc(food_plus_1 * sizeof(struct foods));
						//allocating mamory to arrays in the structure
						for (int j = 0; j < food_plus_1; j++) {
							food[j].food_name = (char*)malloc(sizeof(char*));
						}
						food = storing_foods_data(food, food_plus_1);
						FILE* hall_file;
						hall_file = fopen("halls.txt", "r");
						int num_of_halls;
						num_of_halls = counting_halls(hall_file);
						fclose(hall_file);
						int hall_plus_1 = num_of_halls + 1;
						struct halls* hall = (struct halls*)malloc(hall_plus_1 * sizeof(struct halls));
						for (int j = 0; j < hall_plus_1; j++) {
							hall[j].hall_name = (char*)malloc(sizeof(char*));
						}
						hall = storing_halls_data(hall, hall_plus_1);
						adding_food(food, hall, num_of_foods, food_plus_1, hall_plus_1);
						free(command);
						free(food);
						free(hall);
					}
					//adding coupon
					else if (strcmp(command, command3) == 0) {
						FILE* coupon_file;
						coupon_file = fopen("coupons.txt", "r");
						//creating the file if it doesn't exist
						if (coupon_file == NULL) {
							coupon_file = fopen("coupons.txt", "w");
						}
						int num_of_coupons;
						num_of_coupons = counting_coupons(coupon_file);
						fclose(coupon_file);
						int coupon_plus_1 = num_of_coupons + 1;
						struct coupons* coupon = (struct coupons*)malloc(coupon_plus_1 * sizeof(struct coupons));
						coupon = storing_coupons_data(coupon, coupon_plus_1);
						adding_coupon(coupon, num_of_coupons, coupon_plus_1);
						free(command);
						free(coupon);
					}
					//logging out
					else if (strcmp(command, command4) == 0) {
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
				}

			}
			//when a regular member logins
			else if (login1 == 2) {
				while (1) {
					char command1[] = "add_hall";
					char command2[] = "add_food";
					char command3[] = "add_coupon";
					char command4[] = "logout";
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
					if ((strcmp(command, command1) == 0) || (strcmp(command, command2) == 0) || (strcmp(command, command3) == 0)) {
						printf("permission denied\n");
						free(command);
					}
					//logging out
					else if (strcmp(command, command4) == 0) {
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
				}
			}
			free(start);
		}
		//using admin's commands before logging in
		else if ((strcmp(start, "add_hall") == 0) || (strcmp(start, "add_food") == 0) || (strcmp(start, "add_coupon") == 0)) {
					printf("access denied\n");
					free(start);
		}
		//logging out before logging in
		else if ((strcmp(start, "logout") == 0)) {
		printf("You should login first\n");
		free(start);
		}
	}
}