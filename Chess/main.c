#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define row 7
#define column 7
char MainArray[row+1][column+1];
char captureW[16];
char captureB[16];
int IndexOfCaptureW;
int IndexOfCaptureB;
int undoA[6][500];
int IndexOfUndo;
int redoA[6][500];
int IndexOfRedo;
char redo_prom[500];

bool LOAD(int*pointer_move)
{
    //THIS FUNCTION READ FROM A TEXT FILE THE BOARD, CAPTURED PIECES OF EACH PLAYER,INDEX OF THE ARRAY OF THE CAPTURE,NUMBER OF MOVES
    int move;
    FILE *ptr;
    ptr=fopen("load.txt","r");
    if(ptr == NULL)
    {
        printf("No saved games found!");
        return false;
    }
    for(int i=0; i<=row; i++)
    {
        for(int j=0; j<=column; j++)
        {
            fscanf(ptr,"%c",&MainArray[i][j]);
        }
    }
    for(int i=0; i<16; i++)
        fscanf(ptr,"%c",&captureW[i]);
    fscanf(ptr,"%d",&IndexOfCaptureW);
    for(int i=0; i<16; i++)
        fscanf(ptr,"%c",&captureB[i]);
    fscanf(ptr,"%d",&IndexOfCaptureB);
    fscanf(ptr,",%d",&move);
    *pointer_move=move;

    return true;
}
void SAVE(int move)
{
    //THIS FUNCTION SAVES TO A TEXT FILE THE BOARD, CAPTURED PIECES OF EACH PLAYER,INDEX OF THE ARRAY OF THE CAPTURE,NUMBER OF MOVES
    FILE *ptr;
    ptr=fopen("load.txt","w");
    for(int i=0; i<=row; i++)
    {
        for(int j=0; j<=column; j++)
        {
            fprintf(ptr,"%c",MainArray[i][j]);
        }
    }
    for(int i=0; i<16; i++)
        fprintf(ptr,"%c",captureW[i]);
    fprintf(ptr,"%d",IndexOfCaptureW);
    for(int i=0; i<16; i++)
        fprintf(ptr,"%c",captureB[i]);
    fprintf(ptr,"%d",IndexOfCaptureB);
    fprintf(ptr,",%d",move);

}

bool REDO(int move)
{
    //REDO 2D ARRAY CONSISTS OF 6 ROWS AND 500 COLUMNS: THE FIRST FOUR ROWS SAVE ALL THE UNDO HISTORY, THE FIFTH ROW RESAMBELS IF A PEICE WAS CAPTURED DURING THIS MOVE, THE SIXTH ROW RESAMBELS IF A PROMOTION WAS DONE DURING THIS MOVE AND THE 500 COLUMN RESAMBLES ALL ALLOWED MOVES IN THE GAME
    // ALL PROMOTED PEICES ARE SAVED IN A redo_prom[500] string
    // THIS FUNCTION USES THE SWAP METHOD TO REDO EVRY MOVE WAS UNDONE
    char temp;
    if(IndexOfRedo>0)
    {
        undoA[0][IndexOfUndo]=redoA[0][IndexOfRedo-1];
        undoA[1][IndexOfUndo]=redoA[1][IndexOfRedo-1];
        undoA[2][IndexOfUndo]=redoA[2][IndexOfRedo-1];
        undoA[3][IndexOfUndo]=redoA[3][IndexOfRedo-1];
        undoA[4][IndexOfUndo]=redoA[4][IndexOfRedo-1];
        undoA[5][IndexOfUndo]=redoA[5][IndexOfRedo-1];
        IndexOfUndo++;
        temp=MainArray[redoA[2][IndexOfRedo-1]][redoA[3][IndexOfRedo-1]];
        MainArray[redoA[2][IndexOfRedo-1]][redoA[3][IndexOfRedo-1]]=MainArray[redoA[0][IndexOfRedo-1]][redoA[1][IndexOfRedo-1]];
        MainArray[redoA[0][IndexOfRedo-1]][redoA[1][IndexOfRedo-1]]=' ';
        if(redoA[4][IndexOfRedo-1]!=-1)
        {
            if(move%2==0)
            {
                captureW[redoA[4][IndexOfRedo-1]]=temp;
                IndexOfCaptureW++;
            }
            else if(move%2!=0)
            {
                captureB[redoA[4][IndexOfRedo-1]]=temp;
                IndexOfCaptureB++;
            }
        }
        if(redoA[5][IndexOfRedo-1]==1)
        {
            MainArray[redoA[2][IndexOfRedo-1]][redoA[3][IndexOfRedo-1]]=redo_prom[IndexOfRedo-1];
        }
        IndexOfRedo--;
        return true;
    }
    return false;
}

