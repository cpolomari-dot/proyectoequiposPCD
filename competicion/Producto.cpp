#include "Producto.h"
#include "Valor.h"

#include <thread>
#include <vector>

std::uint64_t producto(int hilos) {
    if (hilos <= 1) {
        std::uint64_t r = 1;
        for (std::uint64_t i = 0; i < N_VALORES; ++i) {
            r = (r * valor(i)) % MODULO;
        }
        return r;
    }

    const int num_hilos = hilos;
    std::vector<std::uint64_t> resultados(num_hilos, 1);
    std::vector<std::thread> trabajadores;
    trabajadores.reserve(num_hilos);

    const std::uint64_t por_hilo = N_VALORES / num_hilos;
    const std::uint64_t resto = N_VALORES % num_hilos;

    std::uint64_t inicio = 0;
    for (int t = 0; t < num_hilos; ++t) {
        std::uint64_t fin = inicio + por_hilo + (t < static_cast<int>(resto) ? 1 : 0);
        trabajadores.emplace_back([inicio, fin, t, &resultados]() {
            std::uint64_t acum = 1;
            for (std::uint64_t i = inicio; i < fin; ++i) {
                acum = (acum * valor(i)) % MODULO;
            }
            resultados[t] = acum;
        });
        inicio = fin;
    }

    for (auto& th : trabajadores) {
        th.join();
    }

    std::uint64_t total = 1;
    for (int t = 0; t < num_hilos; ++t) {
        total = (total * resultados[t]) % MODULO;
    }
    return total;
}
