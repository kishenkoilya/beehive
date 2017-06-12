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

const int varnum = 2;//äëèíà ìàññèâîâ - êîëè÷åñòâî ïåðåìåííûõ
const int scoutbeecount = 300;
const int selectedbeecount = 10;
const int bestbeecount = 30;
const int selsitescount = 15;
const int bestsitescount = 5;
const int maxfunccounter = 1;
const double rangeshrinker = 0.5;
const int maxiter = 100;
const int maxstag = 50;
ofstream severaltests("beehive025.txt");
const int beecount = scoutbeecount + selectedbeecount * selsitescount + bestbeecount * bestsitescount;
const double minvalues[varnum] = {-5.10,-5.10};
const double maxvalues[varnum] = {5.10, 5.10};
ofstream fout("beetest.txt");

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
//                double f = (double)rand() / RAND_MAX;
//                return fMin + f * (fMax - fMin);
                position[i] = minval[i] + ((double)rand() / (RAND_MAX + 1.0)) * (maxval[i] - minval[i]);
//                position[i] = (double)(rand() % (int)((maxval[i] - minval[i]) * 1000.0 + 1.0)) / 1000.0 + minval[i];
//                fitness -= position[i] * position[i];//ìèíèìàëüíûé îáúåì ãèïåðñôåðû
                i++;
            }