bool UNDO(int move,bool real)
{
    //UNDO 2D ARRAY CONSISTS OF 6 ROWS AND 500 COLUMNS: THE FIRST FOUR ROWS SAVE ALL THE MOVES HISTORY, THE FIFTH ROW RESAMBELS IF A PEICE WAS CAPTURED DURING THIS MOVE, THE SIXTH ROW RESAMBELS IF A PROMOTION WAS DONE DURING THIS MOVE AND THE 500 COLUMN RESAMBLES ALL ALLOWED MOVES IN THE GAME
    // IF bool real is true this means its a real undo done by the user and saves all undone moves to the redo array if it is false this means the undo is done due to other functions trying to see possible moves(checkmate,stalemate)
    // THIS FUNCTION USES THE SWAP METHOD TO UNDO EVRY MOVE WAS DONE
    if(IndexOfUndo!=0)
    {
        if(real)
        {
            redoA[0][IndexOfRedo]=undoA[0][IndexOfUndo-1];
            redoA[1][IndexOfRedo]=undoA[1][IndexOfUndo-1];
            redoA[2][IndexOfRedo]=undoA[2][IndexOfUndo-1];
            redoA[3][IndexOfRedo]=undoA[3][IndexOfUndo-1];
            redoA[4][IndexOfRedo]=undoA[4][IndexOfUndo-1];
            redoA[5][IndexOfRedo]=undoA[5][IndexOfUndo-1];
            redo_prom[IndexOfRedo]=MainArray[undoA[2][IndexOfUndo-1]][undoA[3][IndexOfUndo-1]];
            IndexOfRedo++;
        }
        MainArray[undoA[0][IndexOfUndo-1]][undoA[1][IndexOfUndo-1]]=MainArray[undoA[2][IndexOfUndo-1]][undoA[3][IndexOfUndo-1]];
        if(undoA[4][IndexOfUndo-1]!=-1)
        {
            if(move%2==0)
            {
                MainArray[undoA[2][IndexOfUndo-1]][undoA[3][IndexOfUndo-1]]=captureB[undoA[4][IndexOfUndo-1]];
                captureB[undoA[4][IndexOfUndo-1]]=' ';
                IndexOfCaptureB--;
            }
            else
            {
                MainArray[undoA[2][IndexOfUndo-1]][undoA[3][IndexOfUndo-1]]=captureW[undoA[4][IndexOfUndo-1]];
                IndexOfCaptureW--;
                captureW[undoA[4][IndexOfUndo-1]]=' ';
            }
        }
        else
        {
            MainArray[undoA[2][IndexOfUndo-1]][undoA[3][IndexOfUndo-1]]=' ';
        }
        if(undoA[5][IndexOfUndo-1]==1)
        {
            if(move%2==0)
                MainArray[undoA[0][IndexOfUndo-1]][undoA[1][IndexOfUndo-1]]='P';
            else
                MainArray[undoA[0][IndexOfUndo-1]][undoA[1][IndexOfUndo-1]]='p';
        }

        IndexOfUndo--;

        return true;
    }
    return false;

}

bool King(int r1,int c1,int r2,int c2)
{
    //BOOLEAN FUNCTION CHECK IF THE KING CAN MOVE TO THE REQUIRED LOCATION OR NOT AND RETURN TRUE OR FALSE
    if(MainArray[r1][c1]=='K')
    {
        if((c1==c2-1&&(r1==r2||r1==r2+1||r1==r2-1))||(c1==c2+1&&(r1==r2||r1==r2+1||r1==r2-1))||(c1==c2&&(r1==r2-1||r1==r2+1)))
            return true;
    }
    else if(MainArray[r1][c1]=='k')
    {
        if((c1==c2-1&&(r1==r2||r1==r2+1||r1==r2-1))||(c1==c2+1&&(r1==r2||r1==r2+1||r1==r2-1))||(c1==c2&&(r1==r2-1||r1==r2+1)))
            return true;
    }
    return false;
}

