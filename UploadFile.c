/*
 * =====================================================================================
 *
 *       Filename:  UploadFile.c
 *
 *    Description:  Reads and stores the files used in the arugments of
 *    		    the main program.
 *
 *        Version:  2.5
 *        Created:  01/29/2017 10:11:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 *
 * =====================================================================================
 */

 // Precompiler directives ////////////////////////////////////////////////////////////
 #ifndef UPLOAD_FILE_c
 #define UPLOAD_FILE_c

// Header files ///////////////////////////////////////////////////////////////////////
 #include "UploadFile.h"

 //Functions///////////////////////////////////////////////////////////////////////////

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  uploadConfig
 *  Description: First reads and then stores the configuration file's data to the
 *               correct data type contained in the CONFIGFILE structure
 *  Parameters : config - the structure to store the data to.
 *               file - The file handle for the configuration file.
 * =====================================================================================
 */
 void uploadConfig(struct CONFIGFILE *config, FILE *file)
 {
   //Enter loop to read the rest of file untill the end of the file(EOF) is found
   char readChar;           //File iterator used to read file
   int lineInFile = 1;      //Current line to store the data to the correct type
   char* checkSTR;
   readChar = fgetc(file);  //Grab the first string

   while(readChar != EOF) //EOF means End of File
   {
      if(readChar == ':')
      {
        //Handle 'Version/Phase:' data
        if(lineInFile == 1)
        {
          config->version = handleInteger(file);
        }

        //Handle 'File Path:' data
        else if(lineInFile == 2)
        {
         config->filePath = handleString(file);
        }

        //Handle 'CPU Scheduling Code'
        else if(lineInFile == 3)
        {
         checkSTR = handleString(file);
         config->CPUSched = checkCPUSched(checkSTR, file);
        }

        //Handle 'Quantum Time (cycles):' data
        else if(lineInFile == 4)
        {
         config->quantumTime = handleInteger(file);
        }

        //Handle 'Memory Available (KB):'
        else if(lineInFile == 5)
        {
         config->memoryAvailable = handleInteger(file);
        }

        //Handle 'Processor Cycle Time (msec):'
        else if(lineInFile == 6)
        {
         config->processTime = handleInteger(file);
        }

        //Handle 'I/O Cycle Time (msec):'
        else if(lineInFile == 7)
        {
         config->ioTime = handleInteger(file);
        }

        //Handle 'Log To:'
        else if(lineInFile == 8)
        {
         checkSTR = handleString(file);
         config->logTo = checkLogTo(checkSTR, file);
        }

        //Handle 'Log File Path:'
        else if(lineInFile == 9)
        {
         config->logFile = handleString(file);
        }

         lineInFile++;
       }

     readChar = fgetc(file);    //Go to the next string in the file
   }

   //Check if all config data was read
   if(lineInFile != 10)
   {
     fprintf(stderr, "ERROR: Not enough data in the configuration file\n");
     fclose(file);
     exit(-1);
   }
 }


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  uploadMeta
 *  Description: First reads and then stores the meta data file's data to the
 *               correct data type contained in the METAFILE structure
 *  Parameters : file - The file handle containg the meta data file path.
 *  Return     : The linked list containing the stored meta data.
 * =====================================================================================
 */
struct METAFILE *uploadMetaData(FILE *file, struct PROCESSES **PCB, int *totalProcesses)
{
  //Used to read through the meta data file
  struct METAFILE *currentLink = NULL; //
  int cmdCounter = 0;
  char fileReader;        //To iterate through the file
  int charInSTR = 0;      //Characters in the current buffer
  char fileBuffer[256];   //For reading the descriptor
  char intBuffer[256];    //For reading the cycle time
  int processCounter = 0;
  struct PROCESSES *currentPCB;
  fileReader = fgetc(file);

