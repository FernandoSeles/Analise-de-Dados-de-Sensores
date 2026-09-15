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
	int opcao;
	
	do{
	cout << "===== MENU PRINCIPAL =====" << endl;
	cout << "1. Inserir Leitura" << endl;
	cout << "2. Calcular Media" << endl;
	cout << "3. Exibir Leituras" << endl;
	cout << "4. Sair" << endl;
	cin >> opcao;
	
	switch(opcao){
		case 1:
			cout << "Insira a Leitura:" << endl;
			break;
		
		case 2: {
			cout << "A media calculada e:" << endl;
			break;
}		
		case 3:
			cout << "As leituras sao:" << endl;
			exibir_leituras();
			break;
		
		case 4:
			cout << "Saida com sucesso" << endl;
			break;
		
		default:
			cout << "Opcao invalida!" << endl;
			
		}
}	while(opcao !=4);
	
	return 0;
}
