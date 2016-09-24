/* 
Dijkstra1 am trying to implement DV. If not working, submit Dijkstra.c
*/
 
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>//
#include <string.h>//
 
// Number of vertices in the graph
#define V 6

 void dij_file(char *data)
{
  //  Function saves segments into file
    FILE *f = fopen("LS.txt", "a+");
    if(!f)
          return;

  // Put data into LS.txt  
  fprintf(f, "%s\n", data);
 
  return;
  fclose(f);
} // End dij_file  

// Finds the vertex with minimum distance value
int minDist(int dist[], bool visited[])
{
   // Initialize min value
   int min = INT_MAX, min_index;
 
   for (int v = 0; v < V; v++)
     if (visited[v] == false && dist[v] <= min)
         min = dist[v], min_index = v;
         
   return min_index; 
}
 
// Prints output and calls dij_file to save output
int printOutput(int dist[], int n, int src)
{
  int i; 
  char buffer[1024];

  printf("         Distance from Source\n");
  if(src == 0){ // Router U
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++) //
        if(c =='u')
          continue;
        else if(c == 'v'){
          printf("%c (u, v)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (u, v)", c);
          dij_file(buffer);}
        else{
          printf("%c (u, x)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (u, x)", c);
          dij_file(buffer);
        }
      snprintf(buffer, 1024, "Router U:");
      dij_file(buffer);
  }
    else if(src == 1){ // Router V
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
        if(c =='u'){ 
          printf("%c (v, u)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (v, u)", c);
          dij_file(buffer);}
        else if (c == 'v')
          continue; 
        else if (c == 'w'){
          printf("%c (v, w)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (v, w)", c);
          dij_file(buffer);}
        else{
          printf("%c (v, x)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (v, x)", c);
          dij_file(buffer);
        }
      snprintf(buffer, 1024, "Router V:");
      dij_file(buffer);
  }
   
    else if(src == 2){ // Router W
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if (c == 'u'){
         printf("%c (w, x) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (w, x)", c);
         dij_file(buffer);}
      else if (c == 'v'){
         printf("%c (w, v)\t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (w, v)", c);
         dij_file(buffer);}
      else if (c == 'w')
        continue; 
      else{
         printf("%c (w, y)\t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (w, y)", c);
         dij_file(buffer);
       }
      snprintf(buffer, 1024, "Router W:");
      dij_file(buffer);
  }
   
    else if(src == 3){ // Router X
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if(c == 'u'){
         printf("%c (x, u) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (x, u)", c);
         dij_file(buffer);}
      else if(c == 'v'){
         printf("%c (x, v) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (x, v)", c);
         dij_file(buffer);}
      else if(c== 'x')
        continue;
      else{
         printf("%c (x, y) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (x, y)", c);
         dij_file(buffer);
       }
    snprintf(buffer, 1024, "Router X:");
    dij_file(buffer);
  }
   
    else if(src == 4){ // Router Y
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if(c == 'w'){
        printf("%c (y, w) \t\t %d\n", c, dist[i]);
        snprintf(buffer, 1024, "%c (y, w)", c);
        dij_file(buffer);
       }
      else if(c == 'z'){
        printf("%c (y, z) \t\t %d\n", c, dist[i]);
        snprintf(buffer, 1024, "%c (y, z)", c);
         dij_file(buffer);
       }
      else if(c == 'y')
         continue; 
       else{
         printf("%c (y, x) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (y, x)", c);
         dij_file(buffer);
       }
    snprintf(buffer, 1024, "Router Y:");
    dij_file(buffer);

  }
   
    else{ // Router Z
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if (c == 'z')
        continue;
      else{
        printf("%c (z, y) \t\t %d\n", c, dist[i]);
        snprintf(buffer, 1024, "%c (z, y)", c);
        dij_file(buffer);
       }
    snprintf(buffer, 1024, "Dijkstra's Algorithm\nRouter Z:");
    dij_file(buffer);
    }
}// End printOutput
 
