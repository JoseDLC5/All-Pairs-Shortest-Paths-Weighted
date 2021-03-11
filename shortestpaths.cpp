/*******************************************************************************
 * Name    : shortestpaths.cpp
 * Author  : Jose de la Cruz & Breanna Shinn
 * Version : 1.0
 * Date    : December 2, 2020
 * Description : Floyd's Algorithm
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <climits>
#include <iomanip>
using namespace std;

string getPathHelper(vector<vector<long> > interVert, int i, int j){
    if (interVert[i][j] == LONG_MAX) //leaf
    {
        string ans;
        ans.push_back((char)(j + 65));
        ans += " -> ";
        return ans;
    }
    else
    {
        return getPathHelper(interVert, i, interVert[i][j]) + getPathHelper(interVert,interVert[i][j],j);
    }
}

string getPath(vector<vector<long> > interVert, int i, int j){
    string ans;
    ans.push_back((char)(i+65));
    ans += " -> " +  getPathHelper(interVert, i, interVert[i][j]) + getPathHelper(interVert,interVert[i][j],j);
    ans.pop_back();
    ans.pop_back();
    ans.pop_back();
    ans.pop_back();
    return ans;
}

int len(long num) {
    int count = 0;
    while (num > 0)
    {
        num = num/10;
        count++;
    }
    return count;
}

void display_table(vector<vector<long> > matrix, string lable, bool use_letters){
    cout << lable << ":" << endl;
    long max_val = 1;
    
    for (size_t i = 0; i < matrix.size(); i++){ //get max_val
        for (size_t j = 0; j < matrix.size(); j++){
            long cell = matrix[i][j];
            if (cell < LONG_MAX && cell > max_val)
            {
                max_val = matrix[i][j];
            }
        }
    }
    int max_cell_width = use_letters ? len(max_val) : len(max(static_cast<long>(matrix.size()), max_val));
    cout << ' ';
    for (size_t j = 0; j < matrix.size(); j++)
    {
        cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
    }
    cout << endl;

    for (size_t i = 0; i < matrix.size(); i++)
    {
        cout << static_cast<char>(i + 'A');
        for (size_t j = 0; j < matrix.size(); j++)
        {
            cout << " " << setw(max_cell_width);
            if (matrix[i][j] == LONG_MAX)
            {
                cout << "-";
            }
            else if (use_letters)
            {
                cout << static_cast<char>(matrix[i][j] + 'A');
            }
            else
            {
                cout << matrix[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    istringstream iss;
    string line;
    int vertices;
    vector<vector<long> > distMat;
    vector<vector<long> > pathLen;
    vector<vector<long> > InterVert;
    ifstream myfile (argv[1]);
    vector<string> vec;
    int edgeWeight;
    
    if (argc != 2)
    {
        cout << "Usage: ./shortestpaths <filename>";
        return 1;
    }
    
    if (myfile.is_open()) //populate vertices with first line in the input file
    {
        getline (myfile,line);
        iss.str(line);
        if(!(iss >> vertices)){
            cout << "Error: Invalid number of vertices " << "'" <<  line << "'" << " on line 1.";
            return 1;
        }
    }
    else{
        cout << "Error: Cannot open file " << "'" << argv[1] << "'.";
        return 1; //todo
    }  
    if ((vertices > 26) || (vertices < 1)){
        cout << "Error: Invalid number of vertices " << "'" <<  line << "'" << " on line 1.";
        return 1;
    }

    distMat.resize(vertices, vector<long>(vertices, LONG_MAX)); //instantiate distance matrix with 0's

    distMat[0][0] = 0;

    int lineCount = 1;

    if (myfile.is_open()){ //proccess edges
        while ( getline(myfile,line) ){
            lineCount++;
            iss.clear();
            iss.str(line);
            vec = vector<string>(istream_iterator<string>(iss), istream_iterator<string>());

            if (vec.size() != 3){ //check amount of things in line
                cerr << "Error: Invalid edge data " << "'" <<  line << "'" << " on line ";
                cerr << lineCount << "." << endl;
                return 1;
            }

            if ((int)vec[0][0] < 65 || (int)vec[0][0] >= 65 + vertices || vec[0].size() > 1){ //check start vertex
            
                cout << "Error: Starting vertex " << "'" << vec[0] << "'" << " on line " << lineCount << " is not among valid values " << "A-" << (char)(64+vertices) << "." << endl; //TODO
                return 1;
            }
            if ((int)vec[1][0] < 65 || (int)vec[1][0] >= 65 + vertices || vec[0].size() > 1){ //check end vertex
            
                cout << "Error: Ending vertex " << "'" << vec[1] << "'" << " on line " << lineCount << " is not among valid values " << "A-" << (char)(64+vertices) << "." << endl; //TODO
                return 1;
            }
            iss.clear();
            iss.str(vec[2]);
            if (!(iss >> edgeWeight)){
                cout << "Error: Invalid edge weight " << "'" << vec[2] << "'" << " on line " << lineCount << ".";
                return 1;
            }
            else if (edgeWeight < 1){
                cout << "Error: Invalid edge weight " << "'" << vec[2] << "'" << " on line " << lineCount << ".";
                return 1;
            }

            distMat[((int)vec[0][0] - 65)][((int)vec[1][0] - 65)] = edgeWeight;
            for (int i = 0; i < vertices; i++)
            {
                distMat[i][i] = 0;
            }
            
        }
        myfile.close();
    }

    pathLen = distMat;
    InterVert.resize(vertices, vector<long>(vertices, LONG_MAX));
    for (int k = 0; k < vertices; k++)
    {
        for (int i = 0; i < vertices; i++)
        {
            for (int j = 0; j < vertices; j++)
            {
                if (pathLen[i][k] == LONG_MAX)
                {
                    continue;
                }
                if (pathLen[k][j] == LONG_MAX)
                {
                    continue;
                }
                if (pathLen[i][k] + pathLen[k][j] < pathLen[i][j])
                {
                    pathLen[i][j] = pathLen[i][k] + pathLen[k][j];
                    InterVert[i][j] = k;
                }
            }
        }
    }

    display_table(distMat, "Distance matrix", false);
    display_table(pathLen, "Path lengths", false);
    display_table(InterVert, "Intermediate vertices", true);
    
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (pathLen[i][j] == 0)
            {
                cout << (char)(i + 'A') << " -> " << (char)(j+65) << ", distance: " << pathLen[i][j] << ", path: " << (char)(i + 'A') << endl;
            }
            else if (pathLen[i][j] == LONG_MAX)
            {
                cout << (char)(i + 'A') << " -> " << (char)(j+65) << ", distance: " << "infinity" << ", path: " << "none"<< endl;
            }
            else if (InterVert[i][j] == LONG_MAX)
            {
                cout << (char)(i + 'A') << " -> " << (char)(j+65) << ", distance: " << pathLen[i][j] << ", path: " << (char)(i + 'A') << " -> " << (char)(j+65) << endl;
            }
            else
            {
                //cout << "special\n";
                cout << (char)(i + 'A') << " -> " << (char)(j+65) << ", distance: " << pathLen[i][j] << ", path: " << getPath(InterVert,i,j) << endl;
            }
            
        }
        
    }
    
    return 0;
}

