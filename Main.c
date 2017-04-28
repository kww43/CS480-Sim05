/*
 * =====================================================================================
 *
 *       Filename:  Main.c
 *
 *    Description:  Main program for Simulator 01
 *
 *        Version:  2.5
 *        Created:  01/29/2017 09:27:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

// Precompiler directives
#ifndef MAIN_c
#define MAIN_c

// Header files ///////////////////////////////////////////////////////////////////////
#include "Main.h"

// Main Function Operating System Simulator ///////////////////////////////////////////
int main(int argc, char *argv[])
{
  //Check if config file is an argument
  if(argc < 2)
  {
    fprintf(stderr,"ERROR: Not enough arguments (Try using: ./sim01 configurationfile.cnf)\n");
    return -2;
  }

  //Open config file
  FILE *configFH; //Configuration file handle
  configFH = fopen(argv[1], "r");

  //Check if config file is valid
  if(configFH == NULL)
  {
    fprintf(stderr, "ERROR: Configuration file could not opened (Try checking if there is a config file named '%s' in your folder)\n",
             argv[1]);
    return -1;
  }

  printf("\nOperating System Simulator\n\n");
  printf("============================================\n\n");

  //Intialize the struct for the config file
  struct CONFIGFILE *configFile;
  configFile = malloc(sizeof(struct CONFIGFILE));

  //Call uploadConfig to read and store config file data
  printf("Now loading configuration file.....\n\n");
  uploadConfig(configFile, configFH);

  //Close config file handle
  fclose(configFH);

  //Open the Meta Data File to be read and stored
  FILE *metaDataFH; //Meta data file handle
  metaDataFH = fopen(configFile->filePath, "r");

  //Check if config file is valid
  if(metaDataFH == NULL)
  {
    fprintf(stderr, "ERROR: Meta data file could not opened Found: %s  (Try changing the File Path in the config file)\n",
	      configFile->filePath);
    return -3;
  }

  //Upload and store the meta data file
  printf("Now loading meta data file.....\n\n");
  int totalProcesses = 0;
  struct PROCESSES *PCB;
  struct METAFILE *metaDataFile = uploadMetaData(metaDataFH, &PCB, &totalProcesses, configFile);

  if(strcmp(configFile->CPUSched, "SJF-N") == 0)
  {
    printf("Sorting Processes based on SJF Scheduling....\n");
    PCB = sortPCB(PCB);
  }

  /*Reverse the linked list containing meta data due to list being
    initially backwards*/
  reverseList(&metaDataFile);

  //Close the meta data file
  fclose(metaDataFH);

  printf("============================================\n\n");

  //Intialize the ready queue
  struct Queue *queueInfo = malloc(sizeof(struct Queue));
  int que[totalProcesses];
  initFCFS(queueInfo, que, totalProcesses);

  //Start the process manager
  printf("Begin Simulator\n\n");

  if(strcmp(configFile->CPUSched, "FCFS-N") == 0 || strcmp(configFile->CPUSched, "SJF-N") == 0)
  {
    //processManager(configFile, metaDataFile, PCB, totalProcesses);
  }

  else if(strcmp(configFile->CPUSched, "FCFS-P") == 0)
  {
    processFCFS_SRTF(configFile, metaDataFile, PCB, totalProcesses);
  }

  else if(strcmp(configFile->CPUSched, "SRTF-P") == 0)
  {
    processFCFS_SRTF(configFile, metaDataFile, PCB, totalProcesses);
  }

  else if(strcmp(configFile->CPUSched, "RR-P") == 0)
  {
    //processRR_P(configFile, metaDataFile, PCB, totalProcesses);
  }

  //End the Simulation
  printf("\nEnd Simulation\n\n");
  printf("============================================\n\n");

  return 0; //Exit with success
}

#endif //end define for MAIN_c