// Dijkstra's Algorithm
void dijkstra(int graph[V][V], int src)
{
     int dist[V];     // dist[i] will hold the shortest distance from src to dest
     bool visited[V]; // visited[i] will true if vertex i has been visited
 
     // Initialize distance as INFINITE
     for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, visited[i] = false;
 
     // Distance of source vertex from itself is always 0
     dist[src] = 0;
 
     // Find shortest path for all vertices
     for (int count = 0; count < V-1; count++)
     {
       // Pick the min dist vertex from those not yet visited
       int u = minDist(dist, visited);
 
       // Mark the picked vertex as visited
       visited[u] = true;
 
       // Update dist value of the adjacent vertices of the picked vertex.
       for (int v = 0; v < V; v++)
 
         // Update dist[v] only if not in visited
         if (!visited[v] && graph[u][v] && dist[u] != INT_MAX 
                                       && dist[u]+graph[u][v] < dist[v])
            dist[v] = dist[u] + graph[u][v];
     }
     // print the constructed distance array
     printOutput(dist, V, src);
}

// a structure to represent a weighted edge in graph
struct Edge
{
    int src, dest, weight;
};
 
// a structure to represent a connected and weighted graph
struct dvGraph
{   //
    //  E-> Number of edges
    int Vert, E;
 
    // graph is represented as an array of edges.
    struct Edge* edge;
};
 
// Creates a graph with V vertices and E edges
struct dvGraph* createGraph(int Vert, int E)
{
    struct dvGraph* graph1 = (struct dvGraph*) malloc( sizeof(struct dvGraph) );
    graph1->Vert = Vert;
    graph1->E = E;
 
    graph1->edge = (struct Edge*) malloc( graph1->E * sizeof( struct Edge ) );
 
    return graph1;
}

// Prints Distance Vector solution
void printDV(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}

// The main function that finds shortest distances from src to
// all other vertices using DV algorithm.  The function
// also detects negative weight cycle
void dvAlgorithm(struct dvGraph* graph1, int src)
{   int Vert = graph1->Vert;      //graph->V
    int E = graph1->E; //graph->E
    int dist[Vert];//10
 
    // Initialize distances from src to all other vertices as INFINITE
    for (int i = 0; i < V; i++)
        dist[i]   = INT_MAX;
    dist[src] = 0;
 
    // Relax all edges |V| - 1 times. A simple shortest 
    // path from src to any other vertex can have at-most |V| - 1 
    // edges
    for (int i = 1; i <= Vert-1; i++)//i=0
    {
        for (int j = 0; j < E; j++)
        {
            int u = graph1->edge[j].src;//graph->edge because Graph* graph passed?
            int v = graph1->edge[j].dest; //
            int weight = graph1->edge[j].weight; //
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
        }
    }
 
    // check for negative-weight cycles.  The above step 
    // guarantees shortest distances if graph doesn't contain 
    // negative weight cycle.  
    for (int i = 0; i < E; i++)
    {
        int u = graph1->edge[i].src; //
        int v = graph1->edge[i].dest; //
        int weight = graph1->edge[i].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
            printf("Graph contains negative weight cycle");
    }
 
    printDV(dist, Vert);
 
    return;
}
 
 // a is the current or source vertex
 // b is the next or destination vertex
 // w is the edge weight or path cost
int main()
{
   int i, j, a, b, w, edges = 10;
   int Graph[V][V];
   struct dvGraph* graph1 = createGraph(V, edges); //
 
    // Set graph to zero
    for(i = 0; i < V; ++i)
        for(j = 0; j < V; ++j)
            Graph[i][j] = 0;
 
    // Input Graph from file
    printf("Scanning (u v w) from file...\n");
    FILE *graph = fopen("router.txt", "r"); // Read only
    for(i = 0; i < edges; ++i){

        fscanf(graph, "%d%d%d", &a, &b, &w);
        // For undirected edge (u,v) = (v,u)
        Graph[a][b] = Graph[b][a] = w;
        graph1->edge[i].src = a; //directed?
        graph1->edge[i].dest = b; //
        graph1->edge[i].weight = w; //
    }
    fclose(graph);
    
    //Calling Dijkstra's Algorithm
    printf("Dijkstra's Algorithm U-Z\nRouter U:");
    dijkstra(Graph, 0); // Router U
    printf("\nRouter V:");
    dijkstra(Graph, 1); // Router V
    printf("\nRouter W:");
    dijkstra(Graph, 2); // Router W
    printf("\nRouter X:");
    dijkstra(Graph, 3); // Router X
    printf("\nRouter Y:");
    dijkstra(Graph, 4); // Router Y
    printf("\nRouter Z:");
    dijkstra(Graph, 5); // Router Z

   
    // DV Algorithm
    dvAlgorithm(graph1, 0); // Router U
    
 
    return 0;
}

