#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 20
#define MAX_PASSENGERS 100
#define FILENAME "passenger_data.txt"

// represents a Passenger
struct Passenger {
    char name[MAX_NAME_LENGTH];
    int seatNumber;
    char date[MAX_DATE_LENGTH];
    int canceledFlight;
};

// array to store passenger information
struct Passenger* passengers[MAX_PASSENGERS];
int totalPassengers = 0;

// check seat availability
int isSeatAvailable(int seatNumber) {
    for (int i = 0; i < totalPassengers; i++) {
        if (passengers[i]->seatNumber == seatNumber) {
            return 0; 
        }
    }
    return 1; 
}

// check in a passenger
void checkIn() {
    if (totalPassengers < MAX_PASSENGERS) {
        struct Passenger* newPassenger = (struct Passenger*)malloc(sizeof(struct Passenger));

        if (newPassenger == NULL) {
            printf("Error: Memory allocation failed.\n");
            return;
        }

        printf("Enter passenger name: ");
        scanf("%s", newPassenger->name);

        int chosenSeat;
        do {
            printf("Enter seat number (1-50): ");
            scanf("%d", &chosenSeat);

            if (chosenSeat < 1 || chosenSeat > 50) {
                printf("Invalid seat number. Please choose between 1 and 50.\n");
            }
            else if (!isSeatAvailable(chosenSeat)) {
                printf("Seat %d is already taken. Please choose another seat.\n", chosenSeat);
            }
            else {
                newPassenger->seatNumber = chosenSeat;
            }
        } while (chosenSeat < 1 || chosenSeat > 50 || !isSeatAvailable(chosenSeat));

        printf("Enter date of flight: ");
        scanf("%s", newPassenger->date);

        printf("Has the flight been canceled? (1 for Yes, 0 for No): ");
        scanf("%d", &newPassenger->canceledFlight);

        passengers[totalPassengers] = newPassenger;
        printf("Check-in successful for %s. Seat number: %d\n", newPassenger->name, newPassenger->seatNumber);

        totalPassengers++;
    }
    else {
        printf("Sorry, the airplane is full. Cannot check in more passengers.\n");
    }
}

// display the list of passengers
void displayPassengerList() {
    printf("\nPassenger List:\n");
    for (int i = 0; i < totalPassengers; i++) {
        printf("%s - Seat: %d - Date: %s - %s\n",
            passengers[i]->name, passengers[i]->seatNumber, passengers[i]->date,
            passengers[i]->canceledFlight ? "Flight Canceled" : "Flight Confirmed");
    }
    printf("\n");
}

// search for a passenger by name
void searchPassengerByName() {
    char searchName[MAX_NAME_LENGTH];
    printf("Enter the name to search for: ");
    scanf("%s", searchName);

    for (int i = 0; i < totalPassengers; i++) {
        if (strcmp(passengers[i]->name, searchName) == 0) {
            printf("Passenger found:\n");
            printf("Name: %s\nSeat: %d\nDate: %s\n%s\n",
                passengers[i]->name, passengers[i]->seatNumber,
                passengers[i]->date, passengers[i]->canceledFlight ? "Flight Canceled" : "Flight Confirmed");
            return;
        }
    }

    printf("Passenger not found with the name: %s\n", searchName);
}

// cancel a flight for a passenger
void cancelFlight() {
    char searchName[MAX_NAME_LENGTH];
    printf("Enter the name of the passenger to cancel the flight: ");
    scanf("%s", searchName);

    for (int i = 0; i < totalPassengers; i++) {
        if (strcmp(passengers[i]->name, searchName) == 0) {
            passengers[i]->canceledFlight = 1;
            printf("Flight canceled for %s.\n", searchName);
            return;
        }
    }

    printf("Passenger not found with the name: %s\n", searchName);
}

// display seat occupancy
void displaySeatOccupancy() {
    int seatOccupancy[50] = { 0 };

    for (int i = 0; i < totalPassengers; i++) {
        seatOccupancy[passengers[i]->seatNumber - 1] = 1;
    }

    printf("\nSeat Occupancy:\n");
    for (int i = 0; i < 50; i++) {
        printf("Seat %d: %s\n", i + 1, seatOccupancy[i] ? "Occupied" : "Available");
    }
    printf("\n");
}

// save data to a file
void saveDataToFile() {
    FILE* file = fopen(FILENAME, "w");
    if (file != NULL) {
        fprintf(file, "%d\n", totalPassengers);

        for (int i = 0; i < totalPassengers; i++) {
            fprintf(file, "%s %d %s %d\n",
                passengers[i]->name, passengers[i]->seatNumber,
                passengers[i]->date, passengers[i]->canceledFlight);
        }

        fclose(file);
        printf("Data saved to file.\n");
    }
    else {
        printf("Error opening file for writing.\n");
    }
}

// load data from a file
void loadDataFromFile() {
    FILE* file = fopen(FILENAME, "r");
    if (file != NULL) {
        fscanf(file, "%d", &totalPassengers);

        for (int i = 0; i < totalPassengers; i++) {
            struct Passenger* loadedPassenger = (struct Passenger*)malloc(sizeof(struct Passenger));
            if (loadedPassenger != NULL) {
                fscanf(file, "%s %d %s %d",
                    loadedPassenger->name, &loadedPassenger->seatNumber,
                    loadedPassenger->date, &loadedPassenger->canceledFlight);

                passengers[i] = loadedPassenger;
            }
            else {
                printf("Error: Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
        }

        fclose(file);
        printf("Data loaded from file.\n");
    }
    else {
        printf("No previous data found.\n");
    }
}

// delete a passenger based on seat number
void deletePassenger() {
    int seatToDelete;
    printf("\nEnter seat number to delete: ");
    scanf("%d", &seatToDelete);

    int found = 0;
    for (int i = 0; i < totalPassengers; i++) {
        if (passengers[i]->seatNumber == seatToDelete) {
            free(passengers[i]);

            for (int j = i; j < totalPassengers - 1; j++) {
                passengers[j] = passengers[j + 1];
            }
            totalPassengers--;

            printf("Passenger with seat number %d deleted.\n", seatToDelete);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Passenger with seat number %d not found.\n", seatToDelete);
    }
}



// free allocated memory
void cleanup() {
    for (int i = 0; i < totalPassengers; i++) {
        free(passengers[i]);
    }
}

// Main function
int main() {
    loadDataFromFile();

    int choice;

    do {
        printf("\nAirplane Check-in System\n");
        printf("1. Check-in\n");
        printf("2. Display Passenger List\n");
        printf("3. Search Passenger by Name\n");
        printf("4. Cancel Flight\n");
        printf("5. Display Seat Occupancy\n");
        printf("6. Save Data to File\n");
        printf("7. Delete Passenger\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            checkIn();
            break;
        case 2:
            displayPassengerList();
            break;
        case 3:
            searchPassengerByName();
            break;
        case 4:
            cancelFlight();
            break;
        case 5:
            displaySeatOccupancy();
            break;
        case 6:
            saveDataToFile();
            break;
        case 7:
            deletePassenger();
            break;
        case 8:
            printf("Exiting the system. Have a great flight!\n");
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 8);

    cleanup();

    return 0;
}