bool Bishops(int r1,int c1,int r2,int c2)
{
    //BOOLEAN FUNCTION CHECK IF THE BISHOP CAN MOVE TO THE REQUIRED LOCATION OR NOT AND RETURN TRUE OR FALSE
    if((MainArray[r1][c1]=='B'||MainArray[r1][c1]=='Q'))
    {
        if(abs(c1-c2)==abs(r1-r2))
        {
            if(c2>c1&&r2>r1)
            {
                int i=c1+1,j=r1+1;
                while(i<c2&&j<r2)
                {
                    if(MainArray[j][i]!=' ')
                        return false;
                    i++;
                    j++;
                }
                return true;
            }
            else if(c2>c1&&r2<r1)
            {
                int i=c1+1,j=r1-1;
                while(i<c2&&j>r2)
                {

                    if(MainArray[j][i]!=' ')
                        return false;
                    i++;
                    j--;
                }
                return true;
            }
            else if(c2<c1&&r2<r1)
            {
                int i=c1-1,j=r1-1;
                while(i>c2&&j>r2)
                {

                    if(MainArray[j][i]!=' ')
                        return false;
                    i--;
                    j--;

                }
                return true;
            }
            else if(c2<c1&&r2>r1)
            {
                int i=c1-1,j=r1+1;
                while(i>c2&&j<r2)

                {

                    if(MainArray[j][i]!=' ')
                        return false;
                    i--;
                    j++;
                }
                return true;
            }
        }
    }
    if((MainArray[r1][c1]=='b'||MainArray[r1][c1]=='q'))
    {
        if(abs(c1-c2)==abs(r1-r2))
        {
            if(c2>c1&&r2>r1)
            {
                int i=c1+1,j=r1+1;
                while(i<c2&&j<r2)
                {
                    if(MainArray[j][i]!=' ')
                        return false;
                    i++;
                    j++;
                }
                return true;
            }
            else if(c2>c1&&r2<r1)
            {
                int i=c1+1,j=r1-1;
                while(i<c2&&j>r2)
                {

                    if(MainArray[j][i]!=' ')
                        return false;
                    i++;
                    j--;
                }
                return true;

            }
            else if(c2<c1&&r2<r1)
            {
                int i=c1-1,j=r1-1;
                while(i>c2&&j>r2)
                {

                    if(MainArray[j][i]!=' ')
                        return false;
                    i--;
                    j--;

                }
                return true;
            }
            else if(c2<c1&&r2>r1)
            {
                int i=c1-1,j=r1+1;
                while(i>c2&&j<r2)

                {

                    if(MainArray[j][i]!=' ')
                        return false;
                    i--;
                    j++;
                }
                return true;
            }
        }
        else
            return false;
    }
    return false;
}

bool Knight(int r1,int c1,int r2,int c2)
{
    //BOOLEAN FUNCTION CHECK IF THE KING CAN MOVE TO THE REQUIRED LOCATION OR NOT AND RETURN TRUE OR FALSE
    if(MainArray[r1][c1]=='N')
    {
        if(((c1==c2-1||c1==c2+1)&&(r1==r2-2||r1==r2+2))||((c1==c2-2||c1==c2+2)&&(r1==r2-1||r1==r2+1)))
            return true;
    }
    if(MainArray[r1][c1]=='n')
    {
        if(((c1==c2-1||c1==c2+1)&&(r1==r2-2||r1==r2+2))||((c1==c2-2||c1==c2+2)&&(r1==r2-1||r1==r2+1)))
            return true;
    }
    return false;
}