  while(fileReader != EOF)
  {
    if(fileReader == 'S' ||
       fileReader == 'A'||
       fileReader == 'P'||
       fileReader == 'M'||
       fileReader == 'I'||
       fileReader == 'O')
    {
      if(fgetc(file) == '(')
      {
        struct METAFILE *tempLink;
        tempLink = (struct METAFILE *) malloc(sizeof(struct METAFILE));
        tempLink->descriptor = malloc(sizeof(char*));

        tempLink->cmdLetter = fileReader; //Save the command letter

        //Save the descriptor
        fileReader = fgetc(file);
        while(fileReader != ')')
        {
          fileBuffer[charInSTR++] = fileReader;
          fileReader = fgetc(file);
        }
        fileBuffer[charInSTR] = '\0';
        charInSTR = 0;

        //check that descriptor is valid
        if(strcmp(fileBuffer, "access") == 0)
        {
          tempLink->descriptor = "access";
        }
        else if(strcmp(fileBuffer, "allocate") == 0)
        {
          tempLink->descriptor = "allocate";
        }
        else if(strcmp(fileBuffer, "end") == 0)
        {
          tempLink->descriptor = "end";
        }
        else if(strcmp(fileBuffer, "hard drive") == 0)
        {
          tempLink->descriptor = "hard drive";
        }
        else if(strcmp(fileBuffer, "keyboard") == 0)
        {
          tempLink->descriptor = "keyboard";
        }
        else if(strcmp(fileBuffer, "printer") == 0)
        {
          tempLink->descriptor = "printer";
        }
        else if(strcmp(fileBuffer, "monitor") == 0)
        {
          tempLink->descriptor = "monitor";
        }
        else if(strcmp(fileBuffer, "run") == 0)
        {
          tempLink->descriptor = "run";
        }
        else if(strcmp(fileBuffer, "start") == 0)
        {
          tempLink->descriptor = "start";
        }
        else
        {
          fprintf(stderr, "ERROR: Invalid string in the descriptor found %c(%s)\n",
                  tempLink->cmdLetter, fileBuffer);
          fclose(file);
          exit(-3);
        }

        //Get the cycle time
        fileReader = fgetc(file);
        while(fileReader != ';' && fileReader != '.')
        {
          intBuffer[charInSTR++] = fileReader;
          fileReader = fgetc(file);
        }
        intBuffer[charInSTR] = '\0';

        //Check if the data is only a valid integer
        if(strlen(intBuffer) != strspn(intBuffer, "0123456789.;"))
        {
          fprintf(stderr, "ERROR: Inalid data '%c(%s)%s' found (data should only contain an integer value (0-9 & '.'))\n",
                  tempLink->cmdLetter, tempLink->descriptor, intBuffer);
          fclose(file);
          exit(-4);
        }

        //Store the cycle time
        tempLink->cycleTime = atoi(intBuffer);  //Converts a char* to an int
        charInSTR = 0;

        tempLink->cmdOrder = cmdCounter++;

        //Linked list handling
        tempLink->nextCMD = currentLink;
        currentLink = tempLink;

        //Handle differnt processes
        if(tempLink->cmdLetter == 'A' && strcmp(tempLink->descriptor, "start") == 0)
        {
          *totalProcesses += 1;
          struct PROCESSES *tempPCB;
          tempPCB = (struct PROCESSES *) malloc(sizeof(struct PROCESSES));
          tempPCB = malloc(sizeof(struct PROCESSES));
          tempPCB->totalRunTime = 0;
          tempPCB->processNumber = processCounter++;
          tempPCB->appStart = currentLink->cmdOrder;
          if(tempPCB->processNumber == 0)
          {
            *PCB = tempPCB;
            currentPCB = tempPCB;
          }
          else
          {
            currentPCB->nextProcess = tempPCB;
            currentPCB = tempPCB;
          }
        }
        else if(currentLink->cmdLetter != 'M' && currentLink->cmdLetter != 'S')
        {
            currentPCB->totalRunTime += currentLink->cycleTime;
        }

        //Last meta data command if its followed by a '.' instead of a ';'
        if(fileReader == '.')
        {
          break;
        }
      }
    }
    fileReader = fgetc(file);
  }
  return currentLink;
}

///Helper Functions/////////////////////////////////////////////////////////////////////

/*
 * Name       : handleInteger
 * Description: Reads a string from a file and converts it to an integer
 * Parameters : The file to read from.
 * Return     : The integer read from the file.
 */
