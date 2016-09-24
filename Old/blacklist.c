#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// function declarations
void blacklist(char *);
void badWords(char *);
void list(char *);

// main function definition
int main()
{
	char website[25];
	char *newline = "\n";
	char *concatWeb = NULL;
	char *concatBad = NULL;
	char *concatVisited = NULL;
	char badword[25];
	char visitedWeb[25];

	printf("Enter a website: ");
	scanf("%s", website);

	// add a new line to the end of the website
	concatWeb = strcat(website, newline);
	blacklist(concatWeb);

//	printf("Enter a word: ");
//	scanf("%s", badword);

	// add a new line to the end of the bad word
//	concatBad = strcat(badword, newline);
//	badWords(concatBad);


//	printf("Enter a website(cache): ");
//	scanf("%s", visitedWeb);
//	printf("%s", visitedWeb);
	// add a new line to end of visited website, is an array
//	concatVisited = strcat(visitedWeb, newline);
//	printf("Concat: %s", concatVisited);//
//	list(concatVisited);

	return 0;
} // end of main function

// blacklist function definition; add start and stop time, skip print  response if blacklisted returns true
void blacklist(char *website)
{
    time_t     now;
    struct tm *ts;
    char       buf[80];
    
    /* Get the current time */
    now = time(NULL);
    
    /* Format and print the time,  */
    ts = localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%S ", ts);
    printf("%s", buf);//print time
    
	// declare and open file
	FILE *blacklistFile = fopen("blacklistFile.txt", "r");

	char buffer[1000];
	int i = 0, result = -1, t = -1;

	// check if file is exist
	if(!blacklistFile)
		return;

	// go through file line by line
	for(i = 0; fgets(buffer, 1000, blacklistFile) != NULL; i++)
	{
		// compare website to those in the list
		result = strncmp(website, buffer, result);
        t = strncmp(buf, buffer, t); //compare buf YYYY.. to content of file

		// if they match, state blacklisted website and skip print response
		if((result == 0)&&(t == 0))
		{
			printf("Blacklisted.\n");
			return;
		}
	}
	// otherwise the website was valid
	printf("Valid.\n");
	return;

	// close file
	fclose(blacklistFile);
} // end of blacklist function

// badWords function definition
void badWords(char *badWords)
{
	// declare and open file
        FILE *badWordsFile = fopen("badWords.txt", "r");

        int i = 0, result = -1;
	char buffer[1000], upper, firstLetter;

	// convert first letter of the word entered
	// to uppercase
	firstLetter = badWords[0];
	upper = toupper(firstLetter);
	badWords[0] = upper;

	// check if file exist
        if(!badWordsFile)
                return;

	// go through file line by line
        for(i = 0; fgets(buffer, 1000, badWordsFile) != NULL; i++)
        {
		// compare upper-cased bad word to those in the list
                result = strncmp(badWords, buffer, result);

		// if they match, state bad word
                if(result == 0)
                {
                        printf("Bad Word\n");
                        return;
                }

		// reset results
		result = -1;
        }
	// otherwise the word is good
	printf("Good Word\n");
	return;

	// close file
        fclose(badWordsFile);
} // end of badWords function

void list(char *visitedWeb)
{
	time_t     now;
    struct tm *ts;
    char       buf[80];

    /* Get the current time */
    now = time(NULL);

    /* Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" */
    ts = localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d %H:%M:%S ", ts);
    puts(buf);


	//FILE *cacheFile = fopen(buf, "a+");//YYYYMM...


	FILE *listFile = fopen("list.txt", "a+");
	//append files YYYYMMDDhhmmss to this file

	char cache[1000];
	int result = -1;
	int i = 0;

	if(!listFile)//if nothing saved
		return;

	for(i = 0; fgets(cache, 1000, listFile) != NULL; i++)
	{
		result = strncmp(visitedWeb, cache, result);

		if(result == 0)
		{
			printf("Visited Already\n");
			return;
		}

		result = -1;
	}
	//fprintf(cacheFile, "%s", visitedWeb); //google.com saved in YYYYMM..
	fprintf(listFile, "%s  %s\n", visitedWeb, buf);//put cacheFile into listFile
	printf("Added to cache\n");
	return;

	fclose(listFile);
}
/*
void writeTofile(char *response)
{
	time_t     now;
    struct tm *ts;
    char       buf[80];

    /* Get the current time */
    //now = time(NULL);

    /* Format the time YYYYMMDDhhmmss 
    ts = localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%S ", ts);
    
	FILE *webData = fopen(buf, "w");//write
	
	if(!dataFile)
		return;

	// put reponse into webData
	fprintf(webData, "%s", response);
	printf("Data stored in file %s\n", buf);
	return;

	fclose(webData);
} */
