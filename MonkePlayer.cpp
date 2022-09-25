#include<ncurses.h>
#include<SDL.h>
#include<SDL_mixer.h>
#include<iostream>
#include<string>
using namespace std;

string getPath();
string getTitle(string path);
void showMenu();
void printList();
bool isEmpty();
void insert(string path);
void deleteNode();
void Play();
bool exists(string path);
string refinePath(string path);
void savePlaylist();
void deleteList();
void loadPlaylist();

int main()
{

  initscr();
  refresh();
  nocbreak();
  setscrreg(0,40);
  idlok(stdscr,true);
  scrollok(stdscr,true);

  keypad(stdscr, TRUE);

  int choice;
  while(1)
  {
    do
    {
      choice=0;
      clear();
      showMenu();
      refresh();
      scanw("%d",&choice);
    }while((choice<1)||(choice>6));

    switch(choice)
    {
      case 1:
      {
        clear();
        printList();
        refresh();
        getchar();
        break;
      }
      case 2:
      {
        clear();
        printw("Drag & Drop the song here then press the Enter key\n");
        std::string path= getPath();
        refresh();
        if(exists(path))
        {
          std::string format=path.substr(path.find_last_of('.'));
          if(format==".mp3" || format==".flac" || format==".wav")
          {
            insert(path);
            printw("\nSong Added!\n");
            refresh();
            getch();
          }
          else
          {
            printw("\nInvalid Input\n");
            printw("\n%s\n",format);
            refresh();
            getch();
          } 
        }
        else
        {
          cout<<"No such file exists"<<endl;
          getch();
        }
       /* if(path.find('.')==std::string::npos)
        {
          printw("\nInvalid Input\n");
          refresh();
          getch();
        }
        else
        {
          std::string format=path.substr(path.find_last_of('.'));
          if(format==".mp3" || format==".flac" || format==".wav")
          {
            insert(path);
            printw("\nSong Added!\n");
            refresh();
            getch();
          }
          else
          {
            printw("\nInvalid Input\n");
            printw("\n%s\n",format);
            refresh();
            getch();
          } 
        }*/
        break;
      }
      case 3:
      {
        clear();
        deleteNode();
        refresh();
        getch();
        break;
      }
      case 4:
      {
        clear();
        savePlaylist();
        refresh();
        getch();
        break;
      }
      case 5:
      {
        clear();
        loadPlaylist();
        refresh();
        getch();
        break;
      }
      case 6:
      {
        cbreak();
        if(isEmpty())
        {
          clear();
          printw("No songs added yet\n");
          refresh();
          getch();
        }
        else
        {
          noecho();
          Play();
          echo();
        }
        nocbreak();
        break;
      }
    }
  }
  endwin();
}


void showMenu()
{
  printw("1. Show Playlist\n2. Add Song \n3. Remove Song\n4. Save Playlist\n5. Load Playlist\n6. Play\n");
}

