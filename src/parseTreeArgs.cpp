# include "parseTreeArgs.h"

/** Class Constructor */
ParseArgs :: ParseArgs () {
    fileParam = "";
    treePath = "";
    imgPath = "";
    mode = -1;
}

/** 
 * Method to parse the command line and store
 * information.
 * @param argc number of parameters in command line.
 * @param argv vector of parameteres in command line.
 */
void ParseArgs :: parseCommandLine(int argc, char **argv) {

    /* 1. CREATE AN OBJECT */
    AnyOption *opt = new AnyOption();

    /* 2. SET PREFERENCES  */
    //opt->noPOSIX(); /* do not check for POSIX style character options */
    opt->setVerbose(); /* print warnings about unknown options */
    opt->autoUsagePrint(false); /* print usage for bad options */

    /* 3. SET THE USAGE/HELP   */
    opt->addUsage( "" );
    opt->addUsage("Usage: ./rf [-t <parameter file> -p <tree_directory_path>|-c <parameter file> -p <tree_directory_path> -i <images_directory_path> | -r <parameter file> -p <tree_directory_path>]");
    opt->addUsage( "" ); 
    opt->addUsage("-h  --help");
    opt->addUsage("                 --train       -t If set then the program will train the tree with the ");
    opt->addUsage("                                  parameters especified in the parameters file and save the trees into directory path.");
    opt->addUsage("                 --classify    -c If set then the program will classify a set of images specified by directory path loading the");
    opt->addUsage("                                  trees in tree_directory_path");
    opt->addUsage("                 --run         -r If set then the program will run real time classification using trees in tree_directory_path ");
    opt->addUsage("");

    /* 4. SET THE OPTION STRINGS/CHARACTERS */

    /* by default all  options  will be checked on the command line and from option/resource file */
    opt->setFlag(  "help", 'h' );   /* a flag (takes no argument), supporting long and short form */ 
    opt->setOption(  "train", 't' ); /* an option (takes an argument), supporting long and short form */
    opt->setOption(  "classify", 'c' ); /* an option (takes an argument), supporting long and short form */
    opt->setOption(  "run", 'r' ); /* an option (takes an argument), supporting long and short form */
    opt->setOption(  "path", 'p' ); /* an option (takes an argument), supporting long and short form */
    opt->setOption(  "image", 'i' ); /* an option (takes an argument), supporting long and short form */

    
    /* for options that will be checked only on the command and line not in option/resource file */
    //opt->setCommandFlag(  "zip" , 'z'); /* a flag (takes no argument), supporting long and short form */

    /* for options that will be checked only from the option/resource file */
    //opt->setFileOption(  "title" ); /* an option (takes an argument), supporting only long form */

    /* 5. PROCESS THE COMMANDLINE AND RESOURCE FILE */

    /* read options from a  option/resource file with ':' separated opttions or flags, one per line */
    //opt->processFile( "/home/user/.options" );  
    /* go through the command line and get the options  */
    opt->processCommandArgs( argc, argv );
    
    if( ! opt->hasOptions()){ 
        opt->printUsage();
        delete opt;
        exit(EXIT_FAILURE);
    }
    
    /* 6. GET THE VALUES */
    if( opt->getFlag( "help" ) || opt->getFlag( 'h' ) ) {
        opt->printUsage();
    }
    
    if( opt->getValue( 't' ) != NULL  || opt->getValue( "train" ) != NULL  ) {
        fileParam = string(opt -> getValue('t'));
        mode = TRAIN;
    }
    
    if( opt->getValue( 'p' ) != NULL  || opt->getValue( "path" ) != NULL  ) {
        treePath = string(opt -> getValue('p'));
    }

    if( opt->getValue( 'c' ) != NULL  || opt->getValue( "classify" ) != NULL  ) {
        
        fileParam = string(opt -> getValue('c'));
        mode = CLASSIFY;
    }

    if( opt->getValue( 'i' ) != NULL  || opt->getValue( "image" ) != NULL  ) {
        imgPath = string(opt -> getValue('i'));
    }

    if( opt->getValue( 'r' ) != NULL  || opt->getValue( "run" ) != NULL  ) {
        fileParam = string(opt -> getValue('r'));
        mode = RUN;
    }

    cout << endl;

    /* 7. DONE */
    delete opt;
}

/** Prints the information of the parse class */
void ParseArgs :: printInfo() {
    
    cout << "*---------Random Forest algorithm for training and classification---------*" << endl;
    cout << endl;

    switch (mode) {
        case TRAIN:
            cout << "Training set" << endl;
            cout << "Parameters file: " << fileParam << endl;
            cout << "Tree save file: " << treePath << endl;
            break;
        case CLASSIFY:
            cout << "Classify set" << endl;
            cout << "Parameters file " << fileParam << endl;
            cout << "Tree load file " << treePath << endl;
            cout << "Image load directory " << imgPath << endl;
            break;
        case RUN:
            cout << "Run set" << endl;
            cout << "Parameters file " << fileParam << endl;
            cout << "Tree load file " << treePath << endl;
            break;
        default:
            break;
    }
    cout << endl;
}