//            fitness = - (pow(1.0 - position[0], 2.0) + 100 * pow(position[1] - pow(position[0], 2.0),2.0));//ðîçåíáðîê 2 ïåðåìåííûå
//            fitness = (1 / (1 + pow(position[0] - 2.0,2.0) + pow(position[1] - 10.0, 2.0))) +
//            (1 / (2 + pow(position[0] - 10.0,2.0) + pow(position[1] - 15.0, 2.0))) +
//            (1 / (1 + pow(position[0] - 18.0,2.0) + pow(position[1] - 4.0, 2.0)));//øåêåëü
//            fitness = - (pow(pow(position[0], 2.0) + position[1] - 11, 2.0) + pow(position[0] + pow(position[1], 2.0) - 7.0, 2.0));//Õèììåëüáëàó
            fitness = - (20.0 + pow(position[0], 2.0) + pow(position[1], 2.0) - 10 * (cos(2.0 * M_PI * position[0]) + cos(2.0 * M_PI * position[1])));//Ðàñòðèãèí

        }

        void flyto(double otherpos[varnum], double range_list[varnum])
        {
            fitness = 0;
            for (int i = 0; i < varnum; i++)
            {
//                double pos = otherpos[i];
//                double ran = range_list[i];
                minval[i] = otherpos[i] - range_list[i];
                if (minval[i] < minvalues[i]) minval[i] = minvalues[i];
                maxval[i] = otherpos[i] + range_list[i];
                if (maxval[i] > maxvalues[i]) maxval[i] = maxvalues[i];
//                position[i] = (double)(rand() % (int)((maxval[i] - minval[i]) * 1000.0 + 1.0)) / 1000.0 + minval[i];
                position[i] = minval[i] + ((double)rand() / (RAND_MAX + 1.0)) * (maxval[i] - minval[i]);

//                fitness -= position[i] * position[i];
            }
//            fitness = - (pow(1.0 - position[0], 2.0) + 100 * pow(position[1] - pow(position[0], 2.0),2.0));
//            fitness = (1 / (1 + pow(position[0] - 2.0,2.0) + pow(position[1] - 10.0, 2.0))) +
//            (1 / (2 + pow(position[0] - 10.0,2.0) + pow(position[1] - 15.0, 2.0))) +
//            (1 / (1 + pow(position[0] - 18.0,2.0) + pow(position[1] - 4.0, 2.0)));
//            fitness = - (pow(pow(position[0],2.0) + position[1] - 11, 2.0) + pow(position[0] + pow(position[1], 2.0) - 7.0, 2.0));
            fitness = - (20.0 + pow(position[0], 2.0) + pow(position[1], 2.0) - 10 * (cos(2.0 * M_PI * position[0]) + cos(2.0 * M_PI * position[1])));//Ðàñòðèãèí
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
//        SYSTEMTIME st;
        doublebee swarm[beecount];
        membee membees[bestsitescount + selsitescount];
        hive()
        {
//            GetLocalTime(&st);//Çàïèñü â ôàéë
//            fout << "Ñîçäàíèå óëüÿ. Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl << endl;

            for (int i = 0; i < beecount; i++)//ñîçäàíèå óëüÿ
            {
                swarm[i] = doublebee();
//                GetLocalTime(&st);//Çàïèñü â ôàéë
//                fout << "Ñîçäàíèå ï÷åëû ¹" << i << " Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
//                for (int j = 0; j < varnum; j++)
//                {
//                    fout << "Ïåðåìåííàÿ ¹" << j << "[" << swarm[i].minval[j] << "; " << swarm[i].maxval[j] << "]" << "Êîîðäèíàòà: " << swarm[i].position[j] << endl;
//                }
//                fout << "Çíà÷åíèå ôèòíåññ ôóíêöèè: " << swarm[i].fitness << endl;
            }
            for (int i = 0; i < bestsitescount + selsitescount; i++)
            {
                membees[i] = membee(swarm[i], 0);
            }
        }

        void sortswarm()
        {
//            GetLocalTime(&st);
//            fout << "Ñîðòèðîâêà óëüÿ íà÷àëàñü: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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

//                    GetLocalTime(&st);
//                    fout << "Âñòàâêà â ñòåê ï÷åë ïàìÿòè íà÷àëàñü: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                    for(int j = bestsitescount + selsitescount - 1; j > i + 1; j--)
                    {
                        membees[j] = membees[j - 1];
//                        GetLocalTime(&st);
//                        fout << "Çàìåíà " << j << " íîìåðà ñòåêà ïðåäûäóùèì. " << "Ôèòíåññ ôóíêöèÿ: " << membees[j].fitness << ". Êîîðäèíàòû: õ = " << membees[j].position[0] << "; y = " << membees[j].position[1] << "  min: x = " << membees[j].minval[0]
//                        << "; y = " << membees[j].minval[1] << "  max: x = " << membees[j].maxval[0] << "; y = " << membees[j].maxval[1]
//                        << " Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                    }
                    membees[i + 1] = reserve;
//                    GetLocalTime(&st);
//                    fout << "Çàìåíà " << i + 1 << " íîìåðà ñòåêà ñìåùåííîé ï÷åëîé. "
//                    << " Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                }
//                GetLocalTime(&st);
//                fout << "Ï÷åëà ¹" << i << " èìååò çíà÷åíèå ôèòíåññ ôóíêöèè: " << swarm[i].fitness << ". Êîîðäèíàòû: õ = " << swarm[i].position[0] << "; y = " << swarm[i].position[1] << "  min: x = " << swarm[i].minval[0]
//                << "; y = " << swarm[i].minval[1] << "  max: x = " << swarm[i].maxval[0] << "; y = " << swarm[i].maxval[1]
//                << " Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
            }
        }//

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

        void beesatwork()//äîáàâèòü çàïèñè â ôàéë!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
//            GetLocalTime(&st);
//            fout << "Íà÷àëàñü îñíîâíàÿ ðàáîòà àëãîðèòìà. " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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
//                        GetLocalTime(&st);
//                        fout << "Ïåðåìåùåíèå ï÷åëû " << j << ". " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                        if(swarm[j].fitness > bestsbee.fitness && swarm[j].fitness > bestsitebee.fitness)
                        {
                            bestsitebee = swarm[j];
                            ind = true;
                        }
                    }
                    if (ind)
                    {
//                        GetLocalTime(&st);
//                        fout << "Èçìåíåíèå ï÷åëû ïàìÿòè " << i << endl << "Ôèòíåñ ôóíêöèÿ áûëà:  " << membees[i].fitness << ". Êîîðäèíàòû: õ = " << membees[i].position[0] << "; y = " << membees[i].position[1] << "  min: x = " << membees[i].minval[0]
//                        << "; y = " << membees[i].minval[1] << "  max: x = " << membees[i].maxval[0] << "; y = " << membees[i].maxval[1] << endl << "Ôèòíåñ ôóíêöèÿ ñòàëà: " << bestsitebee.fitness << ". Êîîðäèíàòû: õ = " << bestsitebee.position[0] << "; y = " << bestsitebee.position[1] << "  min: x = " << bestsitebee.minval[0]
//                        << "; y = " << bestsitebee.minval[1] << "  max: x = " << bestsitebee.maxval[0] << "; y = " << bestsitebee.maxval[1]
//                        << ". Âðåìÿ: "  << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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
                else//îòðÿäû îòîáðàííûõ òî÷åê
                {
                    bool ind = false;
                    for(int j = scoutbeecount + bestsitescount * bestbeecount + (i - bestsitescount) * selectedbeecount; j < scoutbeecount + bestsitescount * bestbeecount + (i - bestsitescount + 1) * selectedbeecount; j++)
                    {
                        swarm[j].flyto(membees[i].position, membees[i].range);
//                        GetLocalTime(&st);
//                        fout << "Ïåðåìåùåíèå ï÷åëû " << j << ". " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                        if(swarm[j].fitness > bestsbee.fitness && swarm[j].fitness > bestsitebee.fitness)
                        {
                            bestsitebee = swarm[j];
                            ind = true;
                        }
                    }
                    if (ind)
                    {
//                        GetLocalTime(&st);
//                        fout << "Èçìåíåíèå ï÷åëû ïàìÿòè " << i << endl << "Ôèòíåñ ôóíêöèÿ áûëà:  " << membees[i].fitness << ". Êîîðäèíàòû: õ = " << membees[i].position[0] << "; y = " << membees[i].position[1] << "  min: x = " << membees[i].minval[0]
//                        << "; y = " << membees[i].minval[1] << "  max: x = " << membees[i].maxval[0] << "; y = " << membees[i].maxval[1] << endl << "Ôèòíåñ ôóíêöèÿ ñòàëà: " << bestsitebee.fitness << ". Êîîðäèíàòû: õ = " << bestsitebee.position[0] << "; y = " << bestsitebee.position[1] << "  min: x = " << bestsitebee.minval[0]
//                        << "; y = " << bestsitebee.minval[1] << "  max: x = " << bestsitebee.maxval[0] << "; y = " << bestsitebee.maxval[1]
//                        << ". Âðåìÿ: "  << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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

            double center[varnum];//ïåðåìåííûå äëÿ ðàçâåä÷èêîâ
            double range[varnum];
            for(int i = 0; i < varnum; i++)
            {
                center[i] = (maxvalues[i] + minvalues[i]) / 2.0;
                range[i] = (maxvalues[i] - minvalues[i]) / 2.0;
            }

//            GetLocalTime(&st);
//            fout << "Íà÷àëàñü ðàçâåäêà. " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
            for(int i = 0; i < scoutbeecount; i++)
            {
                swarm[i].flyto(center, range);
//                GetLocalTime(&st);
//                fout << "Ïåðåìåùåíèå ï÷åëû-ðàçâåä÷èêà " << i << ". Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
            }
//            GetLocalTime(&st);
//            fout << "Êîíåö èòåðàöèè. " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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
//                    GetLocalTime(&st);
//                    fout << "Àëãîðèòì äîñòèã ñòàãíàöèè íà " << z << "-îì öèêëå. Ôèòíåñ çíà÷åíèå = " << bestfit << ". Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                    break;
                }
            }
//            GetLocalTime(&st);
            for(int a = 0; a < bestsitescount + selsitescount; a++)
            {
//                GetLocalTime(&st);
//                fout << "Àëãîðèòì çàêîí÷èëñÿ. Ôèòíåñ çíà÷åíèå = " << bestfit << "Ôèòíåñ çíà÷åíèå = " << membees[a].fitness << ". Êîîðäèíàòû: x = " << membees[a].position[0] << "; y = " << membees[a].position[1] << "  range: x = " << membees[a].range[0] << "; y = " << membees[a].range[1] << "  min: x = " << membees[a].minval[0]
//                << "; y = " << membees[a].minval[1] << "  max: x = " << membees[a].maxval[0] << "; y = " << membees[a].maxval[1] << ". Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
//                cout << "Algorithm finished. Fitness best = " << bestfit << ". Fitness = " << membees[a].fitness << ". Coordinates: x = " << membees[a].position[0] << "; y = " << membees[a].position[1] << "  range: x = " << membees[a].range[0] << "; y = " << membees[a].range[1] << "  min: x = " << membees[a].minval[0]
//                << "; y = " << membees[a].minval[1] << "  max: x = " << membees[a].maxval[0] << "; y = " << membees[a].maxval[1] << ". Time: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                severaltests << z << "\t" << membees[a].fitness << "\t" << membees[a].position[0] << "\t" << membees[a].position[1] << endl;
            }
            severaltests << endl;
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
//        Sleep(500);
    }
}

int main()
{
//    SYSTEMTIME st;

//    GetLocalTime(&st);
    severaltests << "scoutbeecount\tselectedbeecount\tbestbeecount\tselsitescount\tbestsitescount\tmaxfunccounter\trangeshrinker\tmaxiter\tmaxstag" << endl;
    severaltests << scoutbeecount << "\t" << selectedbeecount << "\t" << bestbeecount << "\t" << selsitescount << "\t" << bestsitescount << "\t" << maxfunccounter << "\t" << rangeshrinker << "\t" << maxiter << "\t" << maxstag << endl << endl;
    severaltests << "Iterations\tFitness\tCoord X\tCoord Y" << endl;

//    fout << "Íà÷àëî àëãîðèòìà" << ". Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl << endl;
    numtests(50);

//    GetLocalTime(&st);
//    fout << "Êîíåö àëãîðèòìà" << ". Âðåìÿ: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
    return 0;
}