int handleInteger(FILE *file)
{
  char *readValue;
  int integerValue;
  readValue = malloc(sizeof(char*));
  fscanf(file, "%s", readValue);

  //Check if the data is only a valid integer
  if(strlen(readValue) != strspn(readValue, "0123456789."))
  {
    fprintf(stderr, "ERROR: Inalid data '%s' found (data should only contain an integer value (0-9 & '.'))\n",
            readValue);
    fclose(file);
    exit(-2);
  }

  integerValue = atoi(readValue);  //atoi() turns a character string into a integer
  free(readValue);

  return integerValue;
}

/*
 * Name       : handleString
 * Description: Reads a string from a file and returns the string
 * Parameters : The file to read from.
 * Return     : The string read from the file.
 */
char* handleString(FILE *file)
{
  char* readSTR;
  readSTR = malloc(sizeof(char*));
  fscanf(file, "%s", readSTR);

  return readSTR;
}

/*
 * Name       : checkCPUSched
 * Description: checks if the CPUScheduling is valid or throws an error
 * Parameters : The string to check
 */
char* checkCPUSched(char* CPUSched, FILE *file)
{
  char* returnSTR;

  if(strcmp(CPUSched,"NONE") == 0)
  {
    returnSTR = "FCFS-N";
  }
  else if(strcmp(CPUSched,"FCFS-N") == 0)
  {
    returnSTR = "FCFS-N";
  }
  else if(strcmp(CPUSched,"SJF-N") == 0)
  {
    returnSTR = "SJF-N";
  }
  else if(strcmp(CPUSched,"SRTF-P") == 0)
  {
    returnSTR = "SRTF-P";
  }
  else if(strcmp(CPUSched,"FCFS-P") == 0)
  {
    returnSTR = "FCFS-P";
  }
  else if(strcmp(CPUSched,"RR-P") == 0)
  {
    returnSTR = "RR-P";
  }
  else
  {
    fprintf(stderr, "ERROR: CPU scheduling code is invalid. Read: %s\n", CPUSched);
    fclose(file);
    exit(-5);
  }

  return returnSTR;
}

char* checkLogTo(char* logTo, FILE *file)
{
  char* returnSTR;

  if(strcmp(logTo, "Monitor") == 0 || strcmp(logTo, "monitor") == 0)
  {
    returnSTR = "Monitor";
  }
  else if(strcmp(logTo, "File") == 0 || strcmp(logTo, "file") == 0)
  {
    returnSTR = "File";
  }
  else if(strcmp(logTo, "Both") == 0 || strcmp(logTo, "both") == 0)
  {
    returnSTR = "Both";
  }
  else
  {
    fprintf(stderr, "ERROR: Invalid Log to path found in config file. Read: %s (MUST BE 'Monitor' 'File' or 'Both')\n", logTo);
    fclose(file);
    exit(-6);
  }

  return returnSTR;
}


/*
 * Name       : displayConfigFile
 * Description: Displays the current configuration file's data.
 * Parameters : The structure containing the config file's data.
 */
void displayConfigFile(struct CONFIGFILE *config)
{
  printf("Simulator Configuration File:\n\n");
  printf("Version/Phase: %d\n", config->version);
  printf("File Path: %s\n", config->filePath);
  printf("CPU Scheduling Code: %s\n", config->CPUSched);
  printf("Quantum Time (cycles): %d\n", config->quantumTime);
  printf("Memory Available (KB): %d\n", config->memoryAvailable);
  printf("Processor Cycle Time: %d\n", config->processTime);
  printf("I/O Cycle Time: %d\n", config->ioTime);
  printf("Log To: %s\n", config->logTo);
  printf("Log File Path: %s\n\n", config->logFile);
}

/*
 * Name       : displayMetaDataFile
 * Description: Displays the current meta data file's data.
 * Parameters : The linked list containing the meta file's data.
 */
