#include "nim.h"


Nim::Nim()
{
    initKeyStrs();
    initRegisters();
    initLabels();
}

void Nim::initLabels() {

}
void Nim::initRegisters()
{
    registers.emplace("AC_REG", -1);
    registers.emplace("CMP_REG", -1);
    registers.emplace("EAX_REG", -1);
    registers.emplace("EBX_REG", -1);
    registers.emplace("ECX_REG", -1);
    registers.emplace("EDX_REG", -1);
    registers.emplace("ESI_REG", -1);
    registers.emplace("EBP_REG", -1);
    registers.emplace("ESP_REG", -1);
    registers.emplace("NICK_REG", -1);
    registers.emplace("MARIO_REG", -1);
    registers.emplace("ISRAEL_REG", -1);


}

void Nim::initKeyStrs()
{
    //First -> key, Second -> parameter
    keystrs.emplace("+", 2);
    keystrs.emplace("-", 2);
    keystrs.emplace("*", 2);
    keystrs.emplace("/", 2);
    keystrs.emplace("%", 2);
    keystrs.emplace("<-", 1);
    keystrs.emplace("->", 1);
    keystrs.emplace("=", 2);
    keystrs.emplace("HALT", 0);
    keystrs.emplace("#", 1);
    keystrs.emplace("[]", 2);
    keystrs.emplace("JMP", 1);
    keystrs.emplace("CMP", 2);
    keystrs.emplace("JiGT", 1);
    keystrs.emplace("JiLT", 1);
    keystrs.emplace("JiET", 1);
    keystrs.emplace("CLEAR_RG", 1);
    keystrs.emplace(":)", 1);


}

void Nim::readFromFile(string fileName)
{
    string line;
    ifstream fin(fileName.c_str());
    while(fin.good())
    {
        getline(fin, line);
        if(isAightSyntax(line))
        {
            lines.push_back(line);
        }
    }
}

void Nim::compile(string fileName)
{
    readFromFile(fileName);
}

void Nim::execute()
{
    translate();
}

//============================================================================================
bool Nim::isAightSyntax(string line)
{
    regex emptyLine_regex("[ ]+");
    string newLine = line;

    //Clearing any white space infront.
    if(newLine[0] == ' ')
    {
        int numOfSpaces = 0;
        for (int i = 0; i < newLine.length(); ++i)
        {
            if(newLine[i]!= ' ')
            {
                break;
            }
            numOfSpaces++;
        }
        if(numOfSpaces == newLine.length())
            return true;
        newLine = newLine.substr(numOfSpaces, newLine.length());
    }

    //Removing any comments from the line.
    if(line.find(":)") != string::npos)
    {
        newLine = line.substr(0, line.find(":)"));
        if (newLine.empty())
        {
            return true;
        }
    }

    string keyword = getKeyStr(newLine);
    //Checking if the line is empty
    if(line.empty() || keyword == "HALT")
    {
        //return true
        cout << "Line good" << endl;
        return true;
    }
    //Checking if the key word in the line is valid.
    if(keystrs.find(keyword) == keystrs.end()){
        cout << "Key word not Found @ line: " << endl;
        return false;
    }

    if(regex_match(newLine, emptyLine_regex))
    {
        return true;
    }
    //Checking if the line is a valid syntax.
    if(!validNumOfParameter(newLine))
    {
        cout << "Parameters are not valid @ line: " << endl;
        return false;
    }



    cout << "Line good" << endl; //return true;
    return true;

}//end of isAightSyntax

