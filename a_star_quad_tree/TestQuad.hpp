#include <stdlib.h>
#include <stdio.h>
#include "node.hpp"

using namespace cv;

void Decide(Node* node); // Decide si l'on doit partager selon si un obstacle et une zone libre se trouvent dans la meme region
void Divise(Node* node); // Création des enfants lorsque le node est mixed
void NeigAssign(); // Recherche de voisins

Node* FindNorthNeig(Node* node);
Node* FindSouthNeig(Node* node);
Node* FindEastNeig(Node* node);
Node* FindWestNeig(Node* node);

void NeigFill(Node* node);
