//i suppose it can't decode russian. 
//in main primary function used is numtests. i used this in order to perform several tests with specifications given.
//it also creates a .txt file that contains information about all runs performed. 
//here are lots of comments. Some of them are different fitness functions for different functions to optimize, some are for 
//creation of txt file with extensive information about actions performed in algorithm. i used it for testing. a bit more convenient than debugging.
//the problems are:
//1) May be there is something better than pseudo random rand()?
//2) For some reason the algorithm can't find coordinates (0,0), although it easily finds any others like (3,2). It gets really close to zero coordinates like 1.8 * 10 ^ -60, but still.
//3) I suppose it has some redundant parts, although I still don't know how do they affect performance and do they at all.
//4) There are quite a number of variables for tuning up this algorithm.

#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>
//#include <windows.h>
#include <stdio.h>
#include <math.h>
using namespace std;

const int varnum = 2;
const int scoutbeecount = 300;
const int selectedbeecount = 10;
const int bestbeecount = 30;
const int selsitescount = 15;
const int bestsitescount = 5;
const int maxfunccounter = 1;
const double rangeshrinker = 0.5;
const int maxiter = 100;
const int maxstag = 50;
const int beecount = scoutbeecount + selectedbeecount * selsitescount + bestbeecount * bestsitescount;
const double minvalues[varnum] = {-5.10,-5.10};
const double maxvalues[varnum] = {5.10, 5.10};

class doublebee
{
    public:
    double minval[varnum];
    double maxval[varnum];
    double position[varnum];
    double fitness;
        doublebee()
        {
            fitness = 0.0;
            int i = 0;
            while (i < varnum)
            {
                minval[i] = minvalues[i];
                maxval[i] = maxvalues[i];
                position[i] = minval[i] + ((double)rand() / (RAND_MAX + 1.0)) * (maxval[i] - minval[i]);
                i++;
            }
            fitness = - (20.0 + pow(position[0], 2.0) + pow(position[1], 2.0) - 10 * (cos(2.0 * M_PI * position[0]) + cos(2.0 * M_PI * position[1])));
        }

        void flyto(double otherpos[varnum], double range_list[varnum])
        {
            fitness = 0;
            for (int i = 0; i < varnum; i++)
            {
                minval[i] = otherpos[i] - range_list[i];
                if (minval[i] < minvalues[i]) minval[i] = minvalues[i];
                maxval[i] = otherpos[i] + range_list[i];
                if (maxval[i] > maxvalues[i]) maxval[i] = maxvalues[i];
                position[i] = minval[i] + ((double)rand() / (RAND_MAX + 1.0)) * (maxval[i] - minval[i]);
            }
            fitness = - (20.0 + pow(position[0], 2.0) + pow(position[1], 2.0) - 10 * (cos(2.0 * M_PI * position[0]) + cos(2.0 * M_PI * position[1])));
        }
};

class membee
{
public:
    double minval[varnum];
    double maxval[varnum];
    double position[varnum];
    double fitness;
    double range[varnum];
    int funccounter;
    membee(){}
    membee(doublebee bee, int func)
    {
        fitness = bee.fitness;
        int sites = bestsitescount + selsitescount;
        for(int i = 0; i < varnum; i++)
        {
            minval[i] = bee.minval[i];
            maxval[i] = bee.maxval[i];
            position[i] = bee.position[i];
            if(minval[i] == minvalues[i] && maxval[i] == maxvalues[i])
            {
                range[i] = (maxval[i] - minval[i]) / sites * 2.0;
            }
            else
            {
                range[i] = (maxval[i] - minval[i]) / 2.0;
            }
        }
            funccounter = func;
    }
};

class hive
{
    public:
        double range_list[varnum];
        doublebee swarm[beecount];
        membee membees[bestsitescount + selsitescount];
        hive()
        {
            for (int i = 0; i < beecount; i++)
            {
                swarm[i] = doublebee();
            }
            for (int i = 0; i < bestsitescount + selsitescount; i++)
            {
                membees[i] = membee(swarm[i], 0);
            }
        }

