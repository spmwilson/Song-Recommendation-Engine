#include<fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
using namespace std;

class Songs {
    
public:
    
    //Member Variables
    vector<string> printSongTitles;
    vector<string> songTitles;
    vector<vector<int> > songRatings;
    vector<vector<int> > userRatings;
    vector<vector<double> > newUserRatings;
    vector<double> normMean;
    vector<vector<double> > normSongs;
    vector<double> finalMeans;
    vector<double> finalSDs;
    vector <vector<double> > songSims;
    vector <vector<double> > userSims;
    unsigned long numberOfUsers;
    vector<vector<double> > newNormSongs;
    vector<vector<double> > distances;
    
    
    //Destructor
    ~Songs();
    //Check that lines are equal between Songs and Ratings
    bool countLines(string file, string file2);
    //Read in the Song Titles
    bool readSongTitles(string file);
    //Get a Song Title
    string readSongTitle(ifstream& in);
    //Check if the Song Title is Valid
    bool isTitleValid(string title);
    // Read All Ratings and Check if Valid
    bool readSongRatings(string file);
    //
    void pullUserData();
    // Remove Invalid Users
    void removeBadUsers();
    // Remove Bad Songs that have no ratings
    void removeBadSongs();
    // Calculate Normalized Values
    void calculateNormalized();
    // Calculate Similarity Values
    void calculateSimilarity();
    // Calculate Predicitions for User Ratings
    void calculatePrediction();
    // Find most similar User to user passed
    double findMostSimilarUser(unsigned int songIndex, int userIndex);
    double calculateDistance (vector<double>& vec, vector<double>& vec2);
    vector<double> calculateAverage (vector<vector<double> >& vec);
    void clusterSongs (unsigned int k, int n);
    // Format to print
    void printPredicted();
    void outputFile(string file);
    
    //Calculate Data
    double getMean(vector<int>& nums);
    double getStd(vector<int>& nums, double mean);
    double getMeanDouble(vector<double>& nums);
    double getStdDouble(vector<double>& nums, double mean);
};

