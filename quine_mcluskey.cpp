#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>

#include "main_functions.hpp"

int main(int argc, char *argv[]){

    //defualt files
    std::string input_file = "C:/Users/harry/OneDrive/Documents/301/Problem03/test_input.txt";

    std::ifstream file_read(input_file);

    //debugging
    if (file_read.fail())
    {
        std::cout << "File input error" << std::endl;
        return EXIT_FAILURE;
    }
 

    //read in data
    std::string line;

    //create vector to store our sub strings and minterms
    std::vector<std::string> substrings;
    std::vector<int> minterms;
    
    while (getline(file_read, line))
    {
        //parse based off commas, minterm idx comes before comma
        int comma_idx = line.find(",");

        //first identify the first minterm string 
        substrings.push_back(line.substr(0,comma_idx));
        int parentheses_idx  = substrings[0].find("(");
        std::string first_term = substrings[0].substr(parentheses_idx+1,comma_idx); //get string of num
        minterms.push_back(std::stoi(first_term));

        //get length of the rest
        substrings.push_back(line.substr(comma_idx+1,line.length()));
        int length = substrings[1].length();
        int terms_left = length/2;
        std::string term;

        //for loop designed to peel off each term and store it
        for(int i=1;i<terms_left;i++){ //length/2 bc of commas

            //if we are at final term
            if(i==terms_left){
                parentheses_idx = substrings[i].find(")");
                term = substrings[i].substr(0,parentheses_idx);
                minterms.push_back(std::stoi(term));
                continue;
            }

            //else 

            //find first comma in substr
            comma_idx = substrings[i].find(",");

            //get term ,5, and add to minterms vector
            term = substrings[i].substr(0,comma_idx);
            minterms.push_back( std::stoi(term));

            //create new sub str 
            substrings.push_back(substrings[i].substr(comma_idx+1,line.length()));
        }

    }

   
    //declare vector of groups for first layer
    std::vector<group> layer_one(7);

    //declare vector of strings to represent binary values
    std::vector<binary_term> bin_rep;

    //convert to binary, count num of ones, and group
    int num_ones;
    binary_term binary;

    //testing variables 
    std::string test;

    for(int i=0;i<minterms.size();i++){

        //bin rep is a vector of all the binary values
        test = decToBinary(minterms[i]);
        binary.value = test;
        bin_rep.push_back(binary);
        
        //get number of ones in term
        num_ones = 0;
        for(int j=0;j<6;j++){
            if(bin_rep[i].value.at(j) == '1'){
                num_ones++;
            }
        }

        //assign term to group based on number of ones
        switch (num_ones)
        {
        case 0:
            layer_one[0].values.push_back(bin_rep[i]);
            break;
        case 1:
            layer_one[1].values.push_back(bin_rep[i]);
            break;
        case 2:
            layer_one[2].values.push_back(bin_rep[i]);
            break; 
        case 3:
            layer_one[3].values.push_back(bin_rep[i]);
            break;
        case 4:
            layer_one[4].values.push_back(bin_rep[i]);
            break;
        case 5:
            layer_one[5].values.push_back(bin_rep[i]);
            break;
        default:
            layer_one[6].values.push_back(bin_rep[i]);
            break;
        }

    } 
    
    //declare final terms vector, these are the terms that will be used to generate function
    std::vector<binary_term> final_terms;

    //runs all possible iterations of comparing group and returns a vector of unsimplifiable terms
    final_terms = run_iterations(final_terms,layer_one);

    //removes any incorrect that may have been generated
    final_terms = remove_null_values(final_terms);

    //from each term, figure out which minterms they cover
    for(int i=0;i<final_terms.size();i++){
        //set minterms covered member based off of the binary value
        //for example, input "x00x" returns vector of 0,1,8,9
        final_terms[i].minterms_covered = getMintermsFromTerm(final_terms[i].value);
    }

    bool covered; //boolean flag to see if all minterms in a term are covered elsewhere
    std::vector<binary_term> final_simplified_terms;
    for(int i=0;i<final_terms.size();i++){
        covered = minterms_covered(final_terms[i],final_terms);

        //if not all minterms are covered, term is necessary
        if(covered == false){
            final_simplified_terms.push_back(final_terms[i]);
        }

    }

    //write final simplified terms to output file
    std::ofstream output_file("C:/Users/harry/OneDrive/Documents/301/Problem03/output.txt");
    if (!output_file) {
        std::cout << "Error: Could not open output file." << std::endl;
        return 1;
    }
    std::string temp; //placeholder
    output_file << "f = "; 
    for (size_t i = 0; i < final_simplified_terms.size(); ++i) {
        temp = final_simplified_terms[i].value;
        output_file << temp ;
        if (i != final_simplified_terms.size() - 1) {
            output_file << " + ";
        }
    }


    
}

