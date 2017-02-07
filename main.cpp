//
//  main.cpp
//  imageEditor_v2
//
//  Created by Devin Gearing on 11/28/16.
//  Copyright � 2016 Devin Gearing. All rights reserved.
//
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <locale>
#include <stdio.h>  /* defines FILENAME_MAX */



#ifdef _WIN32
#include <direct.h>
#include <process.h>
#include <windows.h>        // needed to get Sleep() function.
#include <io.h>             // for _access(...), to test if file exists
#define GetCurrentDir _getcwd
#endif

#ifdef __APPLE__
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

//for UNIX



#define PI 3.14159265
using namespace std;

#ifdef __APPLE__
string homepath = string(getenv("HOME")) + "/";
string userpath = "Desktop/sample_images/";
string pathname = homepath + userpath;
char sep = '/';
string dircmd = "ls -R |grep .pgm";
string display_cmd = "open -a ToyViewer ";
#endif
string settingsName = "settings.txt";
#ifdef _WIN32
string dircmd = "dir  /s /b *.pgm | findstr /e .pgm";
string display_cmd = "start ";
string userpath = "Desktop\\sample_images\\";
#pragma warning(push)
#pragma warning(disable: 4996)
string homepath = string(getenv("HOMEPATH")) + "\\";
#pragma warning(pop)
string pathname = homepath + userpath;
char sep = '\\';
#endif


//string settingsLocation=system("getcwd");

//string settingsLocation=system("_getcwd");

void save_recent(string fname);

//function declartion
void clr();
string menu(int opt);
string interpret(string input, string menuType);
string lowerCase(string input);
void do_file();
void save_suffix(const int pgm[], int nc, int nr, int mv, string  type, string filename, string cmd);
void pgm_save(const int pgm[], int nc, int nr, int mv, string  typ, string filename);
void do_settings();
string open_recent();
void change_settings();
void do_help();
void do_quit();
void system_pause();
void multiplatform_system(string cmd);
int average_pgm(int pgm[], int nc, int nr);
bool blur_done=false;


//old func
void load_pgm(int *&pgm, int &nc, int &nr, int &mv, string &ty, string &fname);
void pgm_read_hdr(istream &ifs, int  &nc, int &nr, int &mv, string &ty);           // reads header info from
void skip_comments(istream &ifs);      // skips over whitespace and comments in a PGM image header.
void pgm_read(string fname, int * &pgm, int &nc, int &nr, int &mv, string &ty);               // read a P
bool file_exists(string f);
void edit_options(int pgm[], int nc, int nr, int mv, string ty, string fname);
void print_header(const int pgm[], int nc, int nr, int mv, string ty, string fname, string label, int npix = 10);
void pgm_display(const int pgm[], int nc, int nr, int mv);
void pgm_view(string fname, bool do_delete = false);
void do_view(void);
string getcwd_string();

//image edit functions
void pgm_negate(int *&pgm, int nc, int nr, int mv);
void do_pixelate(int *&pgm, int nc, int nr);
void do_histogram(int *&pgm, int nc, int nr, int mv);
void do_rotate(int pgm[], int nr, int nc);
void do_black_white(int *&pgm, int nc, int nr, int mv);
void do_blur(int *&pgm, int nc, int nr, int mv);
void do_edge(int *&pgm, int nc, int nr, int mv);
void do_clip(int *&pgm, int nc, int nr);


string settingsDirectory;

int main() {
    settingsDirectory = getcwd_string();
    string cmd;
    do_settings();
    
    
    
    cmd = menu(1);
    if (cmd == "file") {
        do_file();
    }
    if (cmd == "settings") {
        change_settings();
        
    }
    if (cmd == "help") {
        do_help();
        cout << "PRESS ENTER..." << endl;
        multiplatform_system("pause");
        main();
        
    }
    if (cmd == "quit") {
        do_quit();
        
    }
    else {
        clr();
        main();
    }
    
    
    return 0;
}

string menu(int opt) {
    clr();
    string cmd = "";
    
    if (opt == 1) {
        //DISPLAYED MENU
        cout << "~~~~IMAGE EDITOR v0.02~~~~" << endl;
        cout << " *File" << endl;
        cout << " *Settings" << endl;
        cout << " *Help" << endl;
        cout << " *Quit" << endl;
        cout << "Input: ";
        string input;
        cin >> (input);
        cmd = interpret(input, "mainmenu");
    }
    
    return cmd;
}

/////helpful functions


void multiplatform_system(string cmd) {
    cmd = lowerCase(cmd);
#ifdef WIN32
    if (cmd == "pause") {
        cmd = "pause";
    }
#else
    if (cmd == "pause") {
        cmd = "read";
    }
    
#endif
    system(cmd.c_str());
}