void displayMetaDataFile(struct METAFILE *metaData)
{
  struct METAFILE *listIterator = metaData;

  printf("Meta Data File:\n\n");

  while(listIterator != NULL)
  {
      printf("Component Letter    : %c\n", listIterator->cmdLetter);
      printf("Meta Data Operation : %s\n", listIterator->descriptor);
      printf("Meta Data Cycle Time: %d\n\n", listIterator->cycleTime);
      listIterator = listIterator->nextCMD;
  }
}

/*
 * Name       : reverseList
 * Description: Due to the intial creation of the linked list it shows
 *              the meta data commands in reverse order, this functions
 *              reverses the list into the correct command order.
 * Parameters : The linked list containing meta data file's data in reverse order.
 */
void reverseList(struct METAFILE **metaData)
{
  struct METAFILE *previousLink, *currentLink, *tempLink;

  previousLink = currentLink = tempLink = *metaData;
  previousLink = previousLink->nextCMD->nextCMD;
  currentLink = currentLink->nextCMD;
  tempLink->nextCMD = NULL;
  currentLink->nextCMD = tempLink;

  while (previousLink != NULL)
  {
      tempLink = currentLink;
      currentLink = previousLink;
      previousLink = previousLink->nextCMD;
      currentLink->nextCMD = tempLink;
  }

  *metaData = currentLink;
}

/*
 * Name       : sortPCB
 * Description: Sorts PCB in SJF Order
 * Parameters : The linked list containing meta data file's data in reverse order.
 * Return     : The sorted linked list
 */
struct PROCESSES* sortPCB(struct PROCESSES *PCB)
{
    struct PROCESSES *currentPCB = PCB;
    struct PROCESSES *nextPCB = PCB->nextProcess;
    int tempValue;

    while(nextPCB != NULL)
    {
     while(nextPCB != currentPCB)
     {
        if(nextPCB->totalRunTime < currentPCB->totalRunTime)
        {
                tempValue = currentPCB->totalRunTime;
                currentPCB->totalRunTime = nextPCB->totalRunTime;
                nextPCB->totalRunTime = tempValue;
        }
        currentPCB = currentPCB->nextProcess;
      }
      currentPCB = PCB;
      nextPCB = nextPCB->nextProcess;
    }

    return currentPCB;
}

/* Bubble sort the given linked lsit */
void bubbleSort(struct PROCESSES **start)
{
    int swapped;
    struct PROCESSES *ptr1;
    struct PROCESSES *lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
    {
        return;
    }

    do
    {
        swapped = 0;
        ptr1 = *start;

        while (ptr1->nextProcess != lptr)
        {
            if (ptr1->totalRunTime > ptr1->nextProcess->totalRunTime)
            {
                swapNodes(start, ptr1->totalRunTime, ptr1->nextProcess->totalRunTime);
                swapped = 1;
            }
            ptr1 = ptr1->nextProcess;
        }
        lptr = ptr1;
    }
    while (swapped);
}


/* Function to swap nodes x and y in linked list by
   changing links */
void swapNodes(struct PROCESSES **head_ref, int x, int y)
{
   // Nothing to do if x and y are same
   if (x == y) return;

   // Search for x (keep track of prevX and CurrX
   struct PROCESSES *prevX = NULL, *currX = *head_ref;
   while (currX && currX->totalRunTime != x)
   {
       prevX = currX;
       currX = currX->nextProcess;
   }

   // Search for y (keep track of prevY and CurrY
   struct PROCESSES *prevY = NULL, *currY = *head_ref;
   while (currY && currY->totalRunTime != y)
   {
       prevY = currY;
       currY = currY->nextProcess;
   }

   // If either x or y is not present, nothing to do
   if (currX == NULL || currY == NULL)
       return;

   // If x is not head of linked list
   if (prevX != NULL)
       prevX->nextProcess = currY;
   else // Else make y as new head
       *head_ref = currY;

   // If y is not head of linked list
   if (prevY != NULL)
       prevY->nextProcess = currX;
   else  // Else make x as new head
       *head_ref = currX;

   // Swap next pointers
   struct PROCESSES *temp = currY->nextProcess;
   currY->nextProcess = currX->nextProcess;
   currX->nextProcess  = temp;
}
#endif //UPLOAD_FILE_c end define