bool  Rook(int r1,int c1,int r2,int c2)
{
    //BOOLEAN FUNCTION CHECK IF THE ROOK CAN MOVE TO THE REQUIRED LOCATION OR NOT AND RETURN TRUE OR FALSE
    if((MainArray[r1][c1]=='R'||MainArray[r1][c1]=='Q'))
    {
        if(c1==c2&&r1!=r2)
        {
            if(r1>r2)
            {
                for(int i=r2+1; i<r1; i++)
                {
                    if(MainArray[i][c1]!=' ')
                        return false;

                }
                return true;
            }
            else if (r2>r1)
            {
                for(int i=r2-1; i>r1; i--)
                {
                    if(MainArray[i][c1]!=' ')
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        else if(r2==r1&&c1!=c2)
        {
            if(c1>c2)
            {
                for(int i=c2+1; i<c1; i++)
                {
                    if(MainArray[r1][i]!=' ')
                    {
                        return false;
                    }
                }
                return true;
            }
            else if (c2>c1)
            {
                for(int i=c2-1; i>c1; i--)
                {
                    if(MainArray[r1][i]!=' ')
                    {
                        return false;
                    }
                }
                return true;
            }
        }
    }
    else if((MainArray[r1][c1]=='r'||MainArray[r1][c1]=='q'))
    {
        if(c1==c2&&r1!=r2)
        {
            if(r1>r2)
            {
                for(int i=r2+1; i<r1; i++)
                {
                    if(MainArray[i][c1]!=' ')
                    {
                        return false;
                    }
                }
                return true;
            }
            else if (r2>r1)
            {
                for(int i=r2-1; i>r1; i--)
                {
                    if(MainArray[i][c1]!=' ')
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        else if(r2==r1&&c1!=c2)
        {
            if(c1>c2)
            {
                for(int i=c2+1; i<c1; i++)
                {
                    if(MainArray[r1][i]!=' ')
                    {
                        return false;
                    }
                }
                return true;
            }
            else if (c2>c1)
            {
                for(int i=c2-1; i>c1; i--)
                {
                    if(MainArray[r1][i]!=' ')
                    {
                        return false;
                    }
                }
                return true;
            }
        }
    }

    return false;

}

bool Queen(int r1,int c1,int r2,int c2)
{
    //BOOLEAN FUNCTION CHECK IF THE QUUEN CAN MOVE TO THE REQUIRED LOCATION OR NOT AND RETURN TRUE OR FALSE
    if(Rook(r1,c1,r2,c2))
        return true;
    else if(Bishops(r1,c1,r2,c2))
        return true;
    return false;
}

bool Pawn(int r1,int c1,int r2,int c2,int move)
{
    //BOOLEAN FUNCTION CHECK IF THE KING CAN MOVE TO THE REQUIRED LOCATION OR NOT AND RETURN TRUE OR FALSE
    if(move==-1)
    {
        if(MainArray[r1][c1]=='P')
        {
            if(r2==r1+1&&(c2==c1-1||c2==c1+1))
                return true;
            else
                return false;

        }
        else if(MainArray[r1][c1]=='p')

        {
            if(r1==r2+1&&(c2==c1-1||c2==c1+1))
                return true;
            else
                return false;

        }
    }

    else if(move%2==0)
    {
        if((c1==c2&&r1==r2-1)||(c1==c2&&r1==r2-2&&r1==1&&MainArray[r1+1][c1]==' '))
        {
            if(MainArray[r2][c2]==' '&&MainArray[r1][c1]=='P')
            {
                return true;
            }
            else
                return false;


        }
        else if(r2==r1+1&&(c2==c1-1||c2==c1+1))
        {

            if((MainArray[r2][c2]=='p'||MainArray[r2][c2]=='q'||MainArray[r2][c2]=='r'||MainArray[r2][c2]=='b'||MainArray[r2][c2]=='n')&&MainArray[r1][c1]=='P')
                return true;
            else
                return false;
        }

    }
    else if(move%2!=0)
    {
        if((c1==c2&&r1==r2+1)||(c1==c2&&r1==r2+2&&r1==6&&MainArray[r2+1][c1]==' '))
        {
            if(MainArray[r2][c2]==' '&&MainArray[r1][c1]=='p')
            {
                return true;
            }
            else
                return false;
        }
        else if(r1==r2+1&&(c2==c1-1||c2==c1+1))
        {

            if((MainArray[r2][c2]=='P'||MainArray[r2][c2]=='Q'||MainArray[r2][c2]=='R'||MainArray[r2][c2]=='B'||MainArray[r2][c2]=='N')&&MainArray[r1][c1]=='p')
                return true;
            else
                return false;
        }
    }

    return false;
}

bool MOVE(int r1,int c1,int r2,int c2,int move)
{
    //THIS FUNCTION USES SWAP METHOD TO MAKE THE MOVE AND THEN SAVES THE MOVE DATA TO THE UNDO ARRAY ELSE RETURN FALSE
    if(MainArray[r2][c2]==' ')
    {
        MainArray[r2][c2]=MainArray[r1][c1];
        MainArray[r1][c1]=' ';
        undoA[0][IndexOfUndo]=r1;
        undoA[1][IndexOfUndo]=c1;
        undoA[2][IndexOfUndo]=r2;
        undoA[3][IndexOfUndo]=c2;
        undoA[4][IndexOfUndo]=-1;
        undoA[5][IndexOfUndo]=0;
        IndexOfUndo++;
        return true;
    }
    else if(move%2==0)
    {
        if(MainArray[r2][c2]=='p'||MainArray[r2][c2]=='q'||MainArray[r2][c2]=='r'||MainArray[r2][c2]=='b'||MainArray[r2][c2]=='n')
        {
            captureB[IndexOfCaptureB]=MainArray[r2][c2];
            MainArray[r2][c2]=MainArray[r1][c1];
            MainArray[r1][c1]=' ';
            undoA[0][IndexOfUndo]=r1;
            undoA[1][IndexOfUndo]=c1;
            undoA[2][IndexOfUndo]=r2;
            undoA[3][IndexOfUndo]=c2;
            undoA[4][IndexOfUndo]=IndexOfCaptureB;
            undoA[5][IndexOfUndo]=0;
            IndexOfUndo++;
            IndexOfCaptureB++;
            return true;
        }

    }
    else if(move%2!=0)
    {
        if(MainArray[r2][c2]=='P'||MainArray[r2][c2]=='Q'||MainArray[r2][c2]=='R'||MainArray[r2][c2]=='B'||MainArray[r2][c2]=='N')
        {
            captureW[IndexOfCaptureW]=MainArray[r2][c2];
            undoA[0][IndexOfUndo]=r1;
            undoA[1][IndexOfUndo]=c1;
            undoA[2][IndexOfUndo]=r2;
            undoA[3][IndexOfUndo]=c2;
            undoA[4][IndexOfUndo]=IndexOfCaptureW;
            undoA[5][IndexOfUndo]=0;
            IndexOfCaptureW++;
            IndexOfUndo++;
            MainArray[r2][c2]=MainArray[r1][c1];
            MainArray[r1][c1]=' ';
            return true;
        }
    }
    return false;
}

bool Movement(int r1,int c1,int r2,int c2,int move,int*pointer_move,bool fake)
{
    //CHECKS IF THE MOVE CAN BE DONE BY THE PIECES FUNTIONS AND THEN CALL THE MOVE FUNCTION TO MAKE THE MOVE ELSE REDUCE THE NUMBER OF MOVES BY ONE AND RETURN FALSE
    // IF FAKE VARIABLE IT TRUE THEN THIS FUNCTION IS USED BY OTHER FUNCTIONS TO CHECK IF THEIR ARE ANY POSSIBLE MOVE TO DO THE REQUIRED THING (CHECKMATE,STALEMATE)

    if(MainArray[r1][c1]=='P'||MainArray[r1][c1]=='p')
    {
        if(Pawn(r1,c1,r2,c2,move))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='r'&&move%2!=0)
    {
        if(Rook(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='R'&&move%2==0)
    {
        if(Rook(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='n'&&move%2!=0)
    {
        if(Knight(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='N'&&move%2==0)
    {
        if(Knight(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='b'&&move%2!=0)
    {
        if(Bishops(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='B'&&move%2==0)
    {
        if(Bishops(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='q'&&move%2!=0)
    {
        if(Queen(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='Q'&&move%2==0)
    {
        if(Queen(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='k'&&move%2!=0)
    {
        if(King(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    else if(MainArray[r1][c1]=='K'&&move%2==0)
    {
        if(King(r1,c1,r2,c2))
        {
            if(MOVE(r1,c1,r2,c2,move))
                return true;
        }
    }
    if(!fake)
        *pointer_move=*pointer_move-1;
    return false;
}

bool CheckPromotion(int move)
{
    //THIS FUNCTION CHECK IF THERE IS A POSSIBLE PROMOTION CAN BE DONE IN THE NEXT MOVE AND RETURN TRUE OR FALSE
    for(int j=0; j<=column; j++)
    {
        if(MainArray[1][j]=='p'&&move%2!=0)
            return true;
        if(MainArray[6][j]=='P'&&move%2==0)
            return true;
    }
    return false;
}

bool Validate_input(char Input[],int move)
{
    //THIS FUNCTION VALIDATE THE INPUT IF TRUE RETURN TRUE ELSE RETURN FALSE
    move ++;
    if(!((Input[0]>72||Input[0]<65)||(Input[1]>56||Input[1]<49)||(Input[2]>72||Input[2]<65)||(Input[3]>56||Input[3]<49)))
    {
        if(Input[4]!='\0'&&CheckPromotion(move))
        {
            return true ;
        }
        else if(Input[4]=='\0')
            return true;
    }
    return false;
}
bool CHECK(char turn)
{
    //THIS FUNCTION CHECKS IF THEIR IS A CHECK ON A PARTICULAR PLAYER ACCORDING TO THE TURN VARIABLE AND RETURNS TRUE OR FALSE
    int r2,c2;
    r2=0;
    c2=0;
    if(turn=='w')
    {
        for(int i=0; i<=row; i++)
        {
            for(int j=0; j<=column; j++)
            {
                if(MainArray[i][j]=='k')
                {
                    r2=i;
                    c2=j;
                    break;
                }
            }
            if(MainArray[r2][c2]=='k')
            {


                break;
            }
        }

        for(int i=0; i<=row; i++)
        {
            for(int j=0; j<=column; j++)
            {
                if(MainArray[i][j]=='Q')
                {
                    if(Queen(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='B')
                {
                    if(Bishops(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='R')
                {
                    if(Rook(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='N')
                {
                    if(Knight(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='K')
                {
                    if(King(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='P')
                {
                    if(Pawn(i,j,r2,c2,-1))
                        return true;
                }
            }
        }
    }
    else if(turn=='b')
    {
        for(int i=0; i<=row; i++)
        {
            for(int j=0; j<=column; j++)
            {
                if(MainArray[i][j]=='K')
                {
                    r2=i;
                    c2=j;
                    break;
                }
            }
            if(MainArray[r2][c2]=='K')
            {
                break;
            }
        }

        for(int i=0; i<=row; i++)
        {
            for(int j=0; j<=column; j++)
            {
                if(MainArray[i][j]=='q')
                {
                    if(Queen(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='b')
                {
                    if(Bishops(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='r')
                {
                    if(Rook(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='n')
                {
                    if(Knight(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='k')
                {
                    if(King(i,j,r2,c2))
                        return true;
                }
                if(MainArray[i][j]=='p')
                {
                    if(Pawn(i,j,r2,c2,-1))
                        return true;
                }
            }
        }
    }
    return false;
}

bool CheckMate(int move,int*pointer_move)
{
    // THIS FUNCTION TRY EVERY POSSIBLE MOVE TO REMOVE THE CHECK ON ANY OF THE KINGS AND UNDO IT AND RETURN TRUE OR FALSE
    if(move!=0)
    {
        if(CHECK('w')&&move%2==0)
        {
            move--;
            for(int i=0; i<=row; i++)
            {
                for(int j=0; j<=column; j++)
                {
                    for(int x=0; x<=row; x++)
                    {
                        for(int y=0; y<=column; y++)
                        {
                            if(Movement(i,j,x,y,move,pointer_move,true))
                            {
                                if(!CHECK('w'))
                                {
                                    UNDO(move,false);
                                    return false;
                                }
                                UNDO(move,false);
                            }

                        }
                    }
                }
            }
        }
        else if(CHECK('b')&&move%2!=0)
        {
            move--;

            for(int i=0; i<=row; i++)
            {
                for(int j=0; j<=column; j++)
                {
                    for(int x=0; x<=row; x++)
                    {
                        for(int y=0; y<=column; y++)
                        {
                            if(Movement(i,j,x,y,move,pointer_move,true))
                            {
                                if(!CHECK('b'))
                                {
                                    UNDO(move,false);
                                    return false;
                                }
                                UNDO(move,false);
                            }

                        }
                    }
                }
            }
        }
        else
            return false;
    }
    else
        return false;

    return true;
}

bool Stalemate(int move,int*pointer_move)
{
    // THIS FUNCTION CHECK IF THEIR IS A POSSIBLE MOVE AND IF FOUND UNDO IT AND RETURN FALSE ELSE RETURNS TRUE
    if(move%2==0)
    {
        move--;
        for(int i=0; i<=row; i++)
        {
            for(int j=0; j<=column; j++)
            {
                for(int x=0; x<=row; x++)
                {
                    for(int y=0; y<=column; y++)
                    {
                        if(Movement(i,j,x,y,move,pointer_move,true))
                        {
                            if(!CHECK('w'))
                            {
                                UNDO(move,false);
                                return false;
                            }
                            UNDO(move,false);
                        }
                    }
                }
            }
        }
        move++;
    }
    if(move%2!=0)
    {
        move--;
        for(int i=0; i<=row; i++)
        {
            for(int j=0; j<=column; j++)
            {
                for(int x=0; x<=row; x++)
                {
                    for(int y=0; y<=column; y++)
                    {
                        if(Movement(i,j,x,y,move,pointer_move,true))
                        {
                            if(!CHECK('b'))
                            {
                                UNDO(move,false);
                                return false;
                            }
                            UNDO(move,false);
                        }
                    }
                }
            }
        }
    }

    return true;
}

void printscreen(char Input[],int move,int*pointer_move,int x,bool end,char pr_input[])
{
    // THIS FUNCTION PRINT THE SCREEN
    //X VARIABLE USED TO PRINT IF THIER IS PROMOTION DONE
    //END VARIABLE IS USED TO KNOW IF THE GAME IS OVER SO THE TURNS WILL NOT BE PRINTED
    system("cls");
    printf("                     <WELCOME>\n\n");
    if(Input[0]==-1)
    {
        MainArray[0][0]='R';
        MainArray[0][1]='N';
        MainArray[0][2]='B';
        MainArray[0][3]='Q';
        MainArray[0][4]='K';
        MainArray[0][5]='B';
        MainArray[0][6]='N';
        MainArray[0][7]='R';
        MainArray[7][0]='r';
        MainArray[7][1]='n';
        MainArray[7][2]='b';
        MainArray[7][3]='q';
        MainArray[7][4]='k';
        MainArray[7][5]='b';
        MainArray[7][6]='n';
        MainArray[7][7]='r';
        for(int i=0; i<=column; i++)
            MainArray[1][i]='P';
        for(int i=0; i<=column; i++)
            MainArray[6][i]='p';
        for(int i=2; i<=5; i++)
        {
            for(int j=0; j<=column; j++)
            {
                MainArray[i][j]=' ';
                captureW[j]=' ';
                captureB[j]=' ';
            }
        }
    }
    printf("  To Undo Write \"u\"|To Redo Write \"r\"|To Save Write \"s\"\n\n");
    printf(" ");
    for(int i=0; i<=column; i++)
    {
        printf("     %c",i+65);
        if(i==column)
            printf("\tMoves:%d",move);
    }
    printf("\n   ");
    for(int j=0; j<column; j++)
    {
        printf("-------");
    }
    printf("\n");
    for (int i =0; i<row+1; i++)
    {
        printf("%d  ",i+1);
        for(int j=0; j<column+1; j++)
        {
            if(i%2!=0&&j%2==0)
                printf(" %c %c %c",176,MainArray[i][j],176);

            else if(i%2==0&&j%2!=0)
                printf(" %c %c %c",176,MainArray[i][j],176);
            else
                printf(" %c %c %c",219,MainArray[i][j],219);
        }

        printf("   ");
        printf("%d   ",i+1);
        if (i==0)
            for(int w=0; w<16; w++)
                if(captureB[w]!=' ')
                    printf("%c ",captureB[w]);
        if (i==7)
            for(int w=0; w<16; w++)
                if(captureW[w]!=' ')
                    printf("%c ",captureW[w]);
        printf("\n   ");
        for(int j=0; j<column; j++)
        {
            printf("-------");
        }
        printf("\n");
    }
    printf(" ");
    for(int i=0; i<=column; i++)
    {
        printf("     %c",i+65);
        if(i==column)
            printf("\tPrevious Move:%c%c%c%c",pr_input[0],pr_input[1],pr_input[2],pr_input[3]);
    }
    if(!end)
    {
        if(move%2==0)
            printf("\n\nWhite Turn");
        else
            printf("\n\nBlack Turn");
    }
    if(x==2)
    {
        printf("\n\nPlease Enter Next Move:");
    }
    if(x==1&&move%2==0)
        printf("\nP promoted to %c",Input[4]);
    if(x==1&&move%2!=0)
        printf("\np promoted to %c",Input[4]);

}

int main(void)
{
    char typeofgame[500];
    typeofgame[0]='1';
    int move=0;
    char Input[500]= { };
    char pr_input[4]= { };
    int r1=-1,r2=-1,c1=-1,c2=-1;
    bool flag_check=false;
    bool flag_print=true;
    bool flag_draw=false;
    int promote_flag=0;
    IndexOfUndo=0;
    IndexOfRedo=0;

    IndexOfCaptureB=0;
    IndexOfCaptureW=0;

    printf("                               WELCOME\n");
    printf("For New Game Enter 1");
    printf("\tFor Saved Game Enter 2\n");
    do
    {
        //LOOP TILL 1 OR 2 IS THE INPUT
        if((typeofgame[0]!='1'&&typeofgame[0]!='2')||typeofgame[1]!='\0')
            printf("Please Re-Enter:");
        gets(typeofgame);
    }
    while((typeofgame[0]!='1' && typeofgame[0]!='2')||typeofgame[1]!='\0');
    if(typeofgame[0]=='1'&&typeofgame[1]=='\0')
    {
        //IF NEW GAME PRINT A FRESH BOARD
        Input[0]=-1;
        printscreen(Input,move,&move,2,false,pr_input);
    }
    if(typeofgame[0]=='2'&&typeofgame[1]=='\0')
    {
        // IF NO SAVED GAMES OR LOAD.TXT IS EMPTY EXIT
        if(!LOAD(&move))
            return 0;
        printscreen(Input,move,&move,2,false,pr_input);
    }
    while(!CheckMate(move,&move))
    {
        // THE MAIN LOOP OF THE GAME AND IS EXPLAIND IN PSEUDO CODE IN THE REPORT
        if(typeofgame[0]=='2'&&typeofgame[1]=='\0')
        {
            typeofgame[0]='1';
            if(move%2!=0)
            {
                if(CHECK('w'))
                {
                    move--;
                    UNDO(move,false);
                    if(flag_print)
                        printscreen(Input,move,&move,promote_flag,false,pr_input);
                    printf("\nCan't make this move due to check");
                    flag_print=false;
                    printf("\nPlease Enter Next Move:");
                }
                if(CHECK('b'))
                    flag_check=true;
            }

            else if(move%2==0)
            {
                if(CHECK('b'))
                {
                    move--;
                    UNDO(move,false);
                    if(flag_print)
                        printscreen(Input,move,&move,promote_flag,false,pr_input);
                    flag_print=false;
                    printf("\nCan't make this move due to CHECK");
                    printf("\nPlease Enter Next Move:");
                }
                if(CHECK('w'))
                    flag_check=true;
            }
            if(flag_check)
            {
                if(flag_print)
                    printscreen(Input,move,&move,promote_flag,false,pr_input);
                printf("\nCHECK");
                printf("\nPlease Enter Next Move:");
                flag_check=false;
            }
        }

        if(Stalemate(move,&move))
        {
            flag_draw=true;
            break;
        }
        gets(Input);
        if(Input[0]=='\0')
        {
            continue;
        }
        if(Input[0]!='u'&&Input[0]!='r'&&Input[0]!='s')
        {
            if(Validate_input(Input,move))
            {
                c1=Input[0]-17-'0';
                r1=Input[1]-1-'0';
                c2=Input[2]-17-'0';
                r2=Input[3]-1-'0';
                pr_input[0]=Input[0];
                pr_input[1]=Input[1];
                pr_input[2]=Input[2];
                pr_input[3]=Input[3];
                move++;

                if(CheckPromotion(move)&&((MainArray[r1][c1]=='P'&&move%2==0)||(MainArray[r1][c1]=='p'&&move%2!=0))&&Input[4]!='\0')
                {
                    if(Movement(r1,c1,r2,c2,move,&move,false))
                    {
                        if((Input[4]=='B'||Input[4]=='Q'||Input[4]=='N'||Input[4]=='R')&&move%2==0)
                        {
                            MainArray[r2][c2]=Input[4];
                            undoA[5][IndexOfUndo-1]=1;
                            promote_flag=1;
                            IndexOfRedo=0;

                        }
                        else if((Input[4]=='b'||Input[4]=='q'||Input[4]=='n'||Input[4]=='r')&&move%2!=0)
                        {
                            promote_flag=1;
                            MainArray[r2][c2]=Input[4];
                            undoA[5][IndexOfUndo-1]=1;
                            IndexOfRedo=0;
                        }

                        else
                        {
                            printf("Invalid Input please re-Enter I am hero:");
                            flag_print=false;
                            UNDO(move,false);
                            move--;
                        }
                    }
                }
                else if(Input[4]=='\0')
                {
                    if(!Movement(r1,c1,r2,c2,move,&move,false))
                    {
                        printf("\nInvalid Move please re-Enter:");
                        flag_print=false;

                    }
                    else
                        IndexOfRedo=0;
                }

            }
            else
            {
                printf("\nInvalid Input please re-Enter:");
                flag_print=false;

            }
        }
        else if(Input[0]=='u'&& Input[1]=='\0')
        {
            if(UNDO(move,true))
            {
                move--;
            }
            else
            {
                printf("Can't Undo anymore!!");
                flag_print=false;

                printf("\nPlease Enter Next Move:");
            }
        }
        else if(Input[0]=='r'&& Input[1]=='\0')
        {
            if(REDO(move))
            {
                move++;
            }
            else
            {
                printf("Can't Redo anymore!!");
                flag_print=false;

                printf("\nPlease Enter Next Move:");
            }
        }
        else if(Input[0]=='s'&& Input[1]=='\0')
        {
            SAVE(move);
            printf("\n GAME SAVED!");
            return 0;
        }
        else
        {
            printf("\nInvalid Input please re-Enter:");
            flag_print=false;

        }
        if(move%2!=0)
        {
            if(CHECK('w'))
            {
                UNDO(move,false);
                move--;
                if(flag_print)
                    printscreen(Input,move,&move,promote_flag,false,pr_input);
                printf("\nCan't make this move due to check");
                flag_print=false;

                printf("\nPlease Enter Next Move:");
            }
            if(CHECK('b'))
                flag_check=true;
        }

        else if(move%2==0)
        {
            if(CHECK('b'))
            {
                UNDO(move,false);
                move--;
                if(flag_print)
                    printscreen(Input,move,&move,promote_flag,false,pr_input);
                flag_print=false;
                printf("\nCan't make this move due to CHECK");
                printf("\n\nPlease Enter Next Move:");
            }
            if(CHECK('w'))
                flag_check=true;
        }
        if(flag_check)
        {
            if(flag_print)
                printscreen(Input,move,&move,promote_flag,false,pr_input);
            printf("\nCHECK");
            flag_print=false;
            printf("\nPlease Enter Next Move:");
            flag_check=false;
        }
        else if(flag_print)
        {
            printscreen(Input,move,&move,promote_flag,false,pr_input);
            printf("\n\nPlease Enter Next Move:");
        }
        flag_print=true;
        promote_flag=0;
    }
    printscreen(Input,move,&move,promote_flag,true,pr_input);
    if(flag_draw)
        printf("  \nSTALEMATE\n\n DRAW");
    else if(move%2==0)
        printf("  \n!!CHECKMATE!!\n***BLACK WINS***");
    else if(move%2!=0)
        printf("  \n!!CHECKMATE!!\n***WHITE WINS***");
    return 0;
}