string interpret(string in, string type) {
    string input;
    string output;
    input = lowerCase(in);
    cout << input << endl;
    //inteprets input to singular input
    if (type == "mainmenu") {
        if (input == "file" || input == "open file" || input == "openfile" || input == "open") output = "file";
        if (input == "settings" || input == "setting" || input == "sett" || input == "set") output = "settings";
        if (input == "?" || input == "help" || input == "wut") output = "help";
        if (input == "quit" || input == "exit" || input == "overboard" || input == "leave") output = "quit";
        
        if (input == "back" || input == "before") output = "back";
    }
    
    if (type == "openmenu") {
        if (input == "open" || input == "read" || input == "load" || input == "import") output = "import";
        if (input == "back" || input == "before") output = "back";
        if (input == "open recent" || input == "recent" || input == "last" || input == "or") output = "recent";
    }
    if (type == "imagemenu") {
        if (input == "print" || input == "printout" || input == "out" || input == "output") output = "print";
        if (input == "display" || input == "disp" || input == "open" || input == "view") output = "display";
        if (input == "negate" || input == "neg" || input == "inverse" || input == "invert") output = "negate";
        if (input == "rotate" || input == "rot" || input == "turn" || input == "angle") output = "rotate";
        if (input == "clip" || input == "section" || input == "portion" || input == "part") output = "clip";
        if (input == "histogram" || input == "hist" || input == "equal" || input == "balance" || input == "histo") output = "histogram";
        if (input == "save" || input == "export" || input == "saveas" || input == "save as") output = "save";
        if (input == "black and white" || input == "white" || input == "bnw" || input == "bw")output = "newspaper";
        if (input == "newspaper" || input == "newpaper" || input == "news" || input == "new")output = "newspaper";
        if (input == "black" || input == "paper" || input == "papers" || input == "np")output = "newspaper";
        if (input == "back" || input == "before" || input == "last") output = "back";
        if (input == "b" || input == "bl" || input == "blu" || input == "blur") output = "blur";
        if (input == "cl" || input == "clip" || input == "c" || input == "cut") output = "clip";
        if (input == "eg" || input == "ed" || input == "edg" || input == "e" || input == "border" || input == "edge") output = "edge";
        if (input == "quit" || input == "exit" || input == "overboard" || input == "leave") output = "quit";
        
    }
    if (type == "savemenu") {
        if (input == "save" || input == "savesame" || input == "same") output = "save";
        if (input == "saveas" || input == "save as" || input == "new" || input == "new save") output = "save_as";
    }
    if (type == "overwrite") {
        if (input == "yes" || input == "yeah" || input == "sure" || input == "y") output = "yes";
        if (input == "no" || input == "nope" || input == "n") output = "no";
        if (input == "e" || input == "can" || input == "c" || input == "exit" || input == "back") output = "no";
    }
    if (type == "edgemenu") {
        if (input == "hori" || input == "hor" || input == "h" || input == "horiz") output = "horizontal";
        if (input == "ver" || input == "vert" || input == "verti" || input == "v") output = "vertical";
        if (input == "b" || input == "bo" || input == "bot" || input == "both" || input == "vh") output = "both";
    }
    
    
    //return singular input
    return output;
}
#ifdef __APPLE__
string getcwd_string() {
    char buff[PATH_MAX];
    GetCurrentDir(buff, PATH_MAX);
    std::string cwd(buff);
    return cwd;
}

#endif

#ifdef _WIN32
string getcwd_string() {
    char buffer[MAX_PATH] = {};
    GetSystemDirectoryA(buffer, _countof(buffer));
    strcat_s(buffer, "\\veresion.dll");
    return string(buffer);
}
#endif

string lowerCase(string in) {
    locale loc;
    string output;
    
    for (int i = 0; i<(in.length()); ++i) {
        output += tolower(in[i], loc);
    }
    
    return output;
}

void clr() {
    cout << string(50, '\n');
}

int average_pgm(int pgm[], int nc, int nr) {
    int average;
    long int hold = 0;
    long int count = 0;
    for (int i = 0; i<nr; i++) {
        for (int j = 0; j<nc; j++) {
            hold += pgm[count];
            count++;
        }
    }
    average = hold / count;
    return average;
}

///header functions
string open_recent() {
    string fname;
    ifstream recentFile;
    //string tempcommand="cd "+settingsDirectory;
    //system(tempcommand.c_str());
    recentFile.open("recent.txt");
    
    if (!recentFile)
    {
        fname = "none";
    }
    else {
        string str;
        int count = 0;
        while (getline(recentFile, str))
        {
            if (count == 0) fname = str;
            count++;
        }
    }
    
    return fname;
}


void do_file() {
    // pgm variables
    int * pgm = NULL;
    int * hold_pgm = NULL;
    //BaseballPlayer** team = new BaseballPlayer*[5];
    
    int nRow = 0, nCol = 0, mv = 255;                     // for 'current' image
    int nRow_orig = 0, nCol_orig = 0, mv_orig = 255;      // for original image, so I can revert to it.
    string type, type_orig;
    string fname = "", orig_name = "";
    
    clr();
    string in;
    string input;
    cout << "~~~~FILE MENU~~~~" << endl;
    cout << " *Open" << endl;
    cout << " *Open Recent" << endl;
    cout << " *Back" << endl;
    cout << "Input: ";
    cin >> in;
    input = lowerCase(in);
    input = interpret(input, "openmenu");
    cout << input << endl;
    
    if (input == "import") {
        clr();
        string openFile;
        cout << "~~~~OPEN FILE~~~~" << endl;
        
        //string hold = "ls *.pgm";
        string hold = dircmd;
        system(("cd " + pathname + " && " + hold).c_str());
        cout << "Input: ";
        cin >> openFile;
        
        load_pgm(pgm, nCol, nRow, mv, type, openFile);
        edit_options(pgm, nCol, nRow, mv, type, openFile);
        
        //        for(int i=0; i<(nCol*nRow);i++){
        //            cout<<"PGM["<<i<<"]: "<<pgm[i]<<endl;
        //        }
        //print_header(pgm, nCol,nRow, mv, type, openFile, "Image read in:", 10);
        
    }
    else if (input == "recent") {
        
        string fname = open_recent();
        cout << "Filename: " << fname << endl;
        if (fname == "none") {
            clr();
            cout << "No recent file detected..." << endl;
            cout << "(Press Enter)" << endl;
            multiplatform_system("pause");
            do_file();
        }
        load_pgm(pgm, nCol, nRow, mv, type, fname);
        edit_options(pgm, nCol, nRow, mv, type, fname);
    }
    else if (input == "back") {
        clr();
        main();
    }
    delete[] pgm;
    pgm = NULL;
    //free(pgm);
}

