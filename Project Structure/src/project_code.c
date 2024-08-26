#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include <conio.h>

#define vertices 35
#define MAX_ROWS 1000
#define MAX_COLS 6
#define MAX_LENGTH 50

float distance[vertices];
    int path[vertices];
    int visited[vertices];
    int count=0;

typedef struct node       // structure to represent node in graph
{
    int src;
    char src_name[30];
    int dest;
    char dest_name[30];
    float dist;
    int police_station;
    struct node *next;
}NODE;

typedef struct list     // structure to represent adjacency list for vertex
{
    struct node *head;
}ADJACENT_LIST;

typedef struct graph    // structure to represent graph
{
    int no_vertices;
    ADJACENT_LIST *arr;
}GRAPH;


//structure to store all the police stations at min distance from crime_node
typedef struct Mindistanceinfo
{
    int node_number;
    char node_name[30];
    float min_distance;
} Mindistanceinfo;


typedef struct 
{
    char id[MAX_LENGTH];
    char name[MAX_LENGTH];
    int age;
    char gender[MAX_LENGTH];
    char crimeDetails[MAX_LENGTH];
    char crimeDate[MAX_LENGTH];
} CriminalRecord;


NODE *create_node(int src, char src_name[], int dest, char dest_name[], float dist, int police_station);
GRAPH *create_graph(int no_vertices);
void insert_edge(GRAPH *graph, int src, char src_name[], int dest, char dest_name[], float dist, int police_station);
char *yes_no(int p);
void display_graph(GRAPH *graph);
void display_police_station(GRAPH *graph);
void Dijkstra(GRAPH *graph, int crime_node);
void store_mindistances(GRAPH *graph, Mindistanceinfo mindistances[]);
void swap(Mindistanceinfo *a, Mindistanceinfo *b);
int Partision(Mindistanceinfo arr[], int low, int high);
void quicksort(Mindistanceinfo arr[], int l, int r);
void display_sorted_mindistances(Mindistanceinfo mindistances[], int num_vertices);
void getPassword(char *password, int max_length);
int validatePassword();
void computeLPSArray(char *pattern, int patternLength, int *lps);
int KMPSearch(char *pattern, char *text);
char *criminal_file_name(char filename[]);
int searchInPoliceFile(char *filename, char *pattern);
char *retrieve_name_of_criminal_file_for_inserting(char ps_name[]);
int isEntryExist(char *filename, char *searchEntry);
int insert_into_criminal_file(GRAPH *graph);
void project_name();
int crimenode(GRAPH *graph, char crime_node_name[]);
char *retrieve_name_of_police_emp_file(char ps_name[]);
char *retrieve_contact_person_police_id(char ps_name[]);
int search_in_police_emp_file();
int search_incharge();
void public_menu(GRAPH *graph, Mindistanceinfo mindistances[]);
void police_menu(GRAPH *graph, Mindistanceinfo mindistances[]);


//Function to create newnode with distance
NODE *create_node(int src, char src_name[], int dest, char dest_name[], float dist, int police_station)
{
    NODE *newnode;
    newnode = (NODE*)malloc(sizeof(NODE));
    newnode->src=src;
    strcpy(newnode->src_name,src_name);
    newnode->dest=dest;
    strcpy(newnode->dest_name,dest_name);
    newnode->dist = dist;
    newnode->police_station = police_station;
    newnode->next = NULL;
    return newnode;
}

// Function to create graph of 35 vertices
GRAPH *create_graph(int no_vertices)
{
    GRAPH *graph;
    graph = (GRAPH *)malloc(sizeof(GRAPH));
    graph->no_vertices = no_vertices;

    //Create an array for holding the starting address of every adjacent list
    graph->arr = (ADJACENT_LIST*)malloc(no_vertices * sizeof(ADJACENT_LIST));

    for(int i=0; i<no_vertices; i++)
        graph->arr[i].head = NULL;

    return graph;
}




void insert_edge(GRAPH *graph, int src, char src_name[], int dest, char dest_name[], float dist, int police_station)
{
    // insert edge from source to dest with distance
    NODE* newnode;
    newnode = create_node(src,src_name,dest,dest_name, dist, police_station);
    newnode->next = graph->arr[src].head;
    graph->arr[src].head = newnode;


    // In undirected graph, there should be adge from dest to source also (here dest becomes the source and source as destination)
    // If destination is a police station then we pass police_staion as 1
    if(dest == 5 || dest == 12 || dest == 14 || dest == 19 || dest == 20 || dest == 21 || dest == 24 || dest == 25 || dest == 27 || dest == 28 || dest == 33)
    {
        newnode = create_node(dest,dest_name,src,src_name, dist, 1);
    }

    // If source is a police station then we pass police_staion as 0
    else if(src == 5 || src == 12 || src == 14 || src == 19 || src == 20 || src == 21 || src == 24 || src == 25 || src == 27 || src == 28 || src == 33)
    {
        newnode = create_node(dest,dest_name,src,src_name, dist, 0);
    }

    else
    {
        newnode = create_node(dest,dest_name,src,src_name, dist, 0);
    }

    newnode->next = graph->arr[dest].head;
    graph->arr[dest].head = newnode;

}


