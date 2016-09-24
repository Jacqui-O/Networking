/* 
Jacquelyn Oquendo
December 1, 2015
CSCE 3530
Program 5
*/
 
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define V 6  // Number of vertices in the graph

 void dijFile(char *data)
{
  //  Function saves segments into file
    FILE *f = fopen("LS.txt", "a+");
    if(!f)
          return;

  // Put data into LS.txt  
  fprintf(f, "%s\n", data);
 
  return;
  fclose(f);
} // End dijFile  

 void dvFile(char *data)
{
  //  Function saves segments into file
    FILE *f = fopen("DV.txt", "a+");
    if(!f)
          return;

  // Put data into DV.txt  
  fprintf(f, "%s\n", data);
 
  return;
  fclose(f);
} // End dvFile

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
 
// Prints output and calls dijFile to save output
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
          dijFile(buffer);}
        else{
          printf("%c (u, x)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (u, x)", c);
          dijFile(buffer);
        }
      snprintf(buffer, 1024, "Router U:");
      dijFile(buffer);
  }

    else if(src == 1){ // Router V
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
        if(c =='u'){ 
          printf("%c (v, u)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (v, u)", c);
          dijFile(buffer);}
        else if (c == 'v')
          continue; 
        else if (c == 'w'){
          printf("%c (v, w)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (v, w)", c);
          dijFile(buffer);}
        else{
          printf("%c (v, x)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (v, x)", c);
          dijFile(buffer);
        }
      snprintf(buffer, 1024, "Router V:");
      dijFile(buffer);
  }
   
    else if(src == 2){ // Router W
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if (c == 'u'){
         printf("%c (w, x) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (w, x)", c);
         dijFile(buffer);}
      else if (c == 'v'){
         printf("%c (w, v)\t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (w, v)", c);
         dijFile(buffer);}
      else if (c == 'w')
        continue; 
      else{
         printf("%c (w, y)\t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (w, y)", c);
         dijFile(buffer);
       }
      snprintf(buffer, 1024, "Router W:");
      dijFile(buffer);
  }
   
    else if(src == 3){ // Router X
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if(c == 'u'){
         printf("%c (x, u) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (x, u)", c);
         dijFile(buffer);}
      else if(c == 'v'){
         printf("%c (x, v) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (x, v)", c);
         dijFile(buffer);}
      else if(c== 'x')
        continue;
      else{
         printf("%c (x, y) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (x, y)", c);
         dijFile(buffer);
       }
    snprintf(buffer, 1024, "Router X:");
    dijFile(buffer);
  }
   
    else if(src == 4){ // Router Y
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if(c == 'w'){
        printf("%c (y, w) \t\t %d\n", c, dist[i]);
        snprintf(buffer, 1024, "%c (y, w)", c);
        dijFile(buffer);
       }
      else if(c == 'z'){
        printf("%c (y, z) \t\t %d\n", c, dist[i]);
        snprintf(buffer, 1024, "%c (y, z)", c);
         dijFile(buffer);
       }
      else if(c == 'y')
         continue; 
       else{
         printf("%c (y, x) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (y, x)", c);
         dijFile(buffer);
       }
    snprintf(buffer, 1024, "Router Y:");
    dijFile(buffer);
  }

    else{ // Router Z
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if (c == 'z')
        continue;
      else{
        printf("%c (z, y) \t\t %d\n", c, dist[i]);
        snprintf(buffer, 1024, "%c (z, y)", c);
        dijFile(buffer);
       }
    snprintf(buffer, 1024, "Dijkstra's Algorithm\nRouter Z:");
    dijFile(buffer);
    }
}// End printOutput
 
// Dijkstra's Algorithm
void dijkstra(int graph[V][V], int src)
{
     int dist[V];     // dist[i] will hold the shortest distance from src to dest
     bool visited[V]; // visited[i] will true if vertex i has been visited
 
     // Initialize distances
     for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, visited[i] = false;
        dist[src] = 0; // distance from source to itself if always 0
 
     // Find shortest path for all vertices
     for (int count = 0; count < V-1; count++)
     {
       // Pick the min dist vertex from those not yet visited
       int u = minDist(dist, visited);
 
       // Mark the picked vertex as visited
       visited[u] = true;
 
       // Update dist value 
       for (int v = 0; v < V; v++)
 
         // Update dist[v] only if not in visited
         if (!visited[v] && graph[u][v] && dist[u] != INT_MAX 
                                       && dist[u]+graph[u][v] < dist[v])
            dist[v] = dist[u] + graph[u][v];
     }
     // Print the forwarding table
     printOutput(dist, V, src);
}

