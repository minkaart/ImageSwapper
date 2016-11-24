/*Image Replacement Tool*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

long file_size(char [], long);
long random_number(long, long);

int main()
{	
//GLOBALS
//Runtime changeable variables max file size and number of exchanges
	long max_file_size = 1000000; //Change this variable to use larger files
	int swaps = 20; //Change this variable to increase/decrease the number of segments each file is divided into 

//3 image files 1: first image 2: second image 3: image to write new image to
	FILE *img; 
	FILE *img2;
	FILE *img3;

//buffers hold image data for manipulation during program run
	char img_buff[max_file_size];
	char img2_buff[max_file_size];

//!!! holds the actual file size of each read file
	long file_size_1 = 0;
	long file_size_2 = 0; 

	long low;
	long high; //low and high representing the entire file-size 
	int range_array_length = 2*swaps+2;
	long range_array[range_array_length];
//set range_array values to zero
	for(int i=0; i<range_array_length; i++){
		range_array[i] = 0; 
	}

//read two images into buffer 
	img = fopen("PalmTrees.jpg", "rb");
	img2 = fopen("BLM.jpg", "rb");
	fread(img_buff, 1, max_file_size, img);
	fread(img2_buff, 1, max_file_size, img2);

//seed random number generator
	srand(time(0));
	
//GET ACTUAL FILE SIZE OF IMAGES ONE AND TWO 
	file_size_1 = file_size(img_buff, max_file_size);
	file_size_2 = file_size(img2_buff, max_file_size);

//COMPARE FILE SIZES? MIGHT CHANGE OTHER CODE LATER
	if (file_size_1 == file_size_2){
		printf("file 1 and 2 are equal and have the value: %lu\n", file_size_1);
	} else {
		printf("file 1 and 2 are not equal\nfile 1: %lu\n", file_size_1);
		printf(" file 2: %lu\n", file_size_2);
	}
//SET LOW/HIGH BASED ON ACTUAL FILE SIZES
	if(file_size_1>file_size_2){
		high = file_size_2;
		low = .05 * file_size_2;
	} else {
		high = file_size_1;
		low = .05 * file_size_1;
	}

//LOOP CREATES RANGES, POPULATES RANGE ARRAY, 
	range_array[0] = low; 
	range_array[1] = high;

	int count = 0; //number of runs of do loop
	int length = 0; //length of range_array 
	do{
		printf("length:%d\n", length );
		for (int i =0; i<length; i++){
			int target = 2*i+1; //position in the array currently being calculated
			int this_skips = 0; //number of skips in the for loop
			if(target<=swaps*2){
				printf("target (%d) is less than swaps (%d) (*2) \n", target, swaps );
				if(range_array[target] - range_array[target-1] > 1000){
					printf("range_array[%d](%lu) - range_array[%d](%lu) = %lu\n", target, range_array[target], target-1, range_array[target-1], range_array[target]-range_array[target-1]);
					long new_val = random_number(range_array[target-1], range_array[target]);
					//Shift array - and place new value
					for(int j = length+i - this_skips; j>=target; j--){
						range_array[j+1] = range_array[j];
					}
					range_array[target] = new_val; 
					//printf("target: %d new value: %lu\n", target, new_val);
				} else {
					printf("SKIP:\n  range_array[%d](%lu) - range_array[%d](%lu) = %lu\n", target, range_array[target], target-1, range_array[target-1], range_array[target]-range_array[target-1]);
					this_skips++; 
				}
			} else {
				printf("target (%d) is greater than swaps (%d) *2 \n", target, swaps);
			}			
		}
		//recreate length based on actual values in the array (req. b/c of skips)
		length = 0; 
		for(int i=0; i<range_array_length; i++){
			if(range_array[i] != 0){
				printf("A %d: %lu\n", i, range_array[i]);	
				length++; 
			}
		}
		count++; //check that this is still necessary? 
		printf("do: while length: %d\n", length);
	}while (length<swaps*2+2);

	//CODE THAT USES range_array to move data between img_buff(1&2) and to write to new file
	char file_name[21];
	long time_ = time(0);
	for(int i=1; i<range_array_length-1; i +=2){
		for(int j = range_array[i]; j<range_array[i+1]; j++){
			img_buff[j] = img2_buff[j];
		}

		sprintf(file_name, "%ldnew%d.jpg", time_, i);
		img3 = fopen(file_name, "wb");
		fwrite(img_buff, 1, max_file_size, img3);
		fclose(img3);
	}




}

//Function returns a random number between two given numbers (low and high)
long random_number(long low, long high) {
	long random = rand() % (high - low + 1) + low;
	return random; 
}

//Function reads through an array and finds its actual size (how much data is written to the array as opposed to null values)
long file_size(char file_buff[], long max){
	long count = 0;
	for(long i=0; i<max; i++){
		if(file_buff[i]){
			count++; 
		}
	}
	return count; 
}