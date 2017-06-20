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
	list<string> l = {  "palabrasCaso1.0", "palabrasCaso1.1", "palabrasCaso1.2", "palabrasCaso1.3",
 						"palabrasCaso1.4", "palabrasCaso1.5", "palabrasCaso1.6", "palabrasCaso1.7",
 						"palabrasCaso1.8", "palabrasCaso1.9", "palabrasCaso1.10", "palabrasCaso1.11",
 						"palabrasCaso1.12", "palabrasCaso1.13", "palabrasCaso1.14", "palabrasCaso1.15", 
 						"palabrasCaso1.16", "palabrasCaso1.17", "palabrasCaso1.18", "palabrasCaso1.19",
 						"palabrasCaso1.20", "palabrasCaso1.21", "palabrasCaso1.22", "palabrasCaso1.23",
 					 	"palabrasCaso1.24", "palabrasCaso1.25",};

	if (argc != 3) {
		cerr << "uso: " << argv[0] << " #tarchivos #tmaximum" << endl;
		return 1;
	}

	vector<double> tiempos(50);

	for (int i = 0; i < 50; ++i){
		init_time();
		p = ConcurrentHashMap::maximum(atoi(argv[1]), atoi(argv[2]), l);
		tiempos[i] = get_time();
	}

  	ofstream salida("resultadoCaso1",std::ofstream::out);
  	salida<<"Tiempos de ejecucion del caso 1 (50 corridas) - No concurrente: "<<endl;
  	for (int i = 0; i < 50; ++i){
 		salida<<tiempos[i]<<endl;
  	}


	double media_caso_1;
  	int size = tiempos.size();
  	sort(tiempos.begin(), tiempos.end());
  	media_caso_1 = (tiempos[size / 2 - 1] + tiempos[size / 2]) / 2;

  	salida<<"La media fue de: "<<media_caso_1<<endl;

  	salida<<endl;

  	vector<double> tiempos2(50);

	for (int i = 0; i < 50; ++i){
		init_time();
		p = ConcurrentHashMap::maximum_ej6(atoi(argv[1]), atoi(argv[2]), l);
		tiempos2[i] = get_time();
	}

  	salida<<"Tiempos de ejecucion del caso 1 (50 corridas) - Concurrente: "<<endl;
  	for (int i = 0; i < 50; ++i){
 		salida<<tiempos2[i]<<endl;
  	}


	double media_caso_2;
  	int size2 = tiempos2.size();
  	sort(tiempos2.begin(), tiempos2.end());
  	media_caso_2 = (tiempos2[size2 / 2 - 1] + tiempos2[size2 / 2]) / 2;

  	salida<<"La media fue de: "<<media_caso_2<<endl;

	return 0;
}