void load_pgm(int * &pgm, int &nc, int &nr, int &mv, string &ty, string &fname) {
    
    if (fname[0] != '.' && fname[0] != sep)
        fname = pathname + fname;
    
    cout << "reading " << fname << endl;
    pgm_read(fname, pgm, nc, nr, mv, ty);
    save_recent(fname);
    
    //print_header(pgm, nc,nr, mv, ty, fname, "Image read in:", 10);
    cout << endl;
}
void save_recent(string fname) {
    
    string tempcommand = "cd " + settingsDirectory;
    system(tempcommand.c_str());
    ifstream recentFile;
    ofstream createFile;
    
    
    remove("recent.txt");
    
    createFile.open("recent.txt");
    createFile << fname;
    createFile.close();
    
    
    string hold = dircmd;
    system(("cd " + pathname + " && " + hold).c_str());
    cout << "recent file saved" << endl;
    return;
    
    
}

void do_settings() {
    
    
    
    //function variables
    string line;
    ifstream settingsFile;
    ofstream createFile;
    string tempcommand = "cd " + settingsDirectory;
    system(tempcommand.c_str());
    settingsFile.open(settingsName);
    
    if (!settingsFile)
    {
        cout << "settings file not found..." << endl;
        createFile.open(settingsName);
        createFile << userpath + '\n';
        createFile << dircmd + '\n';
        createFile << display_cmd + '\n';
        createFile << settingsName + '\n';
        cout << "created settings file..." << endl;
    }
    else {
        
        
        cout << "settings file found..." << endl;
        if (!settingsFile)
        {
            cerr << "error opening settings..." << endl;
            exit(1);
        }
        else {
            string str;
            int count = 0;
            while (getline(settingsFile, str))
            {
                if (count == 0) userpath = str;
                if (count == 1) dircmd = str;
                if (count == 2) display_cmd = str;
                if (count == 3) settingsName = str;
                count++;
                
            }
            cout << "loaded settings from file..." << endl;
            
        }
    }
}
void change_settings() {
    clr();
    string input;
    string newU;
    ofstream saveSettings;
    cout << "~~~~SETTINGS~~~~" << endl;
    cout << "*ImagePath: " << userpath << endl;
    cout << "*ImageView: " << dircmd << endl;
    cout << endl;
    cout << "Edit('ip' or 'iv' || 'e' to exit): ";
    cin >> input;
    input = lowerCase(input);
    if (input == "c" || input == "cancel" || input == "exit" || input == "c") main();
    if (input == "path" || input == "ip" || input == "imagepath") {
        clr();
        cout << "~~~~SETTINGS~~~~" << endl;
        cout << "*Old_ImagePath: " << userpath << endl;
        cout << endl;
        cout << "Input(newPath): ";
        cin >> newU;
        userpath = newU;
    }
    if (input == "view" || input == "iv" || input == "imageview") {
        clr();
        cout << "~~~~SETTINGS~~~~" << endl;
        cout << "*Old_ImageView: " << dircmd << endl;
        cout << endl;
        cout << "Input(newView): ";
        cin >> newU;
        dircmd = newU;
    }
    
    if (remove("settings.txt") != 0)
        perror("settings failed to save");
    else
        puts("settings saved");
    saveSettings.open(settingsName);
    saveSettings << userpath + '\n';
    saveSettings << dircmd + '\n';
    saveSettings << display_cmd + '\n';
    saveSettings << settingsName + '\n';
    change_settings();
}

void do_help() {
    clr();
    cout << "commands:  " << endl;
    cout << "\t" << "'?' or 'help'      help (this screen)" << endl;
    cout << "\t" << "'dir' or 'dir.'    directory listing" << endl;
    // cout << "\t" <<"                        (dir: "<<pathname<<")"<<endl;
    cout << "\t" << "                        (dir.: current directory)" << endl;
    cout << "\t" << "'q' or 'quit'      quit" << endl;
    cout << "\t" << "'save' or 'saveas' save current image" << endl;
    cout << "\t" << "'read'             read an image" << endl;
    cout << "\t" << "'revert'           revert to original (last read) image" << endl;
    cout << "\t" << "'print'            print pixel values of subregion" << endl;
    cout << "\t" << "'display'          display current image using viewer: " << endl;
    // cout << "\t" <<"                        '"<<display_cmd << "'"<<endl;
    cout << "\t" << "'view'             view a file on the disk" << endl;
    cout << "\t" << "'negate'           negate current image" << endl;
    cout << "\t" << "'clear'            clears screen" << endl;
    // cout << "\t" <<"'recent'           loads last opened file"<<endl;
    //cout << "\t" <<"'list'             lists files in directory"<<endl;
    cout << endl;
    
    // cout << "viewer cmd for image display:  '"<<display_cmd<<"'"<<endl;
    // cout << "folder for reading images: '"<<pathname << "'"<<endl<<endl;
}