char *yes_no(int p) 
{
    static char s[5];
    if (p) 
    {
        strcpy(s, "Yes");
    } 
    else 
    {
        strcpy(s, "No");
    }
    return s;
}

void display_graph(GRAPH *graph) 
{
    // Display column headers
    printf("Location            | PS | Destination        \n");
    printf("--------------------|----|---------------------\n");

    for (int i = 0; i < graph->no_vertices; i++) 
    {
        NODE *temp = graph->arr[i].head;

        printf("\n%-19s | %-3s|", temp->src_name, yes_no(temp->police_station));

        while (temp != NULL) 
        {
            printf("%s(%.2f)", temp->dest_name, temp->dist);
            temp = temp->next;

            if (temp != NULL) 
            {
                printf(", ");
            }
        }
        printf("\n");
    }
}



// Function to display on police staions
void display_police_station(GRAPH *graph)
{
    int i;
    printf("\nThe police stations are : ");
    for(i=0; i<graph->no_vertices; i++)
    {
        NODE *temp = graph->arr[i].head;
        while(temp!=NULL)
        {
            if(temp->police_station)
            {
                printf("\n%d : %s",i,temp->src_name);
                break;
            }
            temp=temp->next;
        }
    }
    printf("\n\n");
}


void Dijkstra(GRAPH *graph, int crime_node)
{



    int selected_vertex = -1;
    for(int v=0; v < graph->no_vertices; v++)
    {
        distance[v] = INT_MAX;
        path[v] = -1;
        visited[v] = 0;
    }
    distance[crime_node] = 0;

    for(int i=0; i < graph->no_vertices; i++)
    {
        int min = INT_MAX;
        for(int v=0; v < graph->no_vertices; v++)
        {
            if(distance[v] < min && visited[v] == 0)
            {
                min = distance[v];
                selected_vertex = v;
            }
        }

        visited[selected_vertex] = 1;

        NODE *temp = graph->arr[selected_vertex].head;
        while (temp != NULL)
        {
            if((distance[selected_vertex] + temp->dist < distance[temp->dest]) && (selected_vertex != temp->dest) && visited[temp->dest] ==0 )
            {
                distance[temp->dest] = distance[selected_vertex] + temp->dist;
                path[temp->dest] = selected_vertex;
            }
            temp = temp->next;
        }
    }

    // Find the nearest police station and print the shortest path
    int nearest_police_station = -1;
    float min_distance_to_police_station = INT_MAX;

    for (int i = 0; i < graph->no_vertices; i++) 
    {
        if (distance[i] < min_distance_to_police_station && graph->arr[i].head->police_station) 
        {
            min_distance_to_police_station = distance[i];
            nearest_police_station = i;
        }
    }

    // Print the shortest path from the crime node to the nearest police station
    if (nearest_police_station != -1) 
    {
        printf("Shortest path to the nearest police station from crime node %d is: ", crime_node);
        int current_node = nearest_police_station;

        while (current_node != crime_node) 
        {
            printf("%d(%s) <- ", current_node, graph->arr[current_node].head->src_name);
            current_node = path[current_node];
        }

        printf("%d(%s)\n", crime_node, graph->arr[crime_node].head->src_name);
        printf("Nearest police station to the crime node %d is %d : %s\n", crime_node, nearest_police_station, graph->arr[nearest_police_station].head->src_name);
        printf("Min distance is : %.2f",min_distance_to_police_station);
    }
    
    else 
    {
        printf("There is no police station connected to the graph.\n");
    }
    printf("\n\n");
}


void store_mindistances(GRAPH *graph, Mindistanceinfo mindistances[]) 
{

    for (int i = 0; i < graph->no_vertices; i++) 
    {
        if (graph->arr[i].head->police_station) 
        {
            mindistances[count].node_number = i;
            strcpy(mindistances[count].node_name, graph->arr[i].head->src_name);
            mindistances[count].min_distance = distance[i];
            count++;
        }
    }
}


// Function to swap two Mindistanceinfo structs
void swap(Mindistanceinfo *a, Mindistanceinfo *b)
{
    Mindistanceinfo temp = *a;
    *a = *b;
    *b = temp;
}

// Partision function for quicksort
int Partision(Mindistanceinfo arr[], int low, int high) 
{
    float pivot = arr[low].min_distance;
    int i = low + 1;
    int j = high;

    do {
        while (i <= high && arr[i].min_distance <= pivot) 
        {
            i++;
        }

        while (arr[j].min_distance > pivot) 
        {
            j--;
        }

        if (i < j)
        {
            swap(&arr[i], &arr[j]);
        }

    } while (i <= j);

    swap(&arr[low], &arr[j]);
    return j;
}


