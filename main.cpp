#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <sstream>
#include <string>

using namespace std;

struct Leitura {
	string nomeTeste;
	tm dataHora;
	float temp, umid, pressao;
};

bool ler_dataHora (istream& in, tm& dataHora){ 
/* istream é uma classe que representa um fluxo de entrada que admite diferentes fontes.
Tanto de teclado, quanto de arquivos */

	int mes;
	dataHora = tm{}; // Essa linha inicializa todos os campos numéricos da struct tm com zero.

	if (!(in >> dataHora.tm_mday >> mes >> dataHora.tm_year >> dataHora.tm_hour)) return false;
	// Verifica se a leitura não foi bem-sucedida. Se falhar, a função executa return false; e termina imediatamente.

	dataHora.tm_mon = mes - 1;
	dataHora.tm_year = dataHora.tm_year - 1900;
	
	return true;
}

string formatar_data (tm dataHora) {
	
	char texto[50];
	strftime(texto, 50, "%d/%m/%Y %Hh", &dataHora);
	return texto;

}

bool ler_leitura (istream& in, Leitura& leitura) { 
	// & garante passagem por referência 

	string linha, data, hora;
	string temperatura, umidade, pressao;

	while (getline(in, linha)) {

		stringstream campos(linha); /* stringstream transforma o conteúdo da linha em um fluxo de 
		entrada permitindo ler cada dado através do delimitador (',') do getline*/

		getline(campos, leitura.nomeTeste, ',');

		if (leitura.nomeTeste == "Nome" || linha.empty()) continue;  // Verifica se está lendo o cabeçalho
		// O continue faz o programa ignorar aquela linha e voltar ao início do while para ler a próxima

		getline(campos, data, ',');
		getline(campos, hora, ',');
		getline(campos, temperatura, ',');
		getline(campos, umidade, ',');
		getline(campos, pressao);

		int dia, mes, ano;
		char barra1, barra2;

		/* stringstream pode ser utilizado, também, para converter strings em outros tipos, como int. 
		Isso é efetuado a seguir. */

		stringstream fluxo_data(data);

		if (!(fluxo_data >> dia >> barra1 >> mes >> barra2 >> ano)) return false;

		leitura.dataHora = tm{};
		leitura.dataHora.tm_mday = dia;
		leitura.dataHora.tm_mon = mes - 1;
		leitura.dataHora.tm_year = ano - 1900;

		stringstream fluxo_hora(hora);
		if (!(fluxo_hora >> leitura.dataHora.tm_hour)) return false;

		stringstream fluxo_temp(temperatura);
		stringstream fluxo_umid(umidade);
		stringstream fluxo_pressao(pressao);

		if (!(fluxo_temp >> leitura.temp)) return false;
		if (!(fluxo_umid >> leitura.umid)) return false;
		if (!(fluxo_pressao >> leitura.pressao)) return false;

		return true;
	}

	return false;

}

void inserir_leituras (Leitura leitura) {

	ifstream verificacao("../dados.csv");
	bool cabecalho = !verificacao.is_open() || verificacao.peek() == EOF;
	verificacao.close();

	ofstream saida ("../dados.csv", ios::app);	
	// ../ faz o programa procurar dados.csv na pasta principal e não em output, ou seja, volta uma pasta 
	// ios::app acrescenta as leituras no final sem apagar as leituras anteriores 

	if (!saida.is_open()) {
		cout << "Erro ao abrir dados.csv!" << endl;
		return;
	}

	if (cabecalho) {
		saida << "Nome,Data,Hora,Temperatura,Umidade,Pressao" << endl;
	}

	saida << leitura.nomeTeste << ",";
	saida << leitura.dataHora.tm_mday << "/" << leitura.dataHora.tm_mon + 1 << "/" << leitura.dataHora.tm_year + 1900 << ",";
	saida << leitura.dataHora.tm_hour << ",";
	saida << leitura.temp << ",";
	saida << leitura.umid << ",";
	saida << leitura.pressao << endl;

	saida.close();

}

