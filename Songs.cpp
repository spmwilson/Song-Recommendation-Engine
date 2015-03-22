#include "Songs.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <stdexcept>
#include <vector>
#include <sstream>
using namespace std;

Songs::~Songs(){
    
    
}
bool Songs::countLines(string file, string file2){
    ifstream sin (file.c_str());
    ifstream rin (file2.c_str());
    string title = "";
    string rate = "";
    int ratingcount = 0;
    int songcount = 0;
    
    if (sin.is_open()){
        while (!sin.eof()){
            getline (sin, title);
            songcount++;
        }
    }
    else {
        const char * s = "Invalid File Name";
        throw s;
    }
    sin.close();
    if (rin.is_open()){
        while (!rin.eof()){
            getline (rin, rate);
            ratingcount++;
        }
        rin.close();
    }
    else {
        const char * s = "Invalid File Name";
        throw s;
    }
    if (ratingcount == songcount) {
        return true;
    }
    else {
        return false;
    }
}
bool Songs::readSongTitles(string file){
    ifstream in (file.c_str());
    songTitles.reserve(6000);
    
    while (!in.eof()) {
        string title = readSongTitle(in);
        if (title != "") {
            songTitles.push_back(title);
        }
        else {
            string title = readSongTitle(in);
            songTitles.push_back(title);
        }
    }
    if (songTitles.size() >= 1) {
        return true;
    }
    else {
        return false;
    }
}
string Songs::readSongTitle(ifstream& in){
    string title = "";
    getline(in, title);
    if (isTitleValid(title)){
        return title;
    }
    else {
        const char * s = "A song title is just spaces";
        throw s;
    }
}
bool Songs::isTitleValid(string title){
    unsigned int spaceCount = 0;
    for (unsigned int i = 0; i < title.length(); i++) {
        if (title[i] == ' ') {
            spaceCount++;
        }
    }
    if (spaceCount == title.length() && title.length() > 0) {
        return false;
    }
    return true;
}
bool Songs::readSongRatings(string file){
    ifstream in (file.c_str());
    unsigned int x = 0;
    songRatings.reserve(6000);

    while (!in.eof()) {
        string ratings = "";
        getline (in, ratings);
        stringstream convert(ratings);
        float num = 0;
        vector<int> temp;
        temp.reserve(6000);
        
        for (unsigned int i = 0; i < ratings.length(); i++) {
            if (!isdigit(ratings[i]) && ratings[i] != ' '){
                const char * s = "Rating has a non-numeric char";
                throw s;
            }
        }
        while (convert >> num) {
            if (fmod(num, 1) != 0) {
                const char * s = "Rating is a decimal";
                throw s;
            }
            if (num > 5 || num < 0) {
                const char * s = "Rating out of Range";
                throw s;
            }
            temp.push_back(num);
        }
        if (x == 0) {
            x = temp.size();
        }
        if (temp.size() != x){
            const char * s = "Number of Ratings do not Match";
            throw s;
        }
        numberOfUsers = temp.size();
        songRatings.push_back(temp);
        
    }
    return true;
}
void Songs::pullUserData(){
    vector<int> temp;
    temp.reserve(6000);
    userRatings.reserve(6000);

    for (unsigned int i = 0; i < numberOfUsers; i++) {
        for (unsigned int j = 0; j < songRatings.size(); j++) {
            temp.push_back(songRatings[j][i]);
        }
        userRatings.push_back(temp);
        temp.clear();
    }
}
void Songs::removeBadUsers(){
    bool isUserValid = false;
    // Remove users from user ratings and -- the count from numberOfUsers
    vector<vector<int> > temp;
    temp.reserve(6000);
    
    for (unsigned int i = 0; i < numberOfUsers; i++) {
        isUserValid = false;
        int y = 0;
        for (unsigned int j = 0; j < songTitles.size(); j++) {
            if (userRatings[i][j] != 0){
                y = userRatings[i][j];
            }
        }
        for (unsigned int j = 0; j < songTitles.size(); j++) {
            
            if (userRatings[i][j] == 0) {
                // Do nothing
            }
            
            else if (y != userRatings[i][j] && userRatings[i][j] != 0){
                isUserValid = true;
                
            }
        }
        if (isUserValid == true) {
            temp.push_back(userRatings[i]);
        }
    }
    userRatings = temp;
    numberOfUsers = temp.size();
    
    if (numberOfUsers == 0) {
        const char * s = "No valid users";
        throw s;
    }
}
void Songs::removeBadSongs() {
    
    bool songValid = false;
    vector<vector<int> > ratingtemp;
    ratingtemp.reserve(6000);
    vector<string> songtemp;
    songtemp.reserve(6000);
    vector<int> zerostemp;
    zerostemp.reserve(6000);
    vector<vector<int> > zeros;
    zeros.reserve(6000);
    
    
    
    for (unsigned int i = 0; i < songTitles.size(); i++) {
        for (unsigned int j = 0; j < numberOfUsers; j++) {
            zerostemp.push_back(userRatings[j][i]);
        }
        for (unsigned int a = 0; a < zerostemp.size(); a++) {
            if (zerostemp[a] != 0) {
                songValid = true;
            }
        }
        if (songValid == true){
            ratingtemp.push_back(zerostemp);
            songtemp.push_back(songTitles[i]);
            zeros.push_back(zerostemp);
        }
        ratingtemp.clear();
        zerostemp.clear();
        songValid = false;
    }
    
    songTitles = songtemp;
    vector<int> newtemp;
    newtemp.reserve(6000);
    vector<vector <int> > newtemp2;
    newtemp2.reserve(6000);
    
    for (unsigned int i = 0; i < numberOfUsers; i++) {
        for (unsigned int j = 0; j < songTitles.size(); j++) {
            newtemp.push_back(zeros[j][i]);
        }
        newtemp2.push_back(newtemp);
        newtemp.clear();
    }
    
    userRatings = newtemp2;
}
void Songs::calculateNormalized(){
    double mean = 0;
    double sd = 0;
    vector<double> usersMean;
    usersMean.reserve(6000);
    vector<double> usersSD;
    usersSD.reserve(6000);

    for (unsigned int i = 0; i < userRatings.size(); i++) {
        mean = getMean(userRatings[i]);
        sd = getStd(userRatings[i], mean);
        
        usersMean.push_back(mean);
        usersSD.push_back(sd);
    }
    vector <vector<double> > normRank;
    vector<double> temp;
    
    for (unsigned int i = 0; i < numberOfUsers; i++) {
        for (unsigned int j = 0; j < songTitles.size(); j++) {
            if (userRatings[i][j] == 0){
                temp.push_back(0);
            }
            else {
                double norm = (userRatings[i][j] - usersMean[i]) / usersSD[i];
                temp.push_back(norm);
            }
            
        }
        normRank.push_back(temp);
        temp.clear();
    }
    
    for (unsigned int i = 0; i < songTitles.size(); i++) {
        for (unsigned int j = 0; j < numberOfUsers; j++) {
            
            normMean.push_back(normRank[j][i]);
        }
        double mean = getMeanDouble(normMean);
        finalMeans.push_back(mean);
        normSongs.push_back(normMean);
        normMean.clear();
    }
    newNormSongs = normSongs;
    
}
void Songs::calculateSimilarity(){
    
    
    vector<double> final;
    final.reserve(6000);
    vector<double> test;
    test.reserve(6000);
    double x = 0;
    
    /////////////////////Users
    x = 0;

    for (unsigned int i = 0; i < numberOfUsers; i++) {
        for (unsigned int j = 0; j < numberOfUsers; j++) {
            for (unsigned int z = 0; z < songTitles.size(); z++) {
                x += normSongs[z][i] * normSongs[z][j];
            }
            final.push_back(x);
            test.push_back(x);
            x = 0;
        }
        
        test.clear();
        userSims.push_back(final);
        final.clear();
    }
    
    
}
void Songs::calculatePrediction(){
    
    int songIndex = 0;
    int userIndex = 0;
    
    vector<double> newRatings;
    newRatings.reserve(6000);
    
    
    for (unsigned int i = 0; i < numberOfUsers; i++){
        for (unsigned int j = 0; j < songTitles.size(); j++) {
            if (userRatings[i][j] == 0) {
                userIndex = i;
                songIndex = j;
                double newRating = findMostSimilarUser(userIndex, songIndex);
                newRatings.push_back(newRating);
            }
            else {
                newRatings.push_back(userRatings[i][j]);
            }
        }
        newUserRatings.push_back(newRatings);
        newRatings.clear();
    }
    
    
}
double Songs::findMostSimilarUser(unsigned int userIndex, int songIndex){
    
    double highestSim = -10000;
    //int simIndex = -1;
    vector<double> temp;
    temp.reserve(6000);
    
    
    for (unsigned int i = 0; i < numberOfUsers; i++){
        
        // Calculate the highest similar user score who also has a non zero value for the song
        // Build a vector with all of this highest similar user score
        
        if (userSims[userIndex][i] > highestSim && i != userIndex && userRatings[i][songIndex] != 0) {
            highestSim = userSims[userIndex][i];
        }
    }
    if (highestSim == -10000) {
        // Error case for all users rated zero for a song
    }
    
    // Build array with all users with that similarity score
    
    for (unsigned int i = 0; i < numberOfUsers; i++) {
        
        if (userSims[userIndex][i] == highestSim && userRatings[i][songIndex] != 0) {
            temp.push_back(normSongs[songIndex][i]);
        }
    }
    double sum = 0;
    double average = 0;
    //Average all the normalized numbers in temp
    for (unsigned int i = 0; i < temp.size(); i++) {
        sum += temp[i];
    }
    if (temp.size() == 0) {
        cout << "ERROR" << endl;
        cout << "SONG INDEX: " << songIndex << endl;
    }
    
    average = sum / temp.size();
    
    newNormSongs[songIndex][userIndex] = average;
    
    double newNormValue = average;
    
    
    // Calculate new rating for user who has 0
    
    double mean = getMean(userRatings[userIndex]);
    double sd = getStd(userRatings[userIndex], mean);
    
    double newRating = (newNormValue * sd) + mean;
    
    int finalRating = round(newRating);
    if(finalRating == 0) {
        finalRating = 1;
    }
    if(finalRating == 6) {
        finalRating = 5;
    }
    
    return finalRating;
}
double Songs::calculateDistance (vector<double>& vec, vector<double>& vec2){
    
    double distance = 0;
    
    for (unsigned int i = 0; i < vec.size(); i++) {
        //distance += pow(vec[i] - vec2[i], 2);
        distance += (vec[i] - vec2[i])* (vec[i] - vec2[i]);
    }
    
    return sqrt(distance);
}
vector<double> Songs::calculateAverage (vector<vector<double> >& vec){
    
    vector<double> averageRankings;
    averageRankings.reserve(6000);
    double average = 0;
    
    if (vec.size() == 1) {
        return vec[0];
    }
    
    for (unsigned int i = 0; i < vec[0].size(); i++) {
        for (unsigned int j = 0; j < vec.size(); j++) {
            average += vec[j][i];
        }
        average = average / vec.size();
        averageRankings.push_back(average);
        average = 0;
    }
    
    
    return averageRankings;
}
void Songs::clusterSongs (unsigned int k, int n){
    
    vector<vector<double> > clustersBackup;
    clustersBackup.reserve(6000);
    // Initialize k-means vectors up to as many cluster centers as K
    int ktemp = k;
    vector<vector<double> > clusters;
    clusters.reserve(6000);
    vector<vector<double> > kMeans;
    kMeans.reserve(6000);
    
    for (unsigned int i = 0; i < songTitles.size() && ktemp > 0; i++, ktemp--) {
        kMeans.push_back(newNormSongs[i]);
        vector<double> newCenter;
        clusters.push_back(newCenter);
    }
    
    ktemp = k;
    vector<double> kDistances;
    kDistances.reserve(k);
    double lowestDist = 1000;
    int lowestIndex = 0;
    
    
    // Run the Loop 10 times
    for (unsigned int x = 0; x < 10; x++) {
        
        
        // For Every Song
        for (unsigned int i = 0; i < songTitles.size() ; i++) {
            
            // Compute distance between song's ranking vector and each K cluster centers
            lowestDist = 1000;
            for (unsigned int j = 0; j < kMeans.size(); j++) {
                double dist = calculateDistance(newNormSongs[i], kMeans[j]);
                kDistances.push_back(dist);
            }
            
            // Get the shortest of those distances
            
            for (unsigned int s = 0; s < kDistances.size(); s++) {
                if (kDistances[s] < lowestDist) {
                    lowestDist = kDistances[s];
                    lowestIndex = s;
                }
            }
            
            // Add the song to the closest cluster
            clusters[lowestIndex].push_back(i);
            
            // Clear song to cluster distances
            kDistances.clear();
            
            
        }
        
        // Update the cluster center vector to be the average of the ranking vectors
        
        vector<vector<double> > clustersTemp;
        clustersTemp.reserve(6000);
        vector<vector<double> > kMeansTemp;
        kMeansTemp.reserve(6000);
        
        for (unsigned int c = 0; c < k ; c++) {
            
            // If the cluster has one or more songs
            
            if (clusters[c].size() != 0) {
                
                for (unsigned int f = 0; f < clusters[c].size(); f++) {
                    vector<double> normTemp;
                    normTemp = newNormSongs[clusters[c][f]];
                    clustersTemp.push_back(normTemp);
                }
                vector<double> t = calculateAverage(clustersTemp);
                kMeansTemp.push_back(t);
                clustersTemp.clear();
            }
            
            // If the cluster has no songs
            
            else {
                kMeansTemp.push_back(kMeans[c]);
            }
            
        }
        //Set temp variable back to our cluster centers with new average values
        kMeans = kMeansTemp;
        kMeansTemp.clear();
        
        //Create a backup for the last iteration to save the last clusters
        clustersBackup = clusters;
        //Clear the clusters to reassign through the next iteration
        clusters.clear();
        int ktemp = k;
        
        // Added the re-creation of k clusters because of the clear
        for (unsigned int i = 0; i < songTitles.size() && ktemp > 0; i++, ktemp--) {
            vector<double> newCenter;
            clusters.push_back(newCenter);

        }
    }
    
    // Print song titles for N cluster minus 1 for index
    for (unsigned int i = 0; i < clustersBackup.at(n-1).size(); i++) {
        printSongTitles.push_back(songTitles.at(clustersBackup.at(n-1).at(i)));
    }
}
void Songs::outputFile(string file){
    ofstream out(file.c_str());
    
    vector<string>::iterator it = printSongTitles.begin();
    for (it = printSongTitles.begin(); it != printSongTitles.end(); it++){
        out << *it << endl;
    }
}
double Songs::getMean(vector<int>& nums){
    double mean = 0;
    unsigned int a = 0;
    vector<int>::iterator it = nums.begin();
    for (it = nums.begin(); it != nums.end(); it++){
        if (*it == 0) {
            a++;
        }
        else {
            mean += *it;
        }
    }
    mean = mean / (nums.size()-a);
    return mean;
}
double Songs::getStd(vector<int>& nums, double mean){
    double sum_deviation = 0;
    double sd = 0;
    unsigned int a = 0;
    vector<int>::iterator it = nums.begin();
    for (it = nums.begin(); it != nums.end(); it++){
        if (*it == 0) {
            a++;
        }
        else{
            sum_deviation += (*it - mean) * (*it - mean);
        }
    }
    
    sd = sqrt(sum_deviation/(nums.size()-1-a));
    
    return sd;
}
double Songs::getMeanDouble(vector<double>& nums){
    double mean = 0;
    unsigned int a = 0;
    vector<double>::iterator it = nums.begin();
    for (it = nums.begin(); it != nums.end(); it++){
        if (*it == 0) {
            a++;
        }
        else {
            mean += *it;
        }
    }
    
    mean = mean / (nums.size()-a);
    return mean;
}
double Songs::getStdDouble(vector<double>& nums, double mean){
    double sum_deviation = 0;
    double sd = 0;
    unsigned int a = 0;
    vector<double>::iterator it = nums.begin();
    for (it = nums.begin(); it != nums.end(); it++){
        if (*it == 0) {
            a++;
        }
        else{
            sum_deviation += (*it - mean) * (*it - mean);
        }
    }
    
    sd = sqrt(sum_deviation/(nums.size()-1-a));
    
    return sd;
}

