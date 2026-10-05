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

//	FUNÇÃO PARA CALCULAR A MÉDIA DAS TEMPERATURAS

float media_temperatura (int dia, int mes, int ano, int hora, float temp, float umid, float pressao) {
	float media_temp;
	float soma_temp = 0.0;
	int quantidade = 0;

	ifstream entrada ("dados.txt");

	// WHILE: Enquanto houver dados no arquivo, ele vai somando as temperaturas e contando a quantidade de leituras

	while (entrada >> dia >> mes >> ano >> hora >> temp >> umid >> pressao) {
		soma_temp += temp;
		quantidade++;
	}

	// Calculando a média das temperaturas
	
	media_temp = soma_temp / quantidade;

	entrada.close();
	return media_temp;
}

// FUNÇÃO PARA CALCULAR A MÉDIA DAS UMIDADES

float media_umidade (int dia, int mes, int ano, int hora, float temp, float umid, float pressao) {
	float media_umid;
	float soma_umid = 0.0;
	int quantidade = 0;

	ifstream entrada ("dados.txt");

	// WHILE: Enquanto houver dados no arquivo, ele vai somando as umidades e contando a quantidade de leituras

	while (entrada >> dia >> mes >> ano >> hora >> temp >> umid >> pressao) {
		soma_umid += umid;
		quantidade++;
	}

	// Calculando a média das umidades
	
	media_umid = soma_umid / quantidade;

	entrada.close();
	return media_umid;
}

// FUNÇÃO PARA CALCULAR A MÉDIA DAS PRESSÕES

float media_pressao (int dia, int mes, int ano, int hora, float temp, float umid, float pressao) {
	float media_pressao;
	float soma_pressao = 0.0;
	int quantidade = 0;

	ifstream entrada ("dados.txt");

	// WHILE: Enquanto houver dados no arquivo, ele vai somando as pressões e contando a quantidade de leituras

	while (entrada >> dia >> mes >> ano >> hora >> temp >> umid >> pressao) {
		soma_pressao += pressao;
		quantidade++;
	}

	// Calculando a média das pressões
	
	media_pressao = soma_pressao / quantidade;

	entrada.close();
	return media_pressao;
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
			cout << "2. Exibir Media" << endl;
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
				cout << "===== MEDIA =====" << endl;
				cout << "Media Temperatura: " << media_temperatura(dia, mes, ano, hora, temp, umid, pressao) << " C" << endl;
				cout << "Media Umidade: " << media_umidade(dia, mes, ano, hora, temp, umid, pressao) << " %" << endl;
				cout << "Media Pressao: " << media_pressao(dia, mes, ano, hora, temp, umid, pressao) << " atm" << endl;
				cout << endl;
				break;

			case 3:
				cout << "===== LEITURAS =====" << endl;
				exibir_leituras(dia, mes, ano, hora, temp, umid, pressao);
				cout << endl;
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