void pgm_read_hdr(istream &ifs, int  &nc, int &nr, int &mv, string &ty)             // reads header info from PGM.
// reads the image header, saving the type, ncols, nrows & maxval.
{
    ifs >> ty;
    //    cout << "type: "<<ty<<endl;
    skip_comments(ifs);
    ifs >> nc;
    //    cout << "nc  : "<<nc<<endl;
    skip_comments(ifs);
    ifs >> nr;
    //    cout << "nr  : "<<nr<<endl;
    skip_comments(ifs);
    ifs >> mv;
    //    cout << "mv  : "<<mv<<endl;
    ifs.get();      // final white space.
    
}

void pgm_read(string fname, int * &pgm, int &nc, int &nr, int &mv, string &ty)           // read a PGM from a file
// Reads a PGM image, either P5 (binary) or P2 (ascii)
{
    if (fname.length() == 0) return;
    
    if (!file_exists(fname))
    {
        cerr << "pgm_read(" << fname << "):  cannot open file for reading" << endl;
        cout << "(PRESS ENTER)" << endl;
        multiplatform_system("pause");
        do_file();
        
    }
    ifstream ifs(fname);
    
    // if pgm is a pointer, do something with old memory before allocating new memory
    if (pgm)
    {
        delete[]pgm;
    }
    
    pgm_read_hdr(ifs, nc, nr, mv, ty);              // reads in type, ncols, nrows, maxval, and skips over any comments.
    if (!ifs)
    {
        cerr << "error reading file " << fname << endl;
        do_file();
        return;
    }
    
    if (nr*nc <= 0)
    {
        cerr << "error:  file " << fname << ": type " << ty << " nc: " << nc << " nr: " << nr << " mv: " << mv << endl;
        do_file();
        return;
    }
    //int arraysize=
    
    //pgm= new int[nr*nc];
    pgm = new int[nr*nc];
    //memset(pgm, 0, nr*nc);
    
    if (pgm == nullptr) {
        clr();
        cout << "Error creating allocating memory in array..." << endl;
        return;
    }
    // if pgm is a pointer, allocate memory for the pgm array
    
    if (ty == "P2")		// ascii image.
    {
        for (int i = 0; i<(nr*nc); i++) {
            ifs >> pgm[i];
            
            //cout<<"SETTING VALUE["<<i<<"]: "<<pgm[i]<<endl;
        }
        
    }
    if (ty != "P2")			//binary image. close file, reopen in binary mode, reread header and read pixels.
    {
        ifs.close();
        ifstream ifs(fname, ios::binary);
        if (!ifs) cerr << "problem reopening file in binary mode" << endl;
        pgm_read_hdr(ifs, nc, nr, mv, ty);              // reads in type, ncols, nrows, maxval, and skips over any comments.
        
        // allocate a temporary buffer for 8-bit pixels
        unsigned char *p = new unsigned char[nr*nc];  // allocate temp char array for binary read of pixels
        
        ifs.read(reinterpret_cast<char *>(p), nr*nc * sizeof(unsigned char));		// read the pixels into a char array
        
        if (!ifs) cerr << "problem reading binary" << endl;
        for (int i = 0; i<nr*nc; i++)			// and copy them into the integer pixel array.
            pgm[i] = p[i];
        delete[] p;
    }
    
}
void skip_comments(istream &ifs)			// skips over any comments in the image header.
{
    char c;
    while (isspace(ifs.peek())) ifs.get();  // skip over whitespace.
    if (ifs.peek() == '#')
    {
        //        cout << "PGM comment:  ";
        do
        {
            c = ifs.get();
            //            cout << c;
        } while (ifs && c != '\n');   // read until newline encountered.
    }
}

bool file_exists(string f)		// checks if file exists, returns false if no, true if yes.
{
    ifstream ifs(f);			// this works fine for us...
    if (!ifs) return false;
    return true;
}

void print_header(const int pgm[], int nc, int nr, int mv, string ty, string fname, string label, int npix)     // prints hdr info of current image
{
    if (label.length()>0) cout << label;
    cout << "file: " << fname << endl;
    cout << "header:  type: " << ty << " ncols: " << nc << " nrows: " << nr << " maxval: " << mv << endl;
    cout << pgm[0] << " " << pgm[1] << endl;
    if (npix > 0)
    {
        cout << "pixels:  ";
        for (int i = 0; i<min(nc, 10); i++) {
            cout << setw(3) << pgm[i] << " ";
        }
    }
    
}


