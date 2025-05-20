#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum TYPE {S, I, R};

// TODO: Implement idx
// idx returns an integer to be used for hashing
// this integer should be unique for every x, y pair in your grid
int idx(int x, int y, int k)
{
    //unique index for each var
    int index = (2 * k + 1) * (x + k) + (y + k);
    return index;
}

typedef struct Host
{
    int id;
    int x, y;
    int t;
    enum TYPE type;
} THost;

typedef struct node_tag {
    THost host;
    struct node_tag * next;
} node;

// Create a node whose value is a specific host
// Return a pointer to the created node
node * create_node(THost host) 
{
    node *new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    new_node->host = host;
    new_node->next = NULL;
    return new_node;
}

// add_first() should add to the beginning of a linked list
// Note the type: 'node **head'
// Note that it does not return a value 
void add_first(node **head, node *newnode)
{
    newnode->next = *head; //new node point to current head
    *head = newnode;       //head to the new node
}

// Remove the first node from the list
// Note the type: 'node **head'
// Return a pointer to the removed content
node * remove_first(node **head) 
{
    if (*head == NULL)
    {
        return NULL; //lsit empty
    }
    node *removed_node = *head;   //store node
    *head = (*head)->next;        //head to the next node
    removed_node->next = NULL;
    return removed_node;
}

//remove all the nodes in the list
void remove_all(node **head)
{
    node *current = *head;
    node *next_node;
    while (current != NULL)
    {
        next_node = current->next; //save next node
        free(current);             //free node
        current = next_node;       //next node
    }
    *head = NULL; //head set to null
}

// location_match checks whether a linked list contains
// one or more hosts in the same location as 'host'
// Return 1 if there is a match, 0 if not
int location_match(node *head, THost host)
{
    node *current = head;
    while (current != NULL)
    {
        if (current->host.x == host.x && current->host.y == host.y)
        {
            return 1; // if match
        }
        current = current->next;
    }
    return 0; //no match
}

// Hash function included for your convenience :)
unsigned hash(unsigned a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

// summary prints out the proportions of different host types.
// It returns 1 if the number of infected hosts is not 0. 
int summary(THost hosts[], int m)
{   
    int S_n = 0, I_n = 0, R_n = 0; //counters for each one
    
    //to count each one
    int i;
    for(i = 0; i < m; i++)
    {   
        if (hosts[i].type == S)
            S_n++;
        else if (hosts[i].type == I)
            I_n++;
        else if (hosts[i].type == R)
            R_n++;
    }
    if(I_n == 0)
    {
        printf("    S        I        R\n");
        printf("%lf %lf %lf\n", (double)S_n/(S_n + I_n + R_n), 
               (double)I_n/(S_n + I_n + R_n), (double)R_n/(S_n + I_n + R_n));
    }
    return I_n > 0; //return if no infected
}

int one_round(THost *hosts, int m, node *p_arr[], int n_arr, int k, int T)
{
    int i;
    // S -> I and I -> R
    for(i = 0; i < m; i++)
    {
        if(hosts[i].type == S) //if host susceptible
        {
            //finish the following line of code
            int index = hash(idx(hosts[i].x, hosts[i].y, k)) % n_arr;
            //check if there is an infected host at the same location
            if(location_match(p_arr[index], hosts[i]))
            {
                //becomes infected
                hosts[i].type = I;
                hosts[i].t = 0;
            }
        }
        else if(hosts[i].type == I) //host is infected
        {
            hosts[i].t += 1;
            if(hosts[i].t == T)
            {
                hosts[i].type = R;
            }
        }
    }

    for(i = 0; i < n_arr; i++)
    {
        remove_all(&p_arr[i]);
    }

    for(i = 0; i < m; i++)
    {
        int r = rand() % 4; //rand dir
        switch(r)
        {
            case 0: //move up
                hosts[i].y = (hosts[i].y + k + 1) % (2*k+1) - k;
                break;
            case 1: //move right
                hosts[i].x = (hosts[i].x + k + 1) % (2*k+1) - k;
                break;
            case 2: //move down
                hosts[i].y = (hosts[i].y - k - 1) % (2*k+1) + k;
                break;
            case 3: //move left
                hosts[i].x = (hosts[i].x - k - 1) % (2*k+1) + k;
                break;
        }

        //build linked list for I hosts
        if(hosts[i].type == I)
        {
            node *new_node = create_node(hosts[i]);
            int index = hash(idx(hosts[i].x, hosts[i].y, k)) % n_arr;
            add_first(&(p_arr[index]), new_node);
        }
    }

    // Return 1 if there are still infected hosts
    return summary(hosts, m);
}

int main(int argc, char *argv[])
{

    if(argc != 5)
    {
        printf("Usage: %s k m T N\n", argv[0]);
        return 0;
    }

    int k = atoi(argv[1]);
    int m = atoi(argv[2]);
    int T = atoi(argv[3]);
    int N = atoi(argv[4]);

    assert(k >= 0 && k <= 1000);
    assert(m >= 1 && m <= 100000);
    assert(T >= 1);
    assert(N > 0 && N <= 100000);
    srand(12345);

    //initialize hosts
    THost hosts[m];

    hosts[0].id = 0;
    hosts[0].x = 0;
    hosts[0].y = 0;
    hosts[0].t = 0;
    hosts[0].type = I;

    for(int i = 1; i < m; i ++)
    {
        hosts[i].id = i;
        hosts[i].x = rand() % (2*k + 1) - k;
        hosts[i].y = rand() % (2*k + 1) - k;
        hosts[i].t = 0;
        hosts[i].type = S;		
    }

    //initialize linked lists
    node *p_arr[N];

    for(int i = 0; i < N; i++)
    {
        p_arr[i] = NULL;
    }
    node *r = create_node(hosts[0]);
    int index = hash(idx(hosts[0].x, hosts[0].y, k)) % N;
    add_first(&(p_arr[index]), r);

    //simulation
    while(one_round(hosts, m, p_arr, N, k, T));

    return 0;
}
