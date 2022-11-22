// Markus Buchholz
// https://en.wikiversity.org/wiki/Algorithm_models/Grey_Wolf_Optimizer
#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <math.h>
#include <map>
#include <algorithm>

float x1min = -5.0;
float x1max = 5.0;
float x2min = -5.0;
float x2max = 5.0;
int PACK = 5;
int EVOLUTIONS = 500;

//--------------------------------------------------------------------------------

struct PosXY
{

    float x;
    float y;
};

//--------------------------------------------------------------------------------

float generateRandom()
{

    std::random_device engine;
    std::uniform_real_distribution<float> distrib(0.0, 1.0);
    return distrib(engine);
}

//--------------------------------------------------------------------------------

float valueGenerator(float low, float high)
{

    return low + generateRandom() * (high - low);
}

//--------------------------------------------------------------------------------

std::vector<PosXY> initPosXY()
{

    std::vector<PosXY> pos;

    for (int ii = 0; ii < PACK; ii++)
    {

        pos.push_back({valueGenerator(x1min, x1max), valueGenerator(x2min, x2max)});
    }

    return pos;
}

//--------------------------------------------------------------------------------

std::vector<float> function(std::vector<PosXY> pos)
{
    std::vector<float> funcValue;

    for (auto &ii : pos)
    {

        //funcValue.push_back(ii.x * ii.x - ii.x * ii.y + ii.y * ii.y + 2 * ii.x + 4 * ii.y + 3);
        funcValue.push_back(ii.x * ii.y);
    }

    return funcValue;
}

//--------------------------------------------------------------------------------

float func(PosXY pos)
{

    //return (pos.x * pos.x - pos.x * pos.y + pos.y * pos.y + 2 * pos.x + 4 * pos.y + 3);
    return pos.x * pos.y;
}

//--------------------------------------------------------------------------------

bool compare(std::pair<PosXY, float> a, std::pair<PosXY, float> b)
{

    return a.second < b.second;
}
//--------------------------------------------------------------------------------

bool compareX(std::pair<PosXY, float> a, std::pair<PosXY, float> b)
{

    return a.second > b.second;
}


//--------------------------------------------------------------------------------
// min
std::vector<std::pair<PosXY, float>> abdBestMin(std::vector<PosXY> pos, std::vector<float> func)
{

    std::vector<std::pair<PosXY, float>> tempCont;
    std::vector<std::pair<PosXY, float>> abdBest;

    std::multimap<PosXY, float> bestMap;

    for (int ii = 0; ii < pos.size(); ii++)
    {

        tempCont.push_back(std::pair<PosXY, float>(pos[ii], func[ii]));
    }

    std::sort(tempCont.begin(), tempCont.end(), compare);

    for (int ii = 0; ii < 3; ii++)
    {

        abdBest.push_back(std::pair<PosXY, float>(tempCont[ii].first, tempCont[ii].second));
    }


    return abdBest;
}
//--------------------------------------------------------------------------------
// max
std::vector<std::pair<PosXY, float>> abdBest(std::vector<PosXY> pos, std::vector<float> func)
{

    std::vector<std::pair<PosXY, float>> tempCont;
    std::vector<std::pair<PosXY, float>> abdBest;

    std::multimap<PosXY, float> bestMap;

    for (int ii = 0; ii < pos.size(); ii++)
    {

        tempCont.push_back(std::pair<PosXY, float>(pos[ii], func[ii]));
    }

    std::sort(tempCont.begin(), tempCont.end(), compareX);

    for (int ii = 0; ii < 3; ii++)
    {

        abdBest.push_back(std::pair<PosXY, float>(tempCont[ii].first, tempCont[ii].second));
    }


    return abdBest;
}
//--------------------------------------------------------------------------------




PosXY computeXnew(float a, std::vector<PosXY> pos, PosXY wolf_actual)
{

    // X1
    float A1 = 2.0 * a * generateRandom() - a;
    float C1 = 2.0 * generateRandom();
    PosXY D1 = {std::abs(C1 * pos[0].x - wolf_actual.x), std::abs(C1 * pos[0].y - wolf_actual.y)};
    PosXY X1 = {pos[0].x - A1 * D1.x, pos[0].y - A1 * D1.y};

    // X2
    float A2 = 2.0 * a * generateRandom() - a;
    float C2 = 2.0 * generateRandom();
    PosXY D2 = {std::abs(C2 * pos[1].x - wolf_actual.x), std::abs(C2 * pos[1].y - wolf_actual.y)};
    PosXY X2 = {pos[1].x - A2 * D2.x, pos[1].y - A2 * D2.y};

    // X3
    float A3 = 2.0 * a * generateRandom() - a;
    float C3 = 2.0 * generateRandom();
    PosXY D3 = {std::abs(C3 * pos[2].x - wolf_actual.x), std::abs(C3 * pos[2].y - wolf_actual.y)};
    PosXY X3 = {pos[2].x - A3 * D3.x, pos[2].y - A3 * D3.y};

    PosXY Xnew = {(X1.x + X2.x + X3.x) / 3, (X1.y + X2.y + X3.y) / 3};

    if (Xnew.x < x1min)
    {
        Xnew.x = x1min;
    }

    if (Xnew.x > x1max)
    {
        Xnew.x = x1max;
    }

    if (Xnew.y < x2min)
    {
        Xnew.y = x2min;
    }

    if (Xnew.y > x2max)
    {
        Xnew.y = x2max;
    }

    return Xnew;
}

//--------------------------------------------------------------------------------

void runGWO()
{
    float EVO = (float)EVOLUTIONS;
    std::vector<PosXY> initPos = initPosXY();

    std::vector<float> funcValue = function(initPos);

    for (int ii = 1; ii < EVOLUTIONS; ii++)
    {

        std::vector<std::pair<PosXY, float>> abd = abdBest(initPos, funcValue);


        std::vector<PosXY> bestABD;

        for (auto &ii : abd)
        {

            bestABD.push_back(ii.first);
        }

        float a = 2.0 * (1.0 - ii / EVO);

        for (int jj = 0; jj < PACK; jj++)
        {

            PosXY Xnew = computeXnew(a, bestABD, initPos[jj]);
            float functValue_new = func(Xnew);
            if (functValue_new > funcValue[jj])
            {
                initPos[jj] = Xnew;
                funcValue[jj] = functValue_new;
            }
        }

          std::cout << "---------------------------" << std::endl;
                

          std::vector<std::pair<PosXY, float>> minIter = abdBest(initPos, funcValue);

          std::cout << "function : " << minIter[0].second << " , x: "  << minIter[0].first.x << " ,y : " <<minIter[0].first.y << "\n";
    }
}
//--------------------------------------------------------------------------------

int main()
{

    runGWO();
  
}
