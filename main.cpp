//
//  main.cpp
//  imageEditor_v2

//  Created by Devin Gearing on 11/28/16.
//  Copyright © 2016 Devin Gearing. All rights reserved.
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
string homepath=string(getenv("HOME")) + "/";
string userpath="Desktop/sample_images/";
string pathname=homepath+userpath;
char sep = '/';
string dircmd="ls -R |grep .pgm";
string display_cmd="open -a ToyViewer ";
string settingsName ="settings.txt";
#endif

#ifdef _WIN32
string dircmd = "dir  /s /b *.pgm | findstr /e .pgm";
string display_cmd="start "+'"'+"i_view64.exe"+'"';
string userpath="Desktop\\sample_images\\";
#pragma warning(push)
#pragma warning(disable: 4996)
string homepath=string(getenv("HOMEPATH")) + "\\";
#pragma warning(pop)
#endif


//string settingsLocation=system("getcwd");

//string settingsLocation=system("_getcwd");

void save_recent(string fname);

//function declartion
void clr();
string menu(int opt);
string interpret(string input,string menuType);
string lowerCase(string input);
void do_file();
void save_suffix(const int pgm[], int nc, int nr, int mv, string  type,string filename, string cmd);
void pgm_save(const int pgm[], int nc, int nr, int mv, string  typ, string filename);
void do_settings();
string open_recent();
void change_settings();
void do_help();
void do_quit();
void system_pause();
void multiplatform_system(string cmd);
int average_pgm(int pgm[], int nc, int nr);
void rec2polar(int *&pgm, int x, int y , double &radius, double &theta);
void polar2rec(int *&pgm, double radius, double theta,int &x, int&y);
double magnitude(int x, int y, int origin_X,int origin_Y);
double theta(int x, int y,int origin_X,int origin_Y);


//old func
void load_pgm(int *&pgm, int &nc, int &nr, int &mv, string &ty, string &fname);
void pgm_read_hdr(istream &ifs, int  &nc, int &nr, int &mv, string &ty);           // reads header info from
void skip_comments(istream &ifs);      // skips over whitespace and comments in a PGM image header.
void pgm_read(string fname, int * &pgm, int &nc, int &nr, int &mv, string &ty);               // read a P
bool file_exists(string f);
void edit_options(int pgm[], int nc, int nr, int mv, string ty, string fname, int *&numPixels);
void print_header(const int pgm[], int nc, int nr, int mv, string ty, string fname, string label, int npix=10);
void pgm_display(const int pgm[], int nc, int nr, int mv);
void pgm_view(string fname, bool do_delete=false);
void do_view(void);
string getcwd_string();

//image edit functions
void pgm_negate(int *&pgm, int nc, int nr, int mv);
void do_pixelate(int *&pgm, int nc, int nr);
void do_histogram(int *&pgm, int nc, int nr, int mv);
void do_rotate(int *&pgm, int &nc, int &nr);
void do_black_white(int *&pgm,int nc,int nr,int mv);
void do_blur(int *&pgm, int nc, int nr, int mv);
void do_edge(int *&pgm, int nc, int nr, int mv);
void do_clip(int *&pgm, int &nc, int &nr);
void do_arbirarty_rotation(int *& pgm, int &nr,int &nc);
void resampling_smallPGM(int &nr, int &nc, int *pgm);
void resampling_largePGM(int &nr, int &nc, int *pgm);
void do_resample(int *&pgm, int &nc, int &nr);
void table_histogramPGM(int &nc, int &nr, int maxval, int *pgm, int *numPixels);


string settingsDirectory;

int main() {
    settingsDirectory=getcwd_string();
    string cmd;
    do_settings();
   
    
    
    cmd=menu(1);
    if(cmd=="file"){
        do_file();
    }
    if(cmd=="settings"){
        change_settings();
    
    }
    if(cmd=="help"){
        do_help();
        cout<<"PRESS ENTER..."<<endl;
        multiplatform_system("pause");
        main();
        
    }
    if(cmd=="quit"){
        do_quit();
        
    }
    else{
        clr();
        main();
    }
    
    
       return 0;
}

string menu(int opt){
    clr();
    string cmd="";
   
    if (opt==1){
    //DISPLAYED MENU
    cout<<"~~~~IMAGE EDITOR v0.02~~~~"<<endl;
    cout<<" *File"<<endl;
    cout<<" *Settings"<<endl;
    cout<<" *Help"<<endl;
    cout<<" *Quit"<<endl;
    cout<<"Input: ";
    string input;
    cin>>(input);
    cmd=interpret(input,"mainmenu");
    }

    return cmd;
    
    
}

/////helpful functions
double magnitude(int x, int y, int origin_X,int origin_Y){
    double mag=0;
    mag=sqrt((pow(origin_X-x,2))+pow(origin_Y-y,2));
    return mag;
}
double theta(int x, int y,int origin_X,int origin_Y){
    double theta=0;
    theta=atan2(origin_Y-y,origin_X-x);
    return theta;
}

void multiplatform_system(string cmd){
    cmd=lowerCase(cmd);
#ifdef WIN32
    if (cmd=="pause"){
        cmd="pause";
    }
#else
    if (cmd=="pause"){
        cmd="read";
    }
    
#endif
    system(cmd.c_str());
}

void rec2polar(int *&pgm, int x, int y, int &radius, int &theta){
    radius=sqrt(pow(x,2)+pow(y,2));
    theta=atan2(y, x);

}
void polar2rec(int *&pgm, int radius, int theta, int&x, int&y){
    x=radius*cos(theta);
    y=radius*sin(theta);
}



