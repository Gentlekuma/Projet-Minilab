#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "node.hpp"
#include "TestQuad.hpp"
#include <vector>
#include "Map.hpp"

using namespace cv;

Mat src;
int level;


std::vector<Node *>leaf; //Contient toutes les feuilles libres de l'arbre

int main( int, char** argv )
{
	int i = 0;
	int j = 0;
	int l = 0;
	int id = 0;
	
	Node* root = new Node();
	
	
	//int reso = 32;
	
	
	//Traitement d'image 
	src = imread(argv[1]);
	
	std::cout << " Taille :"<<src.rows<< std::endl;
	std::cout << " Taille :"<<src.cols<< std::endl;
	
	
	cvtColor(src, src, CV_BGR2GRAY);
	
	//Initialisation du node root 
	
	root->x = 0;
	root->y = 0;
	root->resolution = src.rows;
	root->type = MIXED_NODE;
	
	Decide(root);
	
	
	//Tests accès pixels image
	/*
	for(j = root->x ; j<root->resolution ; j++){
		for(l = root->y ; l<root->resolution ; l++){
			if (src.at<uchar>(j,l) != 0 && src.at<uchar>(j,l) != 255){
				std::cout << "Valeur map "<<src.at<uchar>(i,j)<< std::endl;
			}
			//std::cout << "Valeur map "<<src.at<int>(i,j)<< std::endl;
			
		}
	}
	*/
	
	
	//Réalisation de l'arbre de manière recursive
	cvtColor(src, src, CV_GRAY2BGR);
	
	
	//Verification emplacement des feuilles libres
	int taille = leaf.size();
	for( l = 0; l<taille ; l++){
		//std::cout << "Leaf "<< l << " x "<<leaf[l]->x<< std::endl;
		//std::cout << "Leaf "<< l << " y "<<leaf[l]->y<< std::endl;
		Vec3b color = src.at<Vec3b>(leaf[l]->x,leaf[l]->y);
		color[0] = 0;
        color[1] = 0;
        color[2] = 255;
        src.at<Vec3b>(leaf[l]->x,leaf[l]->y) = color;
	}
	
	int k = 0;
	//Recherche des voisins et choix du goal 
	for( k = 0; k<taille ; k++){
		if ( leaf[k]->x == 160 && leaf[k]->y == 192 ) {
			leaf[k]->type = GOAL_NODE;
		}
		NeigFill(leaf[k]);
		
		std::cout << "Nombre de voisins "<<leaf[k]->neighborhood.size()<< std::endl;
	}


	imshow("pad", src);
	
	
	std::cout << "Fin test "<<level<< std::endl;
	waitKey(0);
	return 0;
}

void Decide(Node* node)
{
	std::cout << " Resolution Node "<<node->resolution<< std::endl;
	int i = 0;
	int j = 0;
	int test_w = 0;
	int test_b = 0;
	
	//Parcours du carré correspondant au node
	for (i=node->x ; i<node->x+node->resolution; i++)
	{
		//std::cout << "Valeur i "<<i<< std::endl;
		for(j=node->y ; j<node->y+node->resolution; j++)
		{
			
			//std::cout << "Valeur j "<<j<< std::endl;
			if ( src.at<uchar>(i,j) == 255)
			{
				//std::cout << "Valeur de Test BLANC"<<test_w<< std::endl;
				test_w = test_w + 1;
			}
			if ( src.at<uchar>(i,j) == 0)
			{
				//std::cout << "Pos "<<test_b<< std::endl;
				test_b = test_b + 1;
			}
		}
	}
	
	std::cout << "Valeur de Test BLANC "<<test_w<< std::endl;
	//Choix du type du node en fonction du nombre de pixels obstacles dans le node
	
	//Cas où le node ne contient que des pixels obstacles
	if ( test_w == 0) 
	{

		std::cout << "//// BLOCKED NODE "<<node->x<<" "<<node->y<< std::endl;
		node->type = BLOCKED_NODE;
	}
	
	//Cas où la division est necessaire 
	if ( test_b != 0 && test_w != 0)
	{

		std::cout << "//// DIVISION "<<node->x<<" "<<node->y<< std::endl;
		node->type = MIXED_NODE;
		Divise(node);
	}
	
	//Cas où le node est libre
	if( test_b == 0)
	{
		
		std::cout << "/////// Ajout d'une feuille "<<node->x<<" "<<node->y<< std::endl;
		node->type = FREE_NODE;
		if ( src.at<uchar>(node->x,node->y) == 255){
			leaf.push_back(node);
		}
	}
	
	
}