void quicksort(Mindistanceinfo arr[], int l, int r)
{
    if(l < r)
    {
        int s = Partision(arr, l, r);
        quicksort(arr, l, s-1);
        quicksort(arr, s+1, r);
    }
}


void display_sorted_mindistances(Mindistanceinfo mindistances[], int num_vertices)
{
    printf("Number | Police Staion           | Min Distance\n" );
    printf("-----------------------------------------------\n");
    for (int i = 0; i < num_vertices; i++)
    {
        printf("%-7d | %-26s | %.2f\n", mindistances[i].node_number, mindistances[i].node_name, mindistances[i].min_distance);
    }
}

//Function to hide password
void getPassword(char *password, int max_length) 
{
    int i = 0;
    char ch;

    while (1) 
    {
        ch = getch(); // Get a single character without displaying it
        
        if (ch == 13 || ch == 10) 
        { // Enter key
            password[i] = '\0';
            break;
        } 

        else if (ch == 8 && i > 0) 
        { // Backspace key
            i--;
            printf("\b \b"); // Move the cursor back and erase the character
        }

         
        else if (i < max_length - 1) 
        {
            password[i] = ch;
            i++;
            printf("*");
        }

    }
    printf("\n");
}

// Function to validate password
int validatePassword()
{
    int max_length=20;
    char password[max_length];

    printf("Please enter the password: ");
    getPassword(password,max_length);

    if (strcmp(password, "Police@2023") == 0) 
    {
        return 1; // Password accepted
    } 
    
    else 
    {
        system("cls");
        printf("Incorrect password. Access denied!\n");
        return 0; // Password denied
    }
}



// Preprocess pattern for KMP algorithm
void computeLPSArray(char *pattern, int patternLength, int *lps) 
{
    int len = 0;
    lps[0] = 0;
    int i = 1;

    while (i < patternLength) 
    {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } 
        
        else 
        {
            if (len != 0) 
            {
                len = lps[len - 1];
            } 
            
            else 
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// KMP pattern matching algorithm
int KMPSearch(char *pattern, char *text) 
{
    int patternLength = strlen(pattern);
    int textLength = strlen(text);

    int *lps = (int *)malloc(patternLength * sizeof(int));
    if (lps == NULL) 
    {
        return -1;
    }

    computeLPSArray(pattern, patternLength, lps);

    int i = 0; // index for text[]
    int j = 0; // index for pattern[]

    while (i < textLength) 
    {
        
        if (pattern[j] == text[i]) 
        {
            j++;
            i++;
        }

        if (j == patternLength) 
        {
            free(lps);
            return i - j; // pattern found at index i - j
        } 
        
        else if (i < textLength && pattern[j] != text[i]) 
        {
            if (j != 0) 
            {
                j = lps[j - 1];
            } 
            
            else 
            {
                i++;
            }
        }
    }

    free(lps);
    return -1; // pattern not found
}


char *criminal_file_name(char filename[])
{
    if(strcmp(filename,"Policestation1.csv")==0)
    {
        strcpy(filename,"Udyambagh");
    }
    else if(strcmp(filename,"Policestation2.csv")==0)
    {
        strcpy(filename,"Tilakwadi");
    }
    else if(strcmp(filename,"Policestation3.csv")==0)
    {
        strcpy(filename,"Shahapur");
    }
    else if(strcmp(filename,"Policestation4.csv")==0)
    {
        strcpy(filename,"CBT");
    }
    else if(strcmp(filename,"Policestation5.csv")==0)
    {
        strcpy(filename,"Khade Bazar");
    }
    else if(strcmp(filename,"Policestation6.csv")==0)
    {
        strcpy(filename,"CAMP");
    }
    else if(strcmp(filename,"Policestation7.csv")==0)
    {
        strcpy(filename,"Malmaruti");
    }
    else if(strcmp(filename,"Policestation8.csv")==0)
    {
        strcpy(filename,"North");
    }
    else if(strcmp(filename,"Policestation9.csv")==0)
    {
        strcpy(filename,"Market");
    }
    else if(strcmp(filename,"Policestation10.csv")==0)
    {
        strcpy(filename,"APMC");
    }
    else if(strcmp(filename,"Policestation11.csv")==0)
    {
        strcpy(filename,"Kakati");
    }
    return filename;
}

// Function to search for a pattern (name, age) in a police station file
int searchInPoliceFile(char *filename, char *pattern) 
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        printf("Failed to open %s\n", criminal_file_name(filename));
        return -1;
    }

    char line[MAX_LENGTH * 6];

    while (fgets(line, sizeof(line), file)) 
    {
        int found = KMPSearch(pattern, line);

        if (found != -1) 
        {
            printf("Criminal found in %s Police Station\n%s\n", criminal_file_name(filename), line);
            fclose(file);

            return 1;
        }
    }

    
    fclose(file);
    return 0;
}