void do_quit() {
    clr();
    cout << "Quitting program..." << endl;
    exit(EXIT_SUCCESS);
}
//save function
void save_suffix(const int pgm[], int nc, int nr, int mv, string  type, string filename, string cmd) {
    string fname = filename;
    
    if (cmd == "save_as")
    {
        clr();
        cout << "~~~~SAVE IMAGE~~~~" << endl;
        cout << "please enter name for new file" << endl;
        cout << "Input: ";
        cin >> filename;
        
        if (filename.length()>4) {
            if (filename.substr(filename.length() - 4) == ".pgm") {
                
            }
            else {
                filename += ".pgm";
            }
        }
        if (filename.length() <= 4) {
            filename += ".pgm";
        }
        filename = pathname + filename;
    }
    
    if (file_exists(filename))
    {
        clr();
        string input;
        cout << "file " << filename << " already exists" << endl;
        cout << "~~~~SAVE IMAGE~~~~" << endl;
        cout << "OVERWRITE FILE" << endl;
        cout << " *Yes" << endl;
        cout << " *No" << endl;
        cout << "Input: ";
        cin >> input;
        input = lowerCase(input);
        input = interpret(input, "overwrite");
        
        if (input == "no")
        {
            clr();
            cout << "file not saved..." << endl;
            cout << "(Press Enter)" << endl;
            multiplatform_system("pause");
            return;
            
        }
    }
    cout << "saved image to " << filename << endl;
    cout << "(PRESS ENTER)" << endl;
    multiplatform_system("pause");
    pgm_save(pgm, nc, nr, mv, type, filename);
}

void pgm_save(const int pgm[], int nc, int nr, int mv, string  typ, string filename)  // save a PGM to a file
{
    cout << "save:  typ: " << typ << " filename: " << filename << endl;
    if (filename.length() == 0)
    {
        cerr << "error:  no filename available.  Cannot save PGM" << endl;
        return;
    }
    ofstream ofs;
    
    if (typ != "P2" && typ != "P5") typ = "P5";
    cout << typ << endl;
    if (typ == "P2")
    {
        ofs.open(filename);
        if (!ofs) return;
        
        ofs << typ << " " << nc << " " << nr << " " << mv << endl;
        
        for (int r = 0; r<nr; r++)
        {
            for (int c = 0; c<nc; c++)
            {
                ofs << setw(3) << pgm[r*nc + c];
                if (c % 8 == 7 || c == (nc - 1))
                    ofs << endl;
                else
                    ofs << " ";
            }
        }
        ofs.close();
        return;
    }
    else
    {
        ofs.open(filename, ios::binary);
        ofs << typ << " " << nc << " " << nr << " " << mv << "\n";
        
        unsigned char *p = new unsigned char[nr*nc];        // temporary char buffer
        for (int i = 0; i<nc*nr; i++)
            p[i] = static_cast<unsigned char>(pgm[i]);      // copy pixels to buffer
        ofs.write(reinterpret_cast<char *>(p), nc*nr * sizeof(unsigned char));    // output pixels
        ofs.close();
        delete[] p;
    }
}
////display pgm
void pgm_display(const int pgm[], int nc, int nr, int mv)                       // display using external viewer
{
    
    string tmpname;		//create a temporary filename
    
    // On a PC, make sure we've got a viewer before we bother to write the image to a temporary file
    // On the mac, we'll use Preview, which is part of the MacOS software.
    //#ifdef WIN32
    //						// make sure image viewer is there:
    //	if (!file_exists(display_cmd))
    //	{
    //		cout << endl << "***************problem:  image viewer program " << display_cmd << "doesn't exist\n" << endl;
    //		return;
    //	}
    //#endif
    
    do      // create a unique temporary filename, and make sure the file doesn't exist yet.
    {
        ostringstream oss;
        oss << pathname + "pgm_temp_" << rand() << ".pgm";
        tmpname = oss.str();
    } while (file_exists(tmpname));
    cout << "tmpname: " << tmpname << endl;
    
    // Save current image and spawn image viewer.
    pgm_save(pgm, nc, nr, mv, "P5", tmpname);       // save it as a binary P5 PGM so it reads more quickly...
    //pgm_save(<#const int *pgm#>, <#int nc#>, <#int nr#>, <#int mv#>, <#string typ#>, <#string filename#>)
    
    pgm_view(tmpname, true);
}

void do_view(void)          // display an image from the disk.
{
    string filename;
    cout << "filename to view? ";
    cin >> filename;
    string pfname = pathname + filename;       // same, but looking in default place for images.
    if (file_exists(filename))
        pgm_view(filename, false);
    else if (file_exists(pfname))
        pgm_view(pfname, false);
    else
        cout << "can't find file " << filename << " or " << pfname << endl;
    
}
void pgm_view(string fname, bool do_delete)

{
    
    
#ifdef _WIN32       // This next bit is for visual studio on a PC . Visual Studio uses _execl and _spawnl
    
    // using _spawn(...) . _spawn with P_NOWAIT spawns a separate process
    // and continues current process immediately.
    // NOTE:  display_cmd is passed in twice...2nd time (only) must be quoted if it contains spaces.
    // imagename also must be quoted if it contains spaces.
    //string qprog = quote(display_cmd);		// quoted version.  needed if display_cmd contains spaces.
    string tmpname;							// this is typical in PCs, which tend to install programs in dir "c:\Program Files"
    string qimage = (fname);
    cout << qimage << endl;
    // quoted version.  needed if image_name contains spaces.
    //cout << endl << "executing command: " << display_cmd  + " " + qimage << endl << endl;
    _spawnl(P_NOWAIT, display_cmd.c_str(), qimage.c_str(), NULL);
    if (do_delete)
        Sleep(2000);	// 2 seconds should be long enough for irfanview to open and display the image.
    system(("del " + fname).c_str());	// get rid of the temporary image file
    
    
#else               // This next bit is for OS X & UNIX.  They use execlp and fork.
    
    string cmd = display_cmd + " " + fname;
    cout << endl << "executing command: " << cmd << endl << endl;
    system(cmd.c_str());
    if (do_delete)
    {
        
        system(("sleep 2; rm " + fname).c_str());
    }
#endif
    
}




