#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C:/Program Files/MySQL/MySQL Server 8.0/include/mysql.h"
#define MAX_CAPACITY 10

struct InventoryItem {
    int id;
    char name[50];
    float price;
};

struct QueueNode {
    struct InventoryItem data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

void initializeQueue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

int isQueueEmpty(struct Queue* queue) {
    return queue->front == NULL;
}

void enqueue(struct Queue* queue, struct InventoryItem item) {
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    newNode->data = item;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

struct InventoryItem dequeue(struct Queue* queue) {
    if (isQueueEmpty(queue)) {
        struct InventoryItem emptyItem = {0, "", 0.0};
        return emptyItem;
    }

    struct QueueNode* temp = queue->front;
    struct InventoryItem item = temp->data;

    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);

    return item;
}

void displayInventory(struct Queue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Inventory is empty.\n");
        return;
    }

    printf("Inventory:\n");
    struct QueueNode* current = queue->front;
    while (current != NULL) {
        printf("ID: %d, Name: %s, Price: %.2f\n", current->data.id, current->data.name, current->data.price);
        current = current->next;
    }
}

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // change this parameters
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "enter your SQL database password";
    const char *database = "enter the database name ";

    conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return 1;
    }

    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return 1;
    }

    struct Queue inventoryQueue;
    initializeQueue(&inventoryQueue);

    int option;
    do {
        printf("\nMenu:\n");
        printf("1. Add item to inventory\n");
        printf("2. Display inventory\n");
        printf("3. Delete item from inventory\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1: {
                struct InventoryItem newItem;
                printf("Enter ID: ");
                scanf("%d", &newItem.id);
                printf("Enter Name: ");
                scanf("%s", newItem.name);
                printf("Enter Price: ");
                scanf("%f", &newItem.price);

                enqueue(&inventoryQueue, newItem);

                const char *insert_query = "INSERT INTO items VALUES (%d, '%s', %.2f)";
                char query[150];
                sprintf(query, insert_query, newItem.id, newItem.name, newItem.price);

                if (mysql_query(conn, query)) {
                    fprintf(stderr, "INSERT query failed: %s\n", mysql_error(conn));
                    return 1;
                }
                printf("Data inserted successfully!\n");

                break;
            }
            case 2:
                if (mysql_query(conn, "SELECT * FROM items")) {
                    fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
                    return 1;
                }

                res = mysql_store_result(conn);

                if (res == NULL) {
                    fprintf(stderr, "mysql_store_result() failed\n");
                    return 1;
                }

                while ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < mysql_num_fields(res); i++) {
                        printf("%s ", row[i] ? row[i] : "NULL");
                    }
                    printf("\n");
                }

                mysql_free_result(res);

                break;
            case 3: {
                int deleteId;
                printf("Enter ID of the item to delete: ");
                scanf("%d", &deleteId);

                const char *delete_query = "DELETE FROM items WHERE id = %d";
                char query[150];
                sprintf(query, delete_query, deleteId);

                if (mysql_query(conn, query)) {
                    fprintf(stderr, "DELETE query failed: %s\n", mysql_error(conn));
                    return 1;
                }
                printf("Item with ID %d deleted successfully!\n", deleteId);

                break;
            }
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (option != 4);

    mysql_close(conn);

    // Free memory for the remaining nodes in the queue
    while (!isQueueEmpty(&inventoryQueue)) {
        dequeue(&inventoryQueue);
    }

    return 0;
}