char *retrieve_name_of_criminal_file_for_inserting(char ps_name[])
{
    if(stricmp(ps_name, "Udyambagh")==0)
    {
        return "Policestation1.csv";
    }
    else if(stricmp(ps_name, "Tilakwadi")==0)
    {
        return "Policestation2.csv";
    }
    else if(stricmp(ps_name, "Shahapur")==0)
    {
        return "Policestation3.csv";
    }
    else if(stricmp(ps_name, "CBT")==0)
    {
        return "Policestation4.csv";
    }
    else if(stricmp(ps_name, "Khade Bazar")==0)
    {
        return "Policestation5.csv";
    }
    else if(stricmp(ps_name, "CAMP")==0)
    {
        return "Policestation6.csv";
    }
    else if(stricmp(ps_name, "Malmaruti Police Station")==0)
    {
        return "Policestation7.csv";
    }
    else if(stricmp(ps_name, "North PS Belgaum")==0)
    {
        return "Policestation8.csv";
    }
    else if(stricmp(ps_name, "Market PS Belgaum")==0)
    {
        return "Policestation9.csv";
    }
    else if(stricmp(ps_name, "APMC")==0)
    {
        return "Policestation10.csv";
    }
    else if(stricmp(ps_name, "Kakati")==0)
    {
        return "Policestation11.csv";
    }
    else
    {
        printf("\nPlease enter the correct police station name");
        return NULL;
    }
}


int isEntryExist(char *filename, char *searchEntry) 
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) 
    {
        perror("Error opening file");
        printf("Failed to open %s\n", filename);
        return -1;  
    }

    char line[100];  

    while (fgets(line, sizeof(line), file) != NULL) 
    {
        // Compare the current line with the search entry
        if (strstr(line, searchEntry) != NULL) 
        {
            fclose(file);
            return 1;  // Entry exists
        }
    }

    fclose(file);
    return 0;  // Entry not found
}


int insert_into_criminal_file(GRAPH *graph)
{
    
    char ps_name[30];
    char criminal_id[10];
    char name[30];
    char age[5];
    char gender;
    char crime_details[20];
    char crime_date[15];
    int flag=0;

    display_police_station(graph);  //Displays police staions

    do{
            printf("\nEnter the Police Station name where a criminal is arrested : ");
            fflush(stdin);
            fgets(ps_name, sizeof(ps_name), stdin);
            ps_name[strcspn(ps_name, "\n")] = '\0';  // Remove the newline character

            printf("\nEnter the details ");

            printf("\nEnter id :");
            fflush(stdin);
            fgets(criminal_id, sizeof(criminal_id), stdin);
            criminal_id[strcspn(criminal_id, "\n")] = '\0';  // Remove the newline character

            printf("Criminal name : ");
            fflush(stdin);
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';  // Remove the newline character
            
            printf("Criminal age : ");
            fflush(stdin);
            fgets(age, sizeof(age), stdin);
            age[strcspn(age, "\n")] = '\0';  // Remove the newline character
            
            printf("Gender : ");
            scanf(" %c", &gender);
        
            printf("Crime details : ");
            fflush(stdin);
            fgets(crime_details, sizeof(crime_details), stdin);
            crime_details[strcspn(crime_details, "\n")] = '\0';  // Remove the newline character
            
            printf("Crime date : ");
            fflush(stdin);
            fgets(crime_date, sizeof(crime_date), stdin);
            crime_date[strcspn(crime_date, "\n")] = '\0';  // Remove the newline character
           


    char *filename = retrieve_name_of_criminal_file_for_inserting(ps_name);
    FILE *file;

    if (filename != NULL) 
    {

        char searchEntry[100];  
        sprintf(searchEntry, "%s,%s,%s,%c,%s,%s", criminal_id, name, age, gender, crime_details, crime_date);
        
        if (isEntryExist(filename, searchEntry)) 
        {
            printf("\n\nCriminal already exists in the %s Police station's file.",ps_name);
            // Handle the case where the entry already exists
            return 0;
        } 
        
        else 
        {
            file = fopen(filename, "a+");

            if (file == NULL) 
            {
                perror("Error opening file");
                printf("Failed to open %s\n", filename);
                return -1;
            }
        }
        



    fprintf(file, "%s,%s,%s,%c,%s,%s\n", criminal_id,name, age, gender,crime_details,crime_date);

    printf("\nCriminal inserted succesfully");
    fclose(file);
    }

    else
    {
        flag=1;
    }
    }while(flag);

    return 1;

}

