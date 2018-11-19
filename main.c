//
//  main.c
//  floris
//
//  Created by Dave on 2017-04-04.
//  Copyright © 2017 Dave. All rights reserved.
//
//  This app calculates all the combinations of choosing a team of size M from candidates list of N potentials
//  Each person has one other person they like and two people they dislike
//  Likes are mutual, dislikes are not
//  Goal is to have the largest group where no one is "unhappy", i.e. no more person within the team has more people they dislike than like.
//
//  Using Metthew Belmonte's twiddle code, we create N choose M combinations, and check each to calculate the team happiness.
//

#include <stdio.h>

/*twiddle.c - generate all combinations of M elements drawn without replacement
 from a set of N elements.  This routine may be used in two ways:
 (0) To generate all combinations of M out of N objects, let a[0..N-1]
 contain the objects, and let c[0..M-1] initially be the combination
 a[N-M..N-1].  While twiddle(&x, &y, &z, p) is false, set c[z] = a[x] to
 produce a new combination.
 (1) To generate all sequences of 0's and 1's containing M 1's, let
 b[0..N-M-1] = 0 and b[N-M..N-1] = 1.  While twiddle(&x, &y, &z, p) is
 false, set b[x] = 1 and b[y] = 0 to produce a new sequence.
 In either of these cases, the array p[0..N+1] should be initialised as
 follows:
 p[0] = N+1
 p[1..N-M] = 0
 p[N-M+1..N] = 1..M
 p[N+1] = -2
 if M=0 then p[1] = 1
 In this implementation, this initialisation is accomplished by calling
 inittwiddle(M, N, p), where p points to an array of N+2 ints.
 
 Coded by Matthew Belmonte <mkb4@Cornell.edu>, 23 March 1996.  This
 implementation Copyright (c) 1996 by Matthew Belmonte.  Permission for use and
 distribution is hereby granted, subject to the restrictions that this
 copyright notice and reference list be included in its entirety, and that any
 and all changes made to the program be clearly noted in the program text.
 
 This software is provided 'as is', with no warranty, express or implied,
 including but not limited to warranties of merchantability or fitness for a
 particular purpose.  The user of this software assumes liability for any and
 all damages, whether direct or consequential, arising from its use.  The
 author of this implementation will not be liable for any such damages.
 
 Reference:
 
 Phillip J Chase, `Algorithm 382: Combinations of M out of N Objects [G6]',
 Communications of the Association for Computing Machinery 13:6:368 (1970).
 
 The returned indices x, y, and z in this implementation are decremented by 1,
 in order to conform to the C language array reference convention.  Also, the
 parameter 'done' has been replaced with a Boolean return value.
 */

int twiddle(x, y, z, p)
int *x, *y, *z, *p;
{
    register int i, j, k;
    j = 1;
    while(p[j] <= 0)
        j++;
    if(p[j-1] == 0)
    {
        for(i = j-1; i != 1; i--)
            p[i] = -1;
        p[j] = 0;
        *x = *z = 0;
        p[1] = 1;
        *y = j-1;
    }
    else
    {
        if(j > 1)
            p[j-1] = 0;
        do
            j++;
        while(p[j] > 0);
        k = j-1;
        i = j;
        while(p[i] == 0)
            p[i++] = -1;
        if(p[i] == -1)
        {
            p[i] = p[k];
            *z = p[k]-1;
            *x = i-1;
            *y = k-1;
            p[k] = -1;
        }
        else
        {
            if(i == p[0])
                return(1);
            else
            {
                p[j] = p[i];
                *z = p[i]-1;
                p[i] = 0;
                *x = j-1;
                *y = i-1;
            }
        }
    }
    return(0);
}

void inittwiddle(m, n, p)
int m, n, *p;
{
    int i;
    p[0] = n+1;
    for(i = 1; i != n-m+1; i++)
        p[i] = 0;
        while(i != n+1)
        {
            p[i] = i+m-n;
            i++;
        }
    p[n+1] = -2;
    if(m == 0)
        p[1] = 1;
        }


