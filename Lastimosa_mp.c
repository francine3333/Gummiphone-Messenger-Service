/*********************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying the 
concepts learned. I have constructed the functions and their respective algorithms and corresponding code by 
myself. The program was run, tested, and debugged by my own efforts. I further certify that I have not copied in 
part or whole or otherwise plagiarized the work of other students and/or persons.
 <Rischa Francine C. Lastimosa>, DLSU ID122 <12208175>
*********************************************************************************************/

#include <stdio.h> // provides input/output functions for reading from and writing to files or the console.
#include <stdlib.h> 
#include <string.h> // provides functions for string manipulation, such as copying (strcpy). 
#include <windows.h> // this is used to clear the console screen. Like system("cls");
#include <time.h> //  this is used for the randomization of security questions that will be given to each user

typedef struct Admin{
    char password[20]; 
    int user_count; // number of users na existing sa system
    long long int message_count; // long long int to store many messages
    int question_count; // number of recovery questions na currently nasa system
    char recovery_questions[20][100]; // dito nakastore ung recovery questions. 20 na recovery questions and 100 para sa number of char for each questions
    struct Message *message_list[250]; // gumagawa tayo ng array ng message
    struct User *user_list[30]; 
} Admin;

typedef struct Message{
    int id_num; // This line declares an integer variable named id_num to hold the message ID number.
    char subject[100]; // This line declares a character array named subject to hold the message subject. 
    char content[1000]; // to hold the message content. 
    int message_type; // to hold the type of the message
    int recipient_count; // to hold the number of recipients
    struct User *message_owner; // Pointer variable to store the memory address of a struct user and allows us to dynamically allocate memory for the struct user and modify its contents as needed
    struct User *message_sender; // sender of the message. Same reason as above why it is a pointer variable ^^
    struct User *message_recipients[29]; // declares an array named message_recipients of size 29 to hold pointers to User structs representing the recipients of the message.
}Message;

typedef struct User {
    char full_name[50]; // stores the user's full name
    char user_name[50];  // stores the user's full name
    char user_password[20]; // stores the user's password
    char sec_password[20]; // stores the user's secondary password
    char bio_description[100]; // stores the user's bio description
    int inbox_count;  // stores the number of messages in the user's inbox
    int sent_count;  // stores the number of messages the user has sent
    int announcement_count; // stores the number of announcements the user has received
    int connection_count; // to hold the number of connections the user has.
    int is_locked;  // to hold a boolean value indicating whether the user's account is locked or not.
    int selected_q_num; //  to hold the number of the selected security question for the user.
    struct Message *inbox_list[33];  // to hold pointers to Message structs representing the messages in the user's inbox.
    struct Message *sent_list[33]; // to hold pointers to Message structs representing the messages the user has sent.
    struct Message *announcements_list[33]; // to hold pointers to Message structs representing the announcements the user has made.
    struct User *connection_list[29]; // to hold pointers to User structs representing the user's connections.
} User;