void project_name()
{
    printf(" _    _ _____ _     _____ ________  ___ _____     _____ _____ \n");
    printf("| |  | |  ___| |   /  __ \\  _  |  \\/  ||  ___|   |_   _|  _  |\n");
    printf("| |  | | |__ | |   | /  \\/ | | | .  . || |__       | | | | | |\n");
    printf("| |\\/| |  __|| |   | |   | | | | |\\/| ||  __|      | | | | | |\n");
    printf("| |  | | |___| |___| \\__/\\ \\_/ / |  | || |___      | | \\ \\_/ /\n");
    printf("\\/   \\/\\____/\\_____/\\____/\\___/\\_|  |_/\\____/      \\_/  \\___/ \n");
    printf("                                                               \n");
    printf("                                                               \n");
    printf("    _____ ______ ________  ___ _____ _   _  _____ _____       \n");
    printf("   /  __ \\| ___ \\_   _|  \\/  ||  ___| \\ | ||  ___|_   _|      \n");
    printf("   | /  \\/| |_/ / | | | .  . || |__ |  \\| || |__   | |        \n");
    printf("   | |    |    /  | | | |\\/| ||  __|| . ` ||  __|  | |        \n");
    printf("   | \\__/\\| |\\ \\ _| |_| |  | || |___| |\\  || |___  | |        \n");
    printf("   \\_____/\\_| \\_\\____/\\_|  |_/\\____/\\_| \\_/\\____/  \\_/        \n");

}


int crimenode(GRAPH *graph, char crime_node_name[])
{
    int i;
    NODE *temp;
    for(i=0; i<graph->no_vertices; i++)
    {
        temp = graph->arr[i].head;
        if(stricmp(crime_node_name, temp->src_name) == 0)
        {
            return i;
        }
    }
    return -1;
    
}


char *retrieve_name_of_police_emp_file(char ps_name[])
{
    if(stricmp(ps_name, "Udyambagh")==0)
    {
        return "Policeinfo1.csv";
    }
    else if(stricmp(ps_name, "Tilakwadi")==0)
    {
        return "Policeinfo2.csv";
    }
    else if(stricmp(ps_name, "Shahapur")==0)
    {
        return "Policeinfo3.csv";
    }
    else if(stricmp(ps_name, "CBT")==0)
    {
        return "Policeinfo4.csv";
    }
    else if(stricmp(ps_name, "Khade Bazar")==0)
    {
        return "Policeinfo5.csv";
    }
    else if(stricmp(ps_name, "CAMP")==0)
    {
        return "Policeinfo6.csv";
    }
    else if(stricmp(ps_name, "Malmaruti Police Station")==0)
    {
        return "Policeinfo7.csv";
    }
    else if(stricmp(ps_name, "North PS Belgaum")==0)
    {
        return "Policeinfo8.csv";
    }
    else if(stricmp(ps_name, "Market PS Belgaum")==0)
    {
        return "Policeinfo9.csv";
    }
    else if(stricmp(ps_name, "APMC")==0)
    {
        return "Policeinfo10.csv";
    }
    else if(stricmp(ps_name, "Kakati")==0)
    {
        return "Policeinfo11.csv";
    }
    else
    {
        printf("\nPlease enter the correct police station name");
        return NULL;
    }
}


char *retrieve_contact_person_police_id(char ps_name[])
{
    if(stricmp(ps_name, "Udyambagh")==0)
    {
        return "UDM9";
    }
    else if(stricmp(ps_name, "Tilakwadi")==0)
    {
        return "TIL9";
    }
    else if(stricmp(ps_name, "Shahapur")==0)
    {
        return "SHP9";
    }
    else if(stricmp(ps_name, "CBT")==0)
    {
        return "CBT9";
    }
    else if(stricmp(ps_name, "Khade Bazar")==0)
    {
        return "KB9";
    }
    else if(stricmp(ps_name, "CAMP")==0)
    {
        return "CAMP9";
    }
    else if(stricmp(ps_name, "Malmaruti Police Station")==0)
    {
        return "MPS9";
    }
    else if(stricmp(ps_name, "North PS Belgaum")==0)
    {
        return "NSP9";
    }
    else if(stricmp(ps_name, "Market PS Belgaum")==0)
    {
        return "MRK9";
    }
    else if(stricmp(ps_name, "APMC")==0)
    {
        return "APMC9";
    }
    else if(stricmp(ps_name, "Kakati")==0)
    {
        return "KAK9";
    }
}


int search_in_police_emp_file()
{
    char ps_name[30];

    printf("Enter the Police Station name : ");
    fflush(stdin);
    gets(ps_name);

    char *filename = retrieve_name_of_police_emp_file(ps_name);
    FILE *file;

    
    char line[100];      
    char *contact_person_police_id = retrieve_contact_person_police_id(ps_name);

    
        file = fopen(filename, "r");



        while (fgets(line, sizeof(line), file) != NULL) 
        {
            
            if (strcmp(strtok(line, ","), contact_person_police_id) == 0) 
            {
                char *policeName = strtok(NULL, ",");
                if (policeName != NULL) {

                    system("cls");
                    printf("\nYou should contact %s", policeName);

                    return 0;
                }
            }
        }

    

    //If entered police station is not correct
    return 1;
}