string interpret(string in, string type){
    string input;
    string output;
    input=lowerCase(in);
    cout<<input<<endl;
    //inteprets input to singular input
    if (type=="mainmenu"){
        if(input=="file"||input=="open file"||input=="openfile"||input=="open") output="file";
        if(input=="settings"||input=="setting"||input=="sett"||input=="set") output="settings";
        if(input=="?"||input=="help"||input=="wut") output="help";
        if(input=="quit"||input=="exit"||input=="overboard"||input=="leave") output="quit";
        
        if(input=="back"||input=="before") output="back";
    }
    
    if(type=="openmenu"){
        if(input=="open"||input=="read"||input=="load"||input=="import") output="import";
        if(input=="back"||input=="before") output="back";
        if(input=="open recent"||input=="recent"||input=="last"||input=="or") output="recent";
    }
    if(type=="imagemenu"){
        if(input=="print"||input=="printout"||input=="out"||input=="output") output="print";
        if(input=="display"||input=="disp"||input=="open"||input=="view") output="display";
        if(input=="negate"||input=="neg"||input=="inverse"||input=="invert") output="negate";
        if(input=="rotate"||input=="rot"||input=="turn"||input=="angle"||input=="r") output="rotate";
        if(input=="clip"||input=="section"||input=="portion"||input=="part") output="clip";
        if(input=="histogram"||input=="hist"||input=="equal"||input=="balance"||input=="histo") output="histogram";
        if(input=="save"||input=="export"||input=="saveas"||input=="save as") output="save";
        if(input=="black and white"||input=="white"||input=="bnw"||input=="bw")output="newspaper";
        if(input=="newspaper"||input=="newpaper"||input=="news"||input=="new")output="newspaper";
        if(input=="black"||input=="paper"||input=="papers"||input=="np")output="newspaper";
        if(input=="back"||input=="before"||input=="last") output="back";
        if(input=="table"||input=="hist_table"||input=="tab"||input=="ht") output="hist_table";
         if(input=="b"||input=="bl"||input=="blu"||input=="blur") output="blur";
        if(input=="cl"||input=="clip"||input=="c"||input=="cut") output="clip";
        if(input=="eg"||input=="ed"||input=="edg"||input=="e"||input=="border"||input=="edge") output="edge";
        if(input=="sample"||input=="resample"||input=="resize"||input=="s") output="sample";
        if(input=="quit"||input=="exit"||input=="overboard"||input=="leave") output="quit";

    }
    if(type=="savemenu"){
        if(input=="save"||input=="savesame"||input=="same") output="save";
        if(input=="saveas"||input=="save as"||input=="new"||input=="new save") output="save_as";
    }
    if(type=="overwrite"){
        if(input=="yes"||input=="yeah"||input=="sure"||input=="y") output="yes";
        if(input=="no"||input=="nope"||input=="n") output="no";
        if(input=="e"||input=="can"||input=="c"||input=="exit"||input=="back") output="no";
    }
    if(type=="edgemenu"){
        if(input=="hori"||input=="hor"||input=="h"||input=="horiz") output="horizontal";
        if(input=="ver"||input=="vert"||input=="verti"||input=="v") output="vertical";
        if(input=="b"||input=="bo"||input=="bot"||input=="both"||input=="vh") output="both";
    }
    if(type=="samplemenu"){
        if(input=="sh"||input=="shrink"||input=="thumbnail"||input=="thumb") output="shrink";
         if(input=="small"||input=="little"||input=="nail") output="shrink";
        if(input=="st"||input=="stretch"||input=="large"||input=="expand") output="stretch";
        if(input=="str"||input=="lg"||input=="ex") output="stretch";
        
    }
    
    
    //return singular input
    return output;
}

string getcwd_string() {
    char buff[PATH_MAX];
    GetCurrentDir( buff, PATH_MAX );
    std::string cwd( buff );
    return cwd;
}



string lowerCase(string in){
    locale loc;
    string output;
    
    for (int i=0; i<in.length(); ++i){
        output+= tolower(in[i],loc);
    }
    
    return output;
}

void clr(){
    cout<<string(50,'\n');
}

int average_pgm(int pgm[], int nc, int nr){
    int average;
    long int hold=0;
    long int count=0;
    for(int i=0;i<nr;i++){
        for(int j=0;j<nc;j++){
            hold+=pgm[count];
            count++;
        }
    }
    average=hold/count;
    return average;
}


string open_recent(){
    string fname;
    ifstream recentFile;
       recentFile.open("recent.txt");
    
    if (!recentFile)
    {
        fname="none";
    }
    else{
        string str;
        int count=0;
        while (getline(recentFile, str))
        {
            if (count==0) fname=str;
            count++;
        }
    }

    return fname;
}


void do_file(){
    
    int * pgm=NULL;
    int * numPixels =nullptr;
   
    int nRow=0, nCol=0, mv=255;
    string type, type_orig;
    string fname="", orig_name="";

    clr();
    string in;
    string input;
    cout<<"~~~~FILE MENU~~~~"<<endl;
    cout<<" *Open"<<endl;
    cout<<" *Open Recent"<<endl;
    cout<<" *Back"<<endl;
    cout<<"Input: ";
    cin>>in;
    input=lowerCase(in);
    input=interpret(input, "openmenu");
    cout<<input<<endl;
    
    if(input=="import"){
        clr();
        string openFile;
        cout<<"~~~~OPEN FILE~~~~"<<endl;
        string hold="ls *.pgm";
        system(("cd "+pathname+" && "+hold).c_str());
        cout<<"Input: ";
        cin>>openFile;
        
        load_pgm(pgm, nCol, nRow, mv, type, openFile);
        edit_options(pgm, nCol, nRow, mv, type, openFile, numPixels);
        

    
    }
    else if(input=="recent"){
        
        string fname=open_recent();
        cout<<"Filename: "<<fname<<endl;
        if (fname=="none"){
            clr();
            cout<<"No recent file detected..."<<endl;
            cout<<"(Press Enter)"<<endl;
            multiplatform_system("pause");
            do_file();
        }
        load_pgm(pgm, nCol, nRow, mv, type, fname);
        edit_options(pgm, nCol, nRow, mv, type, fname, numPixels);
    }
    else if(input=="back"){
        clr();
        main();
    }
   delete [] pgm;
    pgm=NULL;
    delete [] numPixels;
    numPixels=NULL;

}