void Divise(Node* node)
{
		if ( node->resolution >= 4 ) {
		level = level + 1;
		std::cout << "Level "<<level<< std::endl;
		std::cout << "Nombre feuilles "<<leaf.size()<< std::endl;
			
			node->sons[0] = new Node( node->x, node->y , (node->resolution)/2, node , FREE_NODE , node->sons , node->neighborhood );
			Decide(node->sons[0]);
			//TypeAssign(node->sons[0]);
			
			node->sons[1] = new Node( node->x+(node->resolution)/2 , node->y , (node->resolution)/2, node , FREE_NODE , node->sons , node->neighborhood );
			Decide(node->sons[1]);
			//TypeAssign(node->sons[1]);
			
			node->sons[2] = new Node( node->x, node->y+(node->resolution)/2, (node->resolution)/2, node , FREE_NODE , node->sons , node->neighborhood );
			Decide(node->sons[2]);
			//TypeAssign(node->sons[2]);
			
			node->sons[3] = new Node( node->x+(node->resolution)/2 , node->y+(node->resolution)/2 , (node->resolution)/2, node , FREE_NODE , node->sons , node->neighborhood );
			Decide(node->sons[3]);
			//TypeAssign(node->sons[3]);
		}
}


// Methode de recherche des voisins fonctionnelle

Node* FindNorthNeig(Node* node){
	
	if ( node->isRoot() == true ){
		//std::cout << " Est Racine" << std::endl;
		return NULL;
	}
	
	if ( node == node->parent_node->sons[2] /*&& node->parent_node->sons[0]->type == FREE_NODE*/ ) {
		//std::cout << " Voisin nord trouvé x =  "<<node->parent_node->sons[0]->x<< " y = " << node->parent_node->sons[0]->y << std::endl;
		return node->parent_node->sons[0];
		
	}
	
	if ( node == node->parent_node->sons[3] /*&& node->parent_node->sons[1]->type == FREE_NODE*/ ) {
		//std::cout << " Voisin nord trouvé x =  "<<node->parent_node->sons[1]->x<< " y = " <<node->parent_node->sons[1]->y << std::endl;
		return node->parent_node->sons[1];
		
	}
	
	//std::cout << " Recherche de voisins dans le niveau au dessus " << std::endl;
	Node* Inter = FindNorthNeig(node->parent_node);
	//std::cout << " Descente d'un niveau " << std::endl;
	
	if ( (Inter == NULL)||(Inter->isLeaf() == true) ){
		//std::cout << " Inter null ou feuille " << std::endl;
		return NULL;
	}
	else
	{
		if ( node == node->parent_node->sons[0] ) { 
			//if( Inter->sons[2]->type == FREE_NODE){
			//std::cout << " Voisin inter nord trouvé "<< std::endl;
			return Inter->sons[2];
			/*else
				return NULL;*/
			
		}
		else{
			//if ( Inter->sons[3]->type == FREE_NODE){
				//std::cout << " Voisin inter nord trouvé "<< std::endl;
				return Inter->sons[3];
			/*else
				return NULL;*/
		}
	}

}

Node* FindEastNeig(Node* node){
	
	if ( node->isRoot() == true ){
		//std::cout << " Est Racine" << std::endl;
		return NULL;
	}
	
	if ( node == node->parent_node->sons[3] /*&& node->parent_node->sons[2]->type == FREE_NODE*/ ) {
		//std::cout << " Voisin est trouvé x =  "<<node->parent_node->sons[2]->x<< " y = " << node->parent_node->sons[2]->y << std::endl;
		return node->parent_node->sons[2];
		
	}
	
	if ( node == node->parent_node->sons[1] /*&& node->parent_node->sons[0]->type == FREE_NODE*/ ) {
		//std::cout << " Voisin est trouvé x =  "<<node->parent_node->sons[0]->x<< " y = " << node->parent_node->sons[0]->y << std::endl;
		return node->parent_node->sons[0];
		
	}

	//std::cout << " Recherche de voisins dans le niveau au dessus " << std::endl;
	Node* Inter = FindEastNeig(node->parent_node);
	//std::cout << " Descente d'un niveau " << std::endl;
	
	if ( (Inter == NULL)||(Inter->isLeaf() == true) ){
		//std::cout << " Inter null ou feuille " << std::endl;
		return NULL;
	}
	else
	{
		if ( node == node->parent_node->sons[2] )  { 
			//if ( Inter->sons[3]->type == FREE_NODE ) {
				//std::cout << " Voisin inter est trouvé"<<  std::endl;
				return Inter->sons[3];
			/*
			else
				return NULL;*/
		}
		else{
			//if ( Inter->sons[1]->type == FREE_NODE ) {
				//std::cout << " Voisin inter est trouvé "<< std::endl;
				return Inter->sons[1];
			/*
			else
				return NULL;*/
		}
	}

}

