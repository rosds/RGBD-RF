/***********************************************
* Header file for structure representation of  *
* a command line parsrer of the game CAYAPA    *
* TOWER DEFENSE using extern anyoption library *
*                                              *
* @author: Alfonso Ros                         *
* @author: Ismael Mendonca                     *
*                                              *
************************************************/


# include "anyoption.h"
# include <cstring>

enum {
    TRAIN = 0,
    CLASSIFY,
    RUN
};

/** Class used to parse the command line arguments. */
class ParseArgs
{
    public: 

        /**
         * Parameters file to be parsed.
         */
        string fileParam;
  
        /**
         * Tree directory path (only for training).
         */
        string treePath;
        
        /**
         * Image directory path (only for classification).
         */
        string imgPath;

        /**
         * Indicates if the option is set to train or classify.
         */
        int mode;

        /** Class Constructor */
        ParseArgs();
        
        /** 
         * Method to parse the command line and store
         * information.
         * @param argc number of parameters in command line.
         * @param argv vector of parameteres in command line.
         */
        void parseCommandLine(int argc, char **argv);

        /** Prints the information of the parse class */
        void printInfo();
};
