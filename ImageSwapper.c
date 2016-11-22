/*Image Replacement Tool*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

struct Range
	{
		int pos;
		long low;
		long high; 
	};
long file_size(char [], long);
long random_number(long, long);
struct Range range_maker(long [][2], int, long, long, long);



int main()
{	
//GLOBALS
//Runtime changeable variables max file size and number of exchanges
	long max_file_size = 1000000;
	int swaps = 10;

//3 image files 1: first image 2: second image 3: image to write new image to
	FILE *img; 
	FILE *img2;
	FILE *img3;

//buffers hold image data for manipulation during program run
	char img_buff[max_file_size];
	char img2_buff[max_file_size];

//!!! - checking variable may not be required in the end. variable holding image size as read by program
	long file_size_1 = 0;
	long file_size_2 = 0; 

	long low;
	long high; //low and high representing the entire file-size 
	long low_range;
	long high_range; //low and high representing ranges on each swap loop
	//long range_array [swaps][2]; //array holding all previous swapped bit ranges
	int range_array_length = 2*swaps+2;
	long range_array[range_array_length];
	for(int i=0; i<range_array_length; i++){
		range_array[i] = 0; 
		printf("range array at %d set to zero\n", i);
	}
	printf("Range Array Length: %d\n", range_array_length);
	printf("Start size of Range Array: %lu\n", sizeof(range_array));
	struct Range single_range; 

//read two images into buffer 
	img = fopen("PalmTrees.jpg", "rb");
	img2 = fopen("BLM.jpg", "rb");
	fread(img_buff, 1, max_file_size, img);
	fread(img2_buff, 1, max_file_size, img2);

//!!img3 = fopen("combined.jpg", "wb"); //make this variable!! change every new image creation loop! 
	srand(time(0));

//GET ACTUAL FILE SIZE OF IMAGES ONE AND TWO 
	file_size_1 = file_size(img_buff, max_file_size);
	file_size_2 = file_size(img2_buff, max_file_size);

//COMPARE FILE SIZES? MAKE SURE THEY ARE EQUAL ???
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

	int count = 0; //number of runs of do loop, which 
	int length; //length of the array each time do: loop is called (function of counts)
	int skips; //divisions skipped as a result of small intervals
	do{
		printf("length:%d\n", length );
		for (int i =0; i<length; i++){
			int target = 2*i+1; //position in the array currently being calculated
			int this_skips = 0; //holds number of skips in this iteration
			if(target<=swaps*2){
				printf("target (%d) is less than swaps (%d) (*2) \n", target, swaps );
				if(range_array[target] - range_array[target-1] > 5){
					printf("range_array[%d](%lu) - range_array[%d](%lu) = %lu\n", target, range_array[target], target-1, range_array[target-1], range_array[target]-range_array[target-1]);
					long new_val = random_number(range_array[target-1], range_array[target]);
					/*Shift array to place new value in appropriate location
					length+i = total length of the array before shift, but 
					accounting for the number of shifts that have already occured*/
					for(int j = length+i - this_skips; j>=target; j--){
						range_array[j+1] = range_array[j];
						//printf("shift: %d\n", j);
					}
					range_array[target] = new_val; 
					//printf("target: %d new value: %lu\n", target, new_val);
				} else {
					printf("SKIP:\n  range_array[%d](%lu) - range_array[%d](%lu) = %lu\n", target, range_array[target], target-1, range_array[target-1], range_array[target]-range_array[target-1]);
					skips++; 
					this_skips++;
				}
			} else {
				printf("target (%d) is greater than swaps (%d) *2 \n", target, swaps);
			}
			
		}
		//output array values (error checking)
		length = 0; 
		for(int i=0; i<range_array_length; i++){
			if(range_array[i] != 0){
				printf("A %d: %lu\n", i, range_array[i]);	
				length++; 
			}
		}
		count++; 
		//length = exp2(count) - skips;
		printf("Exponent Count (length w/out skips) %f\n Actual Length: %d\n Skips: %d\n", exp2(count), length, skips);
	}while (exp2(count-1)<swaps*2+2);

	printf("Exited with length (%d)>swaps*2+2(%d)\n", length, swaps*2+2);
	printf("Size of Range_array%lu\n", sizeof(range_array) );

	char file_name[10];
	for(int i=1; i<range_array_length-1; i +=2){
		low_range = range_array[i];
		high_range = range_array[i+1];
		for(int j = range_array[i]; j<range_array[i+1]; j++){
			img_buff[j] = img2_buff[j];
		}

		sprintf(file_name, "new%d.jpg", i);
		img3 = fopen(file_name, "wb");
		fwrite(img_buff, 1, max_file_size, img3);
		fclose(img3);

		printf("B %d: %lu - %lu\n", i, range_array[i], range_array[i+1]);
		printf("range_array last: %lu\n", range_array[range_array_length]);
	}




}

//EVENTUALLY CLEAN THIS UP TO HANDLE ALL CASES (EQUIVALENTS AND DEFAULTS, PROTECT AGAINST INFINITE LOOP)
struct Range range_maker(long array[][2], int pos, long rm_low, long low, long high){
	printf("range_maker:\n pos: %d\n rm_low: %lu\n low: %lu\n high: %lu\n", pos, rm_low, low, high);
	struct Range this_range;
	long rm_high; 
		if (rm_low < array[0][0]){
			printf("less than\n");
			rm_high = random_number(rm_low, array[0][0]);
			this_range.pos = 0;
			this_range.low = rm_low;
			this_range.high = rm_high;
			printf("A assignment:\n  position: %d\n  low: %lu\n  high: %lu\n", this_range.pos, this_range.low, this_range.high );
		} else if(rm_low > array[pos-1][1]){
			printf("greater than\n");
			rm_high = random_number(rm_low, high);
			this_range.pos = pos; //CHECK THIS LATER!!! 
			this_range.low = rm_low;
			this_range.high = rm_high;
			printf("B assignment:\n  position: %d\n  low: %lu\n  high: %lu\n", this_range.pos, this_range.low, this_range.high );
		} else {
			printf("between\n");
			int recursion = 0;
			for(int i=0; i<pos; i++){ //!! CHECK THIS LATER
				printf("back in loop\n");
				if(rm_low>=array[i][0] && rm_low<=array[i][1]){
					printf("recursion:\n");
					printf("recursion data:\n new low: %lu\n i: %d\n", rm_low, i);
					recursion = 1;
					break; 
				} else if (rm_low<array[i+1][0]){
					printf("Non-recursive condition met:\n");
					rm_high = random_number(rm_low, array[i+1][0]);
					this_range.pos = i+1; 
					this_range.low = rm_low; 
					this_range.high = rm_high; 
					break; 
				} else {
					printf("Not Here!\n");
				}
			}
			if(recursion == 1){
				printf("calling recursion\n");
				recursion = 0;
				rm_low = random_number(low, high);
				range_maker(array, pos, rm_low, low, high);
			}
			//printf("C assignment:\n  position: %d\n  low: %lu\n  high: %lu\n", this_range.pos, this_range.low, this_range.high );		
		}
	printf("FINAL assignment:\n  position: %d\n  low: %lu\n  high: %lu\n", this_range.pos, this_range.low, this_range.high );
	return this_range;
}

long random_number(long low, long high) {

	long random = rand() % (high - low + 1) + low;
	return random; 
}

//check variable type call
long file_size(char file_buff[], long max){
	long count = 0;
	for(long i=0; i<max; i++){
		if(file_buff[i]){
			count++; 
		}
	}
	return count; 

}