Node* FindSouthNeig(Node* node){
	
	if ( node->isRoot() == true ){
		//std::cout << " Est Racine" << std::endl;
		return NULL;
	}
	
	if ( node == node->parent_node->sons[1] /*&& node->parent_node->sons[3]->type == FREE_NODE*/ ) {
		//std::cout << " Voisin sud trouvé x =  "<<node->parent_node->sons[3]->x<< " y = " << node->parent_node->sons[3]->y << std::endl;
		return node->parent_node->sons[3];
		
	}
	
	if ( node == node->parent_node->sons[0] /*&& node->parent_node->sons[2]->type == FREE_NODE*/  ) {
		//std::cout << " Voisin sud trouvé x =  "<<node->parent_node->sons[2]->x<< " y = " << node->parent_node->sons[2]->y << std::endl;
		return node->parent_node->sons[2];
		
	}

	//std::cout << " Recherche de voisins dans le niveau au dessus " << std::endl;
	Node* Inter = FindSouthNeig(node->parent_node);
	//std::cout << " Descente d'un niveau " << std::endl;
	
	if ( (Inter == NULL)||(Inter->isLeaf() == true) ){
		//std::cout << " Inter null ou feuille " << std::endl;
		return NULL;
	}
	else
	{
		if ( node == node->parent_node->sons[3]) { 
			//if ( Inter->sons[1]->type == FREE_NODE ){
				//std::cout << " Voisin inter sud trouvé "<< std::endl;
				return Inter->sons[1];
			/*
			else
				return NULL;*/
		}
		else{
			//if (Inter->sons[0]->type == FREE_NODE){
				//std::cout << " Voisin inter sud trouvé "<< std::endl;
				return Inter->sons[0];
			/*
			else
				return NULL;*/
			
		}
	}

}

Node* FindWestNeig(Node* node){
	
	if ( node->isRoot() == true ){
		//std::cout << " Est Racine" << std::endl;
		return NULL;
	}
	
	if ( node == node->parent_node->sons[0] /*&& node->parent_node->sons[1]->type == FREE_NODE*/ ) {
		//std::cout << " Voisin ouest trouvé x =  "<<node->parent_node->sons[1]->x<< " y = " << node->parent_node->sons[1]->y << std::endl;
		return node->parent_node->sons[1];
		
	}
	
	if ( node == node->parent_node->sons[2] /*&& node->parent_node->sons[3]->type == FREE_NODE*/ ) {
		//std::cout << " Voisin ouest trouvé x =  "<<node->parent_node->sons[3]->x<< " y = " << node->parent_node->sons[3]->y << std::endl;
		return node->parent_node->sons[3];
		
	}

	//std::cout << " Recherche de voisins dans le niveau au dessus " << std::endl;
	Node* Inter = FindWestNeig(node->parent_node);
	//std::cout << " Descente d'un niveau " << std::endl;
	
	if ( (Inter == NULL)||(Inter->isLeaf() == true) ){
		//std::cout << " Inter null ou feuille " << std::endl;
		return NULL;
	}
	else
	{
		if ( node == node->parent_node->sons[1] ) { 
			//if ( Inter->sons[0]->type == FREE_NODE){
				//std::cout << " Voisin inter ouest trouvé " << std::endl;
				return Inter->sons[0];
			/*
			else
				return NULL;*/
			
		}
		else{
			//if ( Inter->sons[2]->type == FREE_NODE){
				//std::cout << " Voisin inter ouest trouvé " << std::endl;
				return Inter->sons[2];
			/*
			else
				return NULL;*/
		}	
	}

}

void NeigFill(Node* node){
	
	std::cout << " Debut recherche voisins sur le node x =  "<<node->x<< " y = " << node->y << std::endl;
	node->neighborhood.push_back(FindNorthNeig(node));
	if( node->neighborhood.back() != NULL ){
		
		if( node->neighborhood.back()->type == BLOCKED_NODE  ){
			std::cout << " Depop "  << std::endl;
			node->neighborhood.pop_back();
		}
	}
	node->neighborhood.push_back(FindEastNeig(node));
	if( node->neighborhood.back() != NULL ){
		
		if( node->neighborhood.back()->type == BLOCKED_NODE  ){
			std::cout << " Depop "  << std::endl;
			node->neighborhood.pop_back();
		}
	}
	node->neighborhood.push_back(FindSouthNeig(node));
	if( node->neighborhood.back() != NULL ){
		
		if( node->neighborhood.back()->type == BLOCKED_NODE ){
			std::cout << " Depop "  << std::endl;
			node->neighborhood.pop_back();
		}
	}
	node->neighborhood.push_back(FindWestNeig(node));
	if( node->neighborhood.back() != NULL ){
		
		if(node->neighborhood.back()->type == BLOCKED_NODE ){
			std::cout << " Depop "  << std::endl;
			node->neighborhood.pop_back();
		}
	}
	
	
}




	
	
	