//Structure to store the police employees information
typedef struct 
{
    char police_id[10];
    char name[30];
    char rank[30];
    char salary[10];
    
} PoliceEmployee;


int search_incharge()
{
    char ps_name[30];
    

    printf("Enter the Police Station name : ");
    fflush(stdin);
    gets(ps_name);

    char *filename = retrieve_name_of_police_emp_file(ps_name);
    FILE *file;

    if (filename == NULL) {
        return 1;
    }

    file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        printf("Failed to open %s\n", filename);
        return -1;
    }

    char line[MAX_LENGTH * 6];
    fgets(line, sizeof(line), file);  // the first line contains column headers
    
    int i=0,n;
    PoliceEmployee employee[100];
    while (fgets(line, sizeof(line), file)) 
    {

        sscanf(line, "%9[^,],%29[^,],%29[^,],%9[^,]", employee[i].police_id, employee[i].name, employee[i].rank, employee[i].salary);
        n=i;
        i++;
    }

    for(i=0; i<n; i++)
    {
        if(stricmp(employee[i].rank, "Sub-Inspector") == 0)
        {
            system("cls");
            printf("\nThe Incharge(Sub-Inspector) is %s",employee[i].name);
            return 0;
        }
    }

    

}


void public_menu(GRAPH *graph, Mindistanceinfo mindistances[])
{
    int choice=0,crime_node,want,i=0,flag;
    system("cls");
    project_name();
    NODE *temp;
    char crime_node_name[30];
    

    do
    {
        printf("\n1.Display Belgaum City ( In Table Format ) \n2.Display All the Police Stations of the Belgaum");
        printf("\n3.Find nearest police station in the Belgaum city from crime spot by refferring above city map");
        printf("\n4.Whom to contact to case a file in particular Police station");
        printf("\n5.To Know the incharge of particular Police Station");
        printf("\n6.Go Back");
        printf("\nEnter your choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1 : 

            system("cls");
            display_graph(graph);  //Displays the graph
            break;

            case 2 : 
            system("cls");
            display_police_station(graph);  //Displays police staions
            break;

            case 3 :
            system("cls");
            printf("\nThe locations are : \n");
            for (i = 0; i < graph->no_vertices; i++)
            {
                temp = graph->arr[i].head;
                printf("%-20s", temp->src_name); 

                // Add a newline after every 5 vertices
                if ((i + 1) % 5 == 0)
                {
                    printf("\n");
                }

            }  

            do{

            printf("\nEnter the crime location : ");

            fflush(stdin);

            gets(crime_node_name);
            crime_node = crimenode(graph, crime_node_name);

            if(crime_node == -1)
            {
                printf("\nEnter the correct location");
            }

            }while(crime_node == -1);

            Dijkstra(graph, crime_node);

            printf("To display shortest distance from crime node to all police stations in sorted manner w.r.t minimum distances(Press 1 for Yes and 0 for No) : ");
            scanf("%d",&want);
            
            system("cls");

            if(want)
            {
                //Storing minimum distances and related information into the structure array
                store_mindistances(graph, mindistances);

                //Sorting the stored minimum distances using quicksort
                quicksort(mindistances, 0, count-1);

                // Displaying the sorted minimum distances
                display_sorted_mindistances(mindistances, count);
                count=0;
            }
            break;

            case 4 :

            system("cls");
            display_police_station(graph);  //Displays police staions

            do
            {

                flag=search_in_police_emp_file();
                
                printf("\n\n");

            }while(flag);

            break;


            case 5 :

            system("cls");
            display_police_station(graph);  //Displays police staions

            do
            {
                flag = search_incharge();
                printf("\n\n");

            }while(flag);

            break;


            case 6 : 
            system("cls");
            return;
        }
    }while(choice != 6);
            
            
}



