#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_CART 200
#define MAX_HOTELS 5
#define MAX_ITEMS_PER_HOTEL 10

// ==============================
// STRUCTURES
// ==============================
struct User {
    char userName[50];
    int age;
    char email[50];
    char mobile[20];
    char password[30];
};

struct Item {
    int id;
    char name[60];
    int price;
};

struct Hotel {
    int id;
    char name[60];
    struct Item items[MAX_ITEMS_PER_HOTEL];
    int itemCount;
};

struct CartItem {
    int hotelIndex;
    int itemIndex;
    int qty;
};

// ==============================
// GLOBAL ARRAYS
// ==============================
struct User users[MAX_USERS];
struct Hotel hotels[MAX_HOTELS];
struct CartItem cart[MAX_CART];

int userCount = 0;
int hotelCount = 0;
int cartCount = 0;

// ==============================
// FILE NAMES
// ==============================
const char *USER_FILE = "FoodDeliverySystem.txt";
const char *ORDER_FILE = "orders.txt";

// ==============================
// LOWERCASE FUNCTION
// ==============================
void str_to_lower(const char *src, char *dst) {
    int i = 0;
    while (src[i]) {
        char c = src[i];
        if (c >= 'A' && c <= 'Z') c = c + 32;
        dst[i++] = c;
    }
    dst[i] = '\0';
}

// ==============================
// LOAD USERS
// ==============================
void load_users() {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return;

    while (fscanf(fp, "%49s %d %49s %19s %29s",
                  users[userCount].userName,
                  &users[userCount].age,
                  users[userCount].email,
                  users[userCount].mobile,
                  users[userCount].password) == 5)
    {
        userCount++;
    }

    fclose(fp);
}

// ==============================
// SAVE USER
// ==============================
void save_user(struct User u) {
    FILE *fp = fopen(USER_FILE, "a");
    if (!fp) return;

    fprintf(fp, "%s %d %s %s %s\n",
            u.userName, u.age, u.email, u.mobile, u.password);

    fclose(fp);
}

// ==============================
// ACCOUNT EXISTS
// ==============================
int account_exists(const char *email, const char *mobile) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].email, email) == 0)
            return 1;
        if (strcmp(users[i].mobile, mobile) == 0)
            return 2;
    }
    return 0;
}

// ==============================
// PRELOAD HOTELS & FOOD
// ==============================
void preload_hotels() {
    hotelCount = 3;

    hotels[0].id = 1;
    strcpy(hotels[0].name, "Domino's");
    hotels[0].itemCount = 4;
    hotels[0].items[0] = (struct Item){1, "Margherita Pizza", 200};
    hotels[0].items[1] = (struct Item){2, "Pepperoni Pizza", 250};
    hotels[0].items[2] = (struct Item){3, "Garlic Bread", 80};
    hotels[0].items[3] = (struct Item){4, "Cold Drink", 40};

    hotels[1].id = 2;
    strcpy(hotels[1].name, "KFC");
    hotels[1].itemCount = 4;
    hotels[1].items[0] = (struct Item){1, "Zinger Burger", 150};
    hotels[1].items[1] = (struct Item){2, "Chicken Bucket", 499};
    hotels[1].items[2] = (struct Item){3, "Fries", 60};
    hotels[1].items[3] = (struct Item){4, "Pepsi", 35};

    hotels[2].id = 3;
    strcpy(hotels[2].name, "FoodPoint");
    hotels[2].itemCount = 5;
    hotels[2].items[0] = (struct Item){1, "Veg Sandwich", 90};
    hotels[2].items[1] = (struct Item){2, "Pasta Alfredo", 160};
    hotels[2].items[2] = (struct Item){3, "Burger", 120};
    hotels[2].items[3] = (struct Item){4, "Ice Cream", 70};
    hotels[2].items[4] = (struct Item){5, "Cold Coffee", 80};
}

// ==============================
// SHOW HOTEL LIST
// ==============================
void show_all_hotels() {
    printf("\n======= HOTEL LIST =======\n");
    for (int i = 0; i < hotelCount; i++) {
        printf("%d. %s\n", i + 1, hotels[i].name);
    }
}

