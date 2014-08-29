#include<iostream>
#include<unistd.h>
#include<termios.h>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>

using namespace std;
int game=1;
string layout[20][30];

int index = 23 , score=0;
int posX=6 , posY=6;;

void init ()
{
	for ( int i=0 ; i<20 ; i++ )
		for ( int j=0 ; j<30 ; j++ )
		{
			if ( i==posX && j==posY )
			  layout[i][j] = "bb";
			else
			  layout[i][j] = "  ";
		}
}

int highScore;
void print ( )
{
	cout<<"\E[2J\E[H";
	cout<<"                       ";
	cout<<"TERMINAL FLAPPY!"<<endl<<endl<<endl;
	cout<<"High Score : "<<highScore<<endl;
	cout<<"Score : "<<score<<endl;
	for ( int i=0 ; i<20 ; i++ )
	{
		for ( int j=0 ; j<30 ; j++ )
		{
			cout<<layout[i][j];
		}
		cout<<'|'<<endl;
	}
}

void refresh ( )
{
	for ( int i=0 ; i<20 ; i++ )
		for ( int j=0 ; j<29 ; j++ )
		{
			if ( j==posY-1 && i==posX)
				layout[i][j] = "  ";
			else
				layout[i][j]=layout[i][j+1];
		}
	for ( int i=0 ; i<20 ; i++ )
		layout[i][29]="  ";
	if ( posX==18  || layout[posX][posY+1]=="II" || layout[posX-1][posY+1]=="II" || layout [posX-1][posY+1]=="II")
		game=0;
}
int x=10,y;

void addPillar ( )
{
	srand(time(NULL));
	int temp;
L:	temp = rand()%15;
	if ( ((temp - x) < -3) || (x-temp < -3) )
		goto L;
	x=temp;
	y=x+4;

	for ( int i=0; i<x; i++ )
		layout[i][29] = "II";
	for ( int i=x ; i<y ; i++ )
		layout[i][29] = "  ";
	for( int i=y ; i<20 ; i++ )
		layout[i][29] = "II";

}

void decrementBird (  )
{
	if ( posX==18  || layout[posX][posY+1]=="II" || layout[posX-1][posY+1]=="II" || layout [posX-1][posY+1]=="II")
		game= 0;
	layout[posX][posY]="  ";
	posX+=1;
	layout[posX][posY]="bb";
	game= 1;
}

int keyPress=0;

char getch( )
{
    
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
//    printf("%c\n",buf);
	switch ( buf)
	{
	  case 119:
			if (posX>1)
			{
	  	  layout[posX][posY]="  ";
				posX-=1;
				layout[posX][posY]="bb";
			}
			keyPress=1;

			break;
    default:
			keyPress=0;
		  break;
   }
 }

int compare=0;
int main()
{
	ifstream input ("flappyScore.txt");
	input>>highScore;
	input.close();
	ofstream out ("flappyScore.txt");
	out<<highScore;
	out.close();
	init();
	addPillar();
	print();
	while( game )
	{
//		sleep();
	if ( score>highScore)
	{
  	ofstream out ("flappyScore.txt");
		highScore=score;
		out.seekp(ios_base::beg);
		out<<score;
		out.close();
	}
	if ( index==0 )
	{
		index=5;
		score++;
	}
	index--;
		if( compare%5 ==0 )
		{
			compare=0;
			addPillar();
		}
		compare++;
		getch();
		if(!keyPress)
		decrementBird();
		refresh();
		layout[posX][posY]="bb";
		print();
		if ( !game )
		{
			cout<<"GAMEOVER!!"<<endl;
			sleep(2);
		}
	}
	return 0;
}