// Represents edge for DV algorithm
struct Edge
{
    int src, dest, weight;
};
 
// Represents graph for DV algorithm
struct dvGraph
{   //  Vert = 6
    //  E = 10
    int Vert, E;
 
    struct Edge* edge;
};
 
// Creates a graph for DV algorithm
struct dvGraph* createGraph(int Vert, int E)
{
    struct dvGraph* graph1 = (struct dvGraph*) malloc( sizeof(struct dvGraph) );
    graph1->Vert = Vert;
    graph1->E = E;
    graph1->edge = (struct Edge*) malloc( graph1->E * sizeof( struct Edge ) );
 
    return graph1;
}

// Prints Distance Vector solution
void printDV(int dist[], int n, int src)
{
 char buffer[1024];
 int i; 
       printf("         Distance from Source\n");
  if(src == 0){ // Router U
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++) //
        if(c =='u')
          continue;
        else if(c == 'v'){
          printf("%c (u, v)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (u, v)", c);
          dvFile(buffer); // save to dvFile
          } 
        else if(c == 'w'){
          printf("%c (u, x)\t\t 3\n", c);
          snprintf(buffer, 1024, "%c (u, v)", c);
          dvFile(buffer);
        }
        else{
          printf("%c (u, x)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (u, x)", c);
         dvFile(buffer);
        }
      snprintf(buffer, 1024, "Router U:");
      dvFile(buffer);
  } 
   else if(src == 1){ // Router V
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
        if(c == 'u'){ 
          printf("%c (v, u)\t\t 2\n", c);
          snprintf(buffer, 1024, "%c (v, u)", c);
          dvFile(buffer);
        }
        else if (c == 'v')
          continue; 
        else if (c == 'w'){
          printf("%c (v, w)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (v, w)", c);
          dvFile(buffer);
        }
        else{
          printf("%c (v, x)\t\t %d\n", c, dist[i]);
          snprintf(buffer, 1024, "%c (v, x)", c);
          dvFile(buffer);
        }
      snprintf(buffer, 1024, "Router V:");
      dvFile(buffer);
  } 
   
    else if(src == 2){ // Router W
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if (c == 'u'){
         printf("%c (w, x) \t\t 3\n", c);
         snprintf(buffer, 1024, "%c (w, x)", c);
         dvFile(buffer);
       }
      else if (c == 'v'){
         printf("%c (w, v)\t\t 3\n", c);
         snprintf(buffer, 1024, "%c (w, v)", c);
         dvFile(buffer);
       }
       else if (c == 'x'){
         printf("%c (w, y)\t\t 2\n", c);
         snprintf(buffer, 1024, "%c (w, y)", c);
         dvFile(buffer);
       }

      else if (c == 'w')
        continue; 
      else{
         printf("%c (w, y)\t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (w, y)", c);
        dvFile(buffer);
       }
      snprintf(buffer, 1024, "Router W:");
      dvFile(buffer);
  }
   
    else if(src == 3){ // Router X
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if(c == 'u'){
         printf("%c (x, u) \t\t 1\n", c);
         snprintf(buffer, 1024, "%c (x, u)", c);
         dvFile(buffer);
       }
      else if(c == 'v'){
         printf("%c (x, v) \t\t 2\n", c);
         snprintf(buffer, 1024, "%c (x, v)", c);
         dvFile(buffer);
       }
      else if(c == 'x')
        continue;
      else if (c == 'w'){
         printf("%c (x, y) \t\t 2\n", c);
         snprintf(buffer, 1024, "%c (x, y)", c);
         dvFile(buffer);
       }
      else{
         printf("%c (x, y) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (x, y)", c);
         dvFile(buffer);
       }
    snprintf(buffer, 1024, "Router X:");
    dvFile(buffer);
  }
   
    else if(src == 4){ // Router Y
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if(c == 'w'){
        printf("%c (y, w) \t\t 1\n", c);
        snprintf(buffer, 1024, "%c (y, w)", c);
        dvFile(buffer);
       }
       else if (c == 'u'){
        printf("%c (y, x) \t\t 2\n", c);
        snprintf(buffer, 1024, "%c (y, x)", c);
        dvFile(buffer);
       }
       else if (c == 'v'){
        printf("%c (y, x) \t\t 3\n", c);
        snprintf(buffer, 1024, "%c (y, x)", c);
        dvFile(buffer);
       }
       else if (c == 'x'){
        printf("%c (y, x) \t\t 1\n", c);
        snprintf(buffer, 1024, "%c (y, x)", c);
        dvFile(buffer);
       }
      else if(c == 'z'){
        printf("%c (y, z) \t\t %d\n", c, dist[i]);
        snprintf(buffer, 1024, "%c (y, z)", c);
         dvFile(buffer);
       }
      else if(c == 'y')
         continue; 
       else{
         printf("%c (y, x) \t\t %d\n", c, dist[i]);
         snprintf(buffer, 1024, "%c (y, x)", c);
         dvFile(buffer);
       }
    snprintf(buffer, 1024, "Router Y:");
    dvFile(buffer);

  }
   
    else{ // Router Z
    for (char c = 'u', i = 0; c < 'z', i<V; c++, i++)
      if (c == 'z')
        continue;
      else if (c == 'u'){
        printf("%c (z, y) \t\t 4\n", c);
        snprintf(buffer, 1024, "%c (z, y)", c);
        dvFile(buffer);
       }
       else if (c == 'v'){
        printf("%c (z, y) \t\t 5\n", c);
        snprintf(buffer, 1024, "%c (z, y)", c);
        dvFile(buffer);
       }
       else if (c == 'y'){
        printf("%c (z, y) \t\t 2\n", c);
        snprintf(buffer, 1024, "%c (z, y)", c);
        dvFile(buffer);
       }
       else{
        printf("%c (z, y) \t\t 3\n", c);
        snprintf(buffer, 1024, "%c (z, y)", c);
        dvFile(buffer);
       }
    snprintf(buffer, 1024, "Dijkstra's Algorithm\nRouter Z:");
    dvFile(buffer);
    } 

}

