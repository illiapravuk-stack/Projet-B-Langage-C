#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    char Identifiant[10];
    char Nom[50];
    char Adresse[50];
    char Ville[50];
    char Etat[15];
    int PlacesDisponibles;
    int CapaciteMax;
    char Date[30];
    char AffichagePanneaux[10];
} ParkingLille;

ParkingLille parkingLilles[30];
int nbParkingLilles = 0;

typedef struct {
    char NumeroVeh[12];
    time_t HeureEntre;
    char Identifiant[10];

} Vehicule;

Vehicule vehicules[5];
int nbVehicules = 0;

void nettoyer(char *str) {
    str[strcspn(str, "\r\n")] = 0;
}

int lesparkings() {
    FILE *f = fopen("parking-metropole.csv", "r");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        return 0;
    }

    fscanf(f, "%*[^\n]\n"); 

    while (fscanf(f, "%9[^;];%49[^;];%49[^;];%49[^;];%14[^;];%d;%d;%29[^;];%9[^\n]\n",
                  parkingLilles[nbParkingLilles].Identifiant,
                  parkingLilles[nbParkingLilles].Nom,
                  parkingLilles[nbParkingLilles].Adresse,
                  parkingLilles[nbParkingLilles].Ville,
                  parkingLilles[nbParkingLilles].Etat,
                  &parkingLilles[nbParkingLilles].PlacesDisponibles,
                  &parkingLilles[nbParkingLilles].CapaciteMax,
                  parkingLilles[nbParkingLilles].Date,
                  parkingLilles[nbParkingLilles].AffichagePanneaux) == 9) {

        nbParkingLilles++;
    }

    fclose(f);

    FILE *g = fopen("Vehicule.csv", "r");
    if (g == NULL) {
        printf("Erreur ouverture fichier\n");
        return 0;
    }

    fscanf(g, "%*[^\n]\n");

    while (fscanf(g, "%11[^;];%ld;%9[^\n]\n",
                  vehicules[nbVehicules].NumeroVeh,
                  &vehicules[nbVehicules].HeureEntre,
                  vehicules[nbVehicules].Identifiant) == 3) {

        nettoyer(vehicules[nbVehicules].NumeroVeh);
        nettoyer(vehicules[nbVehicules].Identifiant);

        nbVehicules++;
    }

    fclose(g);

    return 1;
}

void afficheparking(char id[]) {
    for(int i = 0; i < nbParkingLilles; i++) {
        if(strcmp(parkingLilles[i].Identifiant, id) == 0) {
            printf("Identifiant: %s\n", parkingLilles[i].Identifiant);
            printf("Nom: %s\n", parkingLilles[i].Nom);
            printf("Adresse: %s\n", parkingLilles[i].Adresse);
            printf("Ville: %s\n", parkingLilles[i].Ville);
            printf("Etat: %s\n", parkingLilles[i].Etat);
            printf("Places disponibles: %d\n", parkingLilles[i].PlacesDisponibles);
            printf("Capacite max: %d\n", parkingLilles[i].CapaciteMax);
            printf("Date: %s\n", parkingLilles[i].Date);
            printf("Affichage: %s\n", parkingLilles[i].AffichagePanneaux);
            return;
        }
        
    }

}

void afficheparkings() {
    for(int i = 0; i < nbParkingLilles; i++) {
        printf("Parking %d \n", i+1);
        printf(" %s\n", parkingLilles[i].Identifiant);
        printf(" %s\n", parkingLilles[i].Nom);
        printf(" %s\n", parkingLilles[i].Adresse);
        printf(" %s\n", parkingLilles[i].Ville);
        printf(" %s\n", parkingLilles[i].Etat);
        printf(" %d\n", parkingLilles[i].PlacesDisponibles);
        printf(" %d\n", parkingLilles[i].CapaciteMax);
        printf(" %s\n", parkingLilles[i].Date);
        printf(" %s\n", parkingLilles[i].AffichagePanneaux);
    }
}

void suiviClient(char plaque[], double prix, time_t heureEntre, time_t heureSortie, char id[]){
    FILE *f = fopen("clients.csv", "a"); 
    if (f == NULL) {
        printf("Erreur\n" );
        return;
        }
        fprintf(f, "%s ; %f ; %ld ; %ld ; %s\n", plaque, prix, heureEntre, heureSortie, id );
fclose(f);
return;
}

void sortieParking(char plaque[]) {
    nettoyer(plaque);
    for(int i = 0; i < nbVehicules; i++) {
        nettoyer(vehicules[i].NumeroVeh);
        if(strcmp(vehicules[i].NumeroVeh, plaque) == 0) {
            time_t heureSortie = time(NULL);
            double duree = difftime(heureSortie, vehicules[i].HeureEntre);
            double heures = duree / 3600.0;
            printf("Temps de stationnement: %.2f heures\n", heures);
            int trouve = 0;
            for(int j = 0; j < nbParkingLilles; j++) {
                nettoyer(parkingLilles[j].Identifiant);
                if(strcmp(parkingLilles[j].Identifiant, vehicules[i].Identifiant) == 0) {
                    double prix = heures * 3; 
                    parkingLilles[j].PlacesDisponibles++;
                    printf("Parking: %s\n", parkingLilles[j].Nom);
                    printf("Montant a payer: %.2f euros\n", prix);
                    suiviClient(plaque, prix, vehicules[i].HeureEntre, heureSortie, vehicules[i].Identifiant);
                    trouve = 1;
                    break;
                }
            }
            if(!trouve) {
                printf("Parking non trouve !\n");
            }
            for(int k = i; k < nbVehicules - 1; k++) {
                vehicules[k] = vehicules[k + 1];
            }
            nbVehicules--;
                return;
        }
    }
}