//Helper Function
string Nim::getKeyStr(string& line)
{
    if(line.find(" ") == string::npos)
        return line;

    return line.substr(0, line.find(" "));
}
//Helper Function
bool Nim::validNumOfParameter(string line)
{
    bool isValid = false;
    string keyWord = getKeyStr(line);
    string param = getParametersStr(line);
    unordered_map<string, int> keystrs;

    regex paramaters_two_regex("[a-zA-Z0-9_]+, [a-zA-Z0-9_]+");
    regex parameter_one_regex("[a-zA-Z0-9_]+");

    // cout << line << endl;
    // cout << keyWord << endl;
    // cout << param << endl;


    Iter it = keystrs.find(keyWord);
    //cout << it->second << endl;
    switch(it->second)
    {
        case 0:
        case 1:
            if(!regex_match(param, parameter_one_regex))
            {
                cout << "Invalid parameter" << endl;
                return false;
            }
            if(keyWord == "#")
            {
                //add to label map. keyStrs.emplace(param, -1)
            }
            return checkDataType(param);
            break;
        case 2:
            if(!regex_match(param, paramaters_two_regex))
            {
                cout << "Invalid parameter(s)." << endl;
                return false;
            }
            if(keyWord == "=")
            {
                //call the funtion addVariable(variable, value); TODO
                if(checkDataType(param))
                {

                    return (param[0] > 64 && param[0] < 91) || (param[0] > 96 && param[0] < 123);
                }
            }
            return checkDataType(param);
            break;
        default:
            cout << "No operation." << endl;
            break;
    }


    return true;
}
//Helper Function
string Nim::getParametersStr(string line)
{

    line = line.substr(line.find(" ") + 1, line.length());
    int i = 0;
    //removes any white space after the parameters.
    while (line[line.length() - 1 - i] == ' ') {
        i++;
    }
    line = line.substr(0, line.length() - i);
    return line;
}
//Helper Function
bool Nim::checkDataType(string param)
{
    // TODO: must check for registers.
    string first = "";
    string second = "";
    if(param.find(",") != string::npos)
    {
        first = param.substr(0, param.find(","));
        second = param.substr(param.find(" ") + 1, param.length());


        return parseParam(first) && parseParam(second);

    }
    else{
        return parseParam(param); // && variables.find("param")   TODO
    }
}
//Helper Function
bool Nim::parseParam(string line)
{
    regex letters_regex ("[a-zA-Z_]+");
    regex number_regex ("[0-9]+");
    if((line[0] > 96 && line[0] < 123 || line[0] == '_') || (line[0] > 64 && line[0] < 91))
    {
        if(registers.find(line) != registers.end()) return true; //TODO
        else if(variables.find(line) != variables.end()) return true;
        return regex_match(line, letters_regex);// && variables.find(line) != variables.end();   //TODO: check if the variable is stored in the hashmap.
    }
    else{
        if(registers.find(line) != registers.end()) return true; //TODO
        else if(variables.find(line) != variables.end()) return true;
        //TODO: check if the label is stored in the hashmap.
        return regex_match(line, number_regex);
    }
}//end of parseParam
//============================================================================================
void Nim::initialieVariable(string labelName, string line)
{
    Iter it;
    it = labels.find(labelName);
    if(it != labels.end())
    {
        int line = lineNumber;//stoi(lineNumber);
        it->second = line;
    }
}
void Nim::addLabel(string labelName)
{
    labels.emplace(labelName, -1);
    /*
    else
    {
        int line = stoi(lineNumber);
        it.emplace(labelName, line);
    }
    */
}
/*
void Nim::addVariable(string variableName, string value)
{
    Iter it = variables.find(variables);
    Iter it_2 = registers.find(value);

    if(it == variables.end() && it_2 == registers.end())
    {
        //The variable is not in the table
        //The register is not in the table
        int intergerValue = stoi(value);
        variables.emplace(variableName, intergerValue);
    }
    else if(it == variables.end() && it_2 != registers.end())
    {
        //The variable is not in the table
        //The register is in the table
        variables.emplace(variableName, it_2->second);
    }
    else if(it != variables.end() && it_2 == registers.end())
    {
        int intergerValue = stoi(value);
        it->second = intergerValue;
    }
    else
    {
        it-second = it_2-second;
    }

}

*/


// string Nim::getKeyStr(string& line)
// {
//     return line.substr(0, line.find(" "));
// }

void Nim::parseParams(vector<string>& params, string& line, int start) {
    string param = "";

    for(int tx = start + 1; tx < line.length(); tx++)
    {
        if(line.substr(tx, 2) == ":)")
            break;
        else if(isspace(line[tx]) || line[tx] == ',') {
            if(param != "") {
                params.push_back(param);
                param = "";
            }
            continue;
        }
        else {
            param += line[tx];
        }

    } // end for

    if(param != "") {
        params.push_back(param);
    }
}
void Nim::add(string value_1, string value_2)
{

    //converting values to string to check if they are variables
    int val_1, val_2;

    Iter it = variables.find(value_1);
    if(it != variables.end())
    {
        val_1 = it->second;
    }
    else
    {
        val_1 = stoi(value_1);
    }

    it = variables.find(value_2);
    if(it != variables.end())
    {
        val_2 = it->second;
    }
    else
    {
        val_2 = stoi(value_2);
    }

    registers["AC_REG"] = (val_1 + val_2);
}
void Nim::module(string value_1, string value_2)
{
    int valA;
    int valB;
    Iter it = variables.find(value_1);

    if(it != variables.end()) {
        valA = it->second;
    }
    else {
        valA = stoi(value_1);
    }

    it = variables.find(value_2);

    if(it != variables.end()) {
        valB = it->second;
    }
    else {
        valB = stoi(value_2);
    }

    registers["AC_REG"] = (valA % valB);
}

