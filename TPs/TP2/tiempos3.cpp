#include <iostream>
#include <cstdlib>
#include "ConcurrentHashMap.h"
#include <sys/time.h>
#include <vector>
#include <algorithm>


using namespace std;

timeval start, endTime;

void init_time(){

      gettimeofday(&start,NULL);

}


double get_time(){
    
    gettimeofday(&endTime,NULL);
    return (1000000*(endTime.tv_sec-start.tv_sec)+(endTime.tv_usec-start.tv_usec))/1000000.0;
}


int main(int argc, char **argv) {
	pair<string, unsigned int> p;

	if (argc != 3) {
		cerr << "uso: " << argv[0] << " #tarchivos #tmaximum" << endl;
		return 1;
	}


  	list<string> l3 = {  "palabrasCaso3.0", "palabrasCaso3.1", "palabrasCaso3.2", "palabrasCaso3.3",
 						"palabrasCaso3.4"};
	

	vector<double> tiempos5(50);

	for (int i = 0; i < 50; ++i){
		init_time();
		p = ConcurrentHashMap::maximum(atoi(argv[1]), atoi(argv[2]), l3);
		tiempos5[i] = get_time();
	}

  	ofstream salida3("resultadoCaso3",std::ofstream::out);
  	salida3<<"Tiempos de ejecucion del caso 3 (50 corridas) - No concurrente: "<<endl;
  	for (int i = 0; i < 50; ++i){
 		salida3<<tiempos5[i]<<endl;
  	}


	double media_caso_5;
  	int size5 = tiempos5.size();
  	sort(tiempos5.begin(), tiempos5.end());
  	media_caso_5 = (tiempos5[size5 / 2 - 1] + tiempos5[size5 / 2]) / 2;

  	salida3<<"La media fue de: "<<media_caso_5<<endl;

  	salida3<<endl;

  	vector<double> tiempos6(50);

	for (int i = 0; i < 50; ++i){
		init_time();
		p = ConcurrentHashMap::maximum_ej6(atoi(argv[1]), atoi(argv[2]), l3);
		tiempos6[i] = get_time();
	}

  	salida3<<"Tiempos de ejecucion del caso 3 (50 corridas) - Concurrente: "<<endl;
  	for (int i = 0; i < 50; ++i){
 		salida3<<tiempos6[i]<<endl;
  	}


	double media_caso_6;
  	int size6 = tiempos6.size();
  	sort(tiempos6.begin(), tiempos6.end());
  	media_caso_6 = (tiempos6[size6 / 2 - 1] + tiempos6[size6 / 2]) / 2;

  	salida3<<"La media fue de: "<<media_caso_6<<endl;



	return 0;
}

