#ifndef NIM_H
#define NIM_H

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <regex>

#define Iter unordered_map<string, int>::iterator
#define ArrIter unordered_map<string, vector<int> >::iterator
/*
    instead of doing : unordered_map<string, int>::iterator it = ...

    DO : Iter it = ...
*/

using namespace std;

class Nim
{
    private:
        ifstream fin;
        int lineNumber;

        vector<string> lines;
        unordered_map<string, int> variables;
        unordered_map<string, vector<int> > arrays;
        unordered_map<string, int> labels;
        unordered_map<string, int> keystrs;
        unordered_map<string, int> registers;

        bool isAightSyntax(string line);
        void readFromFile(string fileName);
        void translate();


        bool cmp(string& value_1, string& value_2, string comparisonOperator);
        //Precondition:
        //Postcondition:
        //**********************************************************************
        void add(string value_1, string value_2);
        //Precondition:
        //Postcondition:
        //**********************************************************************
        void subtract(string value_1, string value_2);
        //Precondition:
        //Postcondition:
        //**********************************************************************
        void division(string value_1, string value_2);
        //Precondition:
        //Postcondition:
        //**********************************************************************
        void multiply(string value_1, string value_2);
        //Precondition:
        //Postcondition:
        //**********************************************************************
        void module(string value_1, string value_2);
        //Precondition:
        //Postcondition:
        //**********************************************************************
        //=================================================================================
        //Helper functions for isAightSyntax
        //**********************************************************************
        string getKeyStr(string& line);
        //Precondition: The string provided must be proper syntax.
        //Postcondition: Returns the key symbol for that line.
        //
        //**********************************************************************
        string getParametersStr(string line);
        //Precondition: String must be in proper syntax before parsing.
        //Postcondition: The key word is removed and returns a new line that only contains the paramter(s).
        //
        //**********************************************************************
        bool validNumOfParameter(string line);
        //Precondition: String must be in proper syntax.
        //Postcondition: Validates the number of paramters that correspond with each key.
        //
        //**********************************************************************
        bool checkDataType(string param);
        //Precondition: String must be in proper syntax.
        //Postcondition: Determines if the parameters are integers or a properly named variable.
        //
        //**********************************************************************
        bool parseParam(string line); // change the name: isValidParam or something like that
        //Precondition: String must be in proper syntax.
        //Postcondition: Parses through the line and checks if the parameter is an integer or a variable.
        //
        //**********************************************************************
        //=================================================================================

        void parseParams(vector<string>& params, string& line, int start);
        //Precondition: Valid line of assembly that has at least one parameter
        //Postcondition: Puts all the parameters for a line in a vector of strings
        //
        //**********************************************************************
        //------------------------------------------------------------------------------

        void assignVal(string var, string val);
        //Precondition: Variable exists
        //Postcondition: Assigns variable/register to value passed in as val
        //
        //**********************************************************************
        //------------------------------------------------------------------------------

        void inputVal(string& var);
        //Precondition: Valid value inputed
        //Postcondition: value assigned to variable/register specified
        //
        //**********************************************************************
        //------------------------------------------------------------------------------

        void outputVal(string& str);
        //Precondition: Valid value to output
        //Postcondition:  Outputs value specified
        //
        //**********************************************************************
        //------------------------------------------------------------------------------


        void initialieVariable(string labelName, string line);
        void addLabel(string labelName);
        void createArr(string& arrName, string& sz);
        void initLabels();

    public:
        Nim();
        void initKeyStrs();
        void initRegisters();
        void compile(string fileName);
        void execute();
        void addVariable(string variableName, string value);
        void addLabel(string labelName, string lineNumber);
};
#endif
