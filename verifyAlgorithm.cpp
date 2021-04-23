#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

#define N 5
#define M 25

//Template function to print processes and events
template <class T>
void print(vector<vector<T> > &data, int processes, int events)
{
    cout << endl;
    for (size_t i = 0; i < processes; i++)
    {
        cout << "p" << i << ": ";
        for (int j = 0; j < events; j++)
        {
            cout << " " << data[i][j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char *argv[])
{
    //Check for valid args
    if (!argv[1])
    {
        cout << "\nERROR:\n\tNeeds filename to run: ./verify + <Filename>\n" << endl;
        exit(EXIT_FAILURE);
    }
    
    //Prepare file for reading
    string filename = argv[1];
    filename = "inputFiles/" + filename;
    int p, e; //processes and events
    ifstream myfile;
    myfile.open(filename);
    if (!myfile.is_open())
    {
        cout << "\nERROR:\n\t Filename: " << argv[1] << " doesn't exists. Please try again.\n" << endl;
        exit(EXIT_FAILURE);
    }
    else 
    {
        //read in the data the first digit is the number of processes, the second is the events
        myfile >> p >> e;
        vector<vector<int> > data(p);

        for (size_t i = 0; i < p; i++)
        {
            data[i] = vector<int>(e, 0);
            for (size_t j = 0; j < e; j++)
            {
                myfile >> data[i][j];                 
            }
        }
        myfile.close();
        cout << "\nInitial Lamport's LC-Values:";
        print(data, p, e);

        //Algorithm Verify
        // Initialize a N x M 2D string vector with all 0's this will hold the Distributed System Values.
        vector<vector<string> > systemV(N,vector<string> (M, "0"));       
        int temp;
        int processes = p;
        //apha char arrary that will be used to replace INTERNAL events (excludes s and r)
        char alphaChar[] = "abcdefghijklmnopqtuvwxyz";        
        while(processes > 0)            
        {
            int k = 1;
            int index = 0;
            //Loop that finds all 0's and EXTERNAL RECEIVE events   
            for (size_t i = 0; i < p; i++)
            {
                for (size_t j = 0; j < e; j++)
                {
                    //Label any 0's with NULL
                    if (data[i][j] == 0)
                        systemV[i][j] = "NULL";
                    //Label any r's in column j = 0 with rk
                    else if (data[i][j] != 1 && j == 0)                                              
                        systemV[i][j] = "rk";
                    //Label any r's in column j >= 1 with rk 
                    else if (data[i][j] != (data[i][j - 1] + 1) && data[i][j] != 0 && j != 0)
                        systemV[i][j] = "rk";                   
                }
            }
            //Now that we know which indicies are EXTERNAL RECEIVE events we find their corresponding SEND event   
            for (size_t i = 0; i < p; i++)
            {
                for (size_t j = 0; j < e; j++)
                {
                    if (systemV[i][j] == "rk" )
                    {
                        
                        bool RS_check = false; //checks all events for corresponding s(SEND)'s to r(RECEIVE)'s
                        bool exit_flag = false; //flag to break-out of loop
                        string k_val = to_string(k);                    
                        systemV[i][j] = "r" + k_val;                        
                        temp = data[i][j];
                        //Loop that finds the correct EXTERNAL SEND event for the current RECEIVING event
                        //Once found the same k value is assigned to both 'rk' and 'sk'
                        for (size_t i = 0; i < p && !exit_flag; i++)
                        {
                            for (size_t j = 0; j < e && !exit_flag; j++)
                            {
                                if (data[i][j] == (temp - 1))
                                {
                                    RS_check = true;
                                    systemV[i][j] = "sk";                                
                                    systemV[i][j] = "s" + k_val;
                                    k++;                                                                        
                                }
                                //Break that prevents duplicate SEND events
                                int num;
                                num = stoi(k_val);
                                if (num != k)
                                    exit_flag = true;
                            }                            
                        }
                        //At this point there is a RECEIVING event with no corresponding initial SEND event (Entire sequence is INCORRECT)
                        if (RS_check == false)
                        {
                            if (k >= 2)
                            {
                                cout << "\nINCORRECT:\n \tThere exists RECEIVING events that occur without initial SEND events.\n" << endl;
                                exit(EXIT_FAILURE);
                            } 
                        }                                                   
                    }
                }
            }
            //Final loop that replaces all the INTERNAL events with a char (in-order starting with a to z)      
            for (size_t i = 0; i < p; i++)
            {
                for (size_t j = 0; j < e; j++)
                {
                    if (systemV[i][j] == "0")
                    {
                        systemV[i][j] = alphaChar[index % 24];
                        index++;
                    }                    
                }                
            }
            processes--;
        }
        //print the results
        cout << "\nFinal Distributed System Values:";
        print(systemV, p, e);
        cout << endl;    
    }
    return 0;
}