void load_pgm(int * &pgm, int &nc, int &nr, int &mv, string &ty, string &fname){
    
    if (fname[0]!='.' && fname[0] != sep)
        fname = pathname + fname;
    save_recent(fname);
    cout << "reading "<<fname<<endl;
    pgm_read(fname, pgm, nc, nr, mv, ty);
    cout << endl;
}
void save_recent(string fname){
    
    string tempcommand="cd "+settingsDirectory;
    system(tempcommand.c_str());
    ifstream recentFile;
    ofstream createFile;
    
    
    remove( "recent.txt" );
    
        createFile.open("recent.txt");
         createFile << fname;
        createFile.close();
    
    
    string hold="ls *.pgm";
    system(("cd "+pathname+" && "+hold).c_str());
    cout<<"recent file saved"<<endl;
    return;
    
    
}

void do_settings(){
    
    
    
    //function variables
    string line;
    ifstream settingsFile;
    ofstream createFile;
    string tempcommand="cd "+settingsDirectory;
    system(tempcommand.c_str());
    settingsFile.open(settingsName);
    
    if (!settingsFile)
    {
        cout<< "settings file not found..." << endl;
        createFile.open(settingsName);
        createFile << userpath+'\n';
        createFile << dircmd+'\n';
        createFile << display_cmd+'\n';
        createFile << settingsName+'\n';
        cout<< "created settings file..." << endl;
    }
    else{
        
        
        cout<< "settings file found..." << endl;
        if (!settingsFile)
        {
            cerr<< "error opening settings..." << endl;
            exit(1);
        }
        else{
            string str;
            int count=0;
            while (getline(settingsFile, str))
            {
                if (count==0) userpath=str;
                if (count==1) dircmd=str;
                if (count==2) display_cmd=str;
                if (count==3) settingsName=str;
                count++;
                
            }
            cout<< "loaded settings from file..." << endl;

        }
    }
}
void change_settings(){
    clr();
    string input;
    string newU;
    ofstream saveSettings;
    cout<<"~~~~SETTINGS~~~~"<<endl;
    cout<<"*ImagePath: "<<userpath<<endl;
    cout<<"*ImageView: "<<dircmd<<endl;
    cout<<endl;
    cout<<"Edit('ip' or 'iv' || 'e' to exit): ";
    cin>>input;
    input=lowerCase(input);
    if(input=="c"||input=="cancel"||input=="exit"||input=="c"||input=="e") main();
    if(input=="path"||input=="ip"||input=="imagepath"){
        clr();
        cout<<"~~~~SETTINGS~~~~"<<endl;
        cout<<"*Old_ImagePath: "<<userpath<<endl;
        cout<<endl;
        cout<<"Input(newPath): ";
        cin>>newU;
        userpath=newU;
    }
    if(input=="view"||input=="iv"||input=="imageview"){
        clr();
        cout<<"~~~~SETTINGS~~~~"<<endl;
        cout<<"*Old_ImageView: "<<dircmd<<endl;
        cout<<endl;
        cout<<"Input(newView): ";
        cin>>newU;
        dircmd=newU;
    }
    
    if( remove( "settings.txt" ) != 0 )
        perror( "settings failed to save" );
    else
        puts( "settings saved" );
    saveSettings.open(settingsName);
    saveSettings << userpath+'\n';
    saveSettings << dircmd+'\n';
    saveSettings << display_cmd+'\n';
    saveSettings << settingsName+'\n';
    change_settings();
}

void do_help(){
    clr();
    cout << "commands:  "<<endl;
    cout << "\t" <<"'?' or 'help'      help (this screen)"<<endl;
    cout << "\t" <<"'setting' or 'set'    allows you to change settings"<<endl;
    cout << "\t" <<"'q' or 'quit'      quit"<<endl;
    cout << "\t" <<"'save' or 'saveas' save current image"<<endl;
    cout << "\t" <<"'read'             read an image"<<endl;
    cout << "\t" <<"'edge'           finds sobel operater"<<endl;
    cout << "\t" <<"'newspaper'            newspaper effect(b/w)"<<endl;
    cout << "\t" <<"'display'          display current image using viewer: "<<endl;
    cout << "\t" <<"'histogram'             histogram equalization"<<endl;
    cout << "\t" <<"'negate'           negate current image"<<endl;
    cout << endl;
    
   
}


void pgm_read_hdr(istream &ifs, int  &nc, int &nr, int &mv, string &ty)
{
    ifs >> ty ;
    skip_comments(ifs);
    ifs >> nc;
    skip_comments(ifs);
    ifs >> nr;
    skip_comments(ifs);
    ifs >> mv;
    ifs.get();
    
}