void police_menu(GRAPH *graph, Mindistanceinfo mindistances[])
{
    int choice=0,crime_node,want,i,flag;
    char name[30];
    char age[5];
    char pattern[MAX_LENGTH * 2];
    char filename[MAX_LENGTH];
    NODE *temp;
    char crime_node_name[30];

    FILE *file;
    system("cls");
    project_name();

    do
    {
        printf("\n1.Display Belgaum City ( In Table Format ) \n2.Display All the Police Stations of the Belgaum");
        printf("\n3.Find nearest police station in the Belgaum city from crime spot by refferring above city map");
        printf("\n4.Search a criminal (is it available or not in database)");
        printf("\n5.Enter thr criminal data in database of particular Police Station");
        printf("\n6.To Know the incharge of particular Police Station");
        printf("\n7.Go Back");
        printf("\nEnter your choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1 : 
            system("cls");
            display_graph(graph);  //Displays the graph
            break;

            case 2 : 
            system("cls");
            display_police_station(graph);  //Displays police staions
            break;

            case 3 :
            system("cls");
            printf("\nThe locations are : \n");

            for (i = 0; i < graph->no_vertices; i++)
            {
                temp = graph->arr[i].head;
                printf("%-20s", temp->src_name); 

                // Add a newline after every 5 vertices
                if ((i + 1) % 5 == 0)
                    printf("\n");
            }    

            do{

            printf("\nEnter the crime location : ");

            fflush(stdin);

            gets(crime_node_name);
            crime_node = crimenode(graph, crime_node_name);

            if(crime_node == -1)
            {
                printf("\nEnter the correct location");
            }

            }while(crime_node == -1);


            Dijkstra(graph, crime_node);

            printf("To display shortest distance from crime node to all police stations in sorted manner w.r.t minimum distances(Press 1 for Yes and 0 for No) : ");
            scanf("%d",&want);

            system("cls");

            if(want)
            {
                //Storing minimum distances and related information into the structure array
                store_mindistances(graph, mindistances);

                //Sorting the stored minimum distances using quicksort
                quicksort(mindistances, 0, count-1);

                // Displaying the sorted minimum distances
                display_sorted_mindistances(mindistances, count);
                count=0;
            }
            break;


            case 4:
                
                system("cls");
                project_name();

                do{
                    fflush(stdin);
                    printf("\nEnter the name of criminal : ");
                    gets(name);

                    fflush(stdin);
                    printf("Enter the age of criminal : ");
                    gets(age);

                    fflush(stdin);
                    // Creating the pattern "name, age"
                    snprintf(pattern, sizeof(pattern), "%s,%s", name, age);
                    i=1;
                    do{
                        sprintf(filename, "Policestation%d.csv",i);

                        if(searchInPoliceFile(filename, pattern))
                        {
                            break;
                        }

                        i++;
                    }while(i<=11);

                    if(i>11)
                    {
                        printf("\nNot found in any of the police stations\n");
                    }

                printf("\nPress 1 to search again or 0 to exit : ");
                scanf("%d",&want);
                system("cls");
                project_name();

                }while(want);

                break;


            case 5 :
            system("cls");

            do{

            flag=insert_into_criminal_file(graph);
            printf("\n\n");

            }while(!flag);

            break;


            case 6 :

            system("cls");
            display_police_station(graph);  //Displays police staions

            do
            {
                flag = search_incharge();
                printf("\n\n");

            }while(flag);

            break;



            case 7 : 
            system("cls");
            return;
        }

    }while(choice != 7);
            
            
}


