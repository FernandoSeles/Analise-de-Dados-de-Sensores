#include <iostream>
#include <fstream>

using namespace std;

void exibir_leituras() {
	ifstream entrada ("dados.txt");
	
	int dia, mes, ano, hora;
	float temp, umid, pressao;
	
	while (entrada >> dia >> mes >> ano >> hora >> temp >> umid >> pressao) {
		cout << "Data: " << dia << "/" << mes << "/" << ano << endl;
		cout << "Hora: " << hora << "h" << endl;
		cout << "Temperatura: " << temp << " °C" << endl;
		cout << "Umidade: " << umid << "%" << endl;
		cout << "Pressao: " << pressao << " atm" << endl;
	}
	
}
	
int main() {
	exibir_leituras();
	
	return 0;
}
