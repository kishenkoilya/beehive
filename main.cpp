#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>
//#include <windows.h>
#include <stdio.h>
#include <math.h>
using namespace std;

const int varnum = 2;//длина массивов - количество переменных
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
//                fitness -= position[i] * position[i];//минимальный объем гиперсферы
                i++;
            }
//            fitness = - (pow(1.0 - position[0], 2.0) + 100 * pow(position[1] - pow(position[0], 2.0),2.0));//розенброк 2 переменные
//            fitness = (1 / (1 + pow(position[0] - 2.0,2.0) + pow(position[1] - 10.0, 2.0))) +
//            (1 / (2 + pow(position[0] - 10.0,2.0) + pow(position[1] - 15.0, 2.0))) +
//            (1 / (1 + pow(position[0] - 18.0,2.0) + pow(position[1] - 4.0, 2.0)));//шекель
//            fitness = - (pow(pow(position[0], 2.0) + position[1] - 11, 2.0) + pow(position[0] + pow(position[1], 2.0) - 7.0, 2.0));//Химмельблау
            fitness = - (20.0 + pow(position[0], 2.0) + pow(position[1], 2.0) - 10 * (cos(2.0 * M_PI * position[0]) + cos(2.0 * M_PI * position[1])));//Растригин

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
            fitness = - (20.0 + pow(position[0], 2.0) + pow(position[1], 2.0) - 10 * (cos(2.0 * M_PI * position[0]) + cos(2.0 * M_PI * position[1])));//Растригин
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
//            GetLocalTime(&st);//Запись в файл
//            fout << "Создание улья. Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl << endl;

            for (int i = 0; i < beecount; i++)//создание улья
            {
                swarm[i] = doublebee();
//                GetLocalTime(&st);//Запись в файл
//                fout << "Создание пчелы №" << i << " Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
//                for (int j = 0; j < varnum; j++)
//                {
//                    fout << "Переменная №" << j << "[" << swarm[i].minval[j] << "; " << swarm[i].maxval[j] << "]" << "Координата: " << swarm[i].position[j] << endl;
//                }
//                fout << "Значение фитнесс функции: " << swarm[i].fitness << endl;
            }
            for (int i = 0; i < bestsitescount + selsitescount; i++)
            {
                membees[i] = membee(swarm[i], 0);
            }
        }

        void sortswarm()
        {
//            GetLocalTime(&st);
//            fout << "Сортировка улья началась: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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
//                    fout << "Вставка в стек пчел памяти началась: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                    for(int j = bestsitescount + selsitescount - 1; j > i + 1; j--)
                    {
                        membees[j] = membees[j - 1];
//                        GetLocalTime(&st);
//                        fout << "Замена " << j << " номера стека предыдущим. " << "Фитнесс функция: " << membees[j].fitness << ". Координаты: х = " << membees[j].position[0] << "; y = " << membees[j].position[1] << "  min: x = " << membees[j].minval[0]
//                        << "; y = " << membees[j].minval[1] << "  max: x = " << membees[j].maxval[0] << "; y = " << membees[j].maxval[1]
//                        << " Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                    }
                    membees[i + 1] = reserve;
//                    GetLocalTime(&st);
//                    fout << "Замена " << i + 1 << " номера стека смещенной пчелой. "
//                    << " Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                }
//                GetLocalTime(&st);
//                fout << "Пчела №" << i << " имеет значение фитнесс функции: " << swarm[i].fitness << ". Координаты: х = " << swarm[i].position[0] << "; y = " << swarm[i].position[1] << "  min: x = " << swarm[i].minval[0]
//                << "; y = " << swarm[i].minval[1] << "  max: x = " << swarm[i].maxval[0] << "; y = " << swarm[i].maxval[1]
//                << " Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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

        void beesatwork()//добавить записи в файл!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
//            GetLocalTime(&st);
//            fout << "Началась основная работа алгоритма. " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
            for(int i = 0; i < bestsitescount + selsitescount; i++)//цикл поиска для отрядов
            {
                membee bestsbee = membees[i];
                doublebee bestsitebee = swarm[i];
                if(i < bestsitescount)//отряды лучших точек
                {
                    bool ind = false;
                    for(int j = scoutbeecount + i * bestbeecount; j < scoutbeecount + (i + 1) * bestbeecount; j++)
                    {
                        swarm[j].flyto(membees[i].position, membees[i].range);
//                        GetLocalTime(&st);
//                        fout << "Перемещение пчелы " << j << ". " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                        if(swarm[j].fitness > bestsbee.fitness && swarm[j].fitness > bestsitebee.fitness)
                        {
                            bestsitebee = swarm[j];
                            ind = true;
                        }
                    }
                    if (ind)
                    {
//                        GetLocalTime(&st);
//                        fout << "Изменение пчелы памяти " << i << endl << "Фитнес функция была:  " << membees[i].fitness << ". Координаты: х = " << membees[i].position[0] << "; y = " << membees[i].position[1] << "  min: x = " << membees[i].minval[0]
//                        << "; y = " << membees[i].minval[1] << "  max: x = " << membees[i].maxval[0] << "; y = " << membees[i].maxval[1] << endl << "Фитнес функция стала: " << bestsitebee.fitness << ". Координаты: х = " << bestsitebee.position[0] << "; y = " << bestsitebee.position[1] << "  min: x = " << bestsitebee.minval[0]
//                        << "; y = " << bestsitebee.minval[1] << "  max: x = " << bestsitebee.maxval[0] << "; y = " << bestsitebee.maxval[1]
//                        << ". Время: "  << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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
                else//отряды отобранных точек
                {
                    bool ind = false;
                    for(int j = scoutbeecount + bestsitescount * bestbeecount + (i - bestsitescount) * selectedbeecount; j < scoutbeecount + bestsitescount * bestbeecount + (i - bestsitescount + 1) * selectedbeecount; j++)
                    {
                        swarm[j].flyto(membees[i].position, membees[i].range);
//                        GetLocalTime(&st);
//                        fout << "Перемещение пчелы " << j << ". " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                        if(swarm[j].fitness > bestsbee.fitness && swarm[j].fitness > bestsitebee.fitness)
                        {
                            bestsitebee = swarm[j];
                            ind = true;
                        }
                    }
                    if (ind)
                    {
//                        GetLocalTime(&st);
//                        fout << "Изменение пчелы памяти " << i << endl << "Фитнес функция была:  " << membees[i].fitness << ". Координаты: х = " << membees[i].position[0] << "; y = " << membees[i].position[1] << "  min: x = " << membees[i].minval[0]
//                        << "; y = " << membees[i].minval[1] << "  max: x = " << membees[i].maxval[0] << "; y = " << membees[i].maxval[1] << endl << "Фитнес функция стала: " << bestsitebee.fitness << ". Координаты: х = " << bestsitebee.position[0] << "; y = " << bestsitebee.position[1] << "  min: x = " << bestsitebee.minval[0]
//                        << "; y = " << bestsitebee.minval[1] << "  max: x = " << bestsitebee.maxval[0] << "; y = " << bestsitebee.maxval[1]
//                        << ". Время: "  << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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

            double center[varnum];//переменные для разведчиков
            double range[varnum];
            for(int i = 0; i < varnum; i++)
            {
                center[i] = (maxvalues[i] + minvalues[i]) / 2.0;
                range[i] = (maxvalues[i] - minvalues[i]) / 2.0;
            }

//            GetLocalTime(&st);
//            fout << "Началась разведка. " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
            for(int i = 0; i < scoutbeecount; i++)
            {
                swarm[i].flyto(center, range);
//                GetLocalTime(&st);
//                fout << "Перемещение пчелы-разведчика " << i << ". Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
            }
//            GetLocalTime(&st);
//            fout << "Конец итерации. " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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
//                    fout << "Алгоритм достиг стагнации на " << z << "-ом цикле. Фитнес значение = " << bestfit << ". Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
                    break;
                }
            }
//            GetLocalTime(&st);
            for(int a = 0; a < bestsitescount + selsitescount; a++)
            {
//                GetLocalTime(&st);
//                fout << "Алгоритм закончился. Фитнес значение = " << bestfit << "Фитнес значение = " << membees[a].fitness << ". Координаты: x = " << membees[a].position[0] << "; y = " << membees[a].position[1] << "  range: x = " << membees[a].range[0] << "; y = " << membees[a].range[1] << "  min: x = " << membees[a].minval[0]
//                << "; y = " << membees[a].minval[1] << "  max: x = " << membees[a].maxval[0] << "; y = " << membees[a].maxval[1] << ". Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
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

//    fout << "Начало алгоритма" << ". Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl << endl;
    numtests(50);

//    GetLocalTime(&st);
//    fout << "Конец алгоритма" << ". Время: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << "  " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << st.wMilliseconds << endl;
    return 0;
}
