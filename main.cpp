#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

bool ler_dataHora (istream& in, tm& dataHora){ 
/* istream é uma classe que representa um fluxo de entrada que admite diferentes fontes.
Tanto de teclado, quanto de arquivos */

	int mes;
	dataHora = tm{}; // Essa linha inicializa todos os campos numéricos da struct tm com zero.

	if (!(in >> dataHora.tm_mday >> mes >>  dataHora.tm_year >> dataHora.tm_hour)) return false;
	// Verifica se a leitura não foi bem-sucedida. Se falhar, a função executa return false; e termina imediatamente.

	dataHora.tm_mon = mes - 1;
	dataHora.tm_year = dataHora.tm_year - 1900;
	dataHora.tm_isdst = -1; 
	/* tm_isdt = -1 faz com que a informação de horário de verão fique desconhecida. Faz com que a biblioteca 
	determine essa informação em futuras conversões de data com esse campo */

	return true;
}

string formatar_data (tm dataHora) {
	
	char texto[50];
	strftime(texto, 50, "%d/%m/%Y %Hh", &dataHora);
	return texto;

}

bool ler_leitura (istream& in, string& nomeTeste, tm& dataHora, float& temp, float& umid, float& pressao) { 
	// & garante passagem por referência 

	return (in >> ws && getline(in, nomeTeste) && ler_dataHora(in, dataHora) && in >> temp >> umid >> pressao);
	// ws (white space) retira qualquer quebra de linha ou espaço em branco antes do getline 
}

void inserir_leituras (string nomeTeste, tm dataHora, float temp, float umid, float pressao) {

	ofstream saida ("../dados.txt", ios::app);	
	// ../ faz o programa procurar dados.txt na pasta principal e não em output, ou seja, volta uma pasta 
	// ios::app acrescenta as leituras no final sem apagar as leituras anteriores 

	saida << endl << endl;
	saida << nomeTeste << endl << dataHora.tm_mday << " " << dataHora.tm_mon + 1 << " " << dataHora.tm_year + 1900 << endl << dataHora.tm_hour;	
	saida << endl << temp << endl << umid << endl << pressao;

	saida.close();

}

void exibir_leituras (string nomeTeste, tm dataHora, float temp, float umid, float pressao) {
	
	ifstream entrada("../dados.txt");

	while (ler_leitura(entrada, nomeTeste, dataHora, temp, umid, pressao))	{
		cout << "Nome do teste: " << nomeTeste << endl;
		cout << "Data: " << formatar_data(dataHora) << endl;
		cout << "Temperatura: " << temp << " graus Celsius" << endl;
		cout << "Umidade: " << umid << " %" << endl;
		cout << "Pressao: " << pressao << " atm" << endl << endl;
	}

	cout << "\033[2A";

	entrada.close();
}

// Recursão

float soma_recursiva(float* v, int n) {
	if (n == 0) return 0;								
	return v[n-1] + soma_recursiva(v, n-1);				
}


// Alocação dinâmica 

float* carregar_temperaturas(int &n) {
	tm dataHora;
	float temp, umid, pressao;
	string nomeTeste;
	n = 0;

	ifstream entrada ("../dados.txt");
	while (ler_leitura(entrada, nomeTeste, dataHora, temp, umid, pressao)) n++;		

	float* v = new float[n];							
	entrada.clear(); // Limpa o estado de fluxo do arquivo. Apaga o indicador de fim de arquivo (eof)
	entrada.seekg(0); // Volta a posição de leitura para o início do arquivo.					

	for (int i = 0; i < n; i++) {
		ler_leitura(entrada, nomeTeste, dataHora, v[i], umid, pressao); 
		// Armazena as temperaturas no vetor		
	}

	entrada.close();
	return v;											
}

// Carrega temperaturas, umidades e pressões em três vetores dinâmicos.
// Essa função é utilizada para calcular as três médias.

void carregar_dados(float*& temperaturas, float*& umidades, float*& pressoes, int& n) {
	tm dataHora;
	float temp, umid, pressao;
	string nomeTeste;
	n = 0; // Inicializa a quantidade de leituras.

	ifstream entrada("../dados.txt");

	// Verifica se o arquivo foi aberto corretamente.
	if (!entrada.is_open()) {
		temperaturas = nullptr;
		umidades = nullptr;
		pressoes = nullptr;
		return;
	}

	// Conta a quantidade de leituras no arquivo.
	while (ler_leitura(entrada, nomeTeste, dataHora, temp, umid, pressao)) {
		n++;
	}

	// Se não houver leituras, não é necessário criar os vetores.
	if (n == 0) {
		temperaturas = nullptr;
		umidades = nullptr;
		pressoes = nullptr;
		entrada.close();
		return;
	}

	// Aloca os três vetores com a quantidade de leituras encontrada.
	temperaturas = new float[n];
	umidades = new float[n];
	pressoes = new float[n];

	// Volta ao início do arquivo para armazenar os valores nos vetores.
	entrada.clear();
	entrada.seekg(0);

	// Segunda leitura: armazena cada valor no vetor correspondente.
	for (int i = 0; i < n; i++) {
		ler_leitura(entrada, nomeTeste, dataHora, temp, umid, pressao);
		temperaturas[i] = temp;
		umidades[i] = umid;
		pressoes[i] = pressao;
	}

	entrada.close(); // Fecha o arquivo após armazenar os dados.
}