// ==============================
// SHOW FOOD LIST
// ==============================
void show_all_foods() {
    printf("\n======= FOOD LIST =======\n");
    for (int h = 0; h < hotelCount; h++) {
        for (int i = 0; i < hotels[h].itemCount; i++) {
            printf("[%s] %s - Rs %d\n",
                   hotels[h].name,
                   hotels[h].items[i].name,
                   hotels[h].items[i].price);
        }
    }
}

// ==============================
// SEARCH HOTEL
// ==============================
int search_hotels(const char *query, int out[]) {
    char qlow[100];
    str_to_lower(query, qlow);

    int count = 0;

    for (int i = 0; i < hotelCount; i++) {
        char nameLow[100];
        str_to_lower(hotels[i].name, nameLow);

        if (strstr(nameLow, qlow)) {
            out[count++] = i;
        }
    }
    return count;
}

// ==============================
// SEARCH FOOD
// ==============================
int search_food(const char *query, int hotelIndex[], int itemIndex[]) {
    char qlow[100];
    str_to_lower(query, qlow);

    int count = 0;

    for (int h = 0; h < hotelCount; h++) {
        for (int i = 0; i < hotels[h].itemCount; i++) {
            char foodLow[100];
            str_to_lower(hotels[h].items[i].name, foodLow);

            if (strstr(foodLow, qlow)) {
                hotelIndex[count] = h;
                itemIndex[count]  = i;
                count++;
            }
        }
    }

    return count;
}

// ==============================
// ADD TO CART
// ==============================
void add_to_cart(int h, int it, int qty) {
    for (int i = 0; i < cartCount; i++) {
        if (cart[i].hotelIndex == h && cart[i].itemIndex == it) {
            cart[i].qty += qty;
            printf("Quantity updated.\n");
            return;
        }
    }

    cart[cartCount].hotelIndex = h;
    cart[cartCount].itemIndex = it;
    cart[cartCount].qty = qty;
    cartCount++;

    printf("Added to cart.\n");
}

// ==============================
// VIEW CART
// ==============================
void view_cart() {
    if (cartCount == 0) {
        printf("Cart empty.\n");
        return;
    }

    printf("\n======= CART =======\n");
    int grand = 0;

    for (int i = 0; i < cartCount; i++) {
        int h = cart[i].hotelIndex;
        int it = cart[i].itemIndex;

        int price = hotels[h].items[it].price;
        int total = price * cart[i].qty;

        printf("%d) [%s] %s x %d = Rs %d\n",
               i + 1,
               hotels[h].name,
               hotels[h].items[it].name,
               cart[i].qty,
               total);

        grand += total;
    }

    printf("Grand Total : Rs %d\n", grand);
}

// ==============================
// REMOVE ITEM
// ==============================
void remove_item() {
    if (cartCount == 0) {
        printf("Cart empty.\n");
        return;
    }

    view_cart();

    int n;
    printf("Enter item number remove: ");
    scanf("%d", &n);

    if (n <= 0 || n > cartCount) return;

    for (int i = n - 1; i < cartCount - 1; i++)
        cart[i] = cart[i + 1];

    cartCount--;
    printf("Removed.\n");
}

// ==============================
// CHECKOUT
// ==============================
void checkout(const char *username) {
    if (cartCount == 0) {
        printf("Cart empty.\n");
        return;
    }

    FILE *fp = fopen(ORDER_FILE, "a");
    if (!fp) return;

    int grand = 0;

    fprintf(fp, "User : %s\n", username);

    for (int i = 0; i < cartCount; i++) {
        int h = cart[i].hotelIndex;
        int it = cart[i].itemIndex;
        int qty = cart[i].qty;

        int price = hotels[h].items[it].price;
        int total = qty * price;

        fprintf(fp, "[%s] %s x %d = %d\n",
                hotels[h].name,
                hotels[h].items[it].name,
                qty,
                total);

        grand += total;
    }

    fprintf(fp, "Total = %d\n----------------------\n", grand);
    fclose(fp);

    printf("Order placed.\n");

    cartCount = 0;
}