// Game definitions begin here

#define NUMPLAYERS 22
#define N NUMPLAYERS
int M = 13;


char *names[] = {"Borcha","Marnid", "Ymira", "Rolf",
    "Baheshtur", "Firentis", "Deshavi", "Matheld", "Alayen",
    "Bunduk", "Katrin", "Jeremus", "Nizar", "Lezalit", "Artimenner",
    "Klethi", "Floris", "Nadia", "Odval", "Sajjad", "Ghazwan", "Edwyn"
};

// each player has one person they (mutually) like, and two they dislike
struct player {
    int likes;
    int dislikes[2];
};
struct player players[NUMPLAYERS];

// Create singleton group to test for viability
struct caddr {
    int count;
    int playerList[NUMPLAYERS];
};
struct caddr theGroup;

// returns the name of player from their character id (array index)
char *florisLookup (int player)
{
    return names[player];
}

// Linearly search through the team to see if this player is in the group
int inGroup (int plyr) {
    for (int i = 0; i < theGroup.count; i++) {
        if (plyr == theGroup.playerList[i]) {
            return 1;
        }
    }
    return 0;
}

// calculate the happiness of the person by seeing which of their likes and dislikes are in the group
int calcHappiness (int tmember) {
    struct player *p = &players[theGroup.playerList[tmember]];
    // a person is happy if their happiness is >=0, i.e. no one they like or dislike, or 1 person they like and no more than one they dislike
    return (inGroup(p->likes) - inGroup(p->dislikes[0]) - inGroup(p->dislikes[1]));
}

// a group is viable if no one is unhappy
int isViableGroup () {
    for (int i =0; i < theGroup.count; i++) {
        if (calcHappiness (i) < 0)
            return 0;                   // someone is unhappy, makes the group unviable
    }
    return 1;                           // no one's unhappy, the groups is viable
}

// The combiantorics algorithm twiddle creates an array of boolean 0 or 1.
// We take "1" to mean the corresponding player id should be added to the team
void buildTeam (int b[], int siz) {
    theGroup.count = 0;
    for (int i = 0; i < siz; i++) {
        if (b [i]) {
            theGroup.playerList[theGroup.count++] = i;
        }
    }
}

// print the current goup. + indicates someone liked, - means disliked
void printGroup() {
    printf ("group contains %d players\n", theGroup.count);
    for (int i = 0; i < theGroup.count; i++) {
        struct player *p = &players[theGroup.playerList[i]];
        
        printf (" %s", florisLookup (theGroup.playerList[i]));
        if (inGroup (p->likes)) {
            printf ("+%s", florisLookup(p->likes));
        }
        if (inGroup (p->dislikes[0])) {
            printf ("-%s", florisLookup(p->dislikes[0]));
        }
        if (inGroup (p->dislikes[1])) {
            printf ("-%s", florisLookup(p->dislikes[1]));
        }
        printf("\n");
    }

}


