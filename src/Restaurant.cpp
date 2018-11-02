//
// Created by chen on 11/2/18.
//
#include "Restaurant.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//empty ctor
Restaurant::Restaurant():open(false), tables(),menu(),actionsLog() {}

//ctor that takes config file
Restaurant::Restaurant(const std::string &configFilePath):open(true) {
ifstream inFile (configFilePath.c_str());
string line;

/*
 * zero means haven't read number of table yet
 * one means already read number, but not desctiption
 * two means already read number and description, reading menu in progress
 */
int region=0;
int numOfTable;
int tableNum=0;

try{

    while(getline(inFile,line))
    {
        if(line[0]!='#')
        {
            cout<<"the region is: "<<region<<endl;
            cout<<"the line is: "<<line<<endl;
            switch (region)
            {
                //reading numbers
                case 0: {
                    numOfTable = stoul(line);
                    region++;
                }
                    break;
                //reading description
                case 1: {

                    string delimiter=",";
                    size_t pos=0;
                    string token;
                    while((pos=line.find(delimiter))!=string::npos){
                        token=line.substr(0,pos);
                        tableNum=stoul(token);
                        tables.push_back(new Table(tableNum));
                        line.erase(0,pos+delimiter.length());
                    }
                    tables.push_back(new Table(tableNum));
                    }
                    region++;


                    break;

                //reading menu
                case 2: {
                    string delimiter=",";
                    size_t pos=0;
                    string token;
                    while((pos=line.find(delimiter))!=string::npos){
                        token=line.substr(0,pos);

                        //something;
                        line.erase(0,pos+delimiter.length());
                    }
                    token=line.substr(0,pos);
                    //something;
                }
                    break;
            }

        }

    }
    inFile.close();

}catch(const string & msg){
    if(inFile.is_open()) inFile.close();
    cout<<"Error !!";
}
}

void Restaurant::start() {
    cout<<"Restaurant Is Now Open!"<<endl;
}