        void sortswarm()
        {
            for (int i = 0; i < bestsitescount + selsitescount; i++)
            {
                membee bestbee = membees[i];
                doublebee bestswarmbee = swarm[i];
                int bestnum = i;
                bool ind = false;
                for (int j = i+1; j < scoutbeecount; j++)
                {
                    if (swarm[j].fitness > bestbee.fitness && swarm[j].fitness > bestswarmbee.fitness)
                    {
                        bestswarmbee = swarm[j];
                        bestnum = j;
                        ind = true;
                    }
                }
                if (ind)
                {
                    swarm[bestnum] = swarm[i];
                    swarm[i] = bestswarmbee;
                    membee reserve = membees[i];
                    if(bestnum < scoutbeecount)
                    {
                        membees[i] = membee(bestswarmbee, 0);
                    }
                    else
                    {
                        bestnum -= scoutbeecount;
                        int memnum = 100;
                        if(bestnum < bestbeecount * bestsitescount)
                        {
                            double x = (double)bestnum / (double)bestbeecount;
                            memnum = x;
                        }
                        else
                        {
                            bestnum = bestnum - bestbeecount * bestsitescount;
                            double x = (double)bestnum / (double)selectedbeecount;
                            memnum = x + bestsitescount;
                        }
                        membees[i] = membee(bestswarmbee, memnum);
                    }
                    for(int j = bestsitescount + selsitescount - 1; j > i + 1; j--)
                    {
                        membees[j] = membees[j - 1];
                    }
                    membees[i + 1] = reserve;
                }
            }
        }

        void sortmembees()
        {
            for(int i = 0; i < bestsitescount + selsitescount; i++)
            {
                membee best = membees[i];
                int bestnum = i;
                bool ind = false;
                for(int j = i+1; j < bestsitescount + selsitescount; j++)
                {
                    if(membees[j].fitness > best.fitness)
                    {
                        best = membees[j];
                        bestnum = j;
                        ind = true;
                    }
                }
                if (ind)
                {
                    membees[bestnum] = membees[i];
                    membees[i] = best;
                }
            }
        }

        void beesatwork()
        {
            for(int i = 0; i < bestsitescount + selsitescount; i++)//öèêë ïîèñêà äëÿ îòðÿäîâ
            {
                membee bestsbee = membees[i];
                doublebee bestsitebee = swarm[i];
                if(i < bestsitescount)//îòðÿäû ëó÷øèõ òî÷åê
                {
                    bool ind = false;
                    for(int j = scoutbeecount + i * bestbeecount; j < scoutbeecount + (i + 1) * bestbeecount; j++)
                    {
                        swarm[j].flyto(membees[i].position, membees[i].range);
                        if(swarm[j].fitness > bestsbee.fitness && swarm[j].fitness > bestsitebee.fitness)
                        {
                            bestsitebee = swarm[j];
                            ind = true;
                        }
                    }
                    if (ind)
                    {
                        membees[i] = membee(bestsitebee, membees[i].funccounter + 1);
                    }
                    else
                    {
                        membees[i].funccounter++;
                        if (membees[i].funccounter >= maxfunccounter)
                        {
                            membees[i].funccounter = 0;
                            for (int j = 0; j < varnum; j++)
                            {
                                membees[i].range[j] *= rangeshrinker;
                            }
                        }
                    }
                }
                else
                {
                    bool ind = false;
                    for(int j = scoutbeecount + bestsitescount * bestbeecount + (i - bestsitescount) * selectedbeecount; j < scoutbeecount + bestsitescount * bestbeecount + (i - bestsitescount + 1) * selectedbeecount; j++)
                    {
                        swarm[j].flyto(membees[i].position, membees[i].range);
                        if(swarm[j].fitness > bestsbee.fitness && swarm[j].fitness > bestsitebee.fitness)
                        {
                            bestsitebee = swarm[j];
                            ind = true;
                        }
                    }
                    if (ind)
                    {
                        membees[i] = membee(bestsitebee, membees[i].funccounter + 1);
                    }
                    else
                    {
                        membees[i].funccounter++;
                        if (membees[i].funccounter >= maxfunccounter)
                        {
                            membees[i].funccounter = 0;
                            for (int j = 0; j < varnum; j++)
                            {
                                membees[i].range[j] *= rangeshrinker;
                            }
                        }
                    }
                }
            }

            double center[varnum];
            double range[varnum];
            for(int i = 0; i < varnum; i++)
            {
                center[i] = (maxvalues[i] + minvalues[i]) / 2.0;
                range[i] = (maxvalues[i] - minvalues[i]) / 2.0;
            }
            for(int i = 0; i < scoutbeecount; i++)
            {
                swarm[i].flyto(center, range);
            }
        }

        void runcycle()
        {
            double bestfit = -100000.0;
            int stagnationcount = 0;
            int z = 0;
            for (z = 0; z < maxiter; z++)
            {
                beesatwork();
                sortswarm();
                sortmembees();
                if (membees[0].fitness > bestfit)
                {
                    bestfit = membees[0].fitness;
                    stagnationcount = 0;
                }
                else
                {
                    stagnationcount++;
                }
                if (stagnationcount >= maxstag)
                {
                    break;
                }
            }
        }
};

void numtests(int x)
{
    hive numhives[x];
    srand(time(NULL));
    for(int i = 0; i < x; i++)
    {
        numhives[i] = hive();
        numhives[i].sortswarm();
        numhives[i].runcycle();
    }
}

int main()
{
    numtests(50);
    return 0;
}
