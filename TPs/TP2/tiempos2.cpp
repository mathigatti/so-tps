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

 	list<string> l2 = {  "palabrasCaso2.0", "palabrasCaso2.1", "palabrasCaso2.2", "palabrasCaso2.3",
 						"palabrasCaso2.4", "palabrasCaso2.5", "palabrasCaso2.6", "palabrasCaso2.7",
 						"palabrasCaso2.8", "palabrasCaso2.9", "palabrasCaso2.10", "palabrasCaso2.11",
 						"palabrasCaso2.12", "palabrasCaso2.13", "palabrasCaso2.14", "palabrasCaso2.15", 
 						"palabrasCaso2.16", "palabrasCaso2.17", "palabrasCaso2.18", "palabrasCaso2.19",
 						"palabrasCaso2.20", "palabrasCaso2.21", "palabrasCaso2.22", "palabrasCaso2.23",
 					 	"palabrasCaso2.24", "palabrasCaso2.25",};
	

	vector<double> tiempos3(10);

	for (int i = 0; i < 10; ++i){
		init_time();
		p = ConcurrentHashMap::maximum(atoi(argv[1]), atoi(argv[2]), l2);
		tiempos3[i] = get_time();
		cout<<i<<endl;
	}

  	ofstream salida2("resultadoCaso2",std::ofstream::out);
  	salida2<<"Tiempos de ejecucion del caso 2 (10 corridas) - No concurrente: "<<endl;
  	for (int i = 0; i < 10; ++i){
 		salida2<<tiempos3[i]<<endl;
  	}


	double media_caso_3;
  	int size3 = tiempos3.size();
  	sort(tiempos3.begin(), tiempos3.end());
  	media_caso_3 = (tiempos3[size3 / 2 - 1] + tiempos3[size3 / 2]) / 2;

  	salida2<<"La media fue de: "<<media_caso_3<<endl;

  	salida2<<endl;

  	vector<double> tiempos4(10);

	for (int i = 0; i < 10; ++i){
		init_time();
		p = ConcurrentHashMap::maximum_ej6(atoi(argv[1]), atoi(argv[2]), l2);
		tiempos4[i] = get_time();
		cout<<i<<endl;
	}

  	salida2<<"Tiempos de ejecucion del caso 2 (10 corridas) - Concurrente: "<<endl;
  	for (int i = 0; i < 10; ++i){
 		salida2<<tiempos4[i]<<endl;
  	}


	double media_caso_4;
  	int size4 = tiempos4.size();
  	sort(tiempos4.begin(), tiempos4.end());
  	media_caso_4 = (tiempos4[size4 / 2 - 1] + tiempos4[size4 / 2]) / 2;

  	salida2<<"La media fue de: "<<media_caso_4<<endl;


	return 0;
}

