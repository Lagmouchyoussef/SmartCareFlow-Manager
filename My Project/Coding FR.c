#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// les structures de données pour le système de gestion des patients
typedef struct Patient {
    int id;
    char nom[50];
    int age;
    char antecedents[200];
    int niveauUrgence;
    struct Patient* suivant;
    struct Patient* precedent;
} Patient;

typedef struct Consultation {
    char date[20];
    char medecin[50];
    char typeSoin[50];
    struct Consultation* suivant;
} Consultation;

typedef struct Historique {
    Consultation* pile;
} Historique;

typedef struct FilePriorite {
    Patient* tete;
} FilePriorite;

// Fonction pour afficher un patient
void afficherPatient(Patient* patient) {
    if (patient) {
        printf("ID: %d, Nom: %s, Age: %d, Antecedents: %s, Niveau d urgence: %d\n",
               patient->id, patient->nom, patient->age, patient->antecedents, patient->niveauUrgence);
    } else {
        printf("Aucun patient a afficher.\n");
    }
}

// Fonction pour ajouter un patient
Patient* ajouter_patient() {
    Patient* nouveau = (Patient*)malloc(sizeof(Patient));
    if (!nouveau) {
        printf("Erreur d allocation memoire.\n");
        return NULL;
    }

    printf("Entrez l ID du patient : ");
    scanf("%d", &nouveau->id);
    getchar();

    printf("Entrez le nom du patient : ");
    fgets(nouveau->nom, sizeof(nouveau->nom), stdin);
    nouveau->nom[strcspn(nouveau->nom, "\n")] = '\0'; 

    printf("Entrez l age du patient : ");
    scanf("%d", &nouveau->age);
    getchar();

    printf("Entrez les antecedents medicaux du patient : ");
    fgets(nouveau->antecedents, sizeof(nouveau->antecedents), stdin);
    nouveau->antecedents[strcspn(nouveau->antecedents, "\n")] = '\0';

    printf("Entrez le niveau d urgence (1-5) : ");
    scanf("%d", &nouveau->niveauUrgence);
    getchar();

    nouveau->suivant = NULL;
    nouveau->precedent = NULL;

    return nouveau;
}

// Fonction pour rechercher un patient
Patient* rechercher_patient(FilePriorite* file, int id) {
    Patient* courant = file->tete;
    while (courant) {
        if (courant->id == id) {
            return courant;
        }
        courant = courant->suivant;
    }
    return NULL;
}

// Fonction pour modifier le dossier d'un patient
void modifier_dossier_patient(Patient* patient) {
    if (!patient) {
        printf("Patient introuvable.\n");
        return;
    }

    printf("Modifier le nom du patient : ");
    fgets(patient->nom, sizeof(patient->nom), stdin);
    patient->nom[strcspn(patient->nom, "\n")] = '\0';

    printf("Modifier l'age du patient : ");
    scanf("%d", &patient->age);
    getchar();

    printf("Modifier les antecedents medicaux : ");
    fgets(patient->antecedents, sizeof(patient->antecedents), stdin);
    patient->antecedents[strcspn(patient->antecedents, "\n")] = '\0';

    printf("Modifier le niveau d urgence (1-5) : ");
    scanf("%d", &patient->niveauUrgence);
    getchar();

    printf("Dossier mis à jour avec succes.\n");
}

// Fonction pour supprimer un patient
void supprimer_patient(FilePriorite* file, int id) {
    Patient* courant = file->tete;
    Patient* precedent = NULL;

    while (courant) {
        if (courant->id == id) {
            if (precedent) {
                precedent->suivant = courant->suivant;
            } else {
                file->tete = courant->suivant;
            }
            if (courant->suivant) {
                courant->suivant->precedent = precedent;
            }
            free(courant);
            printf("Patient supprime avec succes.\n");
            return;
        }
        precedent = courant;
        courant = courant->suivant;
    }
    printf("Patient introuvable.\n");
}

// Fonction pour ajouter un patient dans la file d'urgence
void ajouter_dans_file_urgence(FilePriorite* file, Patient* patient) {
    if (!file->tete || patient->niveauUrgence > file->tete->niveauUrgence) {
        patient->suivant = file->tete;
        if (file->tete) {
            file->tete->precedent = patient;
        }
        file->tete = patient;
    } else {
        Patient* courant = file->tete;
        while (courant->suivant && courant->suivant->niveauUrgence >= patient->niveauUrgence) {
            courant = courant->suivant;
        }
        patient->suivant = courant->suivant;
        if (courant->suivant) {
            courant->suivant->precedent = patient;
        }
        courant->suivant = patient;
        patient->precedent = courant;
    }
    printf("Patient ajoute dans la file d urgence.\n");
}

