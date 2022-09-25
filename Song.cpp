#include<iostream>
#include<ncurses.h>
#include<SDL.h>
#include<SDL_mixer.h>
#include<unistd.h>
#include<fstream>
using namespace std;

string getTitle(string path);

struct Node
{
  std::string path;
  std::string title;
  int index;
  struct Node *next;
  struct Node *prev;
};

struct Node *head,*temp,*l;
int total=0;
Mix_Music *song= NULL;

string getPath()
{
  char c[100];
  //std::getline(std::cin,path);
  getstr(c);
  refresh();
  std::string path=c;
  path= path.substr(path.find_first_of('\'')+1,path.find_last_of('\'')-1);
  return path;
}

void insert(string x)
{
  l=head;
  if(head==NULL)
  {
    head= new Node();
    head->prev=head;
    head->next=head;
    head->path=x;
    head->title=getTitle(x);
    total++;
    head->index=total;
  }
  else
  {
    while(l->next!=head)
    {
      l=l->next;
    }
    if(l->next==head)
    {
      temp= new Node();
      temp->next=head;
      temp->prev=l;
      temp->path=x;
      temp->title=getTitle(x);
      l->next=temp;
      total++;
      temp->index=total;
    }
  }
}

bool isEmpty()
{
  if(head==NULL)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void deleteNode()
{
  l=head;
  if(isEmpty())
  {
    printw("The playlist is empty\n\n");
  }
  else
  {
    int index;
    printw("Enter the index of the song you want to remove:\n");
    refresh();
    scanw("%d",&index);
    if(index>total || index<1)
    {
      printw("Invalid Song Index!\n\n");
    }
    else
    {
      if(head->index==index && head->next==head)
      {
        head=NULL;
        total--;
        printw("Song Removed!\n");
      }
      else if(head->index==index)
      {
        while(l->next!=head)
        {
          l=l->next;
        }
        if(l->next==head)
        {
          temp=head;
          head=head->next;
          head->prev=head;
          l->next=head;
          delete(temp);
          total--;
          for(l=head;index<=total;index++)
          {
            l->index=index;
            l=l->next;
          }
          printw("Song Removed!\n");
        }
      }
      else
      {
        while(l->next!=head)
        {
          if(l->index==index)
          {
            l->prev->next=l->next;
            l->next->prev=l->prev;
            total--;
            temp=l;
            l=l->next;
            delete(temp);
            for(; index<=total;index++)
            {
              l->index=index;
              l=l->next;
            }
            printw("Song Removed!\n");
            break;
          }

          l=l->next;

          if(l->index==index && l->next==head)
          {
            l->prev->next=head;
            total--;
            delete(l);
            for(l=head; index<=total;index++)
            {
              l->index=index;
              l=l->next;
            }
            printw("Song Removed!\n");
          }
        }
      }
    }
  }
}

void deleteList()
{
  l=head;
  if(!isEmpty())
  {
    while(l->next!=head)
    {
      temp=l;
      l=l->next;
      delete(temp);
      total--;
    }
    delete(l);
    head=NULL;
    total--;
  }
}

string getTitle(string path)
{
  std::string title= path.substr(path.find_last_of('/')+1,path.find('\0'));
  return title;
}


bool exists(string path)
{
  if(FILE *file= fopen(path.c_str(),"r"))
  {
    fclose(file);
    return true;
  }
  else 
    return false;
}

void printList()
{
  l=head;
  if(isEmpty())
  {
    printw("The playlist is empty\n\n");
  }
  else
  {
    while(l->next!=head)
    {
      printw("(%d.) %s\n",l->index,(l->title).c_str());
      l=l->next;
    }
    if(l->next==head)
    {
      printw("(%d.) %s\n",l->index,(l->title).c_str());
    }
  }
}

void Display()
{
  Node *ptr=head;
  while(ptr->next!=head)
  {
    if(ptr->index==l->index)
    {
      printw(">>>(%d.) %s\n",ptr->index,(ptr->title).c_str());
    }
    else
    {
      printw("   (%d.) %s\n",ptr->index,(ptr->title).c_str());
    }
    ptr=ptr->next;
  }
    if(ptr->index==l->index)
    {
      printw(">>>(%d.) %s\n",ptr->index,(ptr->title).c_str());
    }
    else
    {
      printw("   (%d.) %s\n",ptr->index,(ptr->title).c_str());
    }
  refresh();
}

void updateSong()
{
  song= Mix_LoadMUS(l->path.c_str());
  if(!song)
  {
    cout<<"Error"<<Mix_GetError()<<endl;
  }
  if(exists(l->path.c_str()))
  {
    Mix_PlayMusic(song,1);
  }
  else
  {
    printw("Could not locate song!\n");
  }
  clear();
  Display();
}
void nextSong()
{
  l=l->next;
  updateSong();
}

void prevSong()
{
  l=l->prev;
  updateSong();
}

void shuffleSong()
{
  l=head;
  int random= rand()%total+1;
  while(l->index!=random)
  {
    l=l->next;
  }
  if(l->index==random)
  {
    updateSong();
  }
}

void savePlaylist()
{
  l=head;
  if(isEmpty())
  {
    printw("Playlist is empty!\n");
  }
  else
  {
    char name[30];
    printw("Enter name of the playlist:\n");
    getstr(name);
    strcat(name,".txt");
    ofstream file(name);
    while(l->next!=head)
    {
      file<<l->path<<endl;
      l=l->next;
    }
    if(l->next==head)
    {
      file<<l->path<<endl;
    }
    file.close();
    printw("Playlist Saved!\n");
  }
}
void loadPlaylist()
{
  char name[30];
  printw("Enter name of the playlist:\n");
  getstr(name);
  strcat(name,".txt");
  if(exists(name))
  {
    deleteList();
    ifstream file(name);
    std::string path;
    while(getline(file,path))
    {
      insert(path);
    }
    file.close();
    printw("Playlist Loaded!\n");
  }
  else
  {
    printw("Playlist does not exist!\n");
  }
}

void Play()
{
  l=head;

  SDL_Init(SDL_INIT_AUDIO);

  if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,2048)<0)
  {
    cout<<"Error"<<Mix_GetError()<<endl;
  }


  if(!Mix_PlayingMusic())
  {
    int c;
    int volume=MIX_MAX_VOLUME;
    unsigned int ticks= SDL_GetTicks();
    unsigned int time= 0;

    updateSong();
    nodelay(stdscr,true);

    while(c!=27)
    {
      if(!Mix_PlayingMusic())
      {
        time=0;
        nextSong();
      }

      c=wgetch(stdscr);
      switch(c)
      {
        case (int)' ':
        {
          if(Mix_PausedMusic())
          {
            Mix_ResumeMusic();
          }
          else
          {
            Mix_PauseMusic();
          }
          break;
        }
        case (int)'n':
        {
          nextSong();
          break;
        }
        case (int)'p':
        {
          prevSong();
          break;
        }
        case KEY_UP:
        case (int)'0':
        {
          volume=volume+2;
          volume=volume>MIX_MAX_VOLUME? MIX_MAX_VOLUME : volume;
          Mix_VolumeMusic(volume);
          break;
        }
        case KEY_DOWN:
        case (int)'9':
        {
          volume=volume-2;
          volume= volume<0? 0 : volume;
          Mix_VolumeMusic(volume);
          break;
        }

        case KEY_RIGHT:
        {
          Mix_SetMusicPosition((time=time+ticks+10));
          break;
        }
        case KEY_LEFT:
        {
          Mix_SetMusicPosition((time=time-ticks-10));
          break;
        }
        case (int)'s':
        {
          shuffleSong();
          break;
        }
      }
    }

    nodelay(stdscr,false);
    Mix_FreeMusic(song);
    song=nullptr;
    Mix_Quit();
  }
}

