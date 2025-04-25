#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data structures for patient management system
typedef struct Patient {
    int id;
    char name[50];
    int age;
    char medicalHistory[200];
    int urgencyLevel;
    struct Patient* next;
    struct Patient* prev;
} Patient;

typedef struct Consultation {
    char date[20];
    char doctor[50];
    char careType[50];
    struct Consultation* next;
} Consultation;

typedef struct History {
    Consultation* stack;
} History;

typedef struct PriorityQueue {
    Patient* head;
} PriorityQueue;

// Function to display patient
void printPatient(Patient* patient) {
    if (patient) {
        printf("ID: %d, Name: %s, Age: %d, Medical History: %s, Urgency Level: %d\n",
               patient->id, patient->name, patient->age, patient->medicalHistory, patient->urgencyLevel);
    } else {
        printf("No patient to display.\n");
    }
}

// Function to create patient
Patient* createPatient() {
    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    if (!newPatient) {
        printf("Memory allocation error.\n");
        return NULL;
    }

    printf("Enter patient ID: ");
    scanf("%d", &newPatient->id);
    getchar();

    printf("Enter patient name: ");
    fgets(newPatient->name, sizeof(newPatient->name), stdin);
    newPatient->name[strcspn(newPatient->name, "\n")] = '\0';

    printf("Enter patient age: ");
    scanf("%d", &newPatient->age);
    getchar();

    printf("Enter medical history: ");
    fgets(newPatient->medicalHistory, sizeof(newPatient->medicalHistory), stdin);
    newPatient->medicalHistory[strcspn(newPatient->medicalHistory, "\n")] = '\0';

    printf("Enter urgency level (1-5): ");
    scanf("%d", &newPatient->urgencyLevel);
    getchar();

    newPatient->next = NULL;
    newPatient->prev = NULL;

    return newPatient;
}

// Function to find patient
Patient* findPatient(PriorityQueue* queue, int id) {
    Patient* current = queue->head;
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to update patient record
void updatePatient(Patient* patient) {
    if (!patient) {
        printf("Patient not found.\n");
        return;
    }

    printf("Update patient name: ");
    fgets(patient->name, sizeof(patient->name), stdin);
    patient->name[strcspn(patient->name, "\n")] = '\0';

    printf("Update patient age: ");
    scanf("%d", &patient->age);
    getchar();

    printf("Update medical history: ");
    fgets(patient->medicalHistory, sizeof(patient->medicalHistory), stdin);
    patient->medicalHistory[strcspn(patient->medicalHistory, "\n")] = '\0';

    printf("Update urgency level (1-5): ");
    scanf("%d", &patient->urgencyLevel);
    getchar();

    printf("Record updated successfully.\n");
}

// Function to delete patient
void deletePatient(PriorityQueue* queue, int id) {
    Patient* current = queue->head;
    Patient* prev = NULL;

    while (current) {
        if (current->id == id) {
            if (prev) {
                prev->next = current->next;
            } else {
                queue->head = current->next;
            }
            if (current->next) {
                current->next->prev = prev;
            }
            free(current);
            printf("Patient deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Patient not found.\n");
}

// Function to enqueue patient
void enqueuePatient(PriorityQueue* queue, Patient* patient) {
    if (!queue->head || patient->urgencyLevel > queue->head->urgencyLevel) {
        patient->next = queue->head;
        if (queue->head) {
            queue->head->prev = patient;
        }
        queue->head = patient;
    } else {
        Patient* current = queue->head;
        while (current->next && current->next->urgencyLevel >= patient->urgencyLevel) {
            current = current->next;
        }
        patient->next = current->next;
        if (current->next) {
            current->next->prev = patient;
        }
        current->next = patient;
        patient->prev = current;
    }
    printf("Patient added to priority queue.\n");
}

// Function to process priority patient
Patient* dequeuePatient(PriorityQueue* queue) {
    if (!queue->head) {
        printf("No patients to process.\n");
        return NULL;
    }
    Patient* patient = queue->head;
    queue->head = queue->head->next;
    if (queue->head) {
        queue->head->prev = NULL;
    }
    return patient;
}

// Function to add consultation
void addConsultation(History* history) {
    Consultation* newConsult = (Consultation*)malloc(sizeof(Consultation));
    if (!newConsult) {
        printf("Memory allocation error.\n");
        return;
    }

    printf("Enter consultation date (dd/mm/yyyy): ");
    fgets(newConsult->date, sizeof(newConsult->date), stdin);
    newConsult->date[strcspn(newConsult->date, "\n")] = '\0';

    printf("Enter doctor's name: ");
    fgets(newConsult->doctor, sizeof(newConsult->doctor), stdin);
    newConsult->doctor[strcspn(newConsult->doctor, "\n")] = '\0';

    printf("Enter care type: ");
    fgets(newConsult->careType, sizeof(newConsult->careType), stdin);
    newConsult->careType[strcspn(newConsult->careType, "\n")] = '\0';

    newConsult->next = history->stack;
    history->stack = newConsult;

    printf("Consultation added successfully.\n");
}

// Function to view history
void viewHistory(History* history) {
    Consultation* current = history->stack;
    while (current) {
        printf("Date: %s, Doctor: %s, Care Type: %s\n", current->date, current->doctor, current->careType);
        current = current->next;
    }
}

// Function to display waiting patients
void displayQueue(PriorityQueue* queue) {
    Patient* current = queue->head;
    while (current) {
        printf("ID: %d, Name: %s, Urgency Level: %d\n", current->id, current->name, current->urgencyLevel);
        current = current->next;
    }
}

// Function to assess urgency level
int getUrgencyLevel() {
    int level;
    printf("Enter urgency level (1-5): ");
    scanf("%d", &level);
    getchar();
    return level;
}

// Main function
int main() {
    PriorityQueue queue = {NULL};
    History history = {NULL};

    int choice;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Add patient\n");
        printf("2. Find patient\n");
        printf("3. Update patient record\n");
        printf("4. Delete patient\n");
        printf("5. Enqueue patient\n");
        printf("6. Process priority patient\n");
        printf("7. Add consultation\n");
        printf("8. View history\n");
        printf("9. Display waiting patients\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                Patient* newPatient = createPatient();
                if (newPatient) {
                    enqueuePatient(&queue, newPatient);
                }
                break;
            }
            case 2: {
                int id;
                printf("Enter patient ID to search: ");
                scanf("%d", &id);
                getchar();
                Patient* patient = findPatient(&queue, id);
                if (patient) {
                    printPatient(patient);
                } else {
                    printf("Patient not found.\n");
                }
                break;
            }
            case 3: {
                int id;
                printf("Enter patient ID to update: ");
                scanf("%d", &id);
                getchar();
                Patient* patient = findPatient(&queue, id);
                updatePatient(patient);
                break;
            }
            case 4: {
                int id;
                printf("Enter patient ID to delete: ");
                scanf("%d", &id);
                getchar();
                deletePatient(&queue, id);
                break;
            }
            case 5: {
                Patient* newPatient = createPatient();
                if (newPatient) {
                    enqueuePatient(&queue, newPatient);
                }
                break;
            }
            case 6: {
                Patient* patient = dequeuePatient(&queue);
                if (patient) {
                    printf("Processed patient:\n");
                    printPatient(patient);
                    free(patient);
                }
                break;
            }
            case 7:
                addConsultation(&history);
                break;
            case 8:
                viewHistory(&history);
                break;
            case 9:
                displayQueue(&queue);
                break;
            case 10:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 10);

    return 0;
}