void pgm_read(string fname, int * &pgm, int &nc, int &nr, int &mv, string &ty)
{
    if (fname.length()==0) return;
    
    if (!file_exists(fname))
    {
        cerr << "pgm_read("<<fname<<"):  cannot open file for reading"<<endl;
        cout<<"(PRESS ENTER)"<<endl;
        multiplatform_system("pause");
        do_file();
        
    }
    ifstream ifs(fname);
    
    if (pgm)
    {
        delete []pgm;
    }
    
    pgm_read_hdr(ifs, nc, nr, mv, ty);
    if (! ifs)
    {
        cerr << "error reading file "<<fname<<endl;
        do_file();
        return;
    }
    
    if (nr*nc <= 0)
    {
        cerr << "error:  file "<<fname<<": type "<<ty << " nc: "<<nc<<" nr: "<<nr << " mv: "<<mv << endl;
        do_file();
        return;
    }
    
    pgm = new int[nr*nc];
    
    
    if (pgm == nullptr) {
        clr();
        cout<<"Error creating allocating memory in array..."<<endl;
        return;
    }
    
    
    if (ty == "P2")
    {
        for (int i=0;i<(nr*nc);i++){
            ifs >> pgm[i];
            
            
        }
       
    }
    if(ty!="P2")
    {
        ifs.close();
        ifstream ifs(fname, ios::binary);
        if (!ifs) cerr << "problem reopening file in binary mode"<<endl;
        pgm_read_hdr(ifs, nc, nr, mv, ty);
        
        
        unsigned char *p=new unsigned char[nr*nc];
        ifs.read(reinterpret_cast<char *>(p), nr*nc*sizeof(unsigned char));
        if (!ifs) cerr << "problem reading binary"<<endl;
        for (int i=0;i<nr*nc;i++)
            pgm[i] = p[i];
        delete [] p;
    }
    
}
void skip_comments(istream &ifs)
{
    char c;
    while (isspace(ifs.peek())) ifs.get();
    if (ifs.peek() == '#')
    {
        
        do
        {
            c = ifs.get();
                   }
        while (ifs && c != '\n');
    }
}

bool file_exists(string f)
{
    ifstream ifs(f);
    if (!ifs) return false;
    return true;
}

void print_header(const int pgm[], int nc, int nr, int mv, string ty, string fname, string label, int npix)
{
    if (label.length()>0) cout << label;
    cout << "file: "<<fname<<endl;
    cout << "header:  type: "<<ty<<" ncols: "<<nc<<" nrows: "<<nr<<" maxval: "<<mv<<endl;
    cout<<pgm[0]<<" "<<pgm[1]<<endl;
    if (npix > 0)
    {
        cout << "pixels:  ";
        for (int i=0;i<min(nc, 10);i++){
            cout << setw(3)<<pgm[i]<<" ";
        }
    }
    
}


void do_quit(){
    clr();
    cout<<"Quitting program..."<<endl;
    
    exit (EXIT_SUCCESS);
}

void save_suffix(const int pgm[], int nc, int nr, int mv, string  type,string filename, string cmd){
    string fname=filename;
    
    if (cmd == "save_as")
    {
        clr();
        cout<<"~~~~SAVE IMAGE~~~~"<<endl;
        cout << "please enter name for new file"<<endl;
        cout<<"Input: ";
        cin >> filename;
        
        if(filename.length()>4){
            if(filename.substr( filename.length() - 4 ) == ".pgm"){
                
            }
            else{
                filename+=".pgm";
            }
        }
        if(filename.length()<=4){
            filename+=".pgm";
        }
        filename=pathname+filename;
    }
    
    if (file_exists(filename))
    {
        clr();
        string input;
        cout << "file "<<filename<<" already exists"<<endl;
        cout<<"~~~~SAVE IMAGE~~~~"<<endl;
        cout<<"OVERWRITE FILE"<<endl;
        cout<<" *Yes"<<endl;
        cout<<" *No"<<endl;
        cout<<"Input: ";
        cin>>input;
        input=lowerCase(input);
        input=interpret(input, "overwrite");
        
        if (input == "no")
        {
            clr();
            cout << "file not saved..."<<endl;
            cout <<"(Press Enter)"<<endl;
            multiplatform_system("pause");
            return;
           
        }
    }
    cout << "saved image to "<<filename<<endl;
    cout<<"(PRESS ENTER)"<<endl;
    multiplatform_system("pause");
    pgm_save(pgm, nc, nr, mv, type, filename);
}

