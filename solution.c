#include <stdio.h>
#include <stdlib.h>

// Define the Bridge structure representing a bridge between strands
typedef struct
{
    int distance;
    int targetStrand;
    int isNew;
} Bridge;

// Define the Strand structure representing a strand in the spider web
typedef struct
{
    int strandNumber;
    Bridge *bridges;
    int numBridges;
} Strand;

// Define the SpiderWeb structure representing the spider web
typedef struct
{
    int numStrands;
    int numBridges;
    Strand *strands;
} SpiderWeb;

// Define the Spider structure representing a spider in the spider web
typedef struct
{
    int distanceFromCenter;
    Strand *currentStrand, *favStrand;
} Spider;

// Function to swap two bridges
void swap(Bridge *a, Bridge *b)
{
    Bridge temp = *a;
    *a = *b;
    *b = temp;
}

// Function to perform bubble sort on bridges in ascending order of distance
void bubbleSortBridges(Bridge *bridges, int numBridges)
{
    for (int i = 0; i < numBridges - 1; i++)
    {
        for (int j = 0; j < numBridges - i - 1; j++)
        {
            if (bridges[j].distance > bridges[j + 1].distance)
            {
                swap(&bridges[j], &bridges[j + 1]);
            }
        }
    }
}

// Function to sort the bridges in each strand
void sortBridgesInStrands(SpiderWeb *spiderWeb)
{
    for (int i = 0; i < spiderWeb->numStrands; i++)
        bubbleSortBridges(spiderWeb->strands[i].bridges, spiderWeb->strands[i].numBridges);
}

// Function to add a bridge between two strands
void addBridge(SpiderWeb *spiderWeb, int from, int distance, int isNew)
{
    Bridge bridge, *peek;
    bridge.distance = distance;
    bridge.isNew = isNew;

    // Calculate the target strand based on the 'from' strand number
    int to = (0 <= from && from < (spiderWeb->numStrands - 1)) ? from + 1 : 0;

    // Add the bridge from 'from' strand to 'to' strand
    bridge.targetStrand = to;
    Strand *strand = &(spiderWeb->strands[from]);
    strand->bridges = realloc(strand->bridges, (strand->numBridges + 1) * sizeof(Bridge));
    peek = &(strand->bridges[strand->numBridges]);
    strand->bridges[strand->numBridges++] = bridge;

    // Add the bridge from 'to' strand to 'from' strand (bidirectional bridge)
    bridge.targetStrand = from;
    strand = &(spiderWeb->strands[to]);
    strand->bridges = realloc(strand->bridges, (strand->numBridges + 1) * sizeof(Bridge));
    peek = &(strand->bridges[strand->numBridges]);
    strand->bridges[strand->numBridges++] = bridge;
}

// Function to create the spider web
SpiderWeb *createWeb(int n, int m)
{
    SpiderWeb *spiderWeb = malloc(sizeof(SpiderWeb));
    Strand *strands = calloc(n, sizeof(Strand));
    for (int i = 0; i < n; i++)
    {
        strands[i].strandNumber = i + 1;
        strands[i].bridges = NULL;
        strands[i].numBridges = 0;
    }

    spiderWeb->numStrands = n;
    spiderWeb->numBridges = m;
    spiderWeb->strands = strands;

    // Read bridge information and add bridges to the spider web
    for (int i = 0, d = 0, t = 0; i < m; i++)
    {
        scanf(" %d %d", &d, &t);
        addBridge(spiderWeb, t - 1, d, 0);
    }

    // Sort the bridges in each strand in ascending order of distance
    sortBridgesInStrands(spiderWeb);

    return spiderWeb;
}

// Function to explore the spider web starting from a specific strand (WIP)
void startFrom(SpiderWeb *spiderWeb, Spider *spider, Strand *from)
{
    spider->currentStrand = from;
    Bridge *bridge;
    for (int i = 0; i < spider->currentStrand->numBridges; i++)
    {
        bridge = &(spider->currentStrand->bridges[i]);
        if (bridge->distance > spider->distanceFromCenter)
        {
            printf("From %d to ", spider->currentStrand->strandNumber);
            spider->distanceFromCenter = bridge->distance;
            printf("%d\n", spiderWeb->strands[bridge->targetStrand].strandNumber);
            startFrom(spiderWeb, spider, &(spiderWeb->strands[bridge->targetStrand]));
        }
    }
}

// Function to create a new spider
Spider *newSpider(SpiderWeb *spiderWeb, int favStrand)
{
    Spider *spider = malloc(sizeof(Spider));
    spider->distanceFromCenter = 0;
    spider->currentStrand = NULL;
    spider->favStrand = &(spiderWeb->strands[favStrand]);
    return spider;
}

// Function to destroy the spider web and free allocated memory
void destroyWeb(SpiderWeb *spiderWeb)
{
    for (int i = 0; i < spiderWeb->numStrands; i++)
    {
        free(spiderWeb->strands[i].bridges);
    }
    free(spiderWeb->strands);
    free(spiderWeb);
}

// Function to destroy the spider and free allocated memory
void destroySpider(Spider *spider)
{
    free(spider);
}

int main()
{
    int n, m, s;
    scanf(" %d %d %d", &n, &m, &s);

    // Create the spider web and spider
    SpiderWeb *spiderWeb = createWeb(n, m);
    Spider *spider = newSpider(spiderWeb, s - 1);

    // Start from each strand and explore the spider web
    for (int i = 0; i < n; i++)
    {
        startFrom(spiderWeb, spider, &(spiderWeb->strands[i]));
        printf("\n");
        spider->distanceFromCenter = 0;
    }

    // Destroy the spider web and spider to free allocated memory
    destroyWeb(spiderWeb);
    destroySpider(spider);

    return 0;
}