// Fonction pour traiter le patient prioritaire
Patient* traiter_patient_prioritaire(FilePriorite* file) {
    if (!file->tete) {
        printf("Aucun patient a traiter.\n");
        return NULL;
    }
    Patient* patient = file->tete;
    file->tete = file->tete->suivant;
    if (file->tete) {
        file->tete->precedent = NULL;
    }
    return patient;
}

// Fonction pour ajouter une consultation
void ajouter_consultation(Historique* historique) {
    Consultation* nouvelle = (Consultation*)malloc(sizeof(Consultation));
    if (!nouvelle) {
        printf("Erreur d allocation memoire.\n");
        return;
    }

    printf("Entrez la date de la consultation (jj/mm/aaaa) : ");
    fgets(nouvelle->date, sizeof(nouvelle->date), stdin);
    nouvelle->date[strcspn(nouvelle->date, "\n")] = '\0';

    printf("Entrez le nom du medecin : ");
    fgets(nouvelle->medecin, sizeof(nouvelle->medecin), stdin);
    nouvelle->medecin[strcspn(nouvelle->medecin, "\n")] = '\0';

    printf("Entrez le type de soin : ");
    fgets(nouvelle->typeSoin, sizeof(nouvelle->typeSoin), stdin);
    nouvelle->typeSoin[strcspn(nouvelle->typeSoin, "\n")] = '\0';

    nouvelle->suivant = historique->pile;
    historique->pile = nouvelle;

    printf("Consultation ajoutee avec succes.\n");
}

// Fonction pour consulter l'historique
void consulter_historique(Historique* historique) {
    Consultation* courant = historique->pile;
    while (courant) {
        printf("Date: %s, MMedecin: %s, Type de soin: %s\n", courant->date, courant->medecin, courant->typeSoin);
        courant = courant->suivant;
    }
}

// Fonction pour afficher les patients en attente
void afficher_patients_attente(FilePriorite* file) {
    Patient* courant = file->tete;
    while (courant) {
        printf("ID: %d, Nom: %s, Niveau d urgence: %d\n", courant->id, courant->nom, courant->niveauUrgence);
        courant = courant->suivant;
    }
}

// Fonction pour évaluer l'urgence
int evaluer_urgence() {
    int niveau;
    printf("Entrez le niveau d urgence (1-5) : ");
    scanf("%d", &niveau);
    getchar();
    return niveau;
}

// Exemple d'utilisation
int main() {
    FilePriorite file = {NULL};
    Historique historique = {NULL};

    int choix;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Ajouter un patient\n");
        printf("2. Rechercher un patient\n");
        printf("3. Modifier un dossier patient\n");
        printf("4. Supprimer un patient\n");
        printf("5. Ajouter un patient dans la file d urgence\n");
        printf("6. Traiter le patient prioritaire\n");
        printf("7. Ajouter une consultation\n");
        printf("8. Consulter l historique\n");
        printf("9. Afficher les patients en attente\n");
        printf("10. Quitter\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1: {
                Patient* nouveau = ajouter_patient();
                if (nouveau) {
                    ajouter_dans_file_urgence(&file, nouveau);
                }
                break;
            }
            case 2: {
                int id;
                printf("Entrez l ID du patient a rechercher : ");
                scanf("%d", &id);
                getchar();
                Patient* patient = rechercher_patient(&file, id);
                if (patient) {
                    afficherPatient(patient);
                } else {
                    printf("Patient introuvable.\n");
                }
                break;
            }
            case 3: {
                int id;
                printf("Entrez l ID du patient a modifier : ");
                scanf("%d", &id);
                getchar();
                Patient* patient = rechercher_patient(&file, id);
                modifier_dossier_patient(patient);
                break;
            }
            case 4: {
                int id;
                printf("Entrez l ID du patient a supprimer : ");
                scanf("%d", &id);
                getchar();
                supprimer_patient(&file, id);
                break;
            }
            case 5: {
                Patient* nouveau = ajouter_patient();
                if (nouveau) {
                    ajouter_dans_file_urgence(&file, nouveau);
                }
                break;
            }
            case 6: {
                Patient* patient = traiter_patient_prioritaire(&file);
                if (patient) {
                    printf("Patient traite :\n");
                    afficherPatient(patient);
                    free(patient);
                }
                break;
            }
            case 7:
                ajouter_consultation(&historique);
                break;
            case 8:
                consulter_historique(&historique);
                break;
            case 9:
                afficher_patients_attente(&file);
                break;
            case 10:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
        }
    } while (choix != 10);

    return 0;
}