int main(int argc, const char * argv[]) {
    
// Mountainblade characters, likes and dislikes
// http://floris.wikia.com/wiki/Companions_of_Floris
    struct player *p;
    //Borcha
    p = &players[0];
    p->likes = 1;
    p->dislikes[0] = 6;
    p->dislikes[1] = 20;
    //Marnid
    p = &players[1];
    p->likes = 0;
    p->dislikes[0] = 4;
    p->dislikes[1] = 17;
    //Ymira
    p = &players[2];
    p->likes = 8;
    p->dislikes[0] = 18;
    p->dislikes[1] = 19;
    //Rolf
    p = &players[3];
    p->likes = 19;
    p->dislikes[0] = 9;
    p->dislikes[1] = 6;
    //Bahestur
    p = &players[4];
    p->likes = 20;
    p->dislikes[0] = 1;
    p->dislikes[1] = 10;
    //Firentis
    p = &players[5];
    p->likes = 16;
    p->dislikes[0] = 12;
    p->dislikes[1] = 10;
    //Deshavi
    p = &players[6];
    p->likes = 15;
    p->dislikes[0] = 0;
    p->dislikes[1] = 3;
    //Matheld
    p = &players[7];
    p->likes = 12;
    p->dislikes[0] = 11;
    p->dislikes[1] = 21;
    //Alayen
    p = &players[8];
    p->likes = 2;
    p->dislikes[0] = 16;
    p->dislikes[1] = 18;
    //Bunduk
    p = &players[9];
    p->likes = 10;
    p->dislikes[0] = 3;
    p->dislikes[1] = 13;
    //Katrin
    p = &players[10];
    p->likes = 9;
    p->dislikes[0] = 5;
    p->dislikes[1] = 4;
    //Jeremus
    p = &players[11];
    p->likes = 18;
    p->dislikes[0] = 7;
    p->dislikes[1] = 14;
    //Nizar
    p = &players[12];
    p->likes = 7;
    p->dislikes[0] = 17;
    p->dislikes[1] = 5;
    //Lezalit
    p = &players[13];
    p->likes = 14;
    p->dislikes[0] = 21;
    p->dislikes[1] = 9;
    //Artimenner
    p = &players[14];
    p->likes = 13;
    p->dislikes[0] = 11;
    p->dislikes[1] = 20;
    //Klethi
    p = &players[15];
    p->likes = 6;
    p->dislikes[0] = 16;
    p->dislikes[1] = 19;
    //Floris
    p = &players[16];
    p->likes = 5;
    p->dislikes[0] = 8;
    p->dislikes[1] = 15;
    //Nadia
    p = &players[17];
    p->likes = 21;
    p->dislikes[0] = 12;
    p->dislikes[1] = 1;
    //Odval
    p = &players[18];
    p->likes = 11;
    p->dislikes[0] = 2;
    p->dislikes[1] = 8;
    //Sajjad
    p = &players[19];
    p->likes = 3;
    p->dislikes[0] = 15;
    p->dislikes[1] = 2;
    // Ghazwan
    p = &players[20];
    p->likes = 4;
    p->dislikes[0] = 0;
    p->dislikes[1] = 14;
    //Edwyn
    p = &players[21];
    p->likes = 17;
    p->dislikes[0] = 8;
    p->dislikes[1] = 13;
    
 
    // print the likes/dislikes table for verification
    for (int i = 0; i < NUMPLAYERS; i++) {
        p = &players[i];
        printf ("%s likes %s, dislikes %s & %s\n",
                names[i],
                florisLookup (p->likes),
                florisLookup (p->dislikes[0]),
                florisLookup (p->dislikes[1]));
        
    }
  
    
    // init the combination generator "twiddle", which creates an array "b" of booleans
    int i, x, y, z, pp[N+2], b[N];
    inittwiddle(M, N, pp);
    int count = 1;
    int viableTeams = 0;
    
    // initial coditions have to be set in this array b for the n!m algorithm.
    for(i = 0; i != N-M; i++)
    {
        b[i] = 0;
    }
    while(i != N)
    {
        b[i++] = 1;
    }
    
    // test the initial group
    for(i = 0; i != N; i++)
        putchar(b[i]? '1': '0');
    putchar('\n');
    buildTeam(b, N);
    
    if (isViableGroup()) {
        printf("viable!\n");
    } else {
        printf("nope\n");
    }


    while(!twiddle(&x, &y, &z, pp))     // generates all N choose M combinations
    {
        b[x] = 1;
        b[y] = 0;

        buildTeam (b, N);               // construct a team of players whose id corresponds to the "b" array having a "1" in their index
        count++;                        // combinations counter, to make sure we try all N choose M

        
        if (isViableGroup()) {
            viableTeams++;
            printf ("Found a viable group! combo#%d\n", count);
            for(i = 0; i != N; i++)
                putchar(b[i]? '1': '0');
            putchar('\n');
            printGroup();
        }
 
    }

    printf("Found %d teams of %d combinations\n", viableTeams, count);
 
}
