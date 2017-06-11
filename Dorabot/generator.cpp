
#include <iostream>
#include <stdlib.h> // rand
#include <thread>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <iomanip> // to limit number of decimals

/**
	Memory usage: There is a trade-off to make between memory usage and speed of this algorithm, indeed as you increase the size of 
	MAX_BUFFER_SIZE below, less system calls will be made and the program should run faster. Although as you do so you increase the size of the 
	buffer that you store on the stack. 

	Multi-threading is used here because we can notice that when a buffer has reached its maximal capacity, another thread can write into
	the file as the buffer is being refilled again for the next step

	Note: argv[2] is not used but it is meant to handle multiple encoding systems for example to treat chinese characters, had I had more time 
	I would have made sure that the strings were encoded in the encoding system given as a parameter
	
**/

using namespace std;

#define MAX_BUFFER_SIZE 2048

mutex mtx;
condition_variable cv;
string buffer;
bool ready = false;
bool bytes_written = false;
bool job_finished = false;

string generate_random_string(const int length){
	static const char characters_allowed[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    string generated_string;
    generated_string.reserve(length);

    for (int i = 0; i < length; ++i)
    	generated_string.append(1, characters_allowed[rand() % (sizeof(characters_allowed) - 1)]);

    return generated_string;
}

void write_to_file(){

	while(!job_finished){
		// to write the content of the buffer at the end of the file
		std::unique_lock<std::mutex> locker(mtx);
	    cv.wait(locker, []{ return ready; });
		std::ofstream out("output.txt", ios::out | ios::app);
		cout << "writing " << buffer.length() << "bytes" << endl;
		out << buffer;
		bytes_written = true;
		ready = false;
		locker.unlock();
	    cv.notify_one();
	}
}

void display_status_bar(const uint bytes_generated, const uint bytes_to_generate){
	float bytes_as_float(bytes_generated);
	cout << "Task progress: " << bytes_generated << " generated out of " << bytes_to_generate << " - " << (bytes_as_float/bytes_to_generate) * 100 << " %" << endl;
}

int main(int argc, char** argv){

	// clears the file in case the program is run twice in a row so that the strings don't add up
	std::ofstream out("output.txt", ios::out);
	out.close();

	// to print only two decimals
	std::cout << std::fixed;
    std::cout << std::setprecision(2);

	thread writer(write_to_file);

	srand (time(NULL));

	const int string_length_min = stoi(argv[1]);

	const int string_length_max = stoi(argv[2]);

	const string encoding = argv[3];

	const uint bytes_to_generate = stoi(argv[4]);

	// the number of bytes generated so far
	uint bytes_generated(0);

	// a random generated length for the next string
	int string_length(0);

	buffer.reserve(MAX_BUFFER_SIZE);

	while(bytes_generated < bytes_to_generate){
		
		string_length = rand() % (string_length_max-string_length_min) + string_length_min;

		string new_string = generate_random_string(string_length);

		if(buffer.length() + new_string.length() + 1 >= MAX_BUFFER_SIZE){

			if(bytes_generated + buffer.length() > bytes_to_generate)
				buffer.resize(bytes_to_generate - bytes_generated);

			bytes_generated += buffer.length();

			display_status_bar(bytes_generated, bytes_to_generate);

			{
		        std::unique_lock<std::mutex> locker(mtx);
		        ready = true;
		    }

    		cv.notify_one();

    		// wait for the worker thread
		    {
		        std::unique_lock<std::mutex> locker(mtx);
			    cv.wait(locker, []{ return bytes_written; });
			    bytes_written = false;
		    } 
			
			
			buffer.resize(0);
		}
		
		buffer.append(new_string);
		buffer.append(1, '\n');

	}

	job_finished = true;

	writer.join();

	return 0;
}