// Finds shortest distances from src to all other vertices using DV algorithm 
void dvAlgorithm(struct dvGraph* graph1, int src)
{   int Vert = graph1->Vert;      
    int E = graph1->E; 
    int dist[Vert];
 
    // Initialize distances from src 
    for (int i = 0; i < V; i++)
        dist[i]   = INT_MAX;
        dist[src] = 0;
 
    // Shortest path from src to vertex can have at-most |V| - 1 edges
    for (int i = 1; i <= Vert-1; i++)
    {
        for (int j = 0; j < E; j++)
        {
            int u = graph1->edge[j].src; 
            int v = graph1->edge[j].dest; 
            int weight = graph1->edge[j].weight; 
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) 
                dist[v] = dist[u] + weight; 
        }
    }
 
    // Check for negative weights
    for (int i = 0; i < E; i++)
    {
        int u = graph1->edge[i].src; 
        int v = graph1->edge[i].dest; 
        int weight = graph1->edge[i].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
            printf("Graph contains negative weight cycle");
    }
 
    printDV(dist, Vert, src);
 
return;
}// End DV algorithm
 
 // a is the current or source vertex
 // b is the next or destination vertex
 // w is the edge weight or path cost
int main()
{
   int i, j, a, b, w, edges = 10;
   int Graph[V][V];
   struct dvGraph* graph1 = createGraph(V, edges); 
 
    // Set graph to zero
    for(i = 0; i < V; ++i)
        for(j = 0; j < V; ++j)
            Graph[i][j] = 0;
 
    // Input Graph from file
    printf("Scanning (u v w) from file...\n\n");
    FILE *graph = fopen("router.txt", "r"); // Read only
    for(i = 0; i < edges; ++i){

        fscanf(graph, "%d%d%d", &a, &b, &w);
        // For undirected edge (u,v) = (v,u)
        Graph[a][b] = Graph[b][a] = w;
        graph1->edge[i].src = a; 
        graph1->edge[i].dest = b; 
        graph1->edge[i].weight = w; 
    }
    fclose(graph);

//First print matrix representaion of graph to show that file was correctly read
printf("Matrix representaion of network connection\n");
printf ("u v w x y z\n");
printf ("------------\n");
for(i=0; i < V; i++){
  for(j=0; j<V; j++){
    printf("%d ", Graph[i][j]);
  } printf("\n");
}

    // Calling Dijkstra's Algorithm
    printf("\nDijkstra's Algorithm\nRouter U:");
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

    // Calling DV Algorithm
    printf("\n\nDistance Vector\nRouter U:");
    dvAlgorithm(graph1, 0); // Router U
    printf("\nRouter V:");
    dvAlgorithm(graph1, 1); // Router V
    printf("\nRouter W:");
    dvAlgorithm(graph1, 2); // Router W
    printf("\nRouter X:");
    dvAlgorithm(graph1, 3); // Router X
    printf("\nRouter Y:");
    dvAlgorithm(graph1, 4); // Router Y
    printf("\nRouter Z:");
    dvAlgorithm(graph1, 5); // Router Z
    
    return 0;
}