void mettreAJourOccupation(char id[], int entree){    
    for(int i = 0; i < nbParkingLilles; i++) {
        if(strcmp(parkingLilles[i].Identifiant, id) == 0) {
            if(entree == 1){
                if (parkingLilles[i].PlacesDisponibles > 0){
                    parkingLilles[i].PlacesDisponibles -= 1;
                }
            }
            else if(entree == 0){
                if (parkingLilles[i].PlacesDisponibles < parkingLilles[i].CapaciteMax){
                    parkingLilles[i].PlacesDisponibles += 1;
                }
            }
            break; 
        }
    }
}
int modeAdministrateur(){
    int codesec = 7658;
    int codesais; 
    printf("Entrez le code administrateur : ");
    scanf("%d", &codesais);
    if (codesais == codesec) {
        return 1; 
    } else {
        return 0; 
    }
}

int verifierPlacesDisponibles(char id[]){
    for(int i = 0; i < nbParkingLilles; i++){   
        if(strcmp(parkingLilles[i].Identifiant, id) == 0){  
            if(parkingLilles[i].PlacesDisponibles > 0){  
               printf("%d\n", parkingLilles[i].PlacesDisponibles );
                return 1;
            }
            else{   
                return 0;
              }
        }
    } printf("Parking non trouver!\n");
    return 0;
}

int sauvegarderEtatParking(){
    FILE *f = fopen("parking-metropole.csv", "w");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        return 0;
    }
    fprintf(f,"Identifiant;Nom;Adresse;Ville;Etat;Placesdisponibles;Capacitemax;Datedemiseajour;Affichagepanneaux\n");
    for(int i=0; i < nbParkingLilles; i++) {    
        fprintf(f, "%s;%s;%s;%s;%s;%d;%d;%s;%s\n",
                  parkingLilles[i].Identifiant,
                  parkingLilles[i].Nom,
                  parkingLilles[i].Adresse,
                  parkingLilles[i].Ville,
                  parkingLilles[i].Etat,
                  parkingLilles[i].PlacesDisponibles,
                  parkingLilles[i].CapaciteMax,
                  parkingLilles[i].Date,
                  parkingLilles[i].AffichagePanneaux); 
        } 
    fclose(f);
    return 1;
}


int main() {
    printf("Debut programme\n");

    lesparkings();

    char id[10];
    char plaque[12];

    if (modeAdministrateur() != 1) {
        printf("Acces refuse\n");
        return 0;
    }

    printf("Acces autorise\n");

    int adminChoix;

    do {
        printf("\n===== MENU =====\n");
        printf("1. Afficher un parking\n");
        printf("2. Afficher tous les parkings\n");
        printf("3. Sortie vehicule\n");
        printf("4. Verifier places disponibles\n");
        printf("5. Tester mise a jour occupation\n");
        printf("6. Tester suivi client\n");
        printf("0. Quitter\n");
        printf("Votre choix: ");
        scanf("%d", &adminChoix);

        switch(adminChoix) {

            case 1:
                printf("Entrer identifiant: ");
                scanf("%s", id);
                afficheparking(id);
                break;

            case 2:
                afficheparkings();
                break;

            case 3:
                printf("Plaque: ");
                scanf("%s", plaque);
                sortieParking(plaque);
                sauvegarderEtatParking();
                break;

            case 4:
                printf("ID parking: ");
                scanf("%s", id);
                if (verifierPlacesDisponibles(id) == 1) {
                    printf("Places disponibles\n");
                } else {
                    printf("Parking plein\n");
                }
                break;

            case 5: {
                int entree;
                printf("ID parking: ");
                scanf("%s", id);
                printf("1 = entree, 0 = sortie: ");
                scanf("%d", &entree);
                mettreAJourOccupation(id, entree);
                printf("Mise a jour effectuee\n");
                break;
            }

            case 6: {
                double prix;
                time_t h1 = time(NULL);
                time_t h2 = h1 + 3600;

                printf("Plaque: ");
                scanf("%s", plaque);
                printf("ID parking: ");
                scanf("%s", id);
                printf("Prix: ");
                scanf("%lf", &prix);

                suiviClient(plaque, prix, h1, h2, id);
                printf("Client enregistre\n");
                break;
            }

            case 0:
                printf("Au revoir\n");
                break;

            default:
                printf("Choix invalide\n");
        }

    } while(adminChoix != 0);

    return 0;
}

