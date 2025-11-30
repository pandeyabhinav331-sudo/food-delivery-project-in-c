#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ==============================
// USER STRUCTURE
// ==============================
struct User {
    char userName[50];
    int age;
    char email[50];
    char mobile[20];
    char password[30];
};

struct User user[100];
int j = 0; // user count

// ==============================
// MENU ITEM STRUCTURE
// ==============================
struct Item {
    int id;
    char name[50];
    int price;
};

struct Item menu[] = {
    {1, "Burger", 80},
    {2, "Pizza", 150},
    {3, "French Fries", 60},
    {4, "Pasta", 120},
    {5, "Sandwich", 50}
};
int menuSize = 5;

// ==============================
// Load users from file
// ==============================
void load_users() {
    FILE *fp = fopen("E:\\FoodDeliverySystem.txt", "r");
    if (fp == NULL)
        return;

    j = 0;
    while (fscanf(fp, "%s %d %s %s %s", user[j].userName, &user[j].age,
                  user[j].email, user[j].mobile, user[j].password) != EOF) {
        j++;
    }
    fclose(fp);
}

// ==============================
// Save user to file
// ==============================
void save_user(struct User u) {
    FILE *fp = fopen("E:\\FoodDeliverySystem.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "%s %d %s %s %s\n", u.userName, u.age, u.email, u.mobile, u.password);
    fclose(fp);
}

// ==============================
// SIGN UP
// ==============================
void signup() {
    struct User temp;

    printf("\nEnter Username: ");
    scanf("%s", temp.userName);
    printf("Enter Age: ");
    scanf("%d", &temp.age);
    printf("Enter Email: ");
    scanf("%s", temp.email);
    printf("Enter Mobile: ");
    scanf("%s", temp.mobile);
    printf("Create Password: ");
    scanf("%s", temp.password);

    user[j] = temp;
    j++;

    save_user(temp);

    printf("\nUser Registered Successfully & Saved in File!\n");
}

// ==============================
// LOGIN
// ==============================
int login(char loggedUser[]) {
    char email[50], pass[30];
    printf("\nEnter Email: ");
    scanf("%s", email);
    printf("Enter Password: ");
    scanf("%s", pass);

    for (int i = 0; i < j; i++) {
        if (strcmp(email, user[i].email) == 0 && strcmp(pass, user[i].password) == 0) {
            printf("\nLogin Successful! Welcome %s\n", user[i].userName);
            strcpy(loggedUser, user[i].userName);
            return 1;
        }
    }

    printf("\nInvalid Email or Password!\n");
    return 0;
}

// ==============================
// SHOW MENU
// ==============================
void show_menu() {
    printf("\n=========== FOOD MENU ===========\n");
    for (int i = 0; i < menuSize; i++) {
        printf("%d. %s - Rs %d\n", menu[i].id, menu[i].name, menu[i].price);
    }
}

// ==============================
// ORDER FOOD
// ==============================
void order_food(char username[]) {
    int id, qty, total = 0;
    FILE *fp = fopen("orders.txt", "a");

    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    fprintf(fp, "User: %s\n", username);
    fprintf(fp, "Items Ordered:\n");

    while (1) {
        show_menu();
        printf("Enter item ID to order (0 to finish): ");
        scanf("%d", &id);

        if (id == 0)
            break;

        if (id < 1 || id > menuSize) {
            printf("Invalid Item!\n");
            continue;
        }

        printf("Enter Quantity: ");
        scanf("%d", &qty);

        total += menu[id - 1].price * qty;

        fprintf(fp, "%s x %d = Rs %d\n", menu[id - 1].name, qty, menu[id - 1].price * qty);

        printf("Added to cart!\n");
    }

    fprintf(fp, "Total Bill: Rs %d\n", total);
    fprintf(fp, "--------------------------\n");
    fclose(fp);

    printf("\nYour Total Bill: Rs %d\n", total);
    printf("Order Saved Successfully!\n");
}

// ==============================
// USER DASHBOARD
// ==============================
void dashboard(char username[]) {
    int choice;
    while (1) {
        printf("\n===== USER DASHBOARD =====\n");
        printf("1. Order Food\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                order_food(username);
                break;
            case 2:
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}

// ==============================
// MAIN
// ==============================
int main() {
    int choice;
    char loggedUser[50];

    load_users();

    while (1) {
        printf("\n===== FOOD ORDERING SYSTEM =====\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signup();
                break;
            case 2:
                if (login(loggedUser))
                    dashboard(loggedUser);
                break;
            case 3:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}