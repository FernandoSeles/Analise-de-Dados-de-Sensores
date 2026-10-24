#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

bool ler_dataHora (istream& in, tm& dataHora){ /* istream é um tipo que representa um fluxo de entrada que admite diferentes fontes.
	Tanto de teclado, quanto de arquivos */
	int mes;

	dataHora = tm{}; // Essa linha inicializa todos os campos numéricos da struct tm com zero.
	if (!(in >> dataHora.tm_mday >> mes >>  dataHora.tm_year >> dataHora.tm_hour)) return false;
	// Verifica se a leitura não foi bem-sucedida. Se falhar, a função executa return false; e termina imediatamente.
	dataHora.tm_mon = mes - 1;
	dataHora.tm_year = dataHora.tm_year - 1900;
	dataHora.tm_isdst = -1; /* tm_isdt = -1 faz com que a informação de horário de verão fique desconhecida. Faz com que a biblioteca 
	determine essa informação em futuras conevrsões de data com esse campo*/
	return true;
}

string formatar_data (tm dataHora) {
	
	char texto[50];
	strftime(texto, 50, "%d/%m/%Y %Hh", &dataHora);
	return texto;

}

bool ler_leitura (istream& in, tm& dataHora, float& temp, float& umid, float& pressao) {
	
	string nome;

	return (in >> ws && getline(in, nome) && ler_dataHora(in, dataHora) && in >> temp >> umid >> pressao);
	
}

void inserir_leituras (string nomeTeste, tm dataHora, float temp, float umid, float pressao) {

	ofstream saida ("../dados.txt", ios::app);	/* ../ faz o programa procurar dados.txt na pasta principal e não em output, ou seja, volta uma pasta */
												/* ios::app acrescenta as leituras no final sem apagar as leituras anteriores */
	saida << endl << endl;
	saida << nomeTeste << endl << dataHora.tm_mday << " " << dataHora.tm_mon + 1 << " " << dataHora.tm_year + 1900 << endl << dataHora.tm_hour;	
	saida << endl << temp << endl << umid << endl << pressao;

	saida.close();

}

void exibir_leituras (tm dataHora, float temp, float umid, float pressao) {
	
	ifstream entrada ("../dados.txt");

		while (ler_leitura(entrada, dataHora, temp, umid, pressao))	{
		cout << "Data: " << formatar_data(dataHora) << endl;
		cout << "Temperatura: " << temp << " graus Celsius" << endl;
		cout << "Umidade: " << umid << " %" << endl;
		cout << "Pressao: " << pressao << " atm" << endl << endl;
	}

	entrada.close();
}

//	FUNÇÃO PARA CALCULAR A MÉDIA DAS TEMPERATURAS

float media_temperatura (tm dataHora, float temp, float umid, float pressao) {
	float media_temp;
	float soma_temp = 0.0;
	int quantidade = 0;

	ifstream entrada ("../dados.txt");

	// WHILE: Enquanto houver dados no arquivo, ele vai somando as temperaturas e contando a quantidade de leituras

	while (ler_leitura(entrada, dataHora, temp, umid, pressao)) {
		soma_temp += temp;
		quantidade++;
	}

	// Calculando a média das temperaturas
	
	media_temp = soma_temp / quantidade;

	entrada.close();
	return media_temp;
}

// FUNÇÃO PARA CALCULAR A MÉDIA DAS UMIDADES

float media_umidade (tm dataHora, float temp, float umid, float pressao) {
	float media_umid;
	float soma_umid = 0.0;
	int quantidade = 0;

	ifstream entrada ("../dados.txt");

	// WHILE: Enquanto houver dados no arquivo, ele vai somando as umidades e contando a quantidade de leituras

	while (ler_leitura(entrada, dataHora, temp, umid, pressao)) {
		soma_umid += umid;
		quantidade++;
	}

	// Calculando a média das umidades
	
	media_umid = soma_umid / quantidade;

	entrada.close();
	return media_umid;
}

// FUNÇÃO PARA CALCULAR A MÉDIA DAS PRESSÕES

float media_pressao (tm dataHora, float temp, float umid, float pressao) {
	float media_pressao;
	float soma_pressao = 0.0;
	int quantidade = 0;

	ifstream entrada ("../dados.txt");

	// WHILE: Enquanto houver dados no arquivo, ele vai somando as pressões e contando a quantidade de leituras

	while (ler_leitura(entrada, dataHora, temp, umid, pressao)) {
		soma_pressao += pressao;
		quantidade++;
	}

	// Calculando a média das pressões
	
	media_pressao = soma_pressao / quantidade;

	entrada.close();
	return media_pressao;
}

	// ALOCAÇÃO DINÂMICA
float* carregar_temperaturas(int& n) {
	tm dt;
	float t, u, p;
	n = 0;

	ifstream entrada ("../dados.txt");
	while (ler_leitura(entrada, dt, t, u, p)) n++;		

	float* v = new float[n];							
	entrada.clear();
	entrada.seekg(0);									

	for (int i = 0; i < n; i++)
		ler_leitura(entrada, dt, v[i], u, p);			

	entrada.close();
	return v;											
}

// RECURSÃO
float soma_recursiva(float* v, int n) {
	if (n == 0) return 0;								
	return v[n-1] + soma_recursiva(v, n-1);				
}


void ordenar(float* v, int n) {
	for (int i = 1; i < n; i++) {
		float x = v[i];
		int j = i - 1;
		while (j >= 0 && v[j] > x) {
			v[j+1] = v[j];
			j--;
		}
		v[j+1] = x;
	}
}

// BUSCA BINÁRIA
int busca_binaria(float* v, int n, float alvo) {
	int ini = 0, fim = n - 1;
	while (ini <= fim) {
		int meio = (ini + fim) / 2;
		if (v[meio] == alvo) return meio;
		if (v[meio] < alvo) ini = meio + 1;
		else fim = meio - 1;
	}
	return -1;
}

int main()
{
	int opcao, controle = 1;
	string nomeTeste;
	tm dataHora;
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
				cout << "Informe os dados: ";
				cout << endl << "1. Nome do teste de analise de dados" << endl;
				cin.ignore();
				getline(cin, nomeTeste);
				cout << "2. Data e hora (dia mes ano hora)" << endl; 
				ler_dataHora(cin, dataHora);
				cout << "4. Temperatura (graus Celsius)" << endl;
				cin >> temp;
				cout << "5. Umidade relativa do ar (porcentagem)" << endl;
				cin >> umid;
				cout << "6. Pressao (atm)" << endl;
				cin >> pressao;

				inserir_leituras (nomeTeste, dataHora, temp, umid, pressao);

				cout << endl;
				break;

			case 2:
				cout << "===== MEDIA =====" << endl;
				cout << "Media Temperatura: " << media_temperatura(dataHora, temp, umid, pressao) << " graus Celsius" << endl;
				cout << "Media Umidade: " << media_umidade(dataHora, temp, umid, pressao) << " %" << endl;
				cout << "Media Pressao: " << media_pressao(dataHora, temp, umid, pressao) << " atm" << endl;
				cout << endl;
				break;

			case 3:
				cout << "===== LEITURAS =====" << endl;
				exibir_leituras(dataHora, temp, umid, pressao);
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