/* composeMessage handles the messaging portion of the program
   This function allows the current user to compose and send a message to one or multiple recipients.

@param currentUser - User that is currently logged in
@param admin - Program admin
Pre-condition: currentUser can't be null
*/
void composeMessage(User *currentUser, Admin *admin) 
{
    int input_number; 
    Message *new_message = (Message *) malloc(sizeof(Message));
    new_message->message_owner = currentUser; new_message->message_sender = currentUser; new_message->id_num = admin->message_count; new_message->recipient_count = 0;
    char user_name[50];
    User *recepient;
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    printf(" GUMMIPHONE (COMPOSE MESSAGE)\n\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    printf("Message subject: "); fgets(new_message->subject, 100, stdin); strtok(new_message->subject, "\n"); // I used strtok to remove the newline char
    printf("Message content: "); fgets(new_message->content, 250, stdin); strtok(new_message->content, "\n"); // I used strtok to remove the newline char
    printf("\nSend your message: \n|1| - Personal Message\n|2| - Group Message\n|3| - Announcement\n|0| - Back\n");
    
    printf("\nChoice: "); scanf("%d", &input_number); getchar();
    switch(input_number) {
        case 1:
            new_message->message_type = 1;
            printf("Username: "); fgets(user_name, 50, stdin); strtok(user_name, "\n");
            int is_found = 0, found_index = 0;
            for (int i = 0; i < admin->user_count; i++)
                if(strcmp(admin->user_list[i]->user_name, user_name) == 0){
                    is_found = 1; found_index = i;
                }
            if(is_found == 1){
                new_message->message_recipients[0] = admin->user_list[found_index];
                new_message->recipient_count = 1;
                currentUser->sent_list[currentUser->sent_count] = new_message;
                currentUser->sent_count += 1;
                recepient = admin->user_list[found_index];
                recepient->inbox_list[recepient->inbox_count] = new_message;
                recepient->inbox_count += 1;
                admin->message_list[admin->message_count] = new_message;
                admin->message_count += 1;
                printf("Message sent! Going Back...\n"); 
            }
            else{
                printf("Account not found! Going Back...\n"); 
            }
            break;
        case 2:
            new_message->message_type = 2;
            for (int i = 0; i < currentUser->connection_count; i++) {
                new_message->message_recipients[i] = currentUser->connection_list[i];
                new_message->recipient_count += 1;
            }
            for (int i = 0; i < currentUser->connection_count; i++) {
                recepient = currentUser->connection_list[i];
                recepient->inbox_list[recepient->inbox_count] = new_message;
                recepient->inbox_count += 1;
            }
            currentUser->sent_list[currentUser->sent_count] = new_message;
            currentUser->sent_count += 1;
            admin->message_list[admin->message_count] = new_message;
            admin->message_count += 1;
            printf("Message sent! Going Back...\n"); 
            break;
        case 3:
            new_message->message_type = 3;
            for (int i = 0; i < admin->user_count; i++) {
                new_message->message_recipients[i] = admin->user_list[i];
                new_message->recipient_count += 1;
            }
            for (int i = 0; i < admin->user_count; i++) {
                recepient = admin->user_list[i];
                recepient->announcements_list[recepient->announcement_count] = new_message;
                recepient->announcement_count += 1;
            }
            currentUser->sent_list[currentUser->sent_count] = new_message;
            currentUser->sent_count += 1;
            admin->message_list[admin->message_count] = new_message;
            admin->message_count += 1;
            printf("Announcement sent! Going Back...\n"); 
            break;
        case 0:
            printf("Going Back...\n");
            break;
        default:
            printf("Invalid Entry, Going Back...\n");
    }
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar(); 
}

/* inbox displays the messages received by a user and allows them to reply
@param currentUser - User that is currently logged in
@param admin - Program admin
Pre-condition: currentUser can't be null
*/
void inbox(User *currentUser, Admin *admin)
{
    int input_number; 
    Message *new_message;
    User *recepient;
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    printf("     GUMMIPHONE (INBOX)\n\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    printf("   |1| - Personal Messages\n");
    printf("   |2| - Group Messages\n");
    printf("   |0| - Back\n");
    
    printf("\nSelect an option: "); 
    scanf("%d", &input_number);
    getchar();
    int message_num;
    char message[250];
    switch(input_number)
    {
        case 1:
            for (int i = 0; i < currentUser->inbox_count; i++){
                if(currentUser->inbox_list[i]->message_type == 1){
                    printf("[%d] From: %s\n", i+1, currentUser->inbox_list[i]->message_sender->user_name);
                    printf("    Subject: %s\n", currentUser->inbox_list[i]->subject);
                    printf("    Content: %s\n", currentUser->inbox_list[i]->content);
                }
            }
            printf("\nEnter a message number to reply (0 to exit): "); 
            scanf("%d", &message_num);
            getchar();
            if(message_num != 0){
                new_message = (Message *) malloc(sizeof(Message));
                new_message->message_owner = currentUser;
                new_message->message_sender = currentUser;
                new_message->id_num = admin->message_count;
                new_message->recipient_count = 0;
                printf("Message subject: ");
                fgets(new_message->subject, 100, stdin); strtok(new_message->subject, "\n");
                printf("Message content: ");
                fgets(new_message->content, 250, stdin); strtok(new_message->content, "\n");
                int is_found = 0, found_index = 0;
                for (int i = 0; i < admin->user_count; i++) {
                    if(strcmp(admin->user_list[i]->user_name, currentUser->inbox_list[message_num-1]->message_sender->user_name) == 0){
                        is_found = 1;
                        found_index = i;
                    }
                }
                if(is_found == 1){
                    new_message->message_type = 1;
                    new_message->message_recipients[0] = admin->user_list[found_index];
                    new_message->recipient_count = 1;
                    currentUser->sent_list[currentUser->sent_count] = new_message;
                    currentUser->sent_count += 1;
                    recepient = admin->user_list[found_index];
                    recepient->inbox_list[recepient->inbox_count] = new_message;
                    recepient->inbox_count += 1;
                    admin->message_list[admin->message_count] = new_message;
                    admin->message_count += 1;
                    printf("Message sent! Going Back...\n"); 
                }
                else{
                    printf("Account not found! Going Back...\n"); 
                }
            }
            break;
        case 2:
            for (int i = 0; i < currentUser->inbox_count; i++){
                if(currentUser->inbox_list[i]->message_type == 2){
                    printf("[%d] %s's Group\n", i + 1, currentUser->inbox_list[i]->message_owner->user_name);
                    printf("    From: %s\n", currentUser->inbox_list[i]->message_sender->user_name);
                    printf("    Subject: %s\n", currentUser->inbox_list[i]->subject);
                    printf("    Content: %s\n", currentUser->inbox_list[i]->content);
                    printf("    Recepients:\n");
                    for (int j = 0; j < currentUser->inbox_list[i]->recipient_count; j++){
                        printf("    (%d) %s\n", j + 1, currentUser->inbox_list[i]->message_recipients[j]->user_name);
                    }
                    printf("\n");
                }
            }
            printf("\nMessage number to reply (0 to exit): "); 
            scanf("%d", &message_num);
            getchar();
            if(message_num != 0){
                new_message = (Message *) malloc(sizeof(Message));
                new_message->message_owner = currentUser->inbox_list[message_num-1]->message_owner;
                new_message->message_sender = currentUser;
                new_message->id_num = admin->message_count;
                new_message->recipient_count = 0;
                printf("Message subject: ");
                fgets(new_message->subject, 100, stdin); strtok(new_message->subject, "\n");
                printf("Message content: ");
                fgets(new_message->content, 250, stdin); strtok(new_message->content, "\n");
                int is_found = 0, found_index = 0;
                for (int i = 0; i < admin->user_count; i++) {
                    if(strcmp(admin->user_list[i]->user_name, currentUser->inbox_list[message_num-1]->message_owner->user_name) == 0){
                        is_found = 1;
                        found_index = i;
                    }
                }
                if(is_found == 1){
                    new_message->message_type = 2;
                    new_message->message_recipients[0] = admin->user_list[found_index];
                    new_message->recipient_count = 1;
                    currentUser->sent_list[currentUser->sent_count] = new_message;
                    currentUser->sent_count += 1;
                    recepient = admin->user_list[found_index];
                    recepient->inbox_list[recepient->inbox_count] = new_message;
                    recepient->inbox_count += 1;
                }
                for (int i = 0; i < currentUser->inbox_list[message_num-1]->recipient_count - 1; i++) {
                    if(strcmp(currentUser->user_name, currentUser->inbox_list[message_num-1]->message_recipients[i]->user_name) != 0){
                        new_message->message_recipients[i] = currentUser->inbox_list[message_num-1]->message_recipients[i];
                        new_message->recipient_count += 1;
                    }
                }
                for (int i = 0; i < currentUser->inbox_list[message_num-1]->recipient_count - 1; i++) {
                    if(strcmp(currentUser->user_name, currentUser->inbox_list[message_num-1]->message_recipients[i]->user_name) != 0){
                        recepient = currentUser->inbox_list[message_num-1]->message_recipients[i];
                        recepient->inbox_list[recepient->inbox_count] = new_message;
                        recepient->inbox_count += 1;
                    }
                }
                currentUser->sent_list[currentUser->sent_count] = new_message;
                currentUser->sent_count += 1;
                admin->message_list[admin->message_count] = new_message;
                admin->message_count += 1;
                printf("Message sent! Going Back...\n"); 
            }
            break;
        case 0:
            printf("Going Back...\n");
            break;
        default:
            printf("Invalid Entry, Going Back...\n");
    }
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar(); 
}

/* sent displays the messages sent by a user 
@param currentUser - User that is currently logged in
@param admin - Program admin
Pre-condition: currentUser can't be null
*/
void sent(User *currentUser)
{
    int input_number; 
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    printf(" GUMMIPHONE (SENT MESSAGES)\n\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    for (int i = 0; i < currentUser->sent_count; i++) {
        printf("[%d] Subject: %s\n", i + 1, currentUser->sent_list[i]->subject);
        printf("     Content: %s\n", currentUser->sent_list[i]->content);
        printf("    Sent to:\n");
        for (int j = 0; j < currentUser->sent_list[i]->recipient_count; j++){
            printf("        (%d)%s\n", j+1, currentUser->sent_list[i]->message_recipients[j]->user_name);
        }
    }
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar(); 
}

/* viewAnnouncements displays the announcements inside the program and allows users to reply to it
@param currentUser - User that is currently logged in
@param admin - Program admin
Pre-condition: currentUser can't be null
*/
void viewAnnouncements(User *currentUser, Admin *admin)
{
    int input_number; 
    int message_num;
    char message[250];
    Message *new_message;
    User *recepient;
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    printf(" GUMMIPHONE (ANNOUNCEMENTS)\n\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    for (int i = 0; i < currentUser->announcement_count; i++) {
        printf("[%d] From: %s\n", i + 1, currentUser->announcements_list[i]->message_sender->user_name);
        printf("     Subject: %s\n", currentUser->announcements_list[i]->subject);
        printf("     Content: %s\n", currentUser->announcements_list[i]->content);
    }
    printf("\nAnnouncement number to reply (0 to exit): "); 
    scanf("%d", &message_num);
    getchar();
    if(message_num != 0){
        new_message = (Message *) malloc(sizeof(Message));
        new_message->message_owner = currentUser;
        new_message->message_sender = currentUser;
        new_message->id_num = admin->message_count;
        new_message->recipient_count = 0;
        printf("Message subject: ");
        fgets(new_message->subject, 100, stdin); strtok(new_message->subject, "\n");
        printf("Message content: ");
        fgets(new_message->content, 250, stdin); strtok(new_message->content, "\n");
        new_message->message_type = 3;
        for (int i = 0; i < admin->user_count; i++) {
            if(strcmp(currentUser->user_name, currentUser->inbox_list[message_num-1]->message_recipients[i]->user_name) != 0){
                new_message->message_recipients[i] = admin->user_list[i];
                new_message->recipient_count += 1;
            }
        }
        for (int i = 0; i < admin->user_count; i++) {
            if(strcmp(currentUser->user_name, currentUser->inbox_list[message_num-1]->message_recipients[i]->user_name) != 0){
                recepient = admin->user_list[i];
                recepient->announcements_list[recepient->announcement_count] = new_message;
                recepient->announcement_count += 1;
            }
        }
        currentUser->sent_list[currentUser->sent_count] = new_message;
        currentUser->sent_count += 1;
        admin->message_list[admin->message_count] = new_message;
        admin->message_count += 1;
        printf("Announcement sent! Going Back...\n"); 
    }
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar(); 
}


/* modifyContents gives the user an option to change their name or their description
@param currentUser - User that is currently logged in
Pre-condition: currentUser can't be null
*/
void modifyContents(User *currentUser)
{
    int input_number; 
    char pass[20], newValue[100];
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    printf("GUMMIPHONE (MODIFY CONTENTS)\n\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    printf("|1| - Change Name\n");
    printf("|2| - Change Description\n");
    printf("|0| - Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &input_number);
    getchar();
    switch(input_number)
    {
        case 1:
            printf("Old Name: %s\n", currentUser->full_name);
            printf("New Name: ");
            fgets(newValue, 100, stdin); strtok(newValue, "\n");
            printf("Password to Continue: ");
            fgets(pass, 20, stdin); strtok(pass, "\n");
            if(strcmp(pass, currentUser->user_password) == 0){
                strcpy(currentUser->full_name, newValue);
                printf("Name changed to: %s\n", currentUser->full_name);
            }
            break;
        case 2:
            printf("Old Description: %s\n", currentUser->bio_description);
            printf("New Description: ");
            fgets(newValue, 100, stdin); strtok(newValue, "\n");
            printf("Password to Continue: ");
            fgets(pass, 20, stdin); strtok(pass, "\n");
            if(strcmp(pass, currentUser->user_password) == 0){
                strcpy(currentUser->bio_description, newValue);
                printf("Description changed to: %s\n", currentUser->bio_description);
            }
            break;
        case 0:
            printf("Going Back...\n");
            break;
        default:
            printf("Invalid Entry, Going Back...\n");
    }
}

/* modifySecurity gives the user an option to change their password or their Security Question Answer
@param currentUser - User that is currently logged in
@param admin - Program admin
Pre-condition: currentUser can't be null
*/
void modifySecurity(User *currentUser, Admin *admin)
{
    int input_number; 
    char oldPass[20], newPass[20];
    printf("GUMMIPHONE (MODIFY SECURITY)\n\n");
    printf("[1] Change Password\n");
    printf("[2] Change Security Question Answer\n");
    printf("[0] Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &input_number);
    getchar();
    switch(input_number)
    {
        case 1:
            printf("Enter Old Password: ");
            fgets(oldPass, 20, stdin); strtok(oldPass, "\n");
            if(strcmp(oldPass, currentUser->user_password) == 0){
                do{
                    printf("Enter New Password: ");
                    fgets(currentUser->user_password, 20, stdin); strtok(currentUser->user_password, "\n");
                    printf("Re-enter New Password: ");
                    fgets(newPass, 20, stdin); strtok(newPass, "\n");
                    if(strcmp(newPass, currentUser->user_password) != 0)
                        printf("Re-Entered Password Doesn't Match! Try Again...");
                }while(strcmp(newPass, currentUser->user_password) != 0);
            }
            break;
        case 2:
            do{
                printf("%s\n", admin->recovery_questions[currentUser->selected_q_num]);
                printf("Enter Answer: ");
                fgets(currentUser->sec_password, 20, stdin); strtok(currentUser->sec_password, "\n");
                printf("Re-enter Answer: ");
                fgets(newPass, 20, stdin); strtok(newPass, "\n");
                if(strcmp(newPass, currentUser->sec_password) != 0)
                    printf("Re-Entered Answer Doesn't Match! Try Again...");
            }while(strcmp(newPass, currentUser->sec_password) != 0);
            break;
        case 0:
            printf("Going Back...\n");
            break;
        default:
            printf("Invalid Entry, Going Back...\n");
    }
}

/* modifyConnections gives the user an option to add, view, or remove connections
@param currentUser - User that is currently logged in
@param admin - Program admin
Pre-condition: currentUser can't be null
*/
void modifyConnections(User *currentUser, Admin *admin)
{
    int input_number; 
    int tempNum, addNum;
    char addName[50];
    int is_found = 0, found_index = 0;
    int tempNum2, removeNum;
    char removeName[50];
    printf("GUMMIPHONE (MODIFY CONNECTIONS)\n\n");
    printf("[1] Add Connection\n");
    printf("[2] View Personal Connections\n");
    printf("[3] Remove Connection\n");
    printf("[4] View User Connections\n");
    printf("[0] Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &input_number);
    getchar();
    switch(input_number)
    {
        case 1:
            printf("[1] All Users\n");
            printf("[2] Search User\n");
            printf("[0] Back\n");
            
            printf("\nEnter a number to continue: "); 
            scanf("%d", &tempNum);
            getchar();
            switch(tempNum)
            {
                case 1:
                    printf("GUMMIPHONE USERS\n"); 
                    for (int i = 0; i < admin->user_count; i++) {
                        printf("[%d] %s\n", i + 1, admin->user_list[i]->user_name);
                    }
                    printf("[0] Back\n\n"); 
                    printf("Enter user number to add: "); 
                    scanf("%d", &addNum);
                    getchar();
                    if(addNum != 0){
                        currentUser->connection_list[currentUser->connection_count] = admin->user_list[addNum - 1];
                        currentUser->connection_count += 1;
                        printf("User added successfully!");
                    }
                    break;
                case 2:
                    printf("Enter username: ");
                    fgets(addName, 50, stdin); strtok(addName, "\n");
                    is_found = 0; found_index = 0;
                    for (int i = 0; i < admin->user_count; i++) {
                        if(strcmp(admin->user_list[i]->user_name, addName) == 0){
                            is_found = 1;
                            found_index = i;
                        }
                    }
                    if(is_found == 1){
                        currentUser->connection_list[currentUser->connection_count] = admin->user_list[found_index];
                        currentUser->connection_count += 1;
                        printf("User added successfully!");
                    }
                    else{
                        printf("Account not found! Going Back...\n"); 
                    }
                    break;
                case 0:
                    printf("Going Back...\n");
                    break;
                default:
                    printf("Invalid Entry, Going Back...\n");
            }
            break;
        case 2:
            printf("PERSONAL CONNECTIONS\n"); 
            for (int i = 0; i < currentUser->connection_count; i++) {
                printf("[%d] %s\n", i + 1, currentUser->connection_list[i]->user_name);
            }
            printf("\n_____________________________\n\n");
            printf("To continue, press the enter key!"); getchar();
            break;
        case 3:
            printf("[1] All Users\n");
            printf("[2] Search User\n");
            printf("[0] Back\n");
            
            printf("\nEnter a number to continue: "); 
            scanf("%d", &tempNum2);
            getchar();
            switch(tempNum2)
            {
                case 1:
                    printf("GUMMIPHONE USERS\n"); 
                    for (int i = 0; i < admin->user_count; i++) {
                        printf("[%d] %s\n", i + 1, admin->user_list[i]->user_name);
                    }
                    printf("[0] Back\n\n"); 
                    printf("Enter user number to remove: "); 
                    scanf("%d", &removeNum);
                    getchar();
                    if(removeNum != 0){
                        for (int i = removeNum; i < currentUser->connection_count; i++) {
                            currentUser->connection_list[i-1] = currentUser->connection_list[i];
                        }
                        currentUser->connection_count -= 1;
                        printf("User removed successfully!\n");
                    }
                    break;
                case 2:
                    printf("Enter username: ");
                    fgets(removeName, 50, stdin); strtok(removeName, "\n");
                    int is_found = 0, found_index = 0;
                    for (int i = 0; i < admin->user_count; i++) {
                        if(strcmp(admin->user_list[i]->user_name, removeName) == 0){
                            is_found = 1;
                            found_index = i;
                        }
                    }
                    if(is_found == 1){
                        for (int i = found_index; i < currentUser->connection_count; i++) {
                            currentUser->connection_list[i-1] = currentUser->connection_list[i];
                        }
                        currentUser->connection_count -= 1;
                        printf("User removed successfully!");
                    }
                    else{
                        printf("Account not found! Going Back...\n"); 
                    }
                    break;
                case 0:
                    printf("Going Back...\n");
                    break;
                default:
                    printf("Invalid Entry, Going Back...\n");
            }
            break;
        case 4:
            printf("USER CONNECTIONS\n"); 
            for (int i = 0; i < admin->user_count; i++) {
                for (int j = 0; j < admin->user_list[i]->connection_count; j++) {
                    if(strcmp(admin->user_list[i]->connection_list[j]->user_name, currentUser->user_name) == 0){
                        printf("[%d] %s\n", i + 1, admin->user_list[i]->user_name);
                    }
                }
            }
            printf("\n_____________________________\n\n");
            printf("To continue, press the enter key!"); getchar();
            break;
        case 0:
            printf("Going Back...\n");
            break;
        default:
            printf("Invalid Entry, Going Back...\n");
    }
}

/* viewUserProfile gives the user an option to view every user of gummiphone and allows them to add or message the user
@param currentUser - User that is currently logged in
@param viewedUser - User that is currently being viewed
@param admin - Program admin
Pre-condition: currentUser can't be null
*/
void viewUserProfile(User *currentUser, User *viewedUser, Admin *admin)
{
    int input_number, viewNum; 
    int is_found = 0, found_index = 0;
    Message *new_message;
    User *recepient;
    printf("GUMMIPHONE (VIEW USER)\n\n");
    printf("Username: %s\n", viewedUser->user_name);
    printf("Name: %s\n", viewedUser->full_name);
    printf("Description: %s\n\n", viewedUser->bio_description);
    printf("[1] Add User\n");
    printf("[2] Message User\n");
    printf("[0] Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &input_number);
    getchar();
    switch(input_number)
    {
        case 1:
            is_found = 0; found_index = 0;
            for (int i = 0; i < admin->user_count; i++) {
                if(strcmp(admin->user_list[i]->user_name, viewedUser->user_name) == 0){
                    is_found = 1;
                    found_index = i;
                }
            }
            if(is_found == 1){
                currentUser->connection_list[currentUser->connection_count] = admin->user_list[found_index];
                currentUser->connection_count += 1;
                printf("User added successfully!");
            }
            else{
                printf("Account not found! Going Back...\n"); 
            }
            break;
        case 2:
            new_message = (Message *) malloc(sizeof(Message));
            new_message->message_owner = currentUser;
            new_message->message_sender = currentUser;
            new_message->id_num = admin->message_count;
            new_message->recipient_count = 0;
            printf("GUMMIPHONE (COMPOSE MESSAGE)\n\n");
            printf("Enter message subject: ");
            fgets(new_message->subject, 100, stdin); strtok(new_message->subject, "\n");
            printf("Enter message content: ");
            fgets(new_message->content, 250, stdin); strtok(new_message->content, "\n");
            new_message->message_type = 1;
            is_found = 0; found_index = 0;
            for (int i = 0; i < admin->user_count; i++) {
                if(strcmp(admin->user_list[i]->user_name, viewedUser->user_name) == 0){
                    is_found = 1;
                    found_index = i;
                }
            }
            if(is_found == 1){
                new_message->message_recipients[0] = admin->user_list[found_index];
                new_message->recipient_count = 1;
                currentUser->sent_list[currentUser->sent_count] = new_message;
                currentUser->sent_count += 1;
                recepient = admin->user_list[found_index];
                recepient->inbox_list[recepient->inbox_count] = new_message;
                recepient->inbox_count += 1;
                admin->message_list[admin->message_count] = new_message;
                admin->message_count += 1;
                printf("Message sent! Going Back...\n"); 
            }
            else{
                printf("Account not found! Going Back...\n"); 
            }
            break;
        case 0:
            printf("Going Back...\n");
            break;
        default:
            printf("Invalid Entry, Going Back...\n");
    }
}

/* browseUsers gives the user an option to view every user of gummiphone
@param currentUser - User that is currently logged in
@param admin - Program admin
Pre-condition: currentUser can't be null
*/
void browseUsers(User *currentUser, Admin *admin)
{
    int input_number, viewNum; 
    char viewName[50];
    int is_found = 0, found_index = 0;
    printf("GUMMIPHONE (BROWSE USERS)\n\n");
    printf("[1] All Users\n");
    printf("[2] Search User\n");
    printf("[0] Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &input_number);
    getchar();
    switch(input_number)
    {
        case 1:
            printf("GUMMIPHONE USERS\n"); 
            for (int i = 0; i < admin->user_count; i++) {
                printf("[%d] %s\n", i + 1, admin->user_list[i]->user_name);
            }
            printf("[0] Back\n\n"); 
            printf("Enter user number to view: "); 
            scanf("%d", &viewNum);
            getchar();
            if(viewNum != 0){
                system("CLS");
                viewUserProfile(currentUser, admin->user_list[viewNum - 1], admin);
            }
            break;
        case 2:
            printf("Enter username: ");
            fgets(viewName, 50, stdin); strtok(viewName, "\n");
            is_found = 0; found_index = 0;
            for (int i = 0; i < admin->user_count; i++) {
                if(strcmp(admin->user_list[i]->user_name, viewName) == 0){
                    is_found = 1;
                    found_index = i;
                }
            }
            if(is_found == 1){
                system("CLS");
                viewUserProfile(currentUser, admin->user_list[found_index], admin);
            }
            else{
                printf("Account not found! Going Back...\n"); 
            }
            break;
        case 0:
            printf("Going Back...\n");
            break;
        default:
            printf("Invalid Entry, Going Back...\n");
    }
}

/* userModule displays the user module menu 
@param currentUser - User that is currently logged in
@param admin - Program admin
Pre-condition: currentUser can't be null
*/
void userModule(User *currentUser, 
            Admin *admin)
{
    int input_number;
    do{
        system("CLS");
        printf("GUMMIPHONE (USER MODULE)\n\n");
        printf("[1] Compose Message\n");
        printf("[2] Inbox\n");
        printf("[3] Sent\n");
        printf("[4] View Announcements\n");
        printf("[5] Modify Personal Contents\n");
        printf("[6] Modify Account Security\n");
        printf("[7] Modify Personal Connections\n");
        printf("[8] Browse Users\n");
        printf("[0] Logout\n");
        
        printf("\nEnter a number to continue: "); 
        scanf("%d", &input_number);
        getchar();
        switch(input_number)
        {
            case 1:
                system("CLS");
                composeMessage(currentUser, admin);
                break;
            case 2:
                system("CLS");
                inbox(currentUser, admin);
                break;
            case 3:
                system("CLS");
                sent(currentUser);
                break;
            case 4:
                system("CLS");
                viewAnnouncements(currentUser, admin);
                break;
            case 5:
                system("CLS");
                modifyContents(currentUser);
                break;
            case 6:
                system("CLS");
                modifySecurity(currentUser, admin);
                break;
            case 7:
                system("CLS");
                modifyConnections(currentUser, admin);
                break;
            case 8:
                system("CLS");
                browseUsers(currentUser, admin);
                break;
            case 0:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid Entry, try again\n");
        }
    }while(input_number != 0);
}

/* login allows a user to login using their account
@param admin - Program admin
*/
void login(Admin *admin)
{
    char user_name[50], user_password[50];
    int is_found = 0, found_index = 0;
    char newPass[20];
    printf("-+-+-+-+-+--+-+--+-+--+-+--+-\n");
    printf("     GUMMIPHONE (LOGIN)\n");
    printf("-+-+-+-+-+--+-+--+-+--+-+--+-\n\n");
    printf("Username: "); 
    fgets(user_name, 50, stdin); strtok(user_name, "\n");
    printf("Password: "); 
    fgets(user_password, 20, stdin); strtok(user_password, "\n");
    for (int i = 0; i < admin->user_count; i++) {
        if(strcmp(admin->user_list[i]->user_name, user_name) == 0 && strcmp(admin->user_list[i]->user_password, user_password) == 0){
            is_found = 1;
            found_index = i;
        }
    }
    if(is_found == 1){
        system("CLS");
        if(admin->user_list[found_index]->is_locked == 0){
            if(strcmp(admin->user_list[found_index]->user_password, "DEFAULT") != 0)
                userModule(admin->user_list[found_index], admin);
            else{
                printf("Your password has reset!\n");
                printf("Current password: DEFAULT\n");
                do{
                    printf("New Password: ");
                    fgets(admin->user_list[found_index]->user_password, 20, stdin); strtok(admin->user_list[found_index]->user_password, "\n");
                    printf("Re-enter New Password: ");
                    fgets(newPass, 20, stdin); strtok(newPass, "\n");
                    if(strcmp(newPass, admin->user_list[found_index]->user_password) != 0)
                        printf("Re-Entered Password Doesn't Match! Try Again...");
                }while(strcmp(newPass, admin->user_list[found_index]->user_password) != 0);
            }
        }
        else
            printf("Account is locked! Going Back...\n");
    }
    else{
        printf("\nAccount does not exist!\n"); 
    }
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar(); 
}

/* createAccount allows a user to create their gummiphone account
@param admin - Program admin
*/
void createAccount(Admin *admin)
{
    User *newUser;
    int is_found = 0;
    if(admin->question_count != 0){
        newUser = (User *) malloc(sizeof(User));
        newUser->inbox_count = 0;
        newUser->sent_count = 0;
        newUser->announcement_count = 0;
        is_found = 0;
        char reEnter[20];
        do{
            system("CLS");
            printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
            printf(" GUMMIPHONE (CREATE ACCOUNT)\n");
            printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");
            printf("Name: "); fgets(newUser->full_name, 50, stdin); strtok(newUser->full_name, "\n");
            printf("Username: "); fgets(newUser->user_name, 50, stdin); strtok(newUser->user_name, "\n");
            for (int i = 0; i < admin->user_count; i++) {
                if(strcmp(admin->user_list[i]->user_name, newUser->user_name) == 0)
                    is_found = 1;
            }
            if(is_found != 1){
                do{
                    printf("Password: "); fgets(newUser->user_password, 20, stdin); strtok(newUser->user_password, "\n");
                    printf("Re-enter Password: "); fgets(reEnter, 20, stdin); strtok(reEnter, "\n");
                    if(strcmp(newUser->user_password, reEnter) != 0)
                        printf("Password entered is incorrect. Please try again.\n\n");
                }while(strcmp(newUser->user_password, reEnter) != 0);
                newUser->selected_q_num = rand() % admin->question_count + 1;
                printf("%s\n", admin->recovery_questions[newUser->selected_q_num]);
                printf("Enter Answer: "); fgets(newUser->sec_password, 20, stdin); strtok(newUser->sec_password, "\n");
                newUser->is_locked = 0;
                newUser->connection_count = 0;
                strcpy(newUser->bio_description, "DEFAULT USER");
            }
            else{
                printf("Username is already registered!\n");
                printf("_____________________________\n\n");
                printf("To try again, press the enter key!"); getchar(); 
            }
        }while(is_found == 1);
        admin->user_list[admin->user_count] = newUser;
        admin->user_count+=1;
        printf("Account Successfully Created!\n");
        printf("To continue, press the enter key!"); getchar(); 
    }
    else{
        printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
        printf(" GUMMIPHONE (CREATE ACCOUNT)\n");
        printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");
        printf("No security questions found!\n");
        printf("Account creation is locked!\n");
        printf("Contact Admin!\n");
        printf("_____________________________\n\n");
        printf("To continue, press the enter key!"); getchar(); 
    }
}

/* forgotPassword gives the user an option to alert admins that they forgot their password
@param admin - Program admin
*/
void forgotPassword(Admin *admin)
{
    char user_name[50];
    char answer[20];
    int is_found = 0, found_index = 0;
    printf("GUMMIPHONE (FORGOT PASSWORD)\n\n");
    printf("Enter username to continue (0 to exit): "); 
    fgets(user_name, 50, stdin); strtok(user_name, "\n");
    if(strcmp(user_name, "0") != 0){
        is_found = 0; found_index = 0;
        for (int i = 0; i < admin->user_count; i++) {
            if(strcmp(admin->user_list[i]->user_name, user_name) == 0){
                is_found = 1;
                found_index = i;
            }
        }
        if(is_found == 1){
            printf("%s\n", admin->recovery_questions[admin->user_list[found_index]->selected_q_num]);
            printf("Enter Answer: "); fgets(answer, 20, stdin); strtok(answer, "\n");
            if(strcmp(answer, admin->user_list[found_index]->sec_password) == 0){
                admin->user_list[found_index]->is_locked = 1;
                printf("Password Reset Request Sent to Admin!\n"); 
            }
            else{
                printf("Password Invalid! Going Back...\n"); 
            }
        }
        else{
            printf("Username not found! Going Back...\n"); 
        }
    }
    else 
        printf("Going Back...\n"); 
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar(); 
}

/* loginPage displays the login page menu
@param admin - Program admin
*/
void 
loginPage(Admin *admin)
{
    int input_number;
    do{
        system("CLS");
        printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
        printf("   GUMMIPHONE (LOGIN PAGE)\n");
        printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");
        printf(" |1| - Login\n");
        printf(" |2| - Create a new account\n");
        printf(" |3| - Forgot password\n");
        printf(" |4| - Back\n");
        
        printf("\nSelect an option: ");
        scanf("%d", &input_number);
        getchar();
        switch(input_number)
        {
            case 1:
                system("CLS");
                login(admin);
                break;
            case 2:
                system("CLS");
                createAccount(admin);
                break;
            case 3:
                system("CLS");
                forgotPassword(admin);
                break;
            case 4:
                system("CLS");
                break;
            default:
                printf("Invalid Entry, try again\n");
        }
    }while(input_number != 4);
}

/* viewUsers gives the admin an option to view every user of gummiphone
@param admin - Program admin
*/
void viewUsers(Admin *admin)
{
    printf("GUMMIPHONE USERS\n\n"); 
    for (int i = 0; i < admin->user_count; i++) {
        printf("[%d]\tName: %s\n", i + 1, admin->user_list[i]->full_name);
        printf("\tUsername: %s\n", admin->user_list[i]->user_name);
        printf("\tPassword: ");
        for (int j = 0; j < sizeof(admin->user_list[i]->user_password); j++) {
            printf("%c", admin->user_list[i]->user_password[j] + 3);
        }
        printf("\n\tDescription: %s\n", admin->user_list[i]->bio_description);
        printf("\tConnections:\n");
        for (int j = 0; j < admin->user_list[i]->connection_count; j++) {
            printf("[%d] %s\n", j + 1, admin->user_list[i]->connection_list[j]->user_name);
        }
        if(admin->user_list[i]->is_locked == 0)
            printf("\tLocked: False\n");
        else
            printf("\tLocked: True\n");
    }
}

/* editUserDetails allows the admin to modify user details
@param userEditted - User that is currently being modified
@param admin - Program admin
Pre-condition: userEditted can't be null
*/
void editUserDetails(User *userEditted, Admin *admin)
{
    int input_number;
    char newValue[100];
    int is_found = 0, found_index = 0;
    int removeNum;
    int tempNum;
    char addName[50];
    printf("GUMMIPHONE (MODIFY USER)\n\n");
    printf("[1] Name: %s\n", userEditted->full_name);
    printf("[2] Username: %s\n", userEditted->user_name);
    printf("[3] Description: %s\n", userEditted->bio_description);
    printf("[4] Personal Connections:\n");
    for (int j = 0; j < userEditted->connection_count; j++) {
        printf("       [%d] %s\n", j+1, userEditted->connection_list[j]->user_name);
    }
    printf("[0] Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &input_number);
    getchar();
    switch(input_number)
    {
        case 1:
            printf("Old Name: %s\n", userEditted->full_name);
            printf("New Name: ");
            fgets(userEditted->full_name, 50, stdin); strtok(userEditted->full_name, "\n");
            printf("Name changed to: %s\n", userEditted->full_name);
            break;
        case 2:
            printf("Old Username: %s\n", userEditted->user_name);
            printf("New Username: ");
            fgets(userEditted->user_name, 50, stdin); strtok(userEditted->user_name, "\n");
            printf("Username changed to: %s\n", userEditted->user_name);
            break;
        case 3:
            printf("Old Description: %s\n", userEditted->bio_description);
            printf("New Description: ");
            fgets(userEditted->bio_description, 250, stdin); strtok(userEditted->bio_description, "\n");
            printf("Description changed to: %s\n", userEditted->bio_description);
            break;
        case 4:
            printf("[1] Add Connection\n");
            printf("[2] Remove Connection\n");
            printf("\nEnter a number to continue: "); 
            scanf("%d", &tempNum);
            getchar();
            switch (tempNum){
                case 1:
                    printf("Enter username: ");
                    fgets(addName, 50, stdin); strtok(addName, "\n");
                    is_found = 0; found_index = 0;
                    for (int i = 0; i < admin->user_count; i++) {
                        if(strcmp(admin->user_list[i]->user_name, addName) == 0){
                            is_found = 1;
                            found_index = i;
                        }
                    }
                    if(is_found == 1){
                        userEditted->connection_list[userEditted->connection_count] = admin->user_list[found_index];
                        userEditted->connection_count += 1;
                        printf("User added successfully!");
                    }
                    else{
                        printf("Account not found! Going Back...\n"); 
                    }
                    break;
                case 2:
                    printf("Enter user number to remove: "); 
                    scanf("%d", &removeNum);
                    getchar();
                    if(removeNum != 0){
                        for (int i = removeNum; i < userEditted->connection_count; i++) {
                            userEditted->connection_list[i-1] = userEditted->connection_list[i];
                        }
                        userEditted->connection_count -= 1;
                        printf("User removed successfully!");
                    }
                    break;
                default:
                    break;
            }
            break;
        case 0:
            system("CLS");
            break;
        default:
            printf("Invalid Entry, try again\n");
    }
}

/* modifyUsers allows the admin to choose a specific user to modify
@param admin - Program admin
*/
void modifyUsers(Admin *admin)
{
    int viewNum; 
    printf("GUMMIPHONE USERS\n\n"); 
    for (int i = 0; i < admin->user_count; i++) {
        printf("[%d] %s\n", i + 1, admin->user_list[i]->user_name);
    }
    printf("[0] Back\n\n"); 
    printf("Enter user number to modify: "); 
    scanf("%d", &viewNum);
    getchar();
    if(viewNum != 0){
        system("CLS");
        editUserDetails(admin->user_list[viewNum - 1], admin);
    }
}

/* refreshPassword shows the admin the list of users who requested password resets, and allows the admin to choose users to reset password
@param admin - Program admin
*/
void 
refreshPassword(Admin *admin)
{
    int resetNum;
    printf("RESET REQUESTS\n"); 
    for (int i = 0; i < admin->user_count; i++) {
        if(admin->user_list[i]->is_locked == 1)
            printf("[%d] %s\n", i + 1, admin->user_list[i]->user_name);
    }
    printf("Enter user number to reset: "); 
    scanf("%d", &resetNum);
    getchar();
    if(resetNum != 0 && resetNum <= admin->user_count){
        strcpy(admin->user_list[resetNum-1]->user_password, "DEFAULT");
        admin->user_list[resetNum-1]->is_locked = 0;
    }
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar();
}

/* refreshPassword allows the admin to choose users that will be removed from the program
@param admin - Program admin
*/
void 
deleteUsers(Admin *admin)
{
    int tempNum1, removeNum;
    char removeName[50];
    int is_found = 0, found_index = 0;
    printf("GUMMIPHONE (DELETE USERS)\n\n");
    printf("[1] View Users\n");
    printf("[2] Search User\n");
    printf("[0] Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &tempNum1);
    getchar();
    switch(tempNum1) {
        case 1:
            printf("GUMMIPHONE USERS\n"); 
            for (int i = 0; i < admin->user_count; i++) {
                printf("[%d] %s\n", i + 1, admin->user_list[i]->user_name);
            }
            printf("[0] Back\n\n"); 
            printf("Enter user number to remove: "); 
            scanf("%d", &removeNum);
            getchar();
            if(removeNum != 0){
                strcpy(admin->user_list[removeNum-1]->user_name, "DELETED USER");
                for (int i = removeNum; i < admin->user_count; i++) {
                    admin->user_list[i-1] = admin->user_list[i];
                }
                admin->user_count -= 1;
                printf("User removed successfully!");
            }
            break;
        case 2:
            printf("Enter username: ");
            fgets(removeName, 50, stdin); strtok(removeName, "\n");
            is_found = 0; found_index = 0;
            for (int i = 0; i < admin->user_count; i++) {
                if(strcmp(admin->user_list[i]->user_name, removeName) == 0){
                    is_found = 1;
                    found_index = i;
                }
            }
            if(is_found == 1){
                strcpy(admin->user_list[found_index]->user_name, "DELETED USER");
                for (int i = found_index; i < admin->user_count; i++) {
                    admin->user_list[i-1] = admin->user_list[i];
                }
                admin->user_count -= 1;
                printf("User removed successfully!\n");
            }
            else{
                printf("Account not found! Going Back...\n"); 
            }
            break;
        case 0:
            printf("Going Back...\n");
            break;
        default:
            printf("Invalid Entry, Going Back...\n");
    }
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar();
}

/* securityQuestions allows the admin to write security questions
@param admin - Program admin
*/
void 
securityQuestions(Admin *admin)
{
    char question[100];
    printf("GUMMIPHONE (ADMIN - SECURITY QUESTIONS)\n\n");
    printf("Current Security Questions: \n");
    for (int i = 0; i < admin->question_count; i++){
        printf("     %s\n", admin->recovery_questions[i]);
    }
    printf("Enter new question to add (0 to exit): "); 
    fgets(question, 100, stdin); strtok(question, "\n");
    if(strcmp(question, "0") != 0){
        strcpy(admin->recovery_questions[admin->question_count], question);
        printf("Security Question Added!\n"); 
        admin->question_count += 1;
    }
}

/* usersModule displays the user module for the admin
@param admin - Program admin
*/
void 
usersModule(Admin *admin)
{
    int input_number;
    printf("GUMMIPHONE (ADMIN - USER MODULE)\n\n");
    printf("[1] View Users\n");
    printf("[2] Modify Users\n");
    printf("[3] Refresh User Account Password\n");
    printf("[4] Delete Users\n");
    printf("[5] Security Questions\n");
    printf("[0] Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &input_number);
    getchar();
    switch(input_number)
    {
        case 1:
            system("CLS");
            viewUsers(admin);
            break;
        case 2:
            system("CLS");
            modifyUsers(admin);
            break;
        case 3:
            system("CLS");
            refreshPassword(admin);
            break;
        case 4:
            system("CLS");
            deleteUsers(admin);
            break;
        case 5:
            system("CLS");
            securityQuestions(admin);
            break;
        case 0:
            system("CLS");
            break;
        default:
            printf("Invalid Entry, try again\n");
    }
}

/* messagesModule displays the message module for the admin
@param admin - Program admin
*/
void 
messagesModule(Admin *admin)
{
    int input_number;
    int is_found = 0, found_index = 0;
    int tempNum;
    char user_name[50];
    printf("GUMMIPHONE (ADMIN - MESSAGES MODULE)\n\n");
    printf("[1] View Messages\n");
    printf("[2] View by Filter\n");
    printf("[3] Delete Message\n");
    printf("[0] Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &input_number);
    getchar();
    switch(input_number)
    {
        case 1:
            system("CLS");
            printf("GUMMIPHONE (MESSAGE LIST)\n\n");
            for (int i = 0; i < admin->message_count; i++){
                printf("[%d] Sender: %s\n", i+1, admin->message_list[i]->message_sender->user_name);
                printf("    Recepients:\n");
                for (int k = 0; k < admin->message_list[i]->recipient_count; k++){
                    printf("     (%d) %s\n", k+1, admin->message_list[i]->message_recipients[k]->user_name);
                }
                printf("    Subject: %s\n", admin->message_list[i]->subject);
                printf("    Content: %s\n", admin->message_list[i]->content);
            }
            break;
        case 2:
            system("CLS");
            printf("GUMMIPHONE (ADMIN - FILTERED MESSAGE)\n\n");
            printf("[1] Filter by sender\n");
            printf("[2] Filter by recepientr\n");
            printf("[0] Back\n");
            
            printf("\nEnter a number to continue: "); 
            scanf("%d", &tempNum);
            getchar();
            switch(tempNum)
            {
                case 1:
                    system("CLS");
                    printf("GUMMIPHONE (FILTER BY SENDER)\n\n");
                    printf("Enter username to continue (0 to exit): "); 
                    fgets(user_name, 50, stdin); strtok(user_name, "\n");
                    if(strcmp(user_name, "0") != 0){
                        is_found = 0; found_index = 0;
                        for (int i = 0; i < admin->message_count; i++) {
                            if(strcmp(admin->message_list[i]->message_sender->user_name, user_name) == 0){
                                printf("[%d] Subject: %s\n", i+1, admin->message_list[i]->subject);
                                printf("     Content: %s\n", admin->message_list[i]->content);
                                printf("     Recepients:\n");
                                for (int k = 0; k < admin->message_list[i]->recipient_count; k++){
                                    printf("     (%d) %s\n", k+1, admin->message_list[i]->message_recipients[k]->user_name);
                                }
                            }
                        }
                    }
                    else 
                        printf("Going Back...\n"); 
                    break;
                case 2:
                    system("CLS");
                    printf("GUMMIPHONE (FILTER BY RECEPIENT)\n\n");
                    printf("Enter username to continue (0 to exit): "); 
                    fgets(user_name, 50, stdin); strtok(user_name, "\n");
                    if(strcmp(user_name, "0") != 0){
                        for (int i = 0; i < admin->message_count; i++){
                            is_found = 0;
                            for (int j = 0; j < admin->message_list[i]->recipient_count; j++){
                                if(strcmp(admin->message_list[i]->message_recipients[j]->user_name, user_name) == 0){
                                    is_found = 1;
                                }
                            }
                            if(is_found == 1){
                                printf("[%d] Sender: %s\n", admin->message_list[i]->message_sender->user_name);
                                printf("    Recepients:\n");
                                for (int k = 0; k < admin->message_list[i]->recipient_count; k++){
                                    printf("     (%d) %s\n", k+1, admin->message_list[i]->message_recipients[k]->user_name);
                                }
                                printf("    Subject: %s\n", admin->message_list[i]->subject);
                                printf("    Content: %s\n", admin->message_list[i]->content);
                            }
                        }
                    }
                    else 
                        printf("Going Back...\n"); 
                    break;
                case 0:
                    system("CLS");
                    break;
                default:
                    printf("Invalid Entry, try again\n");
            }
            break;
        case 3:
            system("CLS");
            printf("GUMMIPHONE (DELETE MESSAGE)\n\n");
            for (int i = 0; i < admin->message_count; i++){
                printf("[%d] Sender: %s\n", admin->message_list[i]->id_num, admin->message_list[i]->message_sender->user_name);
                printf("    Recepients:\n");
                for (int k = 0; k < admin->message_list[i]->recipient_count; k++){
                    printf("     (%d) %s\n", k+1, admin->message_list[i]->message_recipients[k]->user_name);
                }
                printf("    Subject: %s\n", admin->message_list[i]->subject);
                printf("    Content: %s\n", admin->message_list[i]->content);
            }
            printf("\nEnter a message ID to delete: "); 
            scanf("%d", &tempNum);
            getchar();
            for (int i = 0; i < admin->message_count; i++){
                if(tempNum == admin->message_list[i]->id_num){
                    for (int j = i; j < admin->message_count - i; j++) {
                        admin->message_list[j] = admin->message_list[j+1];
                    }
                }
            }
            for (int i = 0; i < admin->user_count; i++){
                for (int j = 0; j < admin->user_list[i]->inbox_count; j++){
                    if(tempNum == admin->user_list[i]->inbox_list[j]->id_num){
                        for (int k = j; k < admin->user_list[i]->inbox_count - j; k++) {
                            admin->user_list[i]->inbox_list[j] = admin->user_list[i]->inbox_list[j+1];
                        }
                    }
                }
                for (int j = 0; j < admin->user_list[i]->sent_count; j++){
                    if(tempNum == admin->user_list[i]->sent_list[j]->id_num){
                        for (int k = j; k < admin->user_list[i]->sent_count - j; k++) {
                            admin->user_list[i]->sent_list[j] = admin->user_list[i]->sent_list[j+1];
                        }
                    }
                }
                for (int j = 0; j < admin->user_list[i]->announcement_count; j++){
                    if(tempNum == admin->user_list[i]->announcements_list[j]->id_num){
                        for (int k = j; k < admin->user_list[i]->announcement_count - j; k++) {
                            admin->user_list[i]->announcements_list[j] = admin->user_list[i]->announcements_list[j+1];
                        }
                    }
                }
            }
            admin->message_count -= 1;
            printf("Message Deleted!\n");
            break;
        case 0:
            system("CLS");
            break;
        default:
            printf("Invalid Entry, try again\n");
    }
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar(); 
}

/* changePassModule displays the change password module for the admin
@param admin - Program admin
*/
void 
changePassModule(Admin *admin)
{
    int input_number; 
    char oldPass[20], newPass[20];
    printf("GUMMIPHONE (ADMIN PASSWORD)\n\n");
    printf("Enter Old Admin Password: ");
    fgets(oldPass, 20, stdin); strtok(oldPass, "\n");
    if(strcmp(oldPass, admin->password) == 0){
        do{
            printf("Enter New Password: ");
            fgets(admin->password, 20, stdin);
            strtok(admin->password, "\n");
            printf("Re-enter New Password: ");
            fgets(newPass, 20, stdin);
            strtok(newPass, "\n");
            if(strcmp(newPass, admin->password) != 0)
                printf("Re-Entered Password Doesn't Match! Try Again...");
        }while(strcmp(newPass, admin->password) != 0);
    }
    printf("Invalid Password! Going Back...\n");
    printf("\n_____________________________\n\n");
    printf("To continue, press the enter key!"); getchar(); 
}

/* adminMenu displays the admin menu
@param admin - Program admin
*/
void 
adminMenu(Admin *admin)
{
    int input_number;
    printf("GUMMIPHONE (ADMIN MENU)\n\n");
    printf("[1] Users Module\n");
    printf("[2] Messages Module\n");
    printf("[3] Change Password Module\n");
    printf("[0] Back\n");
    
    printf("\nEnter a number to continue: "); 
    scanf("%d", &input_number);
    getchar();
    switch(input_number)
    {
        case 1:
            system("CLS");
            usersModule(admin);
            break;
        case 2:
            system("CLS");
            messagesModule(admin);
            break;
        case 3:
            system("CLS");
            changePassModule(admin);
            break;
        case 0:
            system("CLS");
            break;
        default:
            printf("Invalid Entry, try again\n");
    }
}

/* adminPage asks the admin password before redirecting to the admin menu
@param admin - Program admin
*/
void 
adminPage(Admin *admin)
{
    char pass[20];
    printf("GUMMIPHONE (ADMIN PAGE)\n\n");
    printf("Enter admin password to continue (0 to exit): "); 
    fgets(pass, 20, stdin);
    strtok(pass, "\n");
    if(strcmp(pass, admin->password) == 0){
        system("CLS");
        adminMenu(admin);
    }
    else if(strcmp(pass, "0") == 0){
        printf("Going Back...\n"); 
    }
    else
        printf("Invalid Input! Going Back...\n");
    printf("\n_____________________________\n\n"); 
    printf("To continue, press the enter key!"); getchar(); 
}

void save(Admin *admin){
    FILE *fptr;
    char fileName[30];
    fptr = fopen("admin.txt","w");

    fprintf(fptr, "%s\n", admin->password);
    fprintf(fptr, "%d\n", admin->user_count);
    fprintf(fptr, "%d\n", admin->message_count);
    fprintf(fptr, "%d\n", admin->question_count);
    for (int i = 0; i < admin->question_count; i++){
        fprintf(fptr, "%s\n", admin->recovery_questions[i]);
    }
    fclose(fptr);

    fptr = fopen("users.txt","w");
    fprintf(fptr, "%d\n", admin->user_count);
    for (int i = 0; i < admin->user_count; i++){
        fprintf(fptr, "%s\n", admin->user_list[i]->full_name);
        fprintf(fptr, "%s\n", admin->user_list[i]->user_name);
        fprintf(fptr, "%s\n", admin->user_list[i]->user_password);
        fprintf(fptr, "%s\n", admin->user_list[i]->sec_password);
        fprintf(fptr, "%s\n", admin->user_list[i]->bio_description);
        fprintf(fptr, "%d\n", admin->user_list[i]->inbox_count);
        fprintf(fptr, "%d\n", admin->user_list[i]->sent_count);
        fprintf(fptr, "%d\n", admin->user_list[i]->announcement_count);
        fprintf(fptr, "%d\n", admin->user_list[i]->is_locked);
        fprintf(fptr, "%d\n", admin->user_list[i]->selected_q_num);
        fprintf(fptr, "%d\n", admin->user_list[i]->connection_count);
        for (int j = 0; j < admin->user_list[i]->connection_count; j++){
            fprintf(fptr, "%d\n", admin->user_list[i]->connection_list[j]->user_name);
        }
    }
    fclose(fptr);

    fptr = fopen("messages.txt","w");
    fprintf(fptr, "%d\n", admin->message_count);
    for (int i = 0; i < admin->message_count; i++){
        fprintf(fptr, "%d\n", admin->message_list[i]->id_num);
        fprintf(fptr, "%s\n", admin->message_list[i]->subject);
        fprintf(fptr, "%s\n", admin->message_list[i]->content);
        fprintf(fptr, "%d\n", admin->message_list[i]->message_type);
        fprintf(fptr, "%s\n", admin->message_list[i]->message_owner->user_name);
        fprintf(fptr, "%s\n", admin->message_list[i]->message_sender->user_name);
        fprintf(fptr, "%d\n", admin->message_list[i]->recipient_count);
        for (int j = 0; j < admin->message_list[i]->recipient_count; j++){
            fprintf(fptr, "%s\n", admin->message_list[i]->message_recipients[j]->user_name);
        }
    }
    fclose(fptr);
}

void Load(Admin *admin) {
    FILE *fptr;
    int tempNum = 0, tempNum2 = 0, connCount = 0, is_found = 0, found_index = 0;
    char tempName[50];
    fptr = fopen("admin.txt", "r");
    if (fptr != NULL) {
        fscanf(fptr,"%[^\n]\n", &admin->password);
        fscanf(fptr,"%d\n", &tempNum);
        fscanf(fptr,"%d\n", &tempNum);
        fscanf(fptr,"%d\n", &admin->question_count);
        for (int i = 0; i < admin->question_count; i++){
            fscanf(fptr,"%[^\n]\n", &admin->recovery_questions[i]);
        }
        fclose(fptr);
    }
    fptr = fopen("users.txt", "r");
    if (fptr != NULL) {
        fscanf(fptr,"%d\n", &tempNum);
        for (int i = 0; i < tempNum; i++){
            User *newUser = (User *) malloc(sizeof(User));
            newUser->inbox_count = 0;
            newUser->sent_count = 0;
            newUser->announcement_count = 0;
            char reEnter[20];
            fscanf(fptr,"%[^\n]\n", &newUser->full_name);
            fscanf(fptr,"%[^\n]\n", &newUser->user_name); 
            fscanf(fptr,"%[^\n]\n", &newUser->user_password); 
            fscanf(fptr,"%[^\n]\n", &newUser->sec_password); 
            fscanf(fptr,"%[^\n]\n", &newUser->bio_description); 
            fscanf(fptr,"%d\n", &tempNum2);
            fscanf(fptr,"%d\n", &tempNum2);
            fscanf(fptr,"%d\n", &tempNum2);
            fscanf(fptr,"%d\n", &newUser->is_locked);
            fscanf(fptr,"%d\n", &newUser->selected_q_num);
            newUser->connection_count = 0;
            fscanf(fptr,"%d\n", &connCount);
            for (int j = 0; j < connCount; j++){
                fscanf(fptr,"%[^\n]\n", &tempName); 
                for (int z = 0; z < admin->user_count; z++) {
                    if(strcmp(admin->user_list[z]->user_name, tempName) == 0) {
                        is_found = 1; found_index = z;
                    }
                }
                if(is_found == 1){
                    newUser->connection_list[j] = admin->user_list[found_index];
                    newUser->connection_count += 1;
                }
            }
            
            admin->user_list[admin->user_count] = newUser;
            admin->user_count+=1;
        }
    }
    fclose(fptr);

    fptr = fopen("messages.txt", "r");
    if (fptr != NULL) {
        fscanf(fptr,"%d\n", &tempNum);
        for (int i = 0; i < tempNum; i++){
            Message *new_message = (Message*)malloc(sizeof(Message));
            fscanf(fptr,"%d\n", &new_message->id_num);
            fscanf(fptr,"%[^\n]\n", &new_message->subject);
            fscanf(fptr,"%[^\n]\n", &new_message->content); 
            fscanf(fptr,"%d\n", &new_message->message_type);
            fscanf(fptr,"%[^\n]\n", &tempName); 
            is_found = 0; found_index = 0;
            for (int j = 0; j < admin->user_count; j++) {
                if(strcmp(admin->user_list[j]->user_name, tempName) == 0) {
                    is_found = 1; found_index = j;
                }
            }
            if(is_found == 1){
                new_message->message_owner = admin->user_list[found_index];
            }
            fscanf(fptr,"%[^\n]\n", &tempName); 
            for (int j = 0; j < admin->user_count; j++) {
                if(strcmp(admin->user_list[j]->user_name, tempName) == 0) {
                    is_found = 1; found_index = j;
                }
            }
            if(is_found == 1){
                new_message->message_sender = admin->user_list[found_index];
                admin->user_list[found_index]->sent_list[admin->user_list[found_index]->sent_count] = new_message;
                admin->user_list[found_index]->sent_count += 1;
                admin->message_list[admin->message_count] = new_message;
                admin->message_count += 1;
            }
            fscanf(fptr,"%d\n", &new_message->recipient_count);
            for (int j = 0; j < new_message->recipient_count; j++){
                fscanf(fptr,"%[^\n]\n", &tempName);
                for (int z = 0; z < admin->user_count; z++) {
                    if(strcmp(admin->user_list[z]->user_name, tempName) == 0) {
                        is_found = 1; found_index = z;
                    }
                }
                if(is_found == 1){
                    new_message->message_recipients[j] = admin->user_list[found_index];
                    User *recepient = admin->user_list[found_index];
                    if(new_message->message_type != 3){
                        recepient->inbox_list[recepient->inbox_count] = new_message;
                        recepient->inbox_count += 1;
                    }
                    else{
                        recepient->announcements_list[recepient->announcement_count] = new_message;
                        recepient->announcement_count += 1;
                    }
                }
            }
        }
    }
    fclose(fptr);
}

/* This is the main function of the program, which is responsible for handling the main flow of the program. */
int main() 
{
	/* Declare and initialize variables for the input number and a timestamp to be used later in the program */
	
    int input_number; // to hold a number entered by the user
    
    time_t tTime; //  this can be used to store the current time.
    
    srand((unsigned) time(&tTime)); // sets the seed for the random number generator to the current time. That's why we need to include<time.h> in the library
    
    /* Allocate memory for an Admin struct object using malloc. */
    Admin *admin = (Admin*)malloc(sizeof(Admin)); 
    admin->user_count = 0;
    admin->message_count = 0;
    admin->question_count = 0;
    strcpy(admin->password, "admin");
    
    /* This initializes the values of the fields of the Admin struct object: user_count, message_count, question_count and password. */
    Load(admin);
	
	do {
        system("CLS"); // clears the console screen
        printf("-+-+-+-+-+--+-+--+-+--+-+--+-\n");
        printf("GUMMIPHONE MESSENGER SERVICE\n"); // this displays the main menu options 
        printf("-+-+-+-+-+--+-+--+-+--+-+--+-\n\n");
        printf("\t[1] Login Page\n");
        printf("\t[2] Admin\n");
        printf("\t[3] Quit\n");
        
        printf("\nSelect an option: "); 
        scanf("%d", &input_number);
        getchar(); // to prevent any leftover characters in the input buffer 
        
        switch(input_number) // asks the user to input a number corresponding to the desired menu option
        {
            case 1: 
                loginPage(admin); // if the user chooses option 1, it calls the loginPage function to handle the login process.
                break;
            case 2:
                system("CLS");
                adminPage(admin); // if the user chooses option 2, it clears the console screen and call the adminPage function to handle the admin tasks.
                break;
            case 3:
                system("CLS");
                save(admin); // if the user chooses option 3, it clears the console screen, save the data to files and print a thank you message before exiting the program.
                printf("Thanks for playing!");
                break;
            default: 
                printf("Invalid Entry, try again\n"); // If the user enters an invalid option, display an error message and return to the main menu. Repeat the loop until the user chooses option 3.
        }
	}while(input_number != 3);
	
	/* Exit the program and return 0. */
	return 0; 
}
	