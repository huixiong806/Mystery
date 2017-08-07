#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<string>
#include<time.h>
#define NONE 0
#define BLACK 1
#define WHITE -1
#define INF 999999999999999.0
#define LINF 100000000.0
using namespace std;
const double valmat[8][8]=
{
	{200,-25,20,20,20,20,-25,200},
	{-25,-50,5,5,5,5,-50,-25},
	{20,5,1,1,1,1,5,20},
	{20,5,1,1,1,1,5,20},
	{20,5,1,1,1,1,5,20},
	{20,5,1,1,1,1,5,20},
	{-25,-50,5,5,5,5,-50,-25},
	{200,-25,20,20,20,20,-25,200}
};
int sign(int x)
{
	return x>0?1:(x==0?0:-1);
}
struct board
{
	int chess[8][8];
	void newGame()
	{
		for(int i=0;i<8;++i)
			for(int j=0;j<8;++j)
				chess[i][j]=NONE;
      	chess[3][3]=WHITE;
      	chess[3][4]=BLACK;
      	chess[4][3]=BLACK;
        chess[4][4]=WHITE;
	}
	bool canPut(int type,int x,int y,int dx,int dy)
    {
      	x+=dx;y+=dy;
      	if(x<0||x>7||y<0||y>7)return false;
      	if(chess[x][y]!=-type)return false;
    	while(x>=0&&x<=7&&y>=0&&y<=7)
        {
        	if(chess[x][y]==type)return true;
          	else if(chess[x][y]==NONE)return false;
          	x+=dx;y+=dy;
        }
      	return false;
	}
	bool canPut(int type,int x,int y)
	{
		if(chess[x][y]!=NONE)return false;
		const int dx[8]={-1,1,0,0,-1,1,-1,1};
		const int dy[8]={0,0,1,-1,-1,-1,1,1};
		for(int i=0;i<8;++i)
			if(canPut(type,x,y,dx[i],dy[i]))return true;
		return false;
	}
	void reverse(int type,int x,int y,int dx,int dy)
	{
		x+=dx;y+=dy;
		while(x>=0&&x<=7&&y>=0&&y<=7)
        {
        	if(chess[x][y]==-type)chess[x][y]=type;
        	else return;
        	x+=dx;y+=dy;
        }
	}
	bool put(int type,int x,int y)
	{
		const int dx[8]={-1,1,0,0,-1,1,-1,1};
		const int dy[8]={0,0,1,-1,-1,-1,1,1};
		if(!canPut(type,x,y))return false;
		for(int i=0;i<8;++i)
			if(canPut(type,x,y,dx[i],dy[i]))
				reverse(type,x,y,dx[i],dy[i]);
		chess[x][y]=type;
		return true;
	}
	bool check(int type)
	{
		for(int x=0;x<8;++x)
			for(int y=0;y<8;++y)	
				if(canPut(type,x,y))return true;
		return false;
	}
	int getWinLose()
	{
		int res=0;
		for(int i=0;i<8;++i)
			for(int j=0;j<8;++j)
				res+=chess[i][j];
		return res;
	}
	void output()
	{
		cout<<" ";
		for(int i=0;i<8;++i)
		{
			string temp="";
			temp+=char(-93);
			temp+=char(-63+i);
			cout<<temp;
		}
		cout<<endl; 
		for(int i=0;i<8;++i)
		{
			cout<<i+1;
			for(int j=0;j<8;++j)
				if(chess[i][j]==WHITE)
					cout<<"¡ð";
				else if(chess[i][j]==BLACK)
					cout<<"¡ñ";
				else cout<<"[]";
			cout<<endl; 
		}
	}
	double eval()
	{
		double res=0;
		for(int i=0;i<8;++i)
			for(int j=0;j<8;++j)
				res+=valmat[i][j]*chess[i][j];
		return res;
	}
	bool over()
	{
		return !check(1)&&!check(-1);
	}
	int emptyCount()
	{
		int res=0;
		for(int i=0;i<8;++i)
			for(int j=0;j<8;++j)
				if(chess[i][j]==NONE)++res;
		return res;
	}
	pair<int,int> currentScore()
	{
		int resA=0,resB=0;
		for(int i=0;i<8;++i)
			for(int j=0;j<8;++j)
				if(chess[i][j]==WHITE)++resB;
				else if(chess[i][j]==BLACK)++resA;
		return make_pair(resA,resB);
	}
};
board brd;
int bestMove;
board game;
int maxDepth=0;
int AItype;
double find_best_move(int type,int depth,double alpha,double beta)
{
	if(brd.over())return (double)type*(double)brd.getWinLose()*LINF;
	if(depth==0)return (double)type*brd.eval();
	board sav=brd;
	if(!brd.check(type))return max(alpha,-find_best_move(-type,depth-1,-beta,-alpha));
	for(int i=0;i<8;++i)
	{
		for(int j=0;j<8;++j)
		{
			if(brd.canPut(type,i,j))
			{
				brd.put(type,i,j);
				double score=-find_best_move(-type,depth-1,-beta,-alpha);
				brd=sav;
				if(score>=beta)return score;
				if(score>alpha)
				{
					if(depth==maxDepth)
					{
						if(AItype||rand()%20)
						{
							bestMove=i<<3|j;
							alpha=score;
						}
					}else alpha=score;
				}
			}
		}
	}
	return alpha;
}
void AImove(int type)
{
	bestMove=-1;
	brd=game;
	maxDepth=8;
	int emptyCount=brd.emptyCount();
	if(emptyCount<=15)maxDepth=999;
	double score=find_best_move(type,maxDepth,-INF,INF);
	cout<<""<<score<<" "<<(bestMove>>3)+1<<(char)((bestMove&0x7)+'A')<<endl;
	game.put(type,bestMove>>3,bestMove&0x7);
}
int main()
{
	system("color F0");
	srand(time(0));
	cout<<"Mystery (Alpha 1.03)"<<endl;
	cout<<"copyright orangebird 2017."<<endl;
	game.newGame();
	cout<<"Input a number,1=AI Black,-1=AI white,0 AI both:"<<endl;
	int type=BLACK;
	cin>>AItype;
	while(!game.over())
	{
		game.output();
		if(!game.check(type))
		{
			type=-type;
			continue;
		}
		if(!AItype||AItype==type)
		{
			cout<<(type>0?"Black":"White")<<" Move:"<<endl; 
			
			AImove(type);
			type=-type;
		}
		else
		{
			cout<<(type>0?"Black":"White")<<" Move:"<<endl; 
			string s;
			cin>>s;
			if(game.put(type,s[0]-'1',tolower(s[1])-'a'))
				type=-type;
		}
	}
	game.output();
	type=game.getWinLose();
	cout<<game.currentScore().first<<":"<<game.currentScore().second<<endl;
	if(type<0)cout<<"white win!"<<endl;
	else if(type>0)cout<<"black win!"<<endl;
	else cout<<"tie!"<<endl;
	cout<<"input 'e' to exit"<<endl;
	while(getchar()!='e');
	return 0;
}
