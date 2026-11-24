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

bool ler_dataHora(istream& in, tm& dataHora) {
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

string formatar_data(tm dataHora) {
    char texto[50];
    strftime(texto, 50, "%d/%m/%Y %Hh", &dataHora);
    return texto;
}

bool ler_leitura(istream& in, Leitura& leitura) {
    // & garante passagem por referência

    string linha, data, hora;
    string temperatura, umidade, pressao;

    while (getline(in, linha)) {
        stringstream campos(linha); /* stringstream transforma o conteúdo da linha em um fluxo de
                                      entrada permitindo ler cada dado através do delimitador (',') do getline*/

        getline(campos, leitura.nomeTeste, ',');

        if (leitura.nomeTeste == "Nome" || linha.empty()) continue; // Verifica se está lendo o cabeçalho
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

void inserir_leituras(Leitura& leitura) {
    ifstream verificacao("../dados.csv");
    bool cabecalho = !verificacao.is_open() || verificacao.peek() == EOF;
    /* peek() olha a posição posterior a que o cursor se encontra no momento atual. EOF (end of file) significa
       o fim do arquivo. Esse conjunto verifica se o arquivo está vazio. */
    verificacao.close();

    ofstream saida("../dados.csv", ios::app);
    // ../ faz o programa procurar dados.csv na pasta principal e não em output, ou seja, volta uma pasta
    // ios::app acrescenta as leituras no final sem apagar as leituras anteriores

    if (!saida.is_open()) {
        cout << "Erro ao abrir dados.csv!" << endl;
        return;
    }

    // Criação de cabeçalho
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

void exibir_leituras(Leitura& leitura) {
    ifstream entrada("../dados.csv");

    while (ler_leitura(entrada, leitura)) {
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

// Redimensionamento de vetor dinâmico
void redimensionar(float*& v, int tamanhoAtual, int novoTamanho) {
    float* novoVetor = new float[novoTamanho];

    for (int i = 0; i < tamanhoAtual; i++) {
        novoVetor[i] = v[i];
    }

    delete[] v;
    v = novoVetor;
}

// Alocação dinâmica
float* carregar_temperaturas(int& n) {
    Leitura leitura;
    n = 0;

    int capacidade = 2;
    ifstream entrada("../dados.csv");

    if (!entrada.is_open()) return nullptr;

    float* v = new float[capacidade];

    while (ler_leitura(entrada, leitura)) {
        if (n == capacidade) {
            capacidade *= 2;
            redimensionar(v, n, capacidade);
        }

        v[n] = leitura.temp;
        n++;
    }

    entrada.close();

    if (n == 0) {
        delete[] v;
        return nullptr;
    }

    return v;
}

// Carrega temperaturas, umidades e pressões em três vetores dinâmicos.
// Essa função é utilizada para calcular as três médias.
void carregar_dados(float*& temperaturas, float*& umidades, float*& pressoes, int& n) {
    Leitura leitura;
    n = 0;

    int capacidade = 2;
    ifstream entrada("../dados.csv");

    if (!entrada.is_open()) {
        temperaturas = nullptr;
        umidades = nullptr;
        pressoes = nullptr;
        return;
    }

    temperaturas = new float[capacidade];
    umidades = new float[capacidade];
    pressoes = new float[capacidade];

    while (ler_leitura(entrada, leitura)) {
        if (n == capacidade) {
            capacidade *= 2;
            redimensionar(temperaturas, n, capacidade);
            redimensionar(umidades, n, capacidade);
            redimensionar(pressoes, n, capacidade);
        }

        temperaturas[n] = leitura.temp;
        umidades[n] = leitura.umid;
        pressoes[n] = leitura.pressao;
        n++;
    }

    entrada.close();

    if (n == 0) {
        delete[] temperaturas;
        delete[] umidades;
        delete[] pressoes;

        temperaturas = nullptr;
        umidades = nullptr;
        pressoes = nullptr;
    }
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

// Métodos de ordenação

// Insertion Sort
void insertion_sort(float* v, int n) {
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

// Selection Sort
void selection_sort(float* v, int n) {
    for (int i = 0; i < n - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[menor]) {
                menor = j;
            }
        }

        if (menor != i) {
            float aux = v[i];
            v[i] = v[menor];
            v[menor] = aux;
        }
    }
}

// Quick Sort: função auxiliar que particiona o vetor.
int particionar(float* v, int ini, int fim) {
    float pivo = v[fim];
    int i = ini - 1;

    for (int j = ini; j < fim; j++) {
        if (v[j] <= pivo) {
            i++;
            float aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }
    }

    float aux = v[i+1];
    v[i+1] = v[fim];
    v[fim] = aux;

    return i + 1;
}

// Quick Sort
void quick_sort(float* v, int ini, int fim) {
    if (ini < fim) {
        int posicaoPivo = particionar(v, ini, fim);
        quick_sort(v, ini, posicaoPivo - 1);
        quick_sort(v, posicaoPivo + 1, fim);
    }
}

// Shell Sort
void shell_sort(float* v, int n) {
    for (int intervalo = n / 2; intervalo > 0; intervalo /= 2) {
        for (int i = intervalo; i < n; i++) {
            float valor = v[i];
            int j = i;

            while (j >= intervalo && v[j - intervalo] > valor) {
                v[j] = v[j - intervalo];
                j -= intervalo;
            }

            v[j] = valor;
        }
    }
}

// Merge Sort: intercala duas partes ordenadas do vetor.
void intercalar(float* v, int ini, int meio, int fim) {
    int tamanho = fim - ini + 1;
    float* auxiliar = new float[tamanho];

    int i = ini;
    int j = meio + 1;
    int k = 0;

    while (i <= meio && j <= fim) {
        if (v[i] <= v[j]) {
            auxiliar[k] = v[i];
            i++;
        } else {
            auxiliar[k] = v[j];
            j++;
        }
        k++;
    }

    while (i <= meio) {
        auxiliar[k] = v[i];
        i++;
        k++;
    }

    while (j <= fim) {
        auxiliar[k] = v[j];
        j++;
        k++;
    }

    for (int pos = 0; pos < tamanho; pos++) {
        v[ini + pos] = auxiliar[pos];
    }

    delete[] auxiliar;
}

// Merge Sort
void merge_sort(float* v, int ini, int fim) {
    if (ini < fim) {
        int meio = ini + (fim - ini) / 2;
        merge_sort(v, ini, meio);
        merge_sort(v, meio + 1, fim);
        intercalar(v, ini, meio, fim);
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

int main() {
    int opcao, controle = 1;
    Leitura leitura;

    while (controle) {
        cout << endl << endl << "\033[36m===== SEJA BEM VINDO =====\033[0m" << endl << endl;
        cout << "Caso queira ter acesso ao nosso menu, digite 1." << endl;
        cout << "Se deseja encerrar o programa, digite 0." << endl;
        cout << "Digite sua opcao: ";
        cin >> controle;

        if (controle) {
            cout << endl << "\033[33m===== MENU PRINCIPAL =====\033[0m" << endl << endl;
            cout << "1. Inserir Leitura" << endl;
            cout << "2. Exibir Medias" << endl;
            cout << "3. Exibir Leituras" << endl;
            cout << "4. Encontrar temperatura por busca binaria em vetor ordenado" << endl;
            cout << "5. Sair" << endl;
            cout << "Digite sua opcao: ";
            cin >> opcao;
            cout << endl;

            switch (opcao) {
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
                int metodo;

                float* vetor = carregar_temperaturas(n);

                if (n == 0) {
                    cout << "Nenhuma leitura encontrada!" << endl;
                    delete[] vetor;
                    break;
                }

                cout << "===== METODO DE ORDENACAO =====" << endl;
                cout << "1. Insertion Sort" << endl;
                cout << "2. Selection Sort" << endl;
                cout << "3. Quick Sort" << endl;
                cout << "4. Shell Sort" << endl;
                cout << "5. Merge Sort" << endl;
                cout << "Escolha o metodo que deseja utilizar: ";
                cin >> metodo;

                switch (metodo) {
                	case 1:
                  		insertion_sort(vetor, n);
                		break;
                	case 2:
                  		selection_sort(vetor, n);
                  		break;
               		case 3:
                    	quick_sort(vetor, 0, n - 1);
                    	break;
                	case 4:
                    	shell_sort(vetor, n);
                		break;
            		case 5:
                    	merge_sort(vetor, 0, n - 1);
                    	break;
                	default:
                    	cout << "Metodo de ordenacao invalido!" << endl;
                    	delete[] vetor;
                    	break;
                }

                if (metodo < 1 || metodo > 5) {
                    break;
                }

                cout << "Digite a temperatura que deseja encontrar nos testes realizados: ";
                cin >> leitura.temp;

                int posicao = busca_binaria(vetor, n, leitura.temp);
                cout << endl << "===== RESULTADO =====" << endl;

                if (posicao != -1) {
                    cout << "A temperatura de " << leitura.temp;
                    cout << " graus Celsius foi encontrada na posicao " << posicao << " do vetor ordenado." << endl;
                } else {
                    cout << "A temperatura de " << leitura.temp << " graus Celsius nao foi encontrada." << endl;
                }

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
        } else {
            cout << endl << "\033[31m===== PROGRAMA ENCERRADO =====\033[0m" << endl << endl;
        }
    }

    return 0;
}