///image edit function below
void edit_options(int pgm[], int nc, int nr, int mv, string ty, string fname) {
    clr();
    string input;
    bool selected = false;
    
    while (!selected) {
        cout << "loaded: " << fname << endl;
        cout << "~~~~IMAGE OPTIONS~~~~" << endl;
        //cout<<" *Print"<<endl;
        cout << " *Display" << endl;
        cout << " *Negate" << endl;
        //cout<<" *Rotate"<<endl;
        cout << " *Edge" << endl;
        cout << " *Newspaper" << endl;
        cout << " *Blur" << endl;
        cout << " *Histogram" << endl;
        cout << " *Save" << endl;
        cout << "Input: ";
        cin >> input;
        input = lowerCase(input);
        input = interpret(input, "imagemenu");
        if (input == "back") {
            main();
        }
        if (input == "quit") {
            do_quit();
        }
        if (input == "print") {
            
        }
        if (input == "blur") {
            do_blur(pgm, nc, nr, mv);
        }
        if (input == "display") {
            pgm_display(pgm, nc, nr, mv);
            //pgm_view(fname);
        }
        if (input == "negate") {
            pgm_negate(pgm, nc, nr, mv);
            
            
        }
        if (input == "rotate") {
            
        }
        if (input == "clip") {
            //do_clip
        }
        if (input == "edge") {
            do_edge(pgm, nc, nr, mv);
            
        }
        if (input == "newspaper") {
            do_black_white(pgm, nc, nr, mv);
            
        }
        if (input == "histogram") {
            do_histogram(pgm, nc, nr, mv);
            
        }
        if (input == "save") {
            selected = true;
        }
    }
    clr();
    
    cout << "~~~~SAVE IMAGE~~~~" << endl;
    cout << " *Save" << endl;
    cout << " *Save As" << endl;
    cout << "Input: ";
    cin >> input;
    input = lowerCase(input);
    input = interpret(input, "savemenu");
    save_suffix(pgm, nc, nr, mv, ty, fname, input);
    edit_options(pgm, nc, nr, mv, ty, fname);
}



void pgm_negate(int *&pgm, int nc, int nr, int mv)
{
    for (int i = 0; i<(nr*nc); i++) pgm[i] = mv - pgm[i];
    clr();
    cout << "Image Negated..." << endl;
    cout << "(Press enter)" << endl;
    multiplatform_system("pause");
}

void do_histogram(int *&pgm, int nc, int nr, int mv) {
    long int *histogram=new long int[mv + 1];
    long long int *lookUP = new long long int[mv + 1];
    long int countHist = 0;
   
    for (int i = 0; i<(mv + 1); i++) {
        histogram[i] = 0;
        lookUP[i] = 0;
    }
    for (int i = 0; i<nc; i++) {
        for (int j = 0; j<nr; j++) {
            histogram[pgm[countHist]] = histogram[pgm[countHist]] + 1;
            countHist++;
        }
    }
    
    double alpha = mv / static_cast<double>(nr*nc);
    
    
    
    long long int sum = 0;
    
    
    for (int i = 0; i<(mv + 1); i++) {
        sum += histogram[i];
        lookUP[i] = sum * mv / (nr*nc);
        
        
    }
    countHist = 0;
    long long int hold;
    for (int i = 0; i<nc; i++) {
        for (int j = 0; j<nr; j++) {
            
            hold = lookUP[pgm[countHist]];
            pgm[countHist] = static_cast<int>(hold);
            countHist++;
            
        }
    }
    clr();
    cout << "Image histogram equalized!" << endl;
    cout << "(Press Enter)" << endl;
    multiplatform_system("pause");
    
}

void do_black_white(int *&pgm, int nc, int nr, int mv) {
    int average;
    int count = 0;
    average = average_pgm(pgm, nc, nr);
    for (int i = 0; i<nr; i++) {
        for (int j = 0; j<nc; j++) {
            int value = pgm[count];
            if (value <= average) {
                pgm[count] = 0;
            }
            if (value>average) {
                pgm[count] = mv;
            }
            count++;
        }
    }
    
    clr();
    cout << "Image converted to newspaper!" << endl;
    cout << "(PRESS ENTER)" << endl;
    multiplatform_system("pause");
    
}

