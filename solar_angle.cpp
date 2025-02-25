#include<iostream>
#include<fstream>
#include <string>
#include <sstream>
#include <cmath>

int Day_of_Year(const int month, const int day){
  constexpr int Days_of_Month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  int doy { -1 };

  if ( (month >= 1) && (month <= 12) && (day >= 1) && (day <= Days_of_Month[month-1]) ){
    doy = 0;
    for(int m = 0; m < month-1; m++){
      doy += Days_of_Month[m];
    }

    doy += day;
  }

  return doy;
}

int main(int argc, char **argv){

  constexpr double DEG2RAD { M_PI / 180.0 };
  constexpr double RAD2DEG { 1.0 / DEG2RAD };

  std::string str_month(argv[1]);
  std::string str_day(argv[2]);
  std::string str_hour(argv[3]);
  std::string str_lat(argv[4]);
  std::string str_lon(argv[5]);
  int month = std::stoi(str_month);
  int day = std::stoi(str_day);
  double Ts = std::stod(str_hour) + 9.0;//時刻[h]（中央標準時）
  double phi = std::stod(str_lat) * DEG2RAD;//北緯
  double th = std::stod(str_lon) * DEG2RAD;//東経

  //1月1日からの通算日を計算
  double J = Day_of_Year(month, day) + 0.5;

  double w = 2.0 * M_PI / 365.0;
  double delta = (0.33281 - 22.984*cos(w*J) - 0.34990*cos(2*w*J) - 0.13980*cos(3*w*J)
                  + 3.7872*sin(w*J) + 0.03250*sin(2*w*J) + 0.07187*sin(3*w*J)) * DEG2RAD; /* 太陽赤緯[rad] */

  double e = 0.0072*cos(w*J) - 0.0528*cos(2*w*J) - 0.0012*cos(3*w*J)
    - 0.1229*sin(w*J) - 0.1565*sin(2*w*J) - 0.0041*sin(3*w*J); /* 均時差[h] */

  double T = Ts + (th *RAD2DEG - 135.0)/15.0 + e;
  double t = (15.0*T - 180.0) *DEG2RAD; /* 時角[rad] */

  double h = asin(sin(phi)*sin(delta) + cos(phi)*cos(delta)*cos(t)); /* 太陽高度角[rad] */
  double th_v = M_PI - h;//太陽天頂角[rad]


  double sinA = cos(delta) * sin(t) / cos(h);
  double cosA = (sin(h)*sin(phi) - sin(delta)) / (cos(h)*cos(phi));
  double A = atan2(sinA,cosA) + M_PI;//方位角、北=0、東=90、南=180、西=270

  std::cout << "h[deg]=" << h*180.0/M_PI << std::endl;
  std::cout << "A[deg]=" << A*180.0/M_PI << std::endl;
  std::cout << "h[rad]=" << h << std::endl;
  std::cout << "A[rad]=" << A << std::endl;

  return 0;
}
