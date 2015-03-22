#include <iostream>
#include <fstream>
#include "Songs.h"
using namespace std;

int main(int argc, char* argv[])
{
    try {
        Songs * data = new Songs();
    
        if (argc > 6)
        {
        	const char * s = "Too many arguments";
            throw s;
        }
        if(data->countLines(argv[1], argv[2])){
            data->readSongTitles(argv[1]);
            data->readSongRatings(argv[2]);
            data->pullUserData();
            data->removeBadUsers();
            data->removeBadSongs();
            data->calculateNormalized();
            data->calculateSimilarity();
            data->calculatePrediction();
            unsigned int k = atoi(argv[3]);
            unsigned int n = atoi(argv[4]);
            if (k < 1)
            {
            	const char * s = "Must have at least one cluster";
            	throw s;
            }
            if (n < 1 || n > k)
            {
            	const char * s = "N Value out of range";
            	throw s;
            }
            data->clusterSongs(k,n);
            data->outputFile(argv[5]);
            delete data;
        }
        else {
            const char * s = "Line Mismatch";
            throw s;
        }
    }catch (const char * s) {
        cout << s;
        return -1;
    }
    
    return 0;
}
