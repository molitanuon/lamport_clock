
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
using namespace std;

const int PROCESSES = 5;
const int EVENTS = 25;


void printData(const int arr[PROCESSES][EVENTS], int p, int e)
{
  for (int i = 0; i < p; i++)
  {
    cout << "p" << i << ": ";                   //print the process number
    for(int j = 1; j < e; j++)
    {
      cout << arr[i][j] << " ";
    }
    cout << endl;
  }

}

int main(int argc, char *argv[])
{
  string skip;
  string data[PROCESSES][EVENTS];
  int process = 0;
  int event = 0;
  vector<int> sPosition;      //hold the sent event and their lc-value

  //read the file to get the input
      if (!argv[1])
      {
          cout << "\nERROR:\n\tNeeds filename to run: ./calculate + <Filename>\n" << endl;
          exit(EXIT_FAILURE);
      }

      //Prepare file for reading
      string filename = argv[1];
      filename = "inputFiles/" + filename;

      ifstream infile;
      infile.open(filename);
      if (!infile.is_open())
      {
          cout << "\nERROR:\n\t Filename: " << argv[1] << " doesn't exists. Please try again.\n" << endl;
          exit(EXIT_FAILURE);
      }
      else
      {
        infile >> skip;
        infile >> process;
        infile >> skip;
        infile >> event;

        cout << "\ninput: ";
        for (int i = 0; i < process; i++)         //goes thru rowes
        {
          cout << endl;
          for(int j = 0; j < event+1; j++)        //goes thru columns
          {
            infile >> data[i][j];
            cout << data[i][j] << " ";
          }
        }
        cout << endl;
      }
      infile.close();

int lc[PROCESSES][EVENTS];                             //hold the logical clock values
memset (lc, 0, sizeof(lc));
int ch = 0;
int run = process;
  cout << "\nLamport's logical clock values are: \n";
  while (run > 0)                                       //go thru the whole process and events to find lc values for r
  {
      for (int i = 0; i < process; i++)                        //goes thru rowes
      {
        for(int j = 1; j < event+1; j++)                   //goes thru columns
        {
          if (lc[i][j] != 0 || data[i][j] != "NULL")
          {
            if(j == 1 && data[i][j].at(0) != 'r')
            {
                lc[i][j] = 1;
                if (data[i][j].at(0) == 's')
                  {
                    ch = data[i][j].at(1);
                    sPosition.push_back(ch);
                    sPosition.push_back(lc[i][j]);
                  }
            }
            else if (data[i][j].at(0) != 'r')
            {
              lc[i][j] = lc[i][j-1]+1;
              if (data[i][j].at(0) == 's')
                {
                  ch = data[i][j].at(1);
                  sPosition.push_back(ch);
                  sPosition.push_back(lc[i][j]);
                }
            }
            else
            {
              for(int k = 0; k < sPosition.size(); k+=2)
              {
                ch = data[i][j].at(1);
                if (sPosition[k] == ch)
                {
                  if (sPosition[k+1] >= lc[i][j-1])
                      lc[i][j] = sPosition[k+1] +1;
                  else
                      lc[i][j] = lc[i][j-1] + 1;
                }
              }
            }
          }
      }
    }
    run--;
  }

//print the logical clock
printData(lc, process, event+1);

}
