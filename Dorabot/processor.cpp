#include <iostream>
#include <stdlib.h> // rand
#include <thread>
#include <fstream>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <iomanip> // to limit number of decimals
#include <set>
#include <chrono>
#include <omp.h>
#include <stdio.h>
#include <algorithm>

#define MAX_BUFFER_SIZE 2048

using namespace std;

/**
	OpenMP is used to speed up the calls of expensiveFunc
	as we don't need the function to finish on one string to process another string

	Usage: up to the number of bytes read, should do it on the go but limited time did not allow me to find a solution fast enough
	
**/

int delay;

string expensiveFunc(const string string_to_process){
	chrono::duration<int> chrono_delay(delay);
	this_thread::sleep_for(chrono_delay);
	// typically if we were running a function like SHA256 we would return the result of this function
	return string_to_process;
}

void display_status_bar(const uint string_written, const uint strings_to_write){
	float number_strings(string_written);
	cout << "Task progress: " << string_written + 1 << " written out of " << strings_to_write << " - " << (number_strings/strings_to_write) * 100 << " %" << endl;
}

void write_to_file(const string& buffer){
	// to write the content of the buffer at the end of the file
	std::ofstream out("output_processor.txt", ios::out | ios::app);
	out << buffer;
}

int main(int argc, char** argv){

	std::ofstream out("output_processor.txt", ios::out);
	out.close();

	// to print only two decimals
	std::cout << std::fixed;
    std::cout << std::setprecision(2);

	srand (time(NULL));

	delay = stoi(argv[1]);

	const string input_file = argv[2];

	ifstream input_file_handler(input_file, ios::in);

	// to store all the strings contained in the input file
	vector<string> strings;

	string next_string;

	while(getline(input_file_handler, next_string)){

		strings.push_back(next_string);
		
	}

	// we can operate the function expensiveFunc in parallel and it is thread safe as each call is only modifying the string 
	// it is called with

	#pragma omp parallel for num_threads(8)
		for(size_t i = 0; i < strings.size(); i++)
			strings[i] = expensiveFunc(strings.at(i));


	sort(strings.begin(), strings.end());

	uint current_string_index(0);

	string buffer;

	buffer.reserve(MAX_BUFFER_SIZE);

	while(current_string_index < strings.size()){

		buffer.append(strings[current_string_index]);
		buffer.append(1, '\n');

		if(buffer.length() + strings.at(current_string_index).length() + 1 >= MAX_BUFFER_SIZE){

			display_status_bar(current_string_index, strings.size());

			write_to_file(buffer);
			
			buffer.resize(0);
		}

		if(current_string_index == strings.size()-1){
			write_to_file(buffer);
			display_status_bar(current_string_index, strings.size());
		}	

		current_string_index++;	
	}

	return 0;

}