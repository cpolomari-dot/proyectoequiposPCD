#include <iostream>
#include <thread>
#include <vector>
#include "Banco.h"

int main() {
    const int clientes = 4, cuentas = 10, distancia = 1, operaciones = 20, umbral = 100;

    Banco banco(cuentas, distancia, operaciones, umbral);

    std::vector<std::thread> hilos;
    for (int i = 0; i < clientes; ++i)
        hilos.emplace_back(&Banco::cliente, &banco, i);
    for (auto& h : hilos) h.join();

    std::cout << "Total: " << banco.total() << " (esperado " << 200 * cuentas << ")\n";
}