// Calcula as três médias com a função recursiva e salva em medias.txt.
void calcular_medias() {

	// Declara três ponteiros para armazenar os vetores dinâmicos.
	float* temperaturas;
	float* umidades;
	float* pressoes;

	int n; // Quantidade de leituras encontradas no arquivo.

	// Carrega os dados do arquivo para os três vetores.
	carregar_dados(temperaturas, umidades, pressoes, n);

	// Verifica se existem leituras para calcular as médias.
	if (n == 0) {
		cout << "Nenhuma leitura encontrada ou erro ao abrir dados.txt!" << endl;
		return;
	}

	// Soma os valores de cada vetor usando recursão e divide pela quantidade de leituras para obter as médias.
	float media_temp = soma_recursiva(temperaturas, n) / n;
	float media_umid = soma_recursiva(umidades, n) / n;
	float media_pressao = soma_recursiva(pressoes, n) / n;

	// Exibe as três médias na tela.
	cout << "===== MEDIA =====" << endl;
	cout << "Media Temperatura: " << media_temp << " graus Celsius" << endl;
	cout << "Media Umidade: " << media_umid << " %" << endl;
	cout << "Media Pressao: " << media_pressao << " atm" << endl;

	// Abre o arquivo medias.txt para salvar os resultados.
	ofstream saida("../medias.txt");

	// Verifica se o arquivo foi aberto corretamente.
	if (!saida.is_open()) {
		cout << "Erro ao criar medias.txt!" << endl;
	} else {

		// Grava as médias no arquivo.
		saida << "Media Temperatura: " << media_temp << " graus Celsius" << endl;
		saida << "Media Umidade: " << media_umid << " %" << endl;
		saida << "Media Pressao: " << media_pressao << " atm" << endl;
		saida.close();
		cout << "Medias salvas em medias.txt!" << endl;
	}

	// Libera a memória dos três vetores.
	delete[] temperaturas;
	delete[] umidades;
	delete[] pressoes;
}

// Método de Ordenação: Insertion Sort 

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

// Busca binária 

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
		cout << endl << endl << "\033[36m===== SEJA BEM VINDO =====\033[0m" << endl << endl;
		cout << "Caso queira ter acesso ao nosso menu, digite 1." << endl;
		cout << "Se deseja encerrar o programa, digite 0." << endl;
		cout << "Digite sua opcao: ";
		cin >> controle;

		if (controle)
		{
			cout << endl << "\033[31m===== MENU PRINCIPAL =====\033[0m" << endl << endl;
			cout << "1. Inserir Leitura" << endl;
			cout << "2. Exibir Medias" << endl;
			cout << "3. Exibir Leituras" << endl;
			cout << "4. Encontrar temperatura por busca binaria em vetor ordenado" << endl;
			cout << "5. Sair" << endl;
			cout << "Digite sua opcao: ";
			cin >> opcao;
			cout << endl;

			switch (opcao)
			{
			case 1:
				cout << "Informe os dados: ";
				cout << endl << "1. Nome do teste de analise de dados: ";
				cin.ignore();
				getline(cin, nomeTeste);
				cout << endl << "2. Data e hora (dia mes ano hora): "; 
				ler_dataHora(cin, dataHora);
				cout << endl << "4. Temperatura (graus Celsius): ";
				cin >> temp;
				cout << endl << "5. Umidade relativa do ar (porcentagem): ";
				cin >> umid;
				cout << endl << "6. Pressao (atm): ";
				cin >> pressao;

				inserir_leituras(nomeTeste, dataHora, temp, umid, pressao);

				cout << endl;
				break;

			case 2:
				calcular_medias();
				cout << endl;
				break;

			case 3:
				cout << "===== LEITURAS =====" << endl;
				exibir_leituras(nomeTeste, dataHora, temp, umid, pressao);
				cout << endl;
				break;

			case 4: {
				int n;
				cout << "Digite a temperatura que deseja encontrar nos testes realizados: ";
				cin >> temp;
				float* vetor = carregar_temperaturas(n);
				ordenar(vetor, n);
				int posicao = busca_binaria(vetor, n, temp);
				cout << endl << "===== RESULTADO =====" << endl << "A temperatura de " << temp;
				cout << " graus Celsius se encontra na posicao " << posicao << " do vetor ordenado";
				cout << endl;
				break;
			} 
			// As chaves permitem que variáveis sejam declaradas dentro do case 

			case 5:
				cout << "Programa encerrado." << endl << endl << endl;
				controle = 0;
				break;

			default:
				cout << "Opcao invalida!" << endl;
			}
		}
	}

	return 0;	
}