void do_edge(int *&pgm, int nc, int nr, int mv) {
#ifdef __APPLE__
    clr();
    string input;
    int edge_array[nc*nr];
    int horizontal[nc*nr];
    int vertical[nc*nr];
    memset(vertical, 0, (nc*nr));
    memset(horizontal, 0, (nc*nr));
    memset(edge_array, 0, (nr*nc));
    cout << "~~~~Edge Detection~~~~" << endl;
    cout << " *Horizontal" << endl;
    cout << " *Vertical" << endl;
    cout << " *Both" << endl;
    cout << "Input: ";
    cin >> input;
    input = lowerCase(input);
    input = interpret(input, "edgemenu");
    //int cut_off=average_pgm(pgm, nc, nr)/12;
    int average = average_pgm(pgm, nc, nr);
    int cut_off = average / 8;
    cout << "Average: " << average << endl;
    cout << "Cut_Off: " << cut_off << endl;
    int count = 1;
    bool cutoff = false;
    int left;
    int right;
    int up;
    int down;
    int newpgm[nr][nc];
   
    int con_count = 0;
    for (int i = 0; i<nr; i++) {
        for (int j = 0; j<nc; j++) {
            newpgm[i][j] = pgm[con_count];
            //cout<<"PGM"<<newpgm[i][j]<<endl;
            con_count++;
        }
    }
    int subR;
    int addR;
    int subC;
    int addC;
    if (input == "horizontal" || input == "both") {
        int hoz_count = 0;
        int test = 0;
        for (int i = 0; i<nr; i++) {
            for (int j = 0; j<nc; j++) {
                subR = i - 1;
                addR = i + 1;
                subC = j - 1;
                addC = j + 1;
                //cout<<"subR: "<<subR<<endl;
                //cout<<"addR: "<<addR<<endl;
                //cout<<"subC: "<<subC<<endl;
                //cout<<"addC: "<<addC<<endl;
                if (subR<0) {
                    subR = 0;
                }
                if (subC<0) {
                    subC = 0;
                }
                if (addR>nr) {
                    addR = nr;
                }
                if (addC>nc) {
                    addR = nc;
                }
                left = ((newpgm[subR][subC])*(-1)) + ((newpgm[i][subC])*(-2)) + ((newpgm[addR][subC])*(-1));
                right = ((newpgm[subR][addC])*(1)) + ((newpgm[i][addC])*(2)) + ((newpgm[addR][addC])*(1));
                // cout<<"right+left:"<<right+left<<endl;
                //                before=(pgm[count-1])*-1;
                //                //now=pgm[count];
                //                after=pgm[count+1];
                //                now=(after+before);
                if (input != "both") {
                    cutoff = false;
                    if (right + left>cut_off) {
                        horizontal[count] = mv;
                        cutoff = true;
                    }
                    if (right + left<-cut_off) {
                        horizontal[count] = 0;
                        cutoff = true;
                    }
                    
                    if (cutoff == false) {
                        horizontal[count] = mv / 2;
                    }
                }
                if (input == "both") {
                    horizontal[count] = (left + right);
                }
                //cout<<"Cutoff: "<<cutoff<<" Diffrence: "<<now<<endl;
                count++;
            }
        }
        
        if (input != "both") {
            count = 0;
            for (int i = 0; i<nc; i++) {
                for (int j = 0; j<nr; j++) {
                    pgm[count] = horizontal[count];
                    count++;
                }
            }
            clr();
            cout << "Horizontal edge done...." << endl;
            cout << "(Press Enter)" << endl;
            multiplatform_system("pause");
            return;
        }
        
    }
    if (input == "vertical" || input == "both") {
        int ver_count = 0;
        int test = 0;
        for (int i = 0; i<nr; i++) {
            for (int j = 0; j<nc; j++) {
                subR = i - 1;
                addR = i + 1;
                subC = j - 1;
                addC = j + 1;
                //cout<<"subR: "<<subR<<endl;
                //cout<<"addR: "<<addR<<endl;
                //cout<<"subC: "<<subC<<endl;
                //cout<<"addC: "<<addC<<endl;
                if (subR<0) {
                    subR = 0;
                }
                if (subC<0) {
                    subC = 0;
                }
                if (addR>nr) {
                    addR = nr;
                }
                if (addC>nc) {
                    addR = nc;
                }
                down = ((newpgm[subR][subC])*(-1)) + ((newpgm[subR][j])*(-2)) + ((newpgm[subR][addC])*(-1));
                up = ((newpgm[addR][subC])*(1)) + ((newpgm[addR][j])*(2)) + ((newpgm[addR][addC])*(1));
                //  cout<<"up+down:"<<up+down<<endl;
                //                before=(pgm[count-1])*-1;
                //                //now=pgm[count];
                //                after=pgm[count+1];
                //                now=(after+before);
                if (input != "both") {
                    cutoff = false;
                    if (up + down>cut_off) {
                        vertical[ver_count] = mv;
                        cutoff = true;
                    }
                    if (up + down<-cut_off) {
                        vertical[ver_count] = 0;
                        cutoff = true;
                    }
                    
                    if (cutoff == false) {
                        vertical[ver_count] = mv / 2;
                    }
                }
                if (input == "both") {
                    vertical[ver_count] = (up + down);
                }
                //cout<<"Cutoff: "<<cutoff<<" Diffrence: "<<now<<endl;
                
                ver_count++;
            }
        }
        
        if (input != "both") {
            count = 0;
            for (int i = 0; i<nc; i++) {
                for (int j = 0; j<nr; j++) {
                    pgm[count] = vertical[count];
                    count++;
                }
            }
            clr();
            cout << "vertical edge done...." << endl;
            cout << "(Press Enter)" << endl;
            multiplatform_system("pause");
            return;
        }
    }
    
    
    int *weightpgm = new int[nr*nc];
    int value = 0;
    int hold;
    int direction;
    if (input == "both") {
        for (int i = 0; i<nr; i++) {
            for (int j = 0; j<nc; j++) {
                hold = pow(vertical[value], 2)*pow(horizontal[value], 2);
                hold = static_cast<int>(sqrt(hold));
                cutoff = false;
                direction = atan2(vertical[value], horizontal[value]);
                if (direction >= 0) direction = 1;
                if (direction<0) direction = -1;
                hold *= direction;
                hold *= -1;
                cout << "DIR: " << direction << endl;
                if (hold>cut_off * 2) {
                    weightpgm[value] = mv;
                    cutoff = true;
                }
                if (hold<-cut_off * 2) {
                    weightpgm[value] = 0;
                    cutoff = true;
                }
                if (cutoff == false) {
                    weightpgm[value] = mv / 2;
                }
                
                
                value++;
            }
        }
        value = 0;
        for (int i = 0; i<nr; i++) {
            for (int j = 0; j<nc; j++) {
                hold = weightpgm[value];
                pgm[value] = hold;
                value++;
            }
        }
        
        clr();
        cout << "both edge done...." << endl;
        cout << "(Press Enter)" << endl;
        multiplatform_system("pause");
        return;
    }
}
#endif
#ifdef _WIN32
clr();
cout<<"THIS ONLY WORKS FOR APPLE MACHINES SORRY"<<endl;
cout<<"(Press Enter)"<<endl;
multiplatform_system("pause");
return;