void Nim::multiply(string value_1, string value_2) {
    int valA;
    int valB;
    Iter it = variables.find(value_1);

    if(it != variables.end()) {
        valA = it->second;
    }
    else {
        valA = stoi(value_1);
    }

    it = variables.find(value_2);

    if(it != variables.end()) {
        valB = it->second;
    }
    else {
        valB = stoi(value_2);
    }

    registers["AC_REG"] = (valA * valB);
}

void Nim::division(string value_1, string value_2) {
    int valA;
    int valB;
    Iter it = variables.find(value_1);

    if(it != variables.end())
    {
        valA = it->second;
    }
    else
    {
        valA = stoi(value_1);
    }

    it = variables.find(value_2);

    if(it != variables.end())
    {
        valB = it->second;
    }
    else
    {
        valB = stoi(value_2);
    }

    if(valB == 0) {
        cout << "Error: Division by zero\n";
        exit(1);
    }

    registers["AC_REG"] = (valA / valB);
} // end division

void Nim::subtract(string value_1, string value_2)
{
    int valA;
    int valB;
    Iter it = variables.find(value_1);

    if(it != variables.end())
    {
        valA = it->second;
    }
    else
    {
        valA = stoi(value_1);
    }

    it = variables.find(value_2);

    if(it != variables.end())
    {
        valB = it->second;
    }
    else
    {
        valB = stoi(value_2);
    }

    registers["AC_REG"] = (valA - valB);
} // end subtract()
void Nim::assignVal(string var, string val) {

    if(var[0] == '[')
    {

        int nextSpace = var.find(" ");
        string arrName = var.substr(1, nextSpace - 1);
        nextSpace = var.find(" ", nextSpace + 1);
        //nextSpace = var.find(nextSpace + 1, " ");
        // int len = var.find(nextSpace + 1, "]") - nextSpace - 1;
        // int index = stio( var.substr(nextSpace + 1, len) ) - 1;
        int endBracket = var.find("]");
        int endOfIndex = endBracket - 1;
        int startOfIndex = nextSpace;
        string index = var.substr(nextSpace + 1, (endOfIndex - startOfIndex));

        Iter variablesIterator = variables.find(index);
        ArrIter arrayIterator = arrays.find(arrName);

        if(arrayIterator != arrays.end())
        {
            //index
            if(variablesIterator != variables.end())
            {
                int i = variablesIterator->second;
                int value = stoi(val);

                if((arrayIterator->second.size() - 1) >= i)
                    arrayIterator->second[i] = value;
            }
            else
            {
                int i = stoi(index);
                int value = stoi(val);

                if((arrayIterator->second.size() - 1) >= i)
                    arrayIterator->second[i] = value;
            }
        }
        /*
        if(arrays.find(arrName) != arrays.end() && index < arrays[arrName].size()) {
            arrays[arrName][index] = val;
        }
        else
        {
            cout << "Segmentation fault\n";
            exit(1);
        }
        */
        return;
    }
    Iter it = variables.find(var);
    Iter it_2 = registers.find(val);

    if(it == variables.end() && it_2 == registers.end())
    {
        //The variable is not in the table
        //The register is not in the table
        int intergerValue = stoi(val);
        variables.emplace(var, intergerValue);
    }
    else if(it == variables.end() && it_2 != registers.end())
    {
        //The variable is not in the table
        //The register is in the table
        variables.emplace(var, it_2->second);
    }
    /*
    else if(it != variables.end() && it_2 == registers.end())
    {
        int intergerValue = stoi(val);
        it->second = intergerValue;
    }
    else
    {
        it->second = it_2->second;
    }
    */

    /*
    else if(variables.find(var) != variables.end())
        variables[var] = stoi(val);
    else
        registers[var] = stoi(val);
    */
} // end assignVal

void Nim::inputVal(string& var) {

    int num;
    cin >> num;

    if(!cin) {
        cout << "Incorrect value\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        exit(1);
    }

    if(variables.find(var) != variables.end())
        variables[var] = num;
    else
        registers[var] = num;
} // endinputVal