// ==============================
// SIGNUP
// ==============================
void signup() {
    struct User u;

    printf("\nEnter Username: ");
    scanf("%s", u.userName);

    printf("Enter Age: ");
    scanf("%d", &u.age);

    printf("\nEnter Email: ");
    scanf("%s", u.email);

    printf("\nEnter Mobile: ");
    scanf("%s", u.mobile);

    int chk = account_exists(u.email, u.mobile);

    if (chk == 1) {
        printf("Email already exists.\n");
        return;
    }
    if (chk == 2) {
        printf("Mobile already exists.\n");
        return;
    }

    printf("Enter Password: ");
    scanf("%s", u.password);

    users[userCount++] = u;
    save_user(u);

    printf("Signup Successful.\n");
}

// ==============================
// LOGIN
// ==============================
int login(char loggedUser[]) {
    char email[50], pass[50];

    printf("Enter Email: ");
    scanf("%s", email);

    printf("Enter Password: ");
    scanf("%s", pass);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(email, users[i].email) == 0 &&
            strcmp(pass, users[i].password) == 0)
        {
            strcpy(loggedUser, users[i].userName);
            printf("Login successful.\n");
            return 1;
        }
    }

    printf("Invalid credentials.\n");
    return 0;
}

// ==============================
// USER DASHBOARD
// ==============================
void user_dashboard(const char username[]) {
    while (1) {
        int ch;
        printf("\n===== USER DASHBOARD =====\n");
        printf("1. Show Hotels\n");
        printf("2. Show Foods\n");
        printf("3. Search Hotel\n");
        printf("4. Search Food\n");
        printf("5. View Cart\n");
        printf("6. Remove Item\n");
        printf("7. Checkout\n");
        printf("8. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        if (ch == 1) {
            show_all_hotels();

        } else if (ch == 2) {
            show_all_foods();

        } else if (ch == 3) {
            char q[50];
            printf("Enter Hotel Name: ");
            scanf(" %[^\n]", q);

            int out[10];
            int count = search_hotels(q, out);

            if (count == 0)
                printf("No hotels found.\n");
            else {
                for (int i = 0; i < count; i++)
                    printf("%d) %s\n", i + 1, hotels[out[i]].name);
            }

        } else if (ch == 4) {
            char q[50];
            printf("Enter Food Name: ");
            scanf(" %[^\n]", q);

            int h[20], it[20];
            int count = search_food(q, h, it);

            if (count == 0)
                printf("No food found.\n");
            else {
                for (int i = 0; i < count; i++)
                    printf("%d) [%s] %s - Rs %d\n",
                           i + 1,
                           hotels[h[i]].name,
                           hotels[h[i]].items[it[i]].name,
                           hotels[h[i]].items[it[i]].price);

                int s;
                printf("Select item: ");
                scanf("%d", &s);

                if (s > 0 && s <= count) {
                    int qty;
                    printf("Enter qty: ");
                    scanf("%d", &qty);
                    add_to_cart(h[s-1], it[s-1], qty);
                }
            }

        } else if (ch == 5) {
            view_cart();

        } else if (ch == 6) {
            remove_item();

        } else if (ch == 7) {
            checkout(username);

        } else if (ch == 8) {
            printf("Logged out.\n");
            return;

        } else {
            printf("Invalid.\n");
        }
    }
}

// ==============================
// MAIN
// ==============================
int main() {
    preload_hotels();
    load_users();

    char loggedUser[50];

    while (1) {
        int ch;
        printf("\n===== FOOD ORDERING SYSTEM =====\n");
        printf("1. Signup\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        if (ch == 1) {
            signup();
        }
        else if (ch == 2) {
            if (login(loggedUser)) {
                cartCount = 0;
                user_dashboard(loggedUser);
            }
        }
        else if (ch == 3) {
            printf("Exiting...\n");
            break;
        }
        else {
            printf("Invalid.\n");
        }
    }

    return 0;
}