void pgm_save(const int pgm[], int nc, int nr, int mv, string  typ, string filename)
{
    cout << "save:  typ: "<<typ << " filename: "<<filename<<endl;
    if (filename.length() == 0)
    {
        cerr << "error:  no filename available.  Cannot save PGM" << endl;
        return;
    }
    ofstream ofs;
    if (typ != "P2" && typ != "P5") typ = "P5";
    
    if (typ == "P2")
    {
        ofs.open(filename);
        if (!ofs) return;
        
        ofs << typ << " " << nc << " " << nr << " " << mv << endl;
        
        for (int r=0;r<nr;r++)
        {
            for (int c=0;c<nc;c++)
            {
                ofs << setw(3) << pgm[r*nc+c];
                if (c%8 == 7 || c == (nc-1))
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
        
        unsigned char *p = new unsigned char[nr*nc];
        for (int i=0;i<nc*nr;i++)
            p[i] = static_cast<unsigned char>(pgm[i]);
        ofs.write(reinterpret_cast<char *>(p), nc*nr*sizeof(unsigned char));
        ofs.close();
        delete [] p;
    }
}
////display pgm
void pgm_display(const int pgm[], int nc, int nr, int mv)
{
    
    string tmpname;
    
   #ifdef WIN32
   
    if (!file_exists(display_cmd))
    {
        cout << endl << "***************problem:  image viewer program " << display_cmd << "doesn't exist\n" << endl;
        return;
    }
#endif
    
    do
    {
        ostringstream oss;
        oss << pathname + "pgm_temp_"<<rand()<<".pgm";
        tmpname = oss.str();
    }
    while (file_exists(tmpname));
    cout << "tmpname: "<<tmpname<<endl;
    
    // Save current image and spawn image viewer.
    pgm_save(pgm, nc, nr, mv, "P5", tmpname);
   
    
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
        cout << "can't find file "<<filename << " or " << pfname << endl;
    
}
void pgm_view(string fname, bool do_delete)

{
    
				
#ifdef _WIN32
				
				
    string qprog  = quote(display_cmd);
    string qimage = quote(tmpname);
    
    cout << endl << "executing command: "<<display_cmd + " " +  qprog + " " + qimage << endl << endl;
    _spawnl(P_NOWAIT, display_cmd.c_str(), qprog.c_str(), qimage.c_str(), NULL);
    if (do_delete)
        Sleep(2000);	// 2 seconds pause
    
				
#else
    
    string cmd = display_cmd + " " + fname;
    cout << endl << "executing command: " << cmd << endl << endl;
    system(cmd.c_str());
    if (do_delete)
    {
        
        system(("sleep 2; rm "+ fname).c_str());
    }
#endif
    
}




///image edit function below
void edit_options(int pgm[], int nc, int nr, int mv, string ty, string fname, int *&numPixels){
    clr();
    string input;
    bool selected=false;
   
    
    while(!selected){
        cout<<"loaded: "<<fname<<endl;
        cout<<"~~~~IMAGE OPTIONS~~~~"<<endl;
        cout<<" *Display"<<endl;
        cout<<" *Clip"<<endl;
        cout<<" *Negate"<<endl;
       // cout<<" *Rotate(buggy)"<<endl;
        cout<<" *Edge"<<endl;
        cout<<" *Resample"<<endl;
        cout<<" *Newspaper"<<endl;
        cout<<" *Blur"<<endl;
        cout<<" *Histogram"<<endl;
        cout<<" *Table"<<endl;
        cout<<" *Save"<<endl;
        cout<<"Input: ";
        cin>>input;
        input=lowerCase(input);
        input=interpret(input,"imagemenu");
        if(input=="back"){
            main();
        }
        if(input=="quit"){
            do_quit();
        }
        if (input=="print"){
        
        }
        if(input=="blur"){
            do_blur(pgm, nc, nr, mv);
        }
        if (input=="display"){
            pgm_display(pgm, nc, nr, mv);
            
        }
        if (input=="negate"){
            pgm_negate(pgm, nc, nr, mv);
            
            
        }
        if (input=="rotate"){
           // do_arbirarty_rotation(pgm, nr, nc);
        }
        if (input=="clip"){
            do_clip(pgm, nc, nr);
        }
        if(input=="edge"){
            do_edge(pgm, nc, nr, mv);
        
        }
        if(input=="sample"){
            do_resample(pgm, nc, nr);
        
        }
        if(input=="newspaper"){
            do_black_white(pgm, nc, nr, mv);
        
        }
        if (input=="histogram"){
            do_histogram(pgm, nc, nr, mv);
            
        }
        if (input=="save"){
            selected=true;
        }
        if(input=="hist_table"){
        
        table_histogramPGM(nc, nr, mv, pgm, numPixels);
        
        }
    }
    clr();
    
    cout<<"~~~~SAVE IMAGE~~~~"<<endl;
    cout<<" *Save"<<endl;
    cout<<" *Save As"<<endl;
    cout<<"Input: ";
    cin>>input;
    input=lowerCase(input);
    input=interpret(input, "savemenu");
    save_suffix(pgm, nc, nr, mv, ty, fname, input);
    edit_options(pgm, nc, nr, mv, ty, fname, numPixels);
}



void pgm_negate(int *&pgm, int nc, int nr, int mv)
{
    for (int i=0;i<(nr*nc);i++) pgm[i] = mv - pgm[i];
    clr();
    cout<<"Image Negated..."<<endl;
    cout<<"(Press enter)"<<endl;
    multiplatform_system("pause");
}

void do_histogram(int *&pgm, int nc, int nr, int mv){
    long int histogram[mv+1];
    long long int lookUP[mv+1];
    long int countHist=0;
    int newPGM[nr*nc];
    for(int i=0;i<(nr*nc);i++){
        newPGM[i]=0;
    }
    for(int i=0; i<(mv+1);i++){
        histogram[i]=0;
        lookUP[i]=0;
    }
    for(int i=0; i<nc; i++){
        for(int j=0; j<nr; j++){
            histogram[pgm[countHist]] = histogram[pgm[countHist]] + 1;
            countHist++;
        }
    }
    
    double alpha = mv / static_cast<double>(nr*nc);
    
    
    
    long long int sum=0;
    
    
    for(int i=0;i<(mv+1);i++){
        sum += histogram[i];
        lookUP[i] = sum * mv / (nr*nc);
        
        
    }
    countHist=0;
    long long int hold;
    for(int i=0; i<nc; i++){
        for(int j=0; j<nr; j++){
            
            hold=lookUP[pgm[countHist]];
            pgm[countHist]=static_cast<int>(hold);
            countHist++;
            
        }
    }
    clr();
    cout<<"Image histogram equalized!"<<endl;
    cout<<"(Press Enter)"<<endl;
    multiplatform_system("pause");
    
}

void do_black_white(int *&pgm,int nc,int nr,int mv){
    int average;
    int count=0;
    average = average_pgm(pgm, nc, nr);
    for(int i=0;i<nr;i++){
        for(int j=0;j<nc;j++){
            int value=pgm[count];
            if(value<=average){
                pgm[count]=0;
            }
            if(value>average){
                pgm[count]=mv;
            }
            count++;
        }
    }
    
    clr();
    cout<<"Image converted to newspaper!"<<endl;
    cout<<"(PRESS ENTER)"<<endl;
    multiplatform_system("pause");

}

void do_edge(int *&pgm, int nc, int nr, int mv){
    clr();
    string input;
    int edge_array[nc*nr];
    int horizontal[nc*nr];
    int vertical[nc*nr];
    memset(vertical, 0, (nc*nr));
    memset(horizontal, 0, (nc*nr));
    memset(edge_array, 0, (nr*nc));
    cout<<"~~~~Edge Detection~~~~"<<endl;
    cout<<" *Horizontal"<<endl;
    cout<<" *Vertical"<<endl;
    cout<<" *Both"<<endl;
    cout<<"Input: ";
    cin>>input;
    input=lowerCase(input);
    input=interpret(input, "edgemenu");
    int average=average_pgm(pgm, nc, nr);
    int cut_off=average/8;
    cout<<"Average: "<<average<<endl;
    cout<<"Cut_Off: "<<cut_off<<endl;
    int count=1;
    bool cutoff=false;
    int left;
    int right;
    int up;
    int down;
    int newpgm[nr][nc];
    int con_count=0;
    for(int i=0;i<nr;i++){
        for(int j=0;j<nc;j++){
            newpgm[i][j]=pgm[con_count];
            con_count++;
        }
    }
    int subR;
    int addR;
    int subC;
    int addC;
    if(input=="horizontal"||input=="both"){
        int hoz_count=0;
        int test=0;
        for(int i=0;i<nr;i++){
            for (int j=0;j<nc;j++){
                subR=i-1;
                addR=i+1;
                subC=j-1;
                addC=j+1;
                if(subR<0){
                    subR=0;
                }
                if(subC<0){
                    subC=0;
                }
                if(addR>nr){
                    addR=nr;
                }
                if(addC>nc){
                    addR=nc;
                }
                left=((newpgm[subR][subC])*(-1))+((newpgm[i][subC])*(-2))+((newpgm[addR][subC])*(-1));
                right=((newpgm[subR][addC])*(1))+((newpgm[i][addC])*(2))+((newpgm[addR][addC])*(1));
              
                 if(input!="both"){
                cutoff=false;
                if (right+left>cut_off){
                horizontal[count]=mv;
                    cutoff=true;
                }
                if(right+left<-cut_off){
                    horizontal[count]=0;
                    cutoff=true;
                }
                
                if(cutoff==false){
                    horizontal[count]=mv/2;
                }
                 }
                if(input=="both"){
                    horizontal[count]=(left+right);
                }
                
                count++;
            }
        }
        
        if(input!="both"){
        count=0;
        for(int i=0;i<nc;i++){
            for (int j=0;j<nr;j++){
                pgm[count]=horizontal[count];
                count++;
            }
        }
        clr();
        cout<<"Horizontal edge done...."<<endl;
        cout<<"(Press Enter)"<<endl;
        multiplatform_system("pause");
        return;
        }
        
    }
    if(input=="vertical"||input=="both"){
        int ver_count=0;
        int test=0;
        for(int i=0;i<nr;i++){
            for (int j=0;j<nc;j++){
                subR=i-1;
                addR=i+1;
                subC=j-1;
                addC=j+1;
              
                if(subR<0){
                    subR=0;
                }
                if(subC<0){
                    subC=0;
                }
                if(addR>nr){
                    addR=nr;
                }
                if(addC>nc){
                    addR=nc;
                }
                down=((newpgm[subR][subC])*(-1))+((newpgm[subR][j])*(-2))+((newpgm[subR][addC])*(-1));
                up=((newpgm[addR][subC])*(1))+((newpgm[addR][j])*(2))+((newpgm[addR][addC])*(1));
                
                if(input!="both"){
                cutoff=false;
                if (up+down>cut_off){
                    vertical[ver_count]=mv;
                    cutoff=true;
                }
                if(up+down<-cut_off){
                    vertical[ver_count]=0;
                    cutoff=true;
                }
                
                if(cutoff==false){
                    vertical[ver_count]=mv/2;
                }
                 }
                if(input=="both"){
                    vertical[ver_count]=(up+down);
                }
               
                
                ver_count++;
            }
        }
        
        if(input!="both"){
        count=0;
        for(int i=0;i<nc;i++){
            for (int j=0;j<nr;j++){
                pgm[count]=vertical[count];
                count++;
            }
        }
        clr();
        cout<<"vertical edge done...."<<endl;
        cout<<"(Press Enter)"<<endl;
        multiplatform_system("pause");
        return;
        }
    }
    
    
    int weightpgm[nr*nc];
    int value=0;
    int hold;
    int direction;
    if(input=="both"){
        for (int i=0; i<nr; i++){
            for(int j=0; j<nc; j++){
                hold=pow(vertical[value],2)*pow(horizontal[value],2);
                hold=static_cast<int>(sqrt(hold));
                cutoff=false;
                direction=atan2(vertical[value],horizontal[value]);
                if(direction>=0) direction=1;
                if(direction<0) direction=-1;
                hold*=direction;
                hold*=-1;
                cout<<"DIR: "<<direction<<endl;
                if (hold>cut_off*2){
                    weightpgm[value]=mv;
                    cutoff=true;
                }
                if(hold<-cut_off*2){
                    weightpgm[value]=0;
                    cutoff=true;
                }
                if(cutoff==false){
                   weightpgm[value]=mv/2;
                }

                
                value++;
            }
        }
        value=0;
        for(int i=0;i<nr;i++){
            for(int j=0;j<nc;j++){
            hold=weightpgm[value];
            pgm[value]=hold;
            value++;
            }
        }
        
        clr();
    cout<<"both edge done...."<<endl;
    cout<<"(Press Enter)"<<endl;
    multiplatform_system("pause");
    return;
}
}
void do_blur(int *&pgm, int nc, int nr, int mv){
    clr();
    int input=0;
    int edge_array[nc*nr];
    int blur[nc*nr];
    memset(blur, 0, (nc*nr));
    memset(edge_array, 0, (nr*nc));
    cout<<"How many iterations of Blur would you like to run?"<<endl;
    cout<<"Input: ";
    cin>>input;
    int left;
    int right;
    int center;
    int newpgm[nr][nc];
    for(int w=0;w<input;w++){
        int count=0;
        int con_count=0;
        for(int i=0;i<nr;i++){
            for(int j=0;j<nc;j++){
                newpgm[i][j]=pgm[con_count];
                con_count++;
            }
        }
        int subR;
        int addR;
        int subC;
        int addC;
        for(int i=0;i<nr;i++){
            for (int j=0;j<nc;j++){
                subR=i-1;
                addR=i+1;
                subC=j-1;
                addC=j+1;
                if(subR<0){
                    subR=0;
                }
                if(subC<0){
                    subC=0;
                }
                if(addR>nr){
                    addR=nr;
                }
                if(addC>nc){
                    addR=nc;
                }
                left=((newpgm[subR][subC])*(1))+((newpgm[i][subC])*(1))+((newpgm[addR][subC])*(1));
                center=((newpgm[subR][j])*(1))+((newpgm[i][j])*(1))+((newpgm[addR][j])*(1));
                right=((newpgm[subR][addC])*(1))+((newpgm[i][addC])*(1))+((newpgm[addR][addC])*(1));
                int blure = (left+center+right)/9;
                blur[count]=blure;
                count++;
            }
        }
        
        count=0;
        for(int i=0;i<nc;i++){
            for (int j=0;j<nr;j++){
                pgm[count]=blur[count];
                count++;
            }
        }
        if(w==input-1)
        {
            clr();
            cout<<"Bluring done...."<<endl;
            cout<<"(Press Enter)"<<endl;
            multiplatform_system("pause");
        }
    }
    return;
}

void do_clip(int *&pgm, int &nc, int &nr){
    int clip_array[4]={0};
    for(int i=0; i<4; i++){
        clr();
        cout<<"#Row: "<<nr<<" #Col: "<<nc<<endl;
        cout<<"~~~~Clip Menu~~~~"<<endl;
        if (i==0) cout<<"TopLeft x: ";
        if (i==1) cout<<"TopLeft y: ";
        if (i==2) cout<<"BottomRight x: ";
        if (i==3) cout<<"BottomRight y: ";
        cin>>clip_array[i];
        if((i==2)&&clip_array[i]>nc){
            clr();
            cout<<"Invalid input!"<<endl;
            cout<<"(Press Enter)"<<endl;
            multiplatform_system("pause");
            clip_array[i]=0;
            i=i-1;
        }
        if((i==3)&&clip_array[i]>nr){
            clr();
            cout<<"Invalid input!"<<endl;
            cout<<"(Press Enter)"<<endl;
            multiplatform_system("pause");
            clip_array[i]=0;
            i=i-1;
        }
        if((clip_array[3]<clip_array[1])&& i==3){
            clr();
            cout<<"Invalid input! BottomRight Y must be greater than TopLeft Y"<<endl;
            cout<<"(Press Enter)"<<endl;
            multiplatform_system("pause");
            clip_array[i]=0;
            i=i-1;
        }
        if((clip_array[2]<clip_array[0])&& i==2){
            clr();
            cout<<"Invalid input! BottomRight X must be greater than TopLeft X"<<endl;
            cout<<"(Press Enter)"<<endl;
            multiplatform_system("pause");
            clip_array[i]=0;
            i=i-1;
        }
        
    }
    int count = 0;
    int newPGM[(clip_array[2]-clip_array[0])*(clip_array[3]-clip_array[1])];
    for(int i=clip_array[1]; i<clip_array[3]; i++){
        for(int j=clip_array[0]; j<clip_array[2]; j++){
            newPGM[count] = pgm[(i*nr)+j];
            count++;
        }
    }
    count=0;
    nc = (clip_array[2]-clip_array[0]);
    nr = (clip_array[3]-clip_array[1]);
    for(int i=0;i<nc;i++){
        for (int j=0;j<nr;j++){
            pgm[count]=newPGM[count];
            count++;
            
        }
    }
    
}

void do_rotate(int *&pgm, int &nr, int &nc)
{
    
        int * pixArr = new int[nr*nc];
        int count = 0;
        for (int c = nc; c > 0; c--) {
            for (int r = 0; r < nr; r++) {
                pixArr[count] = pgm[(nc * r) + (c - 1)];
                count++;
            }
        }
    count=0;
    int hold_col=nc;
    int hold_row=nr;
    for(int i=0; i<nr;i++){
        for(int j=0;j<nc;j++){
            pgm[count]=pixArr[count];
            count++;
        }
    }
    clr();
    cout<<"Image Rotated!"<<endl;
    cout<<"(Press Enter)"<<endl;
    multiplatform_system("pause");
    delete [] pixArr;
}
void do_arbirarty_rotation(int *& pgm, int &nr,int &nc){
   
    float origin[2];
    origin[0]=(nr/2);
    origin[1]=(nc/2);
    int newPGM[nr*2][nc*2];
    memset(newPGM, 0, nr*nc*4);
    int twoPGM[nr*nc*4];
    int x_cord[nc];
    int y_cord[nr];
    double corner_radius=sqrt(pow((nc-origin[0]),2)+pow((0-origin[1]),2));
    double radius_pgm[nc*nr];
    double angle_pgm[nr*nc];
    int count=0;
    for(int i=0;i<nc;i++){
        for(int j=0;j<nr;j++){
            radius_pgm[count]=magnitude(j, i,origin[0],origin[1]);
            angle_pgm[count]=theta(j, i,origin[0],origin[1]);
            if (angle_pgm[count]<0){
                angle_pgm[count]=abs(angle_pgm[count])+(PI/2);
            }
            
            count++;
        }
    }
   
    clr();
    double input;
    cout<<"~~~~Rotation~~~~~"<<endl;
    cout<<"How many degrees would you like to rotate..."<<endl;
    cout<<"Input: ";
    cin>>input;
    input=input*(PI/180);
    double holder;
    count=0;
    int row_count=0;
    int col_count=0;
    for(int i=0; i<nc; i++){
        
        for(int j=0; j<nr;j++){
           twoPGM[count]=pgm[count];
            x_cord[col_count]=radius_pgm[col_count]*(cos(angle_pgm[col_count]+input));
            y_cord[row_count]=radius_pgm[row_count]*(cos(angle_pgm[row_count]+input));
            count++;
            col_count++;
        }
        row_count++;
    }
    count=0;
    
    row_count=0;
    col_count=0;
    for(int i=0; i<nr;i++){
        for(int j=0; j<nc;j++){
            newPGM[y_cord[row_count]][x_cord[col_count]]=pgm[count];
            count++;
            col_count++;
        }
        row_count++;
    }
    count=0;
    for(int i=0; i<nr;i++){
        for(int j=0; j<nc;j++){
            
            pgm[count]=newPGM[i][j];
            count++;
        }
    }
    
    
    clr();
    cout<<"Rotated Image!"<<endl;
    cout<<"(PRESS ENTER)"<<endl;
    multiplatform_system("pause");
    
    
}


void do_resample(int *&pgm, int &nc, int &nr){
    clr();
    string input;
    cout<<"~~~~Resampling Menu~~~~"<<endl;
    cout<<" *Shrink"<<endl;
    cout<<" *Stretch"<<endl;
    cout<<"Input: ";
    cin>>input;
    input=lowerCase(input);
    input=interpret(input, "samplemenu");
    if(input=="shrink"){
        resampling_smallPGM(nr, nc, pgm);
        clr();
        cout<<"Shrunk Image..."<<endl;
        cout<<"(Press Enter)"<<endl;
        multiplatform_system("pause");
    }
    if(input=="stretch"){
        resampling_largePGM(nr, nc, pgm);
        clr();
        cout<<"Streched Image..."<<endl;
        cout<<"(Press Enter)"<<endl;
        multiplatform_system("pause");
    }
    

}


void resampling_smallPGM(int &nr, int &nc, int *pgm) {
    int smaller_integer;
    
    int count=0;
    
    
    cout << "How much smaller would you like your pgm? Enter an integer." << endl << endl;
    cin >> smaller_integer;
    int matrixPGM[nr][nc];
    cout << endl << endl;
    int D_arr [nr][nc];
    count = 0;
    for (int r = 0; r < nr; r++) {
        for (int c = 0; c < nc; c++) {
            D_arr[r][c] = pgm[count];
            count++;
        }
    }
    
    int row_count = 0;
    int col_count=0;
    for (int r = 0; r < nr; r ++ ) {
        col_count=0;
        row_count++;
        for (int c = 0; c < nc; c++) {
            matrixPGM[r][c] = D_arr[row_count][col_count];
            
            col_count+=smaller_integer;
        }
    }
    for (int r = 0; r < nr/smaller_integer; r++ ) {
        col_count=0;
        row_count+=smaller_integer;
        for (int c = 0; c < nc; c ++) {
            matrixPGM[r][c] = D_arr[row_count][col_count];
            
            col_count++;
        }
    }
    count=0;
    for(int i=0;i<nr/smaller_integer;i++){
        for(int j=0;j<nc/smaller_integer;j++){
            pgm[count]=matrixPGM[i][j];
            count++;
        }
    }

   
    nr = nr / smaller_integer;
    nc = nc / smaller_integer;
}

void resampling_largePGM(int &nr, int &nc, int *pgm) {
    int larger_integer;
    
    int count=0;
    
    
    cout << "How much smaller would you like your pgm? Enter an integer." << endl << endl;
    cin >> larger_integer;
    int matrixPGM[nr][nc];
    cout << endl << endl;
    int D_arr [nr][nc];  // dlouble array to hold orignal image
    count = 0;             	// put the values in my double array
    for (int r = 0; r < nr; r++) {
        for (int c = 0; c < nc; c++) {
            D_arr[r][c] = pgm[count];
            count++;
        }
    }
    
    int row_count = 0;
    int col_count=0;
    for (int r = 0; r < nr; r ++ ) {
        col_count=0;
        row_count++;
        for (int c = 0; c < nc; c ++) {
            matrixPGM[r][c] = D_arr[row_count][col_count];
            
            col_count+=larger_integer;
        }
    }
    for (int r = 0; r < nr*larger_integer; r ++ ) {
        col_count=0;
        row_count+=larger_integer;
        for (int c = 0; c < nc; c ++) {
            matrixPGM[r][c] = D_arr[row_count][col_count];
            
            col_count++;
        }
    }
    count=0;
    for(int i=0;i<nr*larger_integer;i++){
        for(int j=0;j<nc*larger_integer;j++){
            pgm[count]=matrixPGM[i][j];
            count++;
        }
    }
    
    
    nr = nr * larger_integer;
    nc = nc * larger_integer;
}


void table_histogramPGM(int &nc, int &nr, int maxval, int *pgm, int *numPixels) {
    numPixels = new int(maxval + 1);
    for (int c = 0; c < maxval + 1; c++) {
        int count = 0;
        numPixels[c] = 0;
        while (pgm[count]) {
            if (pgm[count] == c) numPixels[c] += 1;
            count++;
        }
        
    }
    
    
    int highest_spike = 0;
    for (int i = 0; i < maxval + 1; i++) {
        numPixels[i] /= 20;
        if (numPixels[i] > highest_spike)
            highest_spike = numPixels[i];
    }
    
    
    
    int count = 0;
    nr = highest_spike;
    nc = maxval + 1;
    for (int r = 0; r < nr; r++) {
        for (int c = 0; c < nc; c++) {
            if (numPixels[c] >= highest_spike)
                pgm[count] = 0;
            else
                pgm[count] = maxval - 20;
            count++;
        }
        highest_spike--;
    }
    
    clr();
    cout<<"Histogram Table made..."<<endl;
    cout<<"(PRESS ENTER)"<<endl;
    multiplatform_system("pause");
}