void Nim::outputVal(string& str) {
    if(variables.find(str) != variables.end())
        cout << endl << variables[str] << endl;
    else if(registers.find(str) != registers.end())
        cout << endl << registers[str] << endl;
    else
        cout << endl << stoi(str) << endl;
} // end outputVal()

void Nim::translate()
{
    string line = "";
    // Command to perform
    //
    string keystr = "";
    Iter it;
    // num of params associated with each keystr
    //
    int numOfParams = 0;
    // control variable to find start index of keystr
    //
    int jx;
    string jumpCondition;

    // main for loop to iterate thru all lines of the file, one at a time
    //
    for(int ix = 0; ix < lines.size(); ix++) {
        line = lines[ix];                           // get the next line to read
        jx = 0;                                     // reset to zero aka beginning of line
        for(; jx < line.length(); jx++) {           // for loop to iterate thru any beginning white space
            if(isspace(line[jx]))
                continue;
            else
                break;
        } // end for

        if(line.substr(jx, 2) == ":)")              // before reading command, check if line is a comment
            continue;                               // skip to next line if so

        keystr = getKeyStr(line);                   // get the key string from the current line

        it = keystrs.find(keystr);                  // find/check/get the key string from our map

        if(it != keystrs.end()) {
            numOfParams = it->second;               // get the num of params for that key string
            vector<string> params(numOfParams);     // array to hold all the params

            parseParams(params, line, jx + keystr.length() + 1); // get all params into our array


            switch(keystr[0]) {
                case 'H':
                {
                    return;
                 }break;
                case '+':
                {
                 add(params[0], params[1]);
                }break;
                case '-':
                {
                    if(keystr.length() == 1)
                    {//  check if a "-" not a "->"
                        subtract(params[0], params[1]);
                    }
                    else
                    {
                        inputVal(params[0]); // input val "->"
                    }
                } break;
                case '*':
                {
                    multiply(params[0], params[1]);
                }break;
                case '/':
                {
                    division(params[0], params[1]);
                }break;
                case '%':
                {
                    module(params[0], params[1]);
                }break;
                case '<':
                {
                    outputVal(params[0]);
                }break;
                case '=':
                {
                    continue;//assignVal(params[0], params[1]);
                }break;
                case 'c':
                {
                    jumpCondition = getKeyStr(lines[ix + 1]);
                    registers["CMP_REG"] = cmp(params[0], params[1], jumpCondition);
                }break;
                case 'J':
                {
                    //keystrs = jump label
                    /*
                    Access label that is associated with line number
                    change the ix to that value;
                    so "ix" it uhm the control variable of that line uhm
                    */
                    string labelName = keystr.substr(4, keystr.length()-1);
                    Iter it = labels.find(labelName);
                    if(it != labels.end())
                    {
                        ix = it->second;
                    }

                }break;
                case '#':   continue;//labels[params[0]] = ix;             // assign the line index of this label
                    break;                                                 // for jump purposes
                case '[':   continue;//createA//rr(params[0], params[1]);
                    break;
            } // end switch

        } // end if

    } // end outer for

} // end translate()

void Nim::createArr(string& arrName, string& sz)
{
    ArrIter it = arrays.find(arrName);
    int size = stoi(sz);

    if(it == arrays.end()) {
        //vector<int> v(size);
        arrays.insert(make_pair(arrName, vector<int>(size)));
    }
} // end createArr()

bool Nim::cmp(string& value_1, string& value_2, string comparisonOperator)
{
    //converting values to string to check if they are variables
    int val_1, val_2;

    Iter it = variables.find(value_1);
    if(it != variables.end())
    {
        val_1 = it->second;
    }
    else
    {
        val_1 = stoi(value_1);
    }

    it = variables.find(value_2);
    if(it != variables.end())
    {
        val_2 = it->second;
    }
    else
    {
        val_2 = stoi(value_2);
    }

    if(comparisonOperator == "JiET")
    {
        registers["CMP_REG"] = (val_1 == val_2);
    }
    else if(comparisonOperator == "JiGT")
    {
        registers["CMP_REG"] = (val_1 > val_2);
    }
    else if(comparisonOperator == "JiLT")
    {
        registers["CMP_REG"] = (val_1 < val_2);
    }
    else
    {
        cout << "Bad input\n";
        return false;
    }
} // end cmp()

/*
int Nim::divison(int value_1, int value_2)
{
    return value_1 / value_2;
}

int Nim::multiply(int value_1, int value_2)
{
    return value_1 * value_2;
}

int Nim::module(int value_1, int value_2)
{
    return value_1 % value_2;
}
*/
