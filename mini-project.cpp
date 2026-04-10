#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NAME_LEN 50

// Priority levels
#define EMERGENCY   1
#define URGENT      2
#define NORMAL      3

typedef struct {
    int    id;
    char   name[NAME_LEN];
    int    age;
    int    priority;   // 1=Emergency, 2=Urgent, 3=Normal
    char   issue[100];
} Patient;

// Queue using array + size counter
Patient queue[MAX_PATIENTS];
int queue_size = 0;
int next_id = 1;

// ─── Helpers ────────────────────────────────────────────────────────────────

const char* priority_label(int p) {
    if (p == EMERGENCY) return "EMERGENCY";
    if (p == URGENT)    return "URGENT";
    return "NORMAL";
}

// Sort queue: lower priority number = higher urgency (comes first)
// Stable insertion-sort so equal-priority patients keep arrival order
void sort_queue() {
    for (int i = 1; i < queue_size; i++) {
        Patient key = queue[i];
        int j = i - 1;
        while (j >= 0 && queue[j].priority > key.priority) {
            queue[j + 1] = queue[j];
            j--;
        }
        queue[j + 1] = key;
    }
}

// ─── Core Operations ─────────────────────────────────────────────────────────

void add_patient() {
    if (queue_size >= MAX_PATIENTS) {
        printf("\n[ERROR] Queue is full.\n");
        return;
    }

    Patient p;
    p.id = next_id++;

    printf("\n--- Register Patient ---\n");
    printf("Name  : "); scanf(" %[^\n]", p.name);
    printf("Age   : "); scanf("%d", &p.age);
    printf("Issue : "); scanf(" %[^\n]", p.issue);

    printf("Priority (1=Emergency / 2=Urgent / 3=Normal): ");
    scanf("%d", &p.priority);
    if (p.priority < 1 || p.priority > 3) {
        printf("[WARNING] Invalid priority. Set to NORMAL.\n");
        p.priority = NORMAL;
    }

    queue[queue_size++] = p;
    sort_queue();

    printf("\n[OK] Patient #%d (%s) added as %s.\n",
           p.id, p.name, priority_label(p.priority));
}

void serve_next() {
    if (queue_size == 0) {
        printf("\n[INFO] No patients in queue.\n");
        return;
    }

    Patient p = queue[0];

    // Shift everyone forward
    for (int i = 0; i < queue_size - 1; i++)
        queue[i] = queue[i + 1];
    queue_size--;

    printf("\n--- Serving Next Patient ---\n");
    printf("ID       : %d\n",   p.id);
    printf("Name     : %s\n",   p.name);
    printf("Age      : %d\n",   p.age);
    printf("Issue    : %s\n",   p.issue);
    printf("Priority : %s\n",   priority_label(p.priority));
    printf("Remaining in queue: %d\n", queue_size);
}

void view_queue() {
    if (queue_size == 0) {
        printf("\n[INFO] Queue is empty.\n");
        return;
    }

    printf("\n%-5s %-20s %-5s %-12s %s\n",
           "Pos", "Name", "Age", "Priority", "Issue");
    printf("%-5s %-20s %-5s %-12s %s\n",
           "---", "----", "---", "--------", "-----");

    for (int i = 0; i < queue_size; i++) {
        printf("%-5d %-20s %-5d %-12s %s\n",
               i + 1,
               queue[i].name,
               queue[i].age,
               priority_label(queue[i].priority),
               queue[i].issue);
    }
    printf("\nTotal patients: %d\n", queue_size);
}

void search_patient() {
    char name[NAME_LEN];
    printf("\nEnter name to search: ");
    scanf(" %[^\n]", name);

    int found = 0;
    for (int i = 0; i < queue_size; i++) {
        if (strstr(queue[i].name, name) != NULL) {
            printf("\nPosition : %d\n",  i + 1);
            printf("ID       : %d\n",    queue[i].id);
            printf("Name     : %s\n",    queue[i].name);
            printf("Age      : %d\n",    queue[i].age);
            printf("Issue    : %s\n",    queue[i].issue);
            printf("Priority : %s\n",    priority_label(queue[i].priority));
            found = 1;
        }
    }
    if (!found)
        printf("[INFO] No patient found with name containing '%s'.\n", name);
}

void remove_patient() {
    int id;
    printf("\nEnter Patient ID to remove: ");
    scanf("%d", &id);

    for (int i = 0; i < queue_size; i++) {
        if (queue[i].id == id) {
            printf("[OK] Removed patient #%d (%s).\n", queue[i].id, queue[i].name);
            for (int j = i; j < queue_size - 1; j++)
                queue[j] = queue[j + 1];
            queue_size--;
            return;
        }
    }
    printf("[INFO] No patient with ID %d found.\n", id);
}

void queue_stats() {
    int e = 0, u = 0, n = 0;
    for (int i = 0; i < queue_size; i++) {
        if (queue[i].priority == EMERGENCY) e++;
        else if (queue[i].priority == URGENT) u++;
        else n++;
    }
    printf("\n--- Queue Stats ---\n");
    printf("Emergency : %d\n", e);
    printf("Urgent    : %d\n", u);
    printf("Normal    : %d\n", n);
    printf("Total     : %d\n", queue_size);
}

// ─── Menu ────────────────────────────────────────────────────────────────────

void print_menu() {
    printf("\n========= Hospital Queue System =========\n");
    printf(" 1. Add Patient\n");
    printf(" 2. Serve Next Patient\n");
    printf(" 3. View Queue\n");
    printf(" 4. Search Patient\n");
    printf(" 5. Remove Patient\n");
    printf(" 6. Queue Stats\n");
    printf(" 0. Exit\n");
    printf("=========================================\n");
    printf("Choice: ");
}

int main() {
    int choice;

    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_patient();    break;
            case 2: serve_next();     break;
            case 3: view_queue();     break;
            case 4: search_patient(); break;
            case 5: remove_patient(); break;
            case 6: queue_stats();    break;
            case 0: printf("\nExiting. Goodbye.\n"); break;
            default: printf("\n[ERROR] Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}