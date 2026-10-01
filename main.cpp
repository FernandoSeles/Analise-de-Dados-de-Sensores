#include <iostream>
#include <fstream>

using namespace std;

void inserir_leituras (int dia, int mes, int ano, int hora, float temp, float umid, float pressao) {

	ofstream saida ("../dados.txt", ios::app);	/* ../ faz o programa procurar dados.txt na pasta principal e não em output, ou seja, volta uma pasta */
												/* ios::app acrescenta as leituras no final sem apagar as leituras anteriores */
	saida << endl << endl;
	saida << dia << " " << mes << " " << ano << endl << hora;
	saida << endl << temp << endl << umid << endl << pressao;

	saida.close();

}

void exibir_leituras (int dia, int mes, int ano, int hora, float temp, float umid, float pressao) {
	
	ifstream entrada ("dados.txt");

	while (entrada >> dia >> mes >> ano >> hora >> temp >> umid >> pressao)
	{
		cout << "Data: " << dia << "/" << mes << "/" << ano << endl;
		cout << "Hora: " << hora << "h" << endl;
		cout << "Temperatura: " << temp << " °C" << endl;
		cout << "Umidade: " << umid << "%" << endl;
		cout << "Pressao: " << pressao << " atm" << endl;
	}

	entrada.close();
}

int main()
{
	int opcao, controle = 1;
	int dia, mes, ano, hora;
	float temp, umid, pressao;

	while (controle)
	{
		cout << "===== SEJA BEM VINDO =====" << endl;
		cout << "Caso queira ter acesso ao nosso menu, digite 1." << endl;
		cout << "Se deseja encerrar o programa, digite 0." << endl;
		cout << "Digite sua opcao: ";
		cin >> controle;

		if (controle)
		{
			cout << endl << "===== MENU PRINCIPAL =====" << endl;
			cout << "1. Inserir Leitura" << endl;
			cout << "2. Calcular Media" << endl;
			cout << "3. Exibir Leituras" << endl;
			cout << "4. Sair" << endl;
			cin >> opcao;

			switch (opcao)
			{
			case 1:

				cout << endl << "1. Data (dia mes ano)" << endl;
				cout << "2. Hora (hora)" << endl << "3. Temperatura (graus Celsius)" << endl;
				cout << "4. Umidade relativa do ar (porcentagem)" << endl << "5. Pressao (atm)" << endl;
				cout << "Informe os dados nessa ordem: ";
				
				cin >> dia >> mes >> ano >> hora >> temp >> umid >> pressao;

				inserir_leituras (dia, mes, ano, hora, temp, umid, pressao);

				cout << endl;
				break;

			case 2:
				cout << "A media calculada e:" << endl;
				break;

			case 3:
				cout << "As leituras sao:" << endl;
				exibir_leituras(dia, mes, ano, hora, temp, umid, pressao);
				break;

			case 4:
				cout << "Saida com sucesso" << endl;
				break;

			default:
				cout << "Opcao invalida!" << endl;
			}
		}
	}

	return 0;
}