int main()
{
    int no_vertices = 35;                // Number of vertices

    GRAPH *graph = create_graph(no_vertices);     // Creates a graph
    Mindistanceinfo mindistances[vertices];

    // Inserting the edges
    insert_edge(graph, 0, "VTU", 1, "Machhe", 4, 0);
    insert_edge(graph, 1, "Machhe", 2, "Peeranwadi", 1, 0);
    insert_edge(graph, 2, "Peeranwadi", 3, "Brahma nagar", 1, 0);
    insert_edge(graph, 2, "Peeranwadi", 4, "Majgaon", 2.5, 0);
    insert_edge(graph, 2, "Peeranwadi", 5, "Udyambagh", 3, 0);
    insert_edge(graph, 2, "Peeranwadi", 6, "Khadarwadi", 3, 0);
    insert_edge(graph, 3, "Brahma nagar", 4, "Majgaon", 1, 0);
    insert_edge(graph, 4, "Majgaon", 5, "Udyambagh", 2, 0);
    insert_edge(graph, 5, "Udyambagh", 6, "Khadarwadi", 2.6, 1);
    insert_edge(graph, 5, "Udyambagh", 7, "Angol", 1.7, 1);
    insert_edge(graph, 5, "Udyambagh", 8, "Third gate", 1.6, 1);
    insert_edge(graph, 5, "Udyambagh", 9, "Rani Chennamma nagar", 1.8, 1);
    insert_edge(graph, 5, "Udyambagh", 10, "Parwati nagar", 1.6, 1);
    insert_edge(graph, 8, "Third gate", 11, "Hindwadi", 2.3, 0);
    insert_edge(graph, 8, "Third gate", 12, "Tilakwadi", 2, 0);
    insert_edge(graph, 11, "Hindwadi", 13, "Vadagaon", 2, 0);
    insert_edge(graph, 11, "Hindwadi", 14, "Shahapur", 2, 0);
    insert_edge(graph, 12, "Tilakwadi", 11, "Hindwadi", 1.6, 1);
    insert_edge(graph, 12, "Tilakwadi", 14, "Shahapur", 2, 1);
    insert_edge(graph, 12, "Tilakwadi", 15, "RLY Station", 2.3, 1);
    insert_edge(graph, 12, "Tilakwadi", 16, "Nanawadi", 3, 1);
    insert_edge(graph, 13, "Vadagaon", 17, "Khasbag", 1, 0);
    insert_edge(graph, 13, "Vadagaon", 14, "Shahapur", 2.5, 0);
    insert_edge(graph, 14, "Shahapur", 17, "Khasbag", 1.8, 1);
    insert_edge(graph, 14, "Shahapur", 18, "Belgaum Fort", 2.4, 1);
    insert_edge(graph, 14, "Shahapur", 19, "CBT", 2.8, 1);
    insert_edge(graph, 14, "Shahapur", 15, "RLY Station", 2, 1);
    insert_edge(graph, 15, "RLY Station", 19, "CBT", 2.2, 0);
    insert_edge(graph, 15, "RLY Station", 20, "Khade Bazar", 2.2, 0);
    insert_edge(graph, 15, "RLY Station", 21, "CAMP", 1.7, 0);
    insert_edge(graph, 16, "Nanawadi", 21, "CAMP", 2, 0);
    insert_edge(graph, 16, "Nanawadi", 22, "Ganeshpur", 8, 0);
    insert_edge(graph, 17, "Khasbag", 18, "Belgaum Fort", 3, 0);
    insert_edge(graph, 18, "Belgaum Fort", 23, "Gandhi Nagar", 1.8, 0);
    insert_edge(graph, 18, "Belgaum Fort", 19, "CBT", 0.8, 0);
    insert_edge(graph, 19, "CBT", 24, "Malmaruti Police Station", 3.1, 1);
    insert_edge(graph, 19, "CBT", 25, "North PS Belgaum", 1.6, 1);
    insert_edge(graph, 19, "CBT", 20, "Khade Bazar", 0.75, 1);
    insert_edge(graph, 20, "Khade Bazar", 25, "North PS Belgaum", 1, 1);
    insert_edge(graph, 20, "Khade Bazar", 26, "Ayodhya Nagar", 1.5, 1);
    insert_edge(graph, 20, "Khade Bazar", 27, "Market PS Belgaum", 0.5, 1);
    insert_edge(graph, 20, "Khade Bazar", 28, "APMC", 3.7, 1);
    insert_edge(graph, 20, "Khade Bazar", 21, "CAMP", 1.7, 1);
    insert_edge(graph, 21, "CAMP", 29, "Hanuman Nagar", 3.7, 1);
    insert_edge(graph, 21, "CAMP", 30, "Hindalga", 4, 1);
    insert_edge(graph, 22, "Ganeshpur", 30, "Hindalga", 1.3, 0);
    insert_edge(graph, 23, "Gandhi Nagar", 24, "Malmaruti Police Station", 3.2, 0);
    insert_edge(graph, 24, "Malmaruti Police Station", 25, "North PS Belgaum", 3, 1);
    insert_edge(graph, 24, "Malmaruti Police Station", 26, "Ayodhya Nagar", 2.3, 1);
    insert_edge(graph, 24, "Malmaruti Police Station", 31, "Nehru Nagar", 1.8, 1);
    insert_edge(graph, 25, "North PS Belgaum", 26, "Ayodhya Nagar", 0.5, 1);
    insert_edge(graph, 26, "Ayodhya Nagar", 31, "Nehru Nagar", 2, 0);
    insert_edge(graph, 26, "Ayodhya Nagar", 28, "APMC", 2.2, 0);
    insert_edge(graph, 27, "Market PS Belgaum", 28, "APMC", 3.8, 1);
    insert_edge(graph, 28, "APMC", 32, "Kangrali", 4, 1);
    insert_edge(graph, 28, "APMC", 29, "Hanuman Nagar", 4, 1);
    insert_edge(graph, 29, "Hanuman Nagar", 30, "Hindalga", 2, 0);
    insert_edge(graph, 31, "Nehru Nagar", 33, "Kakati", 5.7, 0);
    insert_edge(graph, 31, "Nehru Nagar", 32, "Kangrali", 4, 0);
    insert_edge(graph, 32, "Kangrali", 33, "Kakati", 4, 0);
    insert_edge(graph, 33, "Kakati", 34, "Honaga", 3.5, 1);


    // MENU
    int main_choice;
    int choice=0,crime_node,want,flag,i;
    char name[30];
    char age[5];
    char pattern[MAX_LENGTH * 2];
    char filename[MAX_LENGTH];
    do
    {
        project_name();

        printf("\nEnter your course of action to use the CRIMENET based on the given below options");
        printf("\n1.PUBLIC \n2.POLICE\n3.EXIT\n");
        scanf("%d",&main_choice);

        switch(main_choice)
        {
            case 1 : 
            public_menu(graph,mindistances);
            break;

            case 2 :
            if (validatePassword())
            police_menu(graph,mindistances);
            break;

            case 3 :
            exit(0);
            break;
        }

    }while(main_choice != 3);

}