#endif
void do_blur(int *&pgm, int nc, int nr, int mv) {
    clr();
    if (blur_done){
        clr();
        cout<<"Sorry in this demo you can only blur once"<<endl;
        cout<<"For more features purchase the full application..."<<endl;
        cout<<"(PRESS ENTER)"<<endl;
        multiplatform_system("pause");
        return;
    }
    int input = 0;
    int *edge_array = new int[nc*nr];
    int *blur = new int[nc*nr];
    memset(blur, 0, (nc*nr));
    memset(edge_array, 0, (nr*nc));
    cout << "How many iterations of Blur would you like to run?" << endl;
    cout << "Input: ";
    cin >> input;
    int left;
    int right;
    int center;
    int *newPGM=new int[nc*nr];
   
    
    blur_done=true;
    for (int w = 0; w<input; w++) {
        int count = 0;
        int con_count = 0;
        
        for (int i = 0; i<nr; i++) {
            for (int j = 0; j<nc; j++) {
                newPGM[con_count]=pgm[con_count];
                con_count++;
            }
        }
        int subT=0;
        int addT=0;
        int subC=0;
        int addC=0;
        int subB=0;
        int addB=0;
        int top_center=0;
        int middle_center=0;
        int bottom_center=0;
        int hold_top=0;
        int hold_center=0;
        int hold_bottom=0;
        
        for (int i = 0; i<nr; i++) {
            for (int j = 0; j<nc; j++) {
                
                if(i==0){
                    hold_top=0;
                   
                }
                if(i==nr){
                    hold_bottom=0;
                }
                
                
                
                
                    top_center=((i+(hold_top))*nr);
                    middle_center=((hold_center)*nr);
                    bottom_center=((i+hold_bottom)*nr);
                
                subT = j - 1;
                addT = j + 1;
                subC = j - 1;
                addC = j + 1;
                subB = j - 1;
                addB = j + 1;
                if (subT<0) subT = 0;
                if (addT>nc) addT = nc;
                if (subC<0) subC = 0;
                if (addC>nc) addC = nc;
                if (subB<0) subB = 0;
                if (addB>nc) addB = nc;
                
                
                left =  center = ((newPGM[top_center+subC])) + ((newPGM[top_center+j])) + ((newPGM[top_center+addC]));
                center = ((newPGM[middle_center+subC])) + ((newPGM[middle_center+j])) + ((newPGM[middle_center+addC]));
                right =  center = ((newPGM[bottom_center+subC])) + ((newPGM[bottom_center+j])) + ((newPGM[bottom_center+addC]));
                int blure = (left + center + right) / 9;
                blur[count] = blure;
                count++;
            }
        }
        
        count = 0;
        for (int i = 0; i<nc; i++) {
            for (int j = 0; j<nr; j++) {
                pgm[count] = blur[count];
                count++;
            }
        }
        if (w == input - 1)
        {
            clr();
            cout << "Bluring done...." << endl;
            cout << "(Press Enter)" << endl;
            multiplatform_system("pause");
        }
    }
    return;
}

void do_clip(int *&pgm, int nc, int nr) {
    int clip_array[4];
    
    for (int i = 0; i<4; i++) {
        clr();
        cout << "#Row: " << nr << " #Col: " << nc << endl;
        cout << "~~~~Clip Menu~~~~" << endl;
        if (i == 0) cout << "TopLeft x: ";
        if (i == 1) cout << "TopLeft y: ";
        if (i == 2) cout << "BottomRight x: ";
        if (i == 3) cout << "BottomRight y: ";
        cin >> clip_array[i];
        if ((i == 0 || i == 2) && clip_array[i]>nc) {
            clr();
            cout << "Invalid input!" << endl;
            cout << "(Press Enter)" << endl;
            multiplatform_system("pause");
            clip_array[i] = 0;
            i = i - 1;
        }
        if ((i == 1 || i == 4) && clip_array[i]>nr) {
            clr();
            cout << "Invalid input!" << endl;
            cout << "(Press Enter)" << endl;
            multiplatform_system("pause");
            clip_array[i] = 0;
            i = i - 1;
        }
        int count = 0;
        
        int *newPGM= new int [(clip_array[2] - clip_array[0])*(clip_array[3] - clip_array[1])];
        for (int i = clip_array[1]; i<clip_array[3]; i++) {
            for (int j = clip_array[0]; j<clip_array[2]; j++) {
                newPGM[count] = pgm[(count + clip_array[i])*(count + clip_array[j])];
            }
        }
        
        
        
    }
    
    
    
    
}





