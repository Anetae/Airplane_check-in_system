#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_DATE_LENGTH 20
#define MAX_PASSENGERS 100

// Struct that represent a Passenger
struct Passenger {
    char name[MAX_NAME_LENGTH];
    int seatNumber;
    char date[MAX_DATE_LENGTH];
    int canceledFlight;
};

// Global array to store passenger information
struct Passenger* passengers[MAX_PASSENGERS];
int totalPassengers = 0;

// Function to check seat availability
int isSeatAvailable(int seatNumber) {
    for (int i = 0; i < totalPassengers; i++) {
        if (passengers[i]->seatNumber == seatNumber) {
            return 0; //  not available
        }
    }
    return 1; // available
}

//  check in a passenger
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
        } while (chosenSeat < 1,  chosenSeat > 50 , !isSeatAvailable(chosenSeat));

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

// free allocated memory
void cleanup() {
    for (int i = 0; i < totalPassengers; i++) {
        free(passengers[i]);
    }
}

// Main function
int main() {
    int choice;

    do {
        printf("\nAirplane Check-in System\n");
        printf("1. Check-in\n");
        printf("2. Display Passenger List\n");
        printf("3. Search Passenger by Name\n");
        printf("4. Cancel Flight\n");
        printf("5. Display Seat Occupancy\n");
        printf("6. Exit\n");
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
            printf("Exiting the system. Have a great flight!\n");
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 6);

 
    cleanup();

    return 0;
}