void exibir_leituras (Leitura& leitura) {
	
	ifstream entrada("../dados.csv");

	while (ler_leitura(entrada, leitura))	{
		cout << "Nome do teste: " << leitura.nomeTeste << endl;
		cout << "Data: " << formatar_data(leitura.dataHora) << endl;
		cout << "Temperatura: " << leitura.temp << " graus Celsius" << endl;
		cout << "Umidade: " << leitura.umid << " %" << endl;
		cout << "Pressao: " << leitura.pressao << " atm" << endl << endl;
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
	Leitura leitura;
	n = 0;

	ifstream entrada ("../dados.csv");
	while (ler_leitura(entrada, leitura)) n++;		

	if (n == 0) {
		entrada.close();
		return nullptr;
	}

	float* v = new float[n];							
	entrada.clear(); // Limpa o estado de fluxo do arquivo. Apaga o indicador de fim de arquivo (eof)
	entrada.seekg(0); // Volta a posição de leitura para o início do arquivo.					

	for (int i = 0; i < n; i++) {
		ler_leitura(entrada, leitura);
		v[i] = leitura.temp;
		// Armazena as temperaturas no vetor		
	}

	entrada.close();
	return v;											
}

// Carrega temperaturas, umidades e pressões em três vetores dinâmicos.
// Essa função é utilizada para calcular as três médias.

void carregar_dados(float*& temperaturas, float*& umidades, float*& pressoes, int& n) {
	Leitura leitura;
	n = 0; // Inicializa a quantidade de leituras.

	ifstream entrada("../dados.csv");

	// Verifica se o arquivo foi aberto corretamente.
	if (!entrada.is_open()) {
		temperaturas = nullptr;
		umidades = nullptr;
		pressoes = nullptr;
		return;
	}

	// Conta a quantidade de leituras no arquivo.
	while (ler_leitura(entrada, leitura)) {
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
		ler_leitura(entrada, leitura);
		temperaturas[i] = leitura.temp;
		umidades[i] = leitura.umid;
		pressoes[i] = leitura.pressao;
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
		cout << "Nenhuma leitura encontrada ou erro ao abrir dados.csv!" << endl;
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

// Busca binária recursiva (função auxiliar): recebe os limites do intervalo.
int busca_binaria_rec(float* v, int ini, int fim, float alvo) {
	if (ini > fim) return -1;                 

	int meio = ini + (fim - ini) / 2;

	if (v[meio] == alvo) return meio;         
	if (v[meio] < alvo)
		return busca_binaria_rec(v, meio + 1, fim, alvo);   
	return busca_binaria_rec(v, ini, meio - 1, alvo);       
}

int busca_binaria(float* v, int n, float alvo) {
	return busca_binaria_rec(v, 0, n - 1, alvo);
}

int main()
{
	int opcao, controle = 1;
	Leitura leitura;

	while (controle)
	{
		cout << endl << endl << "\033[36m===== SEJA BEM VINDO =====\033[0m" << endl << endl;
		cout << "Caso queira ter acesso ao nosso menu, digite 1." << endl;
		cout << "Se deseja encerrar o programa, digite 0." << endl;
		cout << "Digite sua opcao: ";
		cin >> controle;

		if (controle)
		{
			cout << endl << "\033[33m===== MENU PRINCIPAL =====\033[0m" << endl << endl;
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
				getline(cin, leitura.nomeTeste);
				cout << endl << "2. Data e hora (dia mes ano hora): "; 
				ler_dataHora(cin, leitura.dataHora);
				cout << endl << "4. Temperatura (graus Celsius): ";
				cin >> leitura.temp;
				cout << endl << "5. Umidade relativa do ar (porcentagem): ";
				cin >> leitura.umid;
				cout << endl << "6. Pressao (atm): ";
				cin >> leitura.pressao;

				inserir_leituras(leitura);

				cout << endl;
				break;

			case 2:
				calcular_medias();
				cout << endl;
				break;

			case 3:
				cout << "===== LEITURAS =====" << endl;
				exibir_leituras(leitura);
				cout << endl;
				break;

			case 4: {
				int n;
				cout << "Digite a temperatura que deseja encontrar nos testes realizados: ";
				cin >> leitura.temp;
				float* vetor = carregar_temperaturas(n);

				if (n == 0) {
					cout << "Nenhuma leitura encontrada!" << endl;
					delete[] vetor;
					break;																				
				}

				ordenar(vetor, n);
				int posicao = busca_binaria(vetor, n, leitura.temp);
				cout << endl << "===== RESULTADO =====" << endl << "A temperatura de " << leitura.temp;
				cout << " graus Celsius se encontra na posicao " << posicao << " do vetor ordenado";
				cout << endl;

				delete[] vetor;
				break;
			} 
			// As chaves permitem que variáveis sejam declaradas dentro do case 

			case 5:
				cout << "\033[31m===== PROGRAMA ENCERRADO =====\033[0m" << endl << endl << endl;
				controle = 0;
				break;

			default:
				cout << "Opcao invalida!" << endl;
			}
		}
        else {
            cout << endl << "\033[31m===== PROGRAMA ENCERRADO =====\033[0m" << endl << endl;
        }
	}

	return 